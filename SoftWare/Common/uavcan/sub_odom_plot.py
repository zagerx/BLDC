import pandas as pd
import matplotlib.pyplot as plt

def plot_odometry():
    df = pd.read_csv("odom_data.csv").iloc[5:]  # 跳过前5行
    df["timestamp(ms)"] = pd.to_numeric(df["timestamp(ms)"])  # 强制类型转换
    
    # 提取初始时间戳
    initial_time = df["timestamp(ms)"].iloc[0]
    df["time_relative"] = df["timestamp(ms)"] - initial_time
    
    # 生成 left_diff 和 right_diff 列（原始 odom 的差值）
    df["left_diff"] = df["left_odom"].diff()
    df["right_diff"] = df["right_odom"].diff()

    # 可视化配置
    plt.figure(figsize=(12, 14))  # 增加高度
    
    # 左轮原始数据
    plt.subplot(4, 1, 1)
    plt.plot(df["time_relative"], df["left_odom"], 
            label="Left Wheel", color="#1f77b4", linewidth=1.5)
    plt.ylabel("Distance (m)", fontsize=10)
    plt.title("Left Wheel Odometry Analysis", fontsize=12)
    plt.grid(alpha=0.3)
    
    # 右轮原始数据
    plt.subplot(4, 1, 2)
    plt.plot(df["time_relative"], df["right_odom"], 
            label="Right Wheel", color="#ff7f0e", linewidth=1.5)
    plt.ylabel("Distance (m)", fontsize=10)
    plt.title("Right Wheel Odometry Analysis", fontsize=12)
    plt.grid(alpha=0.3)
    
    # 左轮差值分析
    plt.subplot(4, 1, 3)
    plt.plot(df["time_relative"], df["left_diff"], 
            label="Step Difference (Δleft)", color="#2ca02c", 
            linewidth=1.5, linestyle="--")
    plt.scatter(df["time_relative"], df["left_diff"], 
                s=15, color="#2ca02c", alpha=0.8)
    plt.axhline(0, color="gray", linestyle=":", alpha=0.5)
    plt.xlabel("Relative Time (ms)", fontsize=10)
    plt.ylabel("Difference (m)", fontsize=10)
    plt.grid(alpha=0.3)
    plt.title("Left Wheel Step Difference Analysis", fontsize=12)

    # 右轮差值分析
    plt.subplot(4, 1, 4)
    plt.plot(df["time_relative"], df["right_diff"], 
            label="Step Difference (Δright)", color="#d62728", 
            linewidth=1.5, linestyle="--")
    plt.scatter(df["time_relative"], df["right_diff"], 
                s=15, color="#d62728", alpha=0.8)
    plt.axhline(0, color="gray", linestyle=":", alpha=0.5)
    plt.xlabel("Relative Time (ms)", fontsize=10)
    plt.ylabel("Difference (m)", fontsize=10)
    plt.grid(alpha=0.3)
    plt.title("Right Wheel Step Difference Analysis", fontsize=12)

    plt.tight_layout(pad=2.0)
    plt.savefig("odometry_step_diff.png", dpi=300, bbox_inches="tight")
    plt.close()

if __name__ == "__main__":
    plot_odometry()