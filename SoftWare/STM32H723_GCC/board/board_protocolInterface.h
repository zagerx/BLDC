/**
 * @file board_protocolInterface.h
 * @brief UAVCAN 协议接口相关函数声明
 * @defgroup board_protocolInterface UAVCAN 协议接口
 * @{
 */

#ifndef __PRO_INTER__H
#define __PRO_INTER__H
#include "canard.h"
#include "main.h"
 /**
  * @brief 初始化从机通信
  * 
  * 该函数用于初始化 UAVCAN 通信相关的数据结构和参数，包括 CANard 实例、发送队列等。
  */
 void slave_comm_init();
 
 /**
  * @brief 订阅启用消息
  * 
  * 该函数用于订阅特定主题 ID 的启用消息，以便接收来自其他节点的启用请求。
  */
 void subscribe_enable(void);
 
 /**
  * @brief 发送响应消息
  * 
  * @param remote_node_id 目标节点 ID
  * @param payload 消息负载数据
  * @param payload_size 负载数据大小
  * 
  * 该函数用于将响应消息推入发送队列，以便通过 UAVCAN 协议发送给指定的目标节点。
  */
 void send_response(CanardNodeID remote_node_id, const void* payload, size_t payload_size);
 
 /**
  * @brief 处理接收到的传输
  * 
  * @param index 接收到的传输索引
  * @param transfer 指向接收到的传输数据结构的指针
  * 
  * 该函数用于处理接收到的 UAVCAN 传输数据，包括反序列化请求、处理请求以及发送响应。
  */
 void process_received_transfer(const uint8_t index, CanardRxTransfer* const transfer);
 
 /**
  * @brief 发送帧
  * 
  * @param hfdcan 指向 FDCAN 外设句柄的指针
  * 
  * 该函数用于将发送队列中的帧通过 FDCAN 外设发送出去。
  */
 void send_frames(FDCAN_HandleTypeDef *hfdcan);
 
 /**
  * @brief 发送心跳包
  * 
  * 该函数用于创建并发送 UAVCAN 心跳包，以向网络中的其他节点报告本机的状态。
  */
 void send_heartpack();
 
 /** @} */ // end of board_protocolInterface
 
 #endif