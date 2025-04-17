/*
libcan.c/.h

1、实现static void* mem_allocate(CanardInstance* const canard, const size_t amount)) 
2、实现static void mem_free(CanardInstance* const canard, void* const pointer)) 
3、作为服务器，需要向libcan.c/.h注册可提供哪些服务
// 参数说明：
int8_t canardRxSubscribe(
    CanardInstance* const ins,                     // [in]  Canard 实例指针，管理所有订阅和会话状态
    const CanardTransferKind transfer_kind,       // [in]  传输类型：消息(Message)/请求(Request)/响应(Response)
    const CanardPortID port_id,                   // [in]  端口ID (Subject-ID 或 Service-ID)
    const size_t extent,                         // [in]  接收缓冲区大小（需大于最大预期负载大小）
    const CanardMicrosecond transfer_id_timeout_usec, // [in] 传输ID超时时间（微秒）
    CanardRxSubscription* const out_subscription  // [out] 输出参数，存储创建的订阅对象
);
// 关键行为：
1. 订阅管理：
   - 如果已存在相同(transfer_kind, port_id)的订阅，会先调用 canardRxUnsubscribe() 移除旧订阅
   - 然后创建新订阅并插入到 CAVL 树中

2. 内存管理：
   - 不分配新内存（会话对象将在首次收到匹配帧时按需创建）
   - 初始化订阅对象的各字段（包括清空会话指针数组）

3. 返回值：
   - 1: 成功创建新订阅
   - 0: 替换了现有订阅
   - 负值: 参数错误（CANARD_ERROR_INVALID_ARGUMENT）

// 参数详细说明：
1. ins: 
   - 指向 Canard 库实例的指针，包含所有订阅和会话状态
   - 必须非 NULL 且已初始化

2. transfer_kind: 
   - 枚举值，指定传输类型：
     * CANARD_TRANSFER_KIND_MESSAGE   (广播消息)
     * CANARD_TRANSFER_KIND_REQUEST   (服务请求)  
     * CANARD_TRANSFER_KIND_RESPONSE  (服务响应)

3. port_id: 
   - 对于消息传输：Subject-ID (0-8191)
   - 对于服务传输：Service-ID (0-511)
   - 决定哪些帧会被传递给该订阅

4. extent: 
   - 接收缓冲区大小（字节）
   - 应大于预期最大负载大小（考虑未来协议扩展）
   - 超出的负载会被截断但CRC仍会验证

5. transfer_id_timeout_usec:
   - 传输ID超时时间（微秒）
   - 推荐使用 CANARD_DEFAULT_TRANSFER_ID_TIMEOUT_USEC
   - 用于检测丢失帧和会话超时

6. out_subscription:
   - 输出参数，接收初始化后的订阅对象
   - 必须由调用方分配内存并保持有效直到调用 canardRxUnsubscribe()

*/

#include "canard.h"
#include "heap.h"
#include "main.h"
#include "debuglog.h"
#include "Enable_1_0.h"
#include "Heartbeat_1_0.h"
#include "uavcan/node/Health_1_0.h"
#include "uavcan/node/Mode_1_0.h"
#include "dinosaurs/actuator/wheel_motor/OdometryAndVelocityPublish_1_0.h"
#include "dinosaurs/PortId_1_0.h"
#include "dinosaurs/actuator/wheel_motor/SetTargetValue_2_0.h"
#include "uavcan/time/Synchronization_1_0.h"

// #include <cstdint>
#include <stdint.h>

CanardInstance canard;
CanardTxQueue txQueue;


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

void send_response(CanardNodeID remote_node_id, CanardPortID port_id,const void* payload, size_t payload_size) 
{
    CanardTransferMetadata transfer_metadata = {
        .priority = CanardPriorityNominal,
        .transfer_kind = CanardTransferKindResponse,
        .port_id = port_id, // 主题 ID
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
        // 从队列中移除帧
        canardTxPop(&txQueue, item);
    }
}

