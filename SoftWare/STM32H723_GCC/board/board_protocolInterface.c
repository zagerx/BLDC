#include "canard.h"
#include "heap.h"
#include "main.h"
#include "debuglog.h"
static void* mem_allocate(CanardInstance* const canard, const size_t amount)
{
    (void) canard;
    return heap_malloc(amount);
}

static void mem_free(CanardInstance* const canard, void* const pointer)
{
    (void) canard;
    heap_free(pointer);
}
CanardInstance canard;
CanardTxQueue txQueue;

void slave_comm_init()
{
    canard = canardInit(&mem_allocate, &mem_free);
    canard.node_id = 28; // 设置本机节点 ID
    txQueue = canardTxInit(100, CANARD_MTU_CAN_CLASSIC); // 初始化发送队列
}
CanardRxSubscription heartbeat_subscription;

void subscribe_enable(void) 
{
    canardRxSubscribe(
        &canard,
        CanardTransferKindRequest,
        113, // 
        1, // 
        CANARD_DEFAULT_TRANSFER_ID_TIMEOUT_USEC,
        &heartbeat_subscription
    );
}

// 定义接收传输的回调函数
void process_received_transfer(const uint8_t index, CanardRxTransfer* const transfer) {
    if (transfer->metadata.transfer_kind == CanardTransferKindRequest) 
    {
        // 处理接收到的消息
        USER_DEBUG_NORMAL("Received message from node %u\n", transfer->metadata.remote_node_id);
        USER_DEBUG_NORMAL("Payload: ");
        for (size_t i = 0; i < transfer->payload_size; i++) {
            USER_DEBUG_NORMAL("%02x ", ((uint8_t*)transfer->payload)[i]);
        }
        USER_DEBUG_NORMAL("\n");
    }
}

// 在接收中断中调用
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
    uint8_t i = 0;
    uint8_t rxdata[8];
    FDCAN_RxHeaderTypeDef FDCAN1_RxHeader;
    CanardRxTransfer transfer;
    CanardFrame canard_frame;
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN1_RxHeader, rxdata);

        // 填充 CanardFrame 结构
        canard_frame.extended_can_id = FDCAN1_RxHeader.Identifier;
        canard_frame.payload_size = FDCAN1_RxHeader.DataLength;
        canard_frame.payload = rxdata;

        // 处理接收到的帧
        int8_t result = canardRxAccept(
            &canard,
            HAL_GetTick() * 1000, // 时间戳
            &canard_frame,
            0, // 冗余接口索引
            &transfer,
            NULL
        );

        if (result < 0) {
            // 处理错误
            USER_DEBUG_NORMAL("Error processing frame\n");
        } else if (result == 1) {
            // 处理接收到的传输
            process_received_transfer(0, &transfer);
            canard.memory_free(&canard, transfer.payload);
        }
    }
}


