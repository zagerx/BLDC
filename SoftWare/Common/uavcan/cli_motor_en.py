import sys
import pathlib
sys.path.insert(0, str(pathlib.Path(".dsdl_generated").resolve()))
import asyncio
import pycyphal
from pycyphal.application import make_node, NodeInfo
from pycyphal.transport.can import CANTransport
from pycyphal.transport.can.media.socketcan import SocketCANMedia
from uavcan.node import  Version_1_0
from dinosaurs.actuator.wheel_motor import Enable_1_0
async def client_motorenable_process():
    # ================== 保持原有节点创建代码 ==================
    transport = CANTransport(
        media=SocketCANMedia("can1", mtu=8),  # 经典CAN配置
        local_node_id=100  # 节点ID硬编码（根据需求调整）
    )
    node = make_node(
        transport=transport,
        info=NodeInfo(
            name="test_node",
            software_version=Version_1_0(major=1, minor=0),
            unique_id=bytes.fromhex("DEADBEEFCAFEBABE12345678ABCDEF01")
        )
    )
    client = node.make_client(Enable_1_0, server_node_id=28, port_name=113)
    client.response_timeout = 0.2  # 100ms超时

    # ================== 简化后的控制逻辑 ==================
    try:
        while True:
            try:
                response = await client.call(Enable_1_0.Request(enable_state=1))
                print(f"[响应] 完整结果: {response}")
            except asyncio.TimeoutError:
                print("[超时] 100ms内无应答")
            await asyncio.sleep(2)  # 1000秒间隔
    finally:
        await client.close()
        await node.close()
        await transport.close()

if __name__ == "__main__":
    asyncio.run(client_motorenable_process())