void send_heartpack() 
{
    static int16_t transfer_id = 0;
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
        // USER_DEBUG_NORMAL("Heartbeat message sent successfully\n");
    }

    // 递增 transfer_id，溢出时自动从 0 开始
    transfer_id = (transfer_id + 1) % (CANARD_TRANSFER_ID_MAX + 1);
}
void send_odom_vect(void)
{
    static int16_t transfer_id = 0;
    // 获取当前时间（以微秒为单位）
    uint64_t current_time_us = HAL_GetTick() * 1000ULL;

    // 创建心跳包数据结构
    custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_OdometryAndVelocityPublish_1_0 odom_vect;
    odom_vect.timestamp.microsecond = (uint32_t)(current_time_us / 1000000ULL);
    odom_vect.current_velocity.elements[0].meter_per_second = sinf(transfer_id*(6.28f/255));
    odom_vect.current_velocity.elements[1].meter_per_second = cosf(transfer_id*(6.28f/255));
    odom_vect.current_velocity.count = 2;
    odom_vect.odometry.elements[0].meter = cosf(transfer_id*(6.28f/255));
    odom_vect.odometry.elements[1].meter = sinf(transfer_id*(6.28f/255));
    odom_vect.odometry.count = 2;

    // 准备缓冲区
    uint8_t buffer[custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_OdometryAndVelocityPublish_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_];
    size_t buffer_size = sizeof(buffer);

    // 序列化心跳包数据
    int8_t result = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_OdometryAndVelocityPublish_1_0_serialize_(&odom_vect, buffer, &buffer_size);
    if (result < 0) {
        USER_DEBUG_NORMAL("Error serializing odom message\n");
        return;
    }

    // 准备传输元数据
    CanardTransferMetadata transfer_metadata = {
        .priority = CanardPriorityNominal,
        .transfer_kind = CanardTransferKindMessage,
        .port_id = custom_data_types_dsdl_dinosaurs_PortId_1_0_actuator_wheel_motor_OdometryAndVelocityPublish_1_0_ID, // 心跳包主题 ID
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
        // USER_DEBUG_NORMAL("Heartbeat message sent successfully\n");
    }

    // 递增 transfer_id，溢出时自动从 0 开始
    transfer_id = (transfer_id + 1) % (CANARD_TRANSFER_ID_MAX + 1);    
}

