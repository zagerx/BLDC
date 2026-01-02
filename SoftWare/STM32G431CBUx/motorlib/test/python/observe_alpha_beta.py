# alpha_beta_viewer.py
from PyQt6 import QtWidgets
import pyqtgraph as pg
import math

class AlphaBetaViewer:
    """
    αβ 平面可视化工具
    - 仅负责创建窗口和绘图
    - 不控制 QApplication 生命周期
    """

    def __init__(self, data):
        self.alpha = data["alpha"]
        self.beta = data["beta"]

        if len(self.alpha) != len(self.beta):
            raise RuntimeError("alpha / beta 长度不一致")

        for i in range(len(self.alpha)):
            if math.isnan(self.alpha[i]) or math.isnan(self.beta[i]):
                raise RuntimeError(f"NaN at {i}")
            if math.isinf(self.alpha[i]) or math.isinf(self.beta[i]):
                raise RuntimeError(f"Inf at {i}")

        self.win = pg.GraphicsLayoutWidget(title="Alpha-Beta Viewer")
        self.plot = self.win.addPlot()
        self.plot.setAspectLocked(True)
        self.plot.showGrid(x=True, y=True)
        self.plot.plot(self.alpha, self.beta, pen='b')

    def show(self):
        self.win.show()
