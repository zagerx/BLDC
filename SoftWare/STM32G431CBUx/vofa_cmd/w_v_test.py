#!/usr/bin/env python3
"""
vol_test.py - 正弦速度指令测试工具

用法:
    python3 vol_test.py

生成正弦速度指令，间隔20ms下发，幅值±30，完成一个周期后停止
"""

import sys
import time
import math
import argparse

def check_and_open_serial(port='COM16', baudrate=115200):
    """
    检查并打开串口
    """
    try:
        import serial
    except ImportError:
        raise Exception("错误: 请先安装pyserial库: pip install pyserial")
    
    try:
        ser = serial.Serial(
            port=port,
            baudrate=baudrate,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=1
        )
        
        time.sleep(0.1)
        return ser
        
    except serial.SerialException as e:
        raise Exception(f"无法打开串口 {port}: {e}\n请检查串口是否正确或设备是否连接")

def send_command(ser, command):
    """
    通过串口发送命令
    """
    data_to_send = command + '\n'
    ser.write(data_to_send.encode('utf-8'))
    print(f"已发送: {command}")
    
    time.sleep(0.1)
    if ser.in_waiting:
        response = ser.readline().decode('utf-8').strip()
        print(f"设备响应: {response}")

def main():
    parser = argparse.ArgumentParser(description='发送正弦速度指令测试')
    parser.add_argument('--port', '-p', default='COM16', help='串口设备路径 (默认: COM16)')
    parser.add_argument('--baudrate', '-b', type=int, default=115200, help='串口波特率 (默认: 115200)')
    parser.add_argument('--interval', '-i', type=float, default=0.02, help='发送间隔时间(s) (默认: 0.02s=20ms)')
    
    args = parser.parse_args()
    
    try:
        ser = check_and_open_serial(args.port, args.baudrate)
        print(f"串口已打开: {args.port}, 波特率: {args.baudrate}")
        print("开始发送正弦速度指令...")
    except Exception as e:
        print(f"错误: {e}")
        sys.exit(1)
    
    try:
        # 正弦波参数
        amplitude = 30.0  # 幅值 ±30
        points_per_cycle = 60  # 一个周期内的点数
        interval = args.interval  # 发送间隔时间
        
        # 计算总时间：一个完整周期
        total_time = points_per_cycle * interval
        
        print(f"正弦波参数:")
        print(f"  幅值: ±{amplitude}")
        print(f"  周期: {total_time:.2f}s ({points_per_cycle}个点, 间隔{interval*1000:.0f}ms)")
        print(f"  频率: {1/total_time:.2f}Hz")
        
        start_time = time.time()
        
        # 发送一个完整的正弦周期
        for i in range(points_per_cycle + 1):
            # 计算当前相位 (0到2π)
            phase = 2 * math.pi * i / points_per_cycle
            
            # 计算正弦值
            value = amplitude * math.sin(phase)
            
            # 构造命令
            command = f"Valocity_tar:{value:.2f}"
            
            # 发送命令
            send_command(ser, command)
            
            # 计算应该等待的时间
            elapsed_time = time.time() - start_time
            expected_time = i * interval
            
            # 确保精确的间隔时间
            if i < points_per_cycle:
                sleep_time = expected_time - elapsed_time + interval
                if sleep_time > 0:
                    time.sleep(sleep_time)
        
        print(f"完成一个周期，共发送 {points_per_cycle} 个点")
        print(f"实际用时: {time.time() - start_time:.2f}s")
        
    except KeyboardInterrupt:
        print("\n用户中断")
    except Exception as e:
        print(f"发送命令时出错: {e}")
        sys.exit(1)
    finally:
        ser.close()
        print("串口已关闭")

if __name__ == '__main__':
    main()