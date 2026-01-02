# alpha_beta_viewer.py
# ------------------------------------------------------------
# αβ 平面可视化模块（PyQt6 + PyQtGraph）
#
# 职责：
# - 仅负责“显示”
# - 不读取 CSV
# - 不做任何数学验证、不做归一化
#
# ------------------------------------------------------------

import sys
import math
from PyQt6.QtWidgets import QApplication
import pyqtgraph as pg


class AlphaBetaViewer:
    def __init__(self, alpha, beta):
        self.alpha = alpha
        self.beta = beta

        # -------- QApplication 兜底创建（关键） --------
        self._app = QApplication.instance()
        if self._app is None:
            self._app = QApplication(sys.argv)

        # -------- 窗口与画布 --------
        self.win = pg.GraphicsLayoutWidget(
            title="Alpha-Beta Plane (SVPWM Modulation Space)"
        )
        self.win.resize(800, 800)

        self.plot = self.win.addPlot()
        self.plot.setAspectLocked(True)
        self.plot.showGrid(x=True, y=True)
        self.plot.setLabel("left", "Beta")
        self.plot.setLabel("bottom", "Alpha")

        # 原点
        self.plot.addItem(
            pg.ScatterPlotItem([0], [0], size=8, brush="r")
        )

        # αβ 轨迹
        self.plot.plot(
            self.alpha,
            self.beta,
            pen=pg.mkPen("y", width=1),
            symbol="o",
            symbolSize=3,
            symbolBrush="y",
            name="αβ trajectory",
        )

        # 归一化 SVPWM 最大六边形
        self._draw_hexagon()

    def _draw_hexagon(self):
        """
        绘制 SVPWM 归一化线性区最大六边形
        契约：
        - α 轴对齐 A 相
        - 顶点必须包含 (1, 0)
        """
        r = 1.0
        pts_x = []
        pts_y = []

        for k in range(6):
            angle = k * math.pi / 3.0  # 0°, 60°, 120° ...
            pts_x.append(r * math.cos(angle))
            pts_y.append(r * math.sin(angle))

        # 闭合
        pts_x.append(pts_x[0])
        pts_y.append(pts_y[0])

        self.plot.plot(
            pts_x,
            pts_y,
            pen=pg.mkPen("c", width=2),
            name="SVPWM linear hexagon",
        )

    def run(self):
        """
        进入 Qt 事件循环
        窗口关闭后自动返回 main.py
        """
        self.win.show()
        self._app.exec()
