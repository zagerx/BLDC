import asyncio

from sub_heart import sub_heart_process
from sub_odom import sub_odom_process

async def main():
    # 创建并发任务组
    heart_task = asyncio.create_task(sub_heart_process())
    odom_task = asyncio.create_task(sub_odom_process())
    
    # 等待所有任务完成（或异常）
    await asyncio.gather(heart_task, odom_task)

if __name__ == "__main__":
    asyncio.run(main())  # 单次事件循环启动

