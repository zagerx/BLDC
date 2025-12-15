#!/usr/bin/env python3
"""
串口速度指令自动下发工具

功能描述:
    本脚本以固定时间间隔（50ms～100ms）通过串口向目标设备自动发送速度指令。
    指令格式为："Valocity_tar:X.X"，其中速度值在-3.0到3.0范围内随机生成。
    脚本持续运行，直到用户手动中断（Ctrl+C）。
    
特点:
    1. 使用固定串口参数，无需外部配置
    2. 指令间隔随机在50ms～100ms之间
    3. 速度值随机变化在-3.0～3.0范围内
    4. 自动检测和报告串口异常
    5. 支持优雅退出（Ctrl+C）

注意事项:
    1. 默认使用/dev/ttyUSB0，波特率115200
    2. 需要安装pyserial库：pip install pyserial
    3. 可能需要串口访问权限：sudo或添加到dialout组

运行方式:
    python3 velocity_auto_sender.py
    按Ctrl+C停止运行
"""

import sys
import os
import time
import random
import serial
import signal

# 固定配置参数
SERIAL_PORT = '/dev/ttyUSB0'      # 串口设备路径
BAUD_RATE = 115200                # 波特率
MIN_INTERVAL = 0.05               # 最小发送间隔(秒) = 50ms
MAX_INTERVAL = 1.50                # 最大发送间隔(秒) = 100ms
MIN_VELOCITY = -3.0               # 最小速度值(m/s)
MAX_VELOCITY = 3.0                # 最大速度值(m/s)
DECIMAL_PLACES = 1                # 速度值小数位数

# 全局变量，用于控制程序运行
running = True

def signal_handler(sig, frame):
    """处理中断信号，用于优雅退出"""
    global running
    print("\n检测到中断信号，正在停止...")
    running = False

def check_serial_config():
    """
    检查串口配置的可用性
    
    Returns:
        tuple: (是否成功, 错误信息或串口对象)
    """
    # 检查串口设备是否存在
    if not os.path.exists(SERIAL_PORT):
        return False, f"错误：串口设备 {SERIAL_PORT} 不存在\n" \
                     "请检查:\n" \
                     "1. 设备是否正确连接\n" \
                     "2. 设备名称是否正确\n" \
                     "3. 使用命令 'ls /dev/tty*' 查看可用串口"
    
    # 检查是否有访问权限
    if not os.access(SERIAL_PORT, os.R_OK | os.W_OK):
        return False, f"错误：没有权限访问串口设备 {SERIAL_PORT}\n" \
                     "请尝试:\n" \
                     "1. 使用sudo运行本脚本\n" \
                     "2. 将用户添加到dialout组: sudo usermod -a -G dialout $USER\n" \
                     "3. 重新登录或重启系统"
    
    return True, "配置检查通过"

def open_serial_connection():
    """
    打开串口连接
    
    Returns:
        tuple: (是否成功, 串口对象或错误信息)
    """
    try:
        # 创建串口对象
        ser = serial.Serial(
            port=SERIAL_PORT,
            baudrate=BAUD_RATE,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=1
        )
        
        # 等待串口稳定
        time.sleep(0.2)
        return True, ser
        
    except serial.SerialException as e:
        error_msg = f"串口打开失败: {e}\n"
        if "Permission denied" in str(e):
            error_msg += "权限被拒绝，请尝试使用sudo运行或检查用户组权限"
        elif "Device or resource busy" in str(e):
            error_msg += "串口设备正忙，可能被其他程序占用"
        elif "No such file or directory" in str(e):
            error_msg += "串口设备不存在，请检查连接"
        else:
            error_msg += "请检查设备连接和串口配置"
        return False, error_msg
    
    except Exception as e:
        return False, f"未知错误: {e}"

def generate_random_velocity():
    """
    生成随机速度值
    
    Returns:
        float: 在MIN_VELOCITY到MAX_VELOCITY范围内的随机速度值
    """
    # 生成指定范围和精度的随机速度值
    velocity = round(random.uniform(MIN_VELOCITY, MAX_VELOCITY), DECIMAL_PLACES)
    return velocity

def send_velocity_command(ser, velocity):
    """
    发送速度指令到串口
    
    Args:
        ser: 串口对象
        velocity: 速度值
    """
    # 构建指令字符串
    command = f"Valocity_tar:{velocity:.{DECIMAL_PLACES}f}"
    
    try:
        # 发送指令，添加换行符作为结束标志
        ser.write((command + '\n').encode('utf-8'))
        ser.flush()  # 确保数据完全发送
        
        # 可选：尝试读取响应（如果设备有响应）
        time.sleep(0.01)  # 短暂等待响应
        if ser.in_waiting > 0:
            response = ser.readline().decode('utf-8', errors='ignore').strip()
            print(f"设备响应: {response}")
        else:
            print(f"已发送: {command}")
            
    except serial.SerialException as e:
        print(f"发送失败: {e}")
        raise  # 重新抛出异常，让上层处理

def main():
    """主函数"""
    global running
    
    print("=" * 50)
    print("串口速度指令自动下发工具")
    print("=" * 50)
    print(f"串口设备: {SERIAL_PORT}")
    print(f"波特率: {BAUD_RATE}")
    print(f"指令间隔: {MIN_INTERVAL*1000:.0f}ms ~ {MAX_INTERVAL*1000:.0f}ms")
    print(f"速度范围: {MIN_VELOCITY} ~ {MAX_VELOCITY} m/s")
    print("按 Ctrl+C 停止运行")
    print("-" * 50)
    
    # 注册信号处理器，用于优雅退出
    signal.signal(signal.SIGINT, signal_handler)
    signal.signal(signal.SIGTERM, signal_handler)
    
    # 检查串口配置
    print("正在检查串口配置...")
    success, result = check_serial_config()
    if not success:
        print(result)
        sys.exit(1)
    
    # 打开串口连接
    print("正在打开串口连接...")
    success, result = open_serial_connection()
    if not success:
        print(result)
        sys.exit(1)
    
    ser = result
    print(f"串口连接成功: {SERIAL_PORT}")
    
    # 开始发送指令
    print("开始发送速度指令...")
    print("-" * 50)
    
    command_count = 0
    
    try:
        while running:
            # 生成随机速度值
            velocity = generate_random_velocity()
            
            # 发送速度指令
            send_velocity_command(ser, velocity)
            
            command_count += 1
            
            # 计算并等待随机间隔
            interval = random.uniform(MIN_INTERVAL, MAX_INTERVAL)
            time.sleep(interval)
            
    except KeyboardInterrupt:
        print("\n用户中断，正在停止...")
    except serial.SerialException as e:
        print(f"串口通信错误: {e}")
    except Exception as e:
        print(f"程序运行错误: {e}")
    finally:
        # 关闭串口连接
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("串口连接已关闭")
        
        print("-" * 50)
        print(f"程序运行结束")
        print(f"总共发送指令: {command_count} 条")
        print("=" * 50)

if __name__ == '__main__':
    main()