#include "canard.h"
#include "heap.h"
#include "main.h"
#include "debuglog.h"
#include "Enable_1_0.h"
CanardInstance canard;
CanardTxQueue txQueue;
CanardRxSubscription subscription_enablehandle;
static void* mem_allocate(CanardInstance* const canard, const size_t amount) {
    (void) canard;
    return malloc(amount);
}
static void mem_free(CanardInstance* const canard, void* const pointer) {
    (void) canard;
    free(pointer);
}
void slave_comm_init() 
{
    canard = canardInit(&mem_allocate, &mem_free);
    canard.node_id = 28; // 设置本机节点 ID
    txQueue = canardTxInit(100, CANARD_MTU_CAN_CLASSIC); // 初始化发送队列
}

void subscribe_enable(void) 
{
    canardRxSubscribe(
        &canard,
        CanardTransferKindRequest,
        113, // 主题 ID
        1, // 范围
        CANARD_DEFAULT_TRANSFER_ID_TIMEOUT_USEC,
        &subscription_enablehandle
    );
}
// 发送响应函数
void send_response(CanardNodeID remote_node_id, const void* payload, size_t payload_size) 
{
    CanardTransferMetadata transfer_metadata = {
        .priority = CanardPriorityNominal,
        .transfer_kind = CanardTransferKindResponse,
        .port_id = 113, // 主题 ID
        .remote_node_id = remote_node_id,
        .transfer_id = 0
    };

    int32_t result = canardTxPush(
        &txQueue,
        &canard,
        HAL_GetTick() * 1000, // 时间戳
        &transfer_metadata,
        payload_size,
        payload
    );

    if (result < 0) {
        // 处理错误
        USER_DEBUG_NORMAL("Error sending response\n");
    }
}
// 定义接收传输的回调函数
void process_received_transfer(const uint8_t index, CanardRxTransfer* const transfer) 
{
    if (transfer->metadata.transfer_kind == CanardTransferKindRequest) {
        // 处理接收到的消息
        USER_DEBUG_NORMAL("Received message from node %u\n", transfer->metadata.remote_node_id);
        USER_DEBUG_NORMAL("Payload: ");
        for (size_t i = 0; i < transfer->payload_size; i++) {
            USER_DEBUG_NORMAL("%02x ", ((uint8_t*)transfer->payload)[i]);
        }
        USER_DEBUG_NORMAL("\n");

        // 反序列化接收到的数据
        custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Enable_Request_1_0 request;
        int8_t result = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Enable_Request_1_0_deserialize_(&request, transfer->payload, &transfer->payload_size);
        if (result < 0) {
            USER_DEBUG_NORMAL("Error deserializing request\n");
            return;
        }

        // 处理请求
        USER_DEBUG_NORMAL("Received request: enable_state = %u\n", request.enable_state);

        // 创建响应
        custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Enable_Response_1_0 response;
        response.status = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Enable_Response_1_0_SET_SUCCESS;

        // 序列化响应
        uint8_t buffer[custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Enable_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_];
        size_t buffer_size = sizeof(buffer);
        result = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Enable_Response_1_0_serialize_(&response, buffer, &buffer_size);
        if (result < 0) {
            USER_DEBUG_NORMAL("Error serializing response\n");
            return;
        }

        // 发送响应
        send_response(transfer->metadata.remote_node_id, buffer, buffer_size);
    }
}
// 发送帧函数
void send_frames(FDCAN_HandleTypeDef *hfdcan) 
{
    CanardTxQueueItem *item = NULL;
    while ((item = canardTxPeek(&txQueue)) != NULL) 
    {
        FDCAN_TxHeaderTypeDef txHeader;
        uint8_t txData[8];

        // 填充 FDCAN_TxHeaderTypeDef 结构
        txHeader.Identifier = item->frame.extended_can_id;
        txHeader.IdType = FDCAN_EXTENDED_ID; // 标准 ID
        txHeader.TxFrameType = FDCAN_DATA_FRAME; // 数据帧
        txHeader.DataLength = item->frame.payload_size; // 数据长度
        txHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE; // 错误状态指示器
        txHeader.BitRateSwitch = FDCAN_BRS_ON; // 波特率切换
        txHeader.FDFormat = FDCAN_FD_CAN; // CAN FD 格式
        txHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS; // 不使用发送事件 FIFO
        txHeader.MessageMarker = 0; // 消息标记

        // 填充发送数据
        for (size_t i = 0; i < item->frame.payload_size; i++) {
            txData[i] = ((uint8_t *)item->frame.payload)[i];
        }

        // 发送帧
        if (HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &txHeader, txData) != HAL_OK) {
            // 处理发送错误
            USER_DEBUG_NORMAL("Error sending frame\n");
            break;
        }
        USER_DEBUG_NORMAL("send can fram\r\n");
        // 从队列中移除帧
        canardTxPop(&txQueue, item);
    }
}

#include "canard.h"
#include "board_protocolInterface.h"
#include "Heartbeat_1_0.h"
#include "uavcan/node/Health_1_0.h"
#include "uavcan/node/Mode_1_0.h"

// 静态变量用于存储 transfer_id，确保每次调用时递增
static uint8_t transfer_id = 0;

void send_heartpack() 
{
    // 获取当前时间（以微秒为单位）
    uint64_t current_time_us = HAL_GetTick() * 1000ULL;

    // 创建心跳包数据结构
    uavcan_node_Heartbeat_1_0 heartbeat;
    heartbeat.uptime = (uint32_t)(current_time_us / 1000000ULL); // 转换为秒
    heartbeat.health.value = uavcan_node_Health_1_0_NOMINAL; // 健康状态
    heartbeat.mode.value = uavcan_node_Mode_1_0_OPERATIONAL; // 操作模式
    heartbeat.vendor_specific_status_code = 0; // 厂商特定状态码

    // 准备缓冲区
    uint8_t buffer[uavcan_node_Heartbeat_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t buffer_size = sizeof(buffer);

    // 序列化心跳包数据
    int8_t result = uavcan_node_Heartbeat_1_0_serialize_(&heartbeat, buffer, &buffer_size);
    if (result < 0) {
        USER_DEBUG_NORMAL("Error serializing heartbeat message\n");
        return;
    }

    // 准备传输元数据
    CanardTransferMetadata transfer_metadata = {
        .priority = CanardPriorityNominal,
        .transfer_kind = CanardTransferKindMessage,
        .port_id = uavcan_node_Heartbeat_1_0_FIXED_PORT_ID_, // 心跳包主题 ID
        .remote_node_id = CANARD_NODE_ID_UNSET, // 广播
        .transfer_id = transfer_id // 使用当前 transfer_id
    };

    // 将心跳包推入发送队列
    int32_t send_result = canardTxPush(
        &txQueue,
        &canard,
        current_time_us, // 时间戳
        &transfer_metadata,
        buffer_size,
        buffer
    );

    if (send_result < 0) {
        // 处理发送错误
        USER_DEBUG_NORMAL("Error sending heartbeat message\n");
    } else {
        USER_DEBUG_NORMAL("Heartbeat message sent successfully\n");
    }

    // 递增 transfer_id，溢出时自动从 0 开始
    transfer_id = (transfer_id + 1) % (CANARD_TRANSFER_ID_MAX + 1);
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
