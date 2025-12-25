#!/usr/bin/env python3
"""
串口位置指令自动下发工具 - 双位置交替模式

功能描述:
    本脚本以固定时间间隔（1秒）通过串口向目标设备自动发送位置指令。
    指令格式为："Pos_tar:X.XXX"，其中位置值仅在0.0和0.4之间交替变化。
    脚本持续运行，直到用户手动中断（Ctrl+C）。

特点:
    1. 支持Windows、Linux、macOS多个平台
    2. 固定1秒发送间隔
    3. 位置值仅在0.0和0.4两个值之间切换
    4. 自动检测和报告串口异常
    5. 支持优雅退出（Ctrl+C）

注意事项:
    1. 根据实际情况修改串口设备名称
    2. 需要安装pyserial库：pip install pyserial
    3. 在Windows上可能需要管理员权限访问某些COM口

运行方式:
    python position_auto_sender.py
    按Ctrl+C停止运行
"""

import sys
import os
import time
import serial
import signal
import platform
import subprocess

# 根据操作系统选择默认串口
SYSTEM = platform.system()
if SYSTEM == "Windows":
    SERIAL_PORT = 'COM15'          # Windows默认串口设备
elif SYSTEM == "Linux":
    SERIAL_PORT = '/dev/ttyUSB0'  # Linux默认串口设备
elif SYSTEM == "Darwin":  # macOS
    SERIAL_PORT = '/dev/tty.usbserial'  # macOS默认串口设备
else:
    SERIAL_PORT = '/dev/ttyUSB0'  # 其他系统默认

BAUD_RATE = 115200                # 波特率
INTERVAL = 2.0                    # 固定发送间隔(秒) = 1s
DECIMAL_PLACES = 3                # 位置值小数位数

# 发送状态标志
send_zero = True                  # True发送0.0，False发送0.4

# 全局变量，用于控制程序运行
running = True

def signal_handler(sig, frame):
    """处理中断信号，用于优雅退出"""
    global running
    print("\n检测到中断信号，正在停止...")
    running = False

def list_serial_ports():
    """列出可用的串口设备"""
    ports = []
    
    if SYSTEM == "Windows":
        # Windows: 扫描COM1到COM20
        for i in range(1, 21):
            port = f"COM{i}"
            try:
                ser = serial.Serial(port)
                ser.close()
                ports.append(port)
                print(f"发现可用串口: {port}")
            except (OSError, serial.SerialException):
                pass
    elif SYSTEM == "Linux":
        # Linux: 扫描常见的串口设备
        common_ports = ['/dev/ttyUSB0', '/dev/ttyUSB1', '/dev/ttyACM0', '/dev/ttyACM1', 
                       '/dev/ttyS0', '/dev/ttyS1', '/dev/ttyS2', '/dev/ttyS3']
        for port in common_ports:
            if os.path.exists(port):
                ports.append(port)
                print(f"发现可用串口: {port}")
    elif SYSTEM == "Darwin":  # macOS
        # macOS: 扫描常见的串口设备
        common_ports = ['/dev/tty.usbserial', '/dev/tty.usbmodem', 
                       '/dev/tty.usbserial-*', '/dev/cu.*']
        try:
            # 使用ls命令列出串口设备
            result = subprocess.run(['ls', '/dev'], capture_output=True, text=True)
            for line in result.stdout.split('\n'):
                if 'tty.usb' in line or 'cu.' in line:
                    port = f"/dev/{line}"
                    ports.append(port)
                    print(f"发现可用串口: {port}")
        except:
            pass
    
    return ports

def check_serial_config():
    """
    检查串口配置的可用性
    
    Returns:
        tuple: (是否成功, 错误信息或串口对象)
    """
    global SERIAL_PORT
    
    # 如果默认串口不可用，列出可用串口
    ports = list_serial_ports()
    
    if not ports:
        error_msg = "错误：未发现可用的串口设备\n"
        if SYSTEM == "Windows":
            error_msg += "请检查:\n"
            error_msg += "1. 设备是否通过USB正确连接\n"
            error_msg += "2. 设备管理器中的COM端口号\n"
            error_msg += "3. 可能需要安装USB转串口驱动程序"
        elif SYSTEM == "Linux":
            error_msg += "请检查:\n"
            error_msg += "1. 设备是否正确连接\n"
            error_msg += "2. 使用命令 'ls /dev/tty*' 查看可用串口\n"
            error_msg += "3. 可能需要将用户添加到dialout组: sudo usermod -a -G dialout $USER"
        else:  # macOS
            error_msg += "请检查:\n"
            error_msg += "1. 设备是否正确连接\n"
            error_msg += "2. 使用命令 'ls /dev/tty.*' 查看可用串口\n"
            error_msg += "3. 可能需要安装USB转串口驱动程序"
        return False, error_msg
    
    # 如果默认串口不在可用列表中，使用第一个可用的串口
    if SERIAL_PORT not in ports:
        print(f"警告：默认串口 {SERIAL_PORT} 不可用")
        SERIAL_PORT = ports[0]
        print(f"已自动切换到: {SERIAL_PORT}")
    
    # Windows和macOS跳过文件权限检查
    if SYSTEM == "Linux":
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
        if "Permission denied" in str(e) and SYSTEM == "Linux":
            error_msg += "权限被拒绝，请尝试使用sudo运行或检查用户组权限"
        elif "Device or resource busy" in str(e):
            error_msg += "串口设备正忙，可能被其他程序占用"
        elif "No such file or directory" in str(e):
            error_msg += "串口设备不存在，请检查连接"
        elif "could not open port" in str(e) and SYSTEM == "Windows":
            error_msg += "无法打开串口，请检查:\n"
            error_msg += "1. COM端口号是否正确\n"
            error_msg += "2. 设备是否被其他程序占用\n"
            error_msg += "3. 可能需要以管理员身份运行"
        else:
            error_msg += "请检查设备连接和串口配置"
        return False, error_msg
    
    except Exception as e:
        return False, f"未知错误: {e}"

def send_position_command(ser):
    """
    发送位置指令到串口（交替发送0.0和0.4）
    
    Args:
        ser: 串口对象
    """
    global send_zero
    
    # 确定本次发送的值
    if send_zero:
        position = -0.3
        value_str = "-0.300"
    else:
        position = 0.4
        value_str = "0.400"
    
    # 构建指令字符串
    command = f"Pos_tar:{value_str}"
    
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
        
        # 切换下一次发送的值
        send_zero = not send_zero
            
    except serial.SerialException as e:
        print(f"发送失败: {e}")
        raise  # 重新抛出异常，让上层处理

def main():
    """主函数"""
    global running
    
    print("=" * 60)
    print("串口位置指令自动下发工具 - 双位置交替模式")
    print("=" * 60)
    print(f"操作系统: {SYSTEM}")
    print(f"串口设备: {SERIAL_PORT}")
    print(f"波特率: {BAUD_RATE}")
    print(f"发送间隔: {INTERVAL} 秒")
    print("位置值: 0.0 和 0.4 交替发送")
    print("指令格式: Pos_tar:X.XXX")
    print("按 Ctrl+C 停止运行")
    print("-" * 60)
    
    # 注册信号处理器，用于优雅退出
    if SYSTEM != "Windows":
        # Windows的signal模块对SIGINT支持有限
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
    print("开始交替发送位置指令 (0.0 和 0.4)...")
    print("-" * 60)
    
    command_count = 0
    
    try:
        while running:
            # 发送位置指令
            send_position_command(ser)
            
            command_count += 1
            
            # 固定间隔等待
            time.sleep(INTERVAL)
            
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
        
        print("-" * 60)
        print(f"程序运行结束")
        print(f"总共发送指令: {command_count} 条")
        print(f"最后发送的值: {'0.0' if send_zero else '0.4'}")  # 注意: send_zero在切换后表示下一个要发的值
        print("=" * 60)

if __name__ == '__main__':
    main()