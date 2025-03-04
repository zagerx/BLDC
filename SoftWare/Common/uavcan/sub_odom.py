import sys
import pathlib
sys.path.append(str(pathlib.Path(".dsdl_generated").resolve()))
import asyncio
import pycyphal
import time
from pycyphal.application import make_node, NodeInfo
from pycyphal.transport.can import CANTransport
from pycyphal.transport.can.media.socketcan import SocketCANMedia
from uavcan.node import Version_1_0
from dinosaurs.actuator.wheel_motor import OdometryAndVelocityPublish_1_0

# 性能优化参数
BUFFER_SIZE = 100  # 缓存100条数据后批量写入
WRITE_TIMEOUT = 1.0  # 最大写入间隔(秒)

class DataLogger:
    def __init__(self):
        self.buffer = []
        self.start_time = time.monotonic()
        self.last_write = time.monotonic()
        self.prev_odometry = None  # 存储前一时刻的里程计数据

    def log(self, msg, transfer):
        ts = int((time.monotonic() - self.start_time) * 1000)

        # 提取数据
        l_vel = msg.current_velocity[0].meter_per_second  # 左轮速度
        r_vel = msg.current_velocity[1].meter_per_second  # 右轮速度
        l_odom = msg.odometry[0].meter                    # 左轮里程计
        r_odom = msg.odometry[1].meter                    # 右轮里程计

        # 保存当前数据
        current_odom = {
            "ts": ts,
            "l_odom": l_odom,
            "r_odom": r_odom
        }

        # 如果有前一时刻的数据，进行增量判断
        if self.prev_odometry is not None:
            # 计算增量
            delta_l = l_odom - self.prev_odometry["l_odom"]
            delta_r = r_odom - self.prev_odometry["r_odom"]

            # 判断增量是否超过阈值
            threshold = 0.05
            if abs(delta_l) > threshold or abs(delta_r) > threshold:
                # 打印信息
                print(f"Time: {ts}ms")
                print(f"Left Odom: current={l_odom:.3f}, previous={self.prev_odometry['l_odom']:.3f}, delta={delta_l:.3f}")
                print(f"Right Odom: current={r_odom:.3f}, previous={self.prev_odometry['r_odom']:.3f}, delta={delta_r:.3f}")
                print("-"*50)

        # 更新前一时刻的数据
        self.prev_odometry = current_odom

        # 避免前5次数据记录前一时刻
        if len(self.buffer) >= 5:
            self.prev_odometry = current_odom

        # 准备写入CSV的数据
        self.buffer.append(f"{ts},{l_vel:.3f},{r_vel:.3f},{l_odom:.3f},{r_odom:.3f}\n")
        
        # 数据缓冲区逻辑
        if len(self.buffer) >= BUFFER_SIZE or (time.monotonic() - self.last_write) > WRITE_TIMEOUT:
            self.flush()

    def flush(self):
        with open("odom_data.csv", "a", buffering=8192, encoding='utf-8') as f:
            f.writelines(self.buffer)
        self.buffer.clear()
        self.last_write = time.monotonic()  

async def sub_odom_process():
    logger = DataLogger()
    media = SocketCANMedia("can1", mtu=8)
    transport = CANTransport(media, local_node_id=28)
    
    node = make_node(
        transport=transport,
        info=NodeInfo(
            name="test_node",
            software_version=Version_1_0(major=1, minor=0),
            unique_id=bytes.fromhex("DEADBEEFCAFEBABE12345678ABCDEF01")
        )
    )
    node.start()

    async def handler(msg, transfer):
        logger.log(msg, transfer)  

    sub = node.make_subscriber(OdometryAndVelocityPublish_1_0, 1100)
    sub.receive_in_background(handler)
    
    try:
        while True:
            await asyncio.sleep(WRITE_TIMEOUT)
            logger.flush()  
    finally:
        logger.flush()
        sub.close()
        node.close()
        transport.close()

if __name__ == "__main__":
    # 初始化CSV文件头
    with open("odom_data.csv", "w", encoding='utf-8') as f:
        f.write("timestamp(ms),left_vel,right_vel,left_odom,right_odom\n")
    asyncio.run(sub_odom_process())