# main.py
# ------------------------------------------------------------
# 电压调制测试主入口
#
# 职责说明：
# 1. 作为测试流程的“调度器”，不参与任何数学计算
# 2. 通过 load_csv_data 加载离线测试数据（CSV）
# 3. 将数据分发给各独立功能模块（viewer / analyzer）
#
# 设计原则：
# - main.py 不解析 CSV 细节
# - main.py 不关心 alpha/beta 的物理含义
# - main.py 不做正确性判断，只负责流程是否继续
#
# 边界条件：
# - CSV 文件必须存在，且列顺序符合约定
# - 下游模块内部异常将直接抛出，main.py 不吞异常
# ------------------------------------------------------------

from load_data import load_csv_data
from alpha_beta_viewer import AlphaBetaViewer


def main():
    print("开始电压调制测试流程...")

    # 1. 加载 CSV 数据（唯一入口）
    data = load_csv_data("voltage_modulation_log.csv")

    # 2. 启动 αβ 平面可视化（纯观察，不做验证）
    viewer = AlphaBetaViewer(
        alpha=data["alpha"],
        beta=data["beta"],
    )
    viewer.run()   # 进入 Qt 事件循环，窗口关闭后自动返回

    # 3. 后续流程（当前可为空）
    print("所有电压调制测试通过！")


if __name__ == "__main__":
    main()
