#!/usr/bin/env python3
"""
轨迹系统极限压力测试脚本（T 型在线 replanning）

测试内容：
    Case 1: 高速 + 高频更新
    Case 2: 低速微抖动
    Case 3: 反向目标快速切换
"""

import time
import random
import serial
import signal
import os
import sys

# =============================
# 串口配置
# =============================
SERIAL_PORT = "/dev/ttyUSB0"
BAUD_RATE = 115200

# =============================
# 全局控制
# =============================
running = True

def signal_handler(sig, frame):
    global running
    print("\n收到退出信号，停止测试")
    running = False

# =============================
# 串口初始化
# =============================
def open_serial():
    if not os.path.exists(SERIAL_PORT):
        print(f"串口不存在: {SERIAL_PORT}")
        sys.exit(1)

    ser = serial.Serial(
        port=SERIAL_PORT,
        baudrate=BAUD_RATE,
        timeout=0.1
    )
    time.sleep(0.2)
    return ser

def send_velocity(ser, v):
    cmd = f"Valocity_tar:{v:.2f}\n"
    ser.write(cmd.encode())
    ser.flush()
    print(f"[TX] {cmd.strip()}")

# =============================
# Case 1: 高速 + 高频更新
# =============================
def test_high_speed_replanning(ser, duration=5.0):
    print("\n===== Case 1: 高速 + 高频更新 =====")
    start = time.time()

    while running and time.time() - start < duration:
        v = random.uniform(-3.0, 3.0)   # 高速
        send_velocity(ser, v)
        time.sleep(random.uniform(0.02, 0.03))  # 20~30ms

# =============================
# Case 2: 低速微小抖动
# =============================
def test_low_speed_dither(ser, duration=5.0):
    print("\n===== Case 2: 低速微小抖动 =====")
    start = time.time()

    v = 0.05
    while running and time.time() - start < duration:
        v = -v  # 高频换向
        send_velocity(ser, v)
        time.sleep(0.05)

# =============================
# Case 3: 反向目标冲击
# =============================
def test_reverse_direction(ser, duration=5.0):
    print("\n===== Case 3: 反向目标冲击 =====")
    start = time.time()

    while running and time.time() - start < duration:
        send_velocity(ser, 2.5)
        time.sleep(0.15)
        send_velocity(ser, -2.5)
        time.sleep(0.15)

# =============================
# 主流程
# =============================
def main():
    global running

    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)

    ser = open_serial()
    print("串口已打开，开始压力测试")

    try:
        while running:
            test_high_speed_replanning(ser)
            test_low_speed_dither(ser)
            test_reverse_direction(ser)

    finally:
        ser.close()
        print("串口已关闭，测试结束")

if __name__ == "__main__":
    main()