typedef void (*MessageHandler)(CanardRxTransfer* transfer);
static void handle_motor_enable(CanardRxTransfer* transfer)
{
    const uint8_t* data; size_t len; CanardNodeID sender_id;CanardPortID port_id;
    data = transfer->payload;
    sender_id = transfer->metadata.remote_node_id;
    port_id = transfer->metadata.port_id;
    len = transfer->payload_size;
    custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Enable_Request_1_0 req;
    size_t inout_size = len;
    if (custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Enable_Request_1_0_deserialize_(&req, data, &inout_size) >= 0) {
        USER_DEBUG_NORMAL("Node %u set enable: %u", sender_id, req.enable_state);

        // 创建响应
        custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Enable_Response_1_0 response;
        response.status = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Enable_Response_1_0_SET_SUCCESS;

        // 序列化响应
        uint8_t buffer[custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Enable_Response_1_0_SERIALIZATION_BUFFER_SIZE_BYTES_];
        size_t buffer_size = sizeof(buffer);
        uint8_t result = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_Enable_Response_1_0_serialize_(&response, buffer, &buffer_size);
        if (result < 0) {
            USER_DEBUG_NORMAL("Error serializing response\n");
            return;
        }
        // 发送响应
        send_response(sender_id, port_id,buffer, buffer_size);
    }
}
static void handle_set_targe(CanardRxTransfer* transfer)
{
    const uint8_t* data; size_t len; CanardNodeID sender_id;CanardPortID port_id;
    data = transfer->payload;
    sender_id = transfer->metadata.remote_node_id;
    len = transfer->payload_size;
    port_id = transfer->metadata.port_id;
    custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetTargetValue_Request_2_0 req;
    size_t inout_size = len;
    if (custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetTargetValue_Request_2_0_deserialize_(&req, data, &inout_size) >= 0) {
        USER_DEBUG_NORMAL("Node %u set targe: %f  %f", sender_id, req.velocity.elements[0],req.velocity.elements[1]);
        USER_DEBUG_NORMAL("Node %u set targe: %f  %f", sender_id, req._torque.elements[0],req.velocity.elements[1]);

        // 创建响应
        custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetTargetValue_Response_2_0 response;
        response.status = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetTargetValue_Response_2_0_PARAMETER_NOT_INIT;

        // 序列化响应
        uint8_t buffer[custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetTargetValue_Response_2_0_EXTENT_BYTES_];
        size_t buffer_size = sizeof(buffer);
        uint8_t result = custom_data_types_dsdl_dinosaurs_actuator_wheel_motor_SetTargetValue_Request_2_0_serialize_(&response, buffer, &buffer_size);
        if (result < 0) {
            USER_DEBUG_NORMAL("Error serializing response\n");
            return;
        }
        // 发送响应
        send_response(sender_id, port_id,buffer, buffer_size);
    }
}
static void handle_time(CanardRxTransfer* transfer)
{
    uavcan_time_Synchronization_1_0 sync_msg;
    size_t buffer_size = transfer->payload_size;
    
    // 反序列化消息
    int8_t result = uavcan_time_Synchronization_1_0_deserialize_(
        &sync_msg, 
        (const uint8_t*)transfer->payload, 
        &buffer_size
    );

    if (result >= 0) {
        // 成功解析
        // 正确提取56位时间戳（使用掩码确保只取56位）
        uint64_t timestamp_us = sync_msg.previous_transmission_timestamp_microsecond & 0x00FFFFFFFFFFFFFFULL;
        
        // 兼容STM32平台的打印方式（分32位打印）
        USER_DEBUG_NORMAL("[TimeSync] Timestamp: %lu.%06lu s (raw: %lu%06lu us)\r\n",
                        (uint32_t)(timestamp_us / 1000000ULL),
                        (uint32_t)(timestamp_us % 1000000ULL),
                        (uint32_t)(timestamp_us / 1000000ULL),
                        (uint32_t)(timestamp_us % 1000000ULL));
    } else {
        USER_DEBUG_NORMAL("[TimeSync] Error deserializing timestamp (code: %d)\r\n", result);
    }
}
void subscribe_enable(void) 
{
    static CanardRxSubscription sub_enable;
    static CanardRxSubscription sub_setTar;
    static CanardRxSubscription sub_time;
        
    // 使能服务订阅
    sub_enable.user_reference = handle_motor_enable;
    canardRxSubscribe(&canard, CanardTransferKindRequest, 113, 1, CANARD_DEFAULT_TRANSFER_ID_TIMEOUT_USEC, &sub_enable);
    sub_setTar.user_reference = handle_set_targe;
    canardRxSubscribe(&canard, CanardTransferKindRequest, 114, 16, CANARD_DEFAULT_TRANSFER_ID_TIMEOUT_USEC, &sub_setTar);   
    sub_time.user_reference = handle_time;
    canardRxSubscribe(&canard, CanardTransferKindMessage, 7168, 16, CANARD_DEFAULT_TRANSFER_ID_TIMEOUT_USEC, &sub_time);   
}

static void process_transfer(void *ctx,CanardRxTransfer* transfer) 
{
    if ((transfer->metadata.transfer_kind == CanardTransferKindRequest) || transfer->metadata.transfer_kind == CanardTransferKindMessage ) {
        USER_DEBUG_NORMAL("Received request from node %u, port %u, size %u",
                    transfer->metadata.remote_node_id,
                    transfer->metadata.port_id,
                    transfer->payload_size);
        USER_DEBUG_NORMAL("\r\n");
        MessageHandler handler = (MessageHandler)ctx;
        if (handler) {
            handler(transfer);
        } 
    }

}
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
    if ((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) == 0) return;

    uint8_t rx_data[64];
    FDCAN_RxHeaderTypeDef rx_header;
    HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rx_header, rx_data);

    CanardFrame frame = {
        .extended_can_id = rx_header.Identifier,
        .payload_size = rx_header.DataLength,
        .payload = rx_data
    };
    CanardRxTransfer transfer;
    CanardRxSubscription *sub = NULL;
    const int8_t result = canardRxAccept(&canard, HAL_GetTick()*1000,&frame,0,&transfer,&sub);
    /* 正确处理逻辑 */
    if (result == 1) {
        // 完整传输 - 处理并释放内存
        if(sub != NULL)
        {
            process_transfer(sub->user_reference,&transfer);
        }    
        canard.memory_free(&canard, transfer.payload);
    }
}