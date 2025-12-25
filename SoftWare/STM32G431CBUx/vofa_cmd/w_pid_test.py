#!/usr/bin/env python3
"""
pi_debug.py - 调试参数下发工具

用法:
    python3 pi_debug.py "D_KpKiKd:1.2,23.3,43.5"
    python3 pi_debug.py "D_Kp:2.345"
    python3 pi_debug.py "Q_KpKi:1.5,2.8"
"""

import sys
import time
import argparse

def check_and_open_serial(port='COM15', baudrate=115200):
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
    parser = argparse.ArgumentParser(description='向嵌入式设备发送调试参数')
    parser.add_argument('command', help='命令字符串，格式如: D_KpKiKd:1.2,23.3,43.5')
    parser.add_argument('--port', '-p', default='COM15', help='串口设备路径 (默认: COM15)')
    parser.add_argument('--baudrate', '-b', type=int, default=115200, help='串口波特率 (默认: 115200)')
    
    args = parser.parse_args()
    
    if ':' not in args.command:
        print("错误: 命令格式不正确，应包含冒号分隔符")
        sys.exit(1)
    
    try:
        ser = check_and_open_serial(args.port, args.baudrate)
        print(f"串口已打开: {args.port}, 波特率: {args.baudrate}")
    except Exception as e:
        print(f"错误: {e}")
        sys.exit(1)
    
    try:
        send_command(ser, args.command)
        print("命令发送成功")
    except Exception as e:
        print(f"发送命令时出错: {e}")
        sys.exit(1)
    finally:
        ser.close()
        print("串口已关闭")

if __name__ == '__main__':
    main()




