#!/usr/bin/env python3
"""
订阅CAN总线上的节点心跳包（uavcan.node.Heartbeat）
需要先运行 compile_dsdl.py 生成DSDL定义
"""

import sys
import pathlib
sys.path.insert(0, str(pathlib.Path(".dsdl_generated").resolve()))
import pycyphal
from pycyphal.application import make_node, NodeInfo
from pycyphal.transport.can import CANTransport
from pycyphal.transport.can.media.socketcan import SocketCANMedia
from uavcan.node import Heartbeat_1_0, Version_1_0
from dinosaurs.actuator.wheel_motor import OdometryAndVelocityPublish_1_0
# 确保DSDL生成路径正确
import asyncio

def optimized_print(msg, transfer):
    """结构化数据提取打印"""
    # 时间戳数值提取
    timestamp = f"({msg.timestamp.microsecond})"
    # 速度值格式化（处理列表结构）
    speed_str = ' '.join(
        f"(meter_per_second={v.meter_per_second})" 
        for v in msg.current_velocity
    )
    # 里程计数值格式化 
    odom_str = ' '.join(
        f"(meter={l.meter})" 
        for l in msg.odometry
    )
    # 结构化输出
    print(f"\n心跳包来自节点 {transfer.source_node_id}:")
    print(f"- 时间戳: {timestamp}")
    print(f"- 速度: [{speed_str}]")
    print(f"- 里程计: [{odom_str}]")
def ori_print(msg, transfer):
    print(f"\n心跳包来自节点 {transfer.source_node_id}:")
    print(f"- 时间戳: {msg.timestamp}")
    print(f"- 速 度: {msg.current_velocity}")
    print(f"- 里程计: {msg.odometry}秒")

async def sub_odom_process() -> None:
    # 初始化 CAN 接口和传输层
    media = SocketCANMedia("can1", mtu=8)
    transport = CANTransport(media, local_node_id=28)
    # 创建节点（显式启动）
    node = make_node(
        transport=transport,
        info=NodeInfo(
            name="test_node",
            software_version=Version_1_0(major=1, minor=0),
            unique_id=bytes.fromhex("DEADBEEFCAFEBABE12345678ABCDEF01")
        )
    )
    node.start()  # 必须显式启动
    async def message_handler(msg: OdometryAndVelocityPublish_1_0, transfer: pycyphal.transport.TransferFrom) -> None:
        optimized_print(msg, transfer)
    # 创建订阅者
    sub = node.make_subscriber(OdometryAndVelocityPublish_1_0,1100)
    sub.receive_in_background(message_handler)
    print("正在监听CAN总线上的心跳包... (Ctrl+C退出)")
    try:
        while True:
            # 接收消息（带超时）
            result = await sub.receive(monotonic_deadline=asyncio.get_event_loop().time() + 1.0)
            if result is not None:
                msg, transfer = result
    except KeyboardInterrupt:
        pass
    finally:
        # 显式关闭所有资源（关键！）
        sub.close()  # 先关闭订阅者
        node.close()  # 同步关闭节点
        transport.close()
        media.close()

if __name__ == "__main__":
    asyncio.run(sub_odom_process())