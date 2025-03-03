import sys  
from PyQt6.QtWidgets import QApplication, QWidget  
from PyQt6.QtWidgets import QLabel, QPushButton, QVBoxLayout  
class MainWindow(QWidget):  
    def __init__(self):  
        super().__init__()  
        self.setWindowTitle("带控件的窗口")  
        layout = QVBoxLayout()  # 垂直布局管理器‌:ml-citation{ref="5" data="citationList"}  

        self.label = QLabel("欢迎使用PyQt6!")  
        self.button = QPushButton("点击我")  
        layout.addWidget(self.label)  
        layout.addWidget(self.button)  

        self.setLayout(layout)  # 应用布局‌:ml-citation{ref="4" data="citationList"}  

app = QApplication(sys.argv)  
window = MainWindow()  
window.show()  
app.exec()  
