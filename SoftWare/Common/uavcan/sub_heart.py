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

async def sub_heart_process() -> None:
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
    
    # 创建订阅者
    sub = node.make_subscriber(Heartbeat_1_0,7509)
    print("正在监听CAN总线上的心跳包... (Ctrl+C退出)")

    try:
        while True:
            # 接收消息（带超时）
            result = await sub.receive(monotonic_deadline=asyncio.get_event_loop().time() + 1.0)
            if result is not None:
                msg, transfer = result
                print(f"\n心跳包来自节点 {transfer.source_node_id}:")
                print(f"- 运行状态: {msg.mode}")
                print(f"- 健康状况: {msg.health}")
                print(f"- Uptime: {msg.uptime}秒")
                # optimized_print(msg)
    except KeyboardInterrupt:
        pass
    finally:
        # 显式关闭所有资源（关键！）
        sub.close()  # 先关闭订阅者
        node.close()  # 同步关闭节点
        transport.close()
        media.close()

if __name__ == "__main__":
    asyncio.run(sub_heart_process())



