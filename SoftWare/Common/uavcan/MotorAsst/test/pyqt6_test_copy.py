import sys  
from PyQt6.QtWidgets import QApplication, QWidget  

# 创建应用实例  
app = QApplication(sys.argv)  
# 创建窗口对象  
window = QWidget()  
window.setWindowTitle("第一个PyQt6窗口")  # 设置窗口标题‌:ml-citation{ref="1" data="citationList"}  
window.resize(400, 300)  # 设置窗口大小‌:ml-citation{ref="1,3" data="citationList"}  
window.show()  # 显示窗口  
sys.exit(app.exec())  # 启动事件循环