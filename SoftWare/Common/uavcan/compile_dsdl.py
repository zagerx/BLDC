#!/usr/bin/env python3
import sys
import pathlib
import pycyphal.dsdl

def compile_dsdl() -> None:
    # 定义路径
    # user_dsdl_repo = pathlib.Path("custom_data_types/dinosaurs/actuator/").resolve()  # 用户DSDL仓库
    user_dsdl_repo = pathlib.Path("custom_data_types").resolve()  # 用户DSDL仓库
    dsdl_repo = pathlib.Path("public_regulated_data_types").resolve()  # 官方DSDL仓库
    output_dir = pathlib.Path(".dsdl_generated")  # 编译输出目录
    
    # 检查是否已编译
    if output_dir.exists():
        print(f"[SKIP] DSDL已编译到目录: {output_dir}")
        return
    
    # 创建输出目录
    output_dir.mkdir(parents=True, exist_ok=True)
    
    # 编译官方DSDL接口
    print(f"正在编译DSDL从 {dsdl_repo} 到 {output_dir}...")
    pycyphal.dsdl.compile_all(
        [
            dsdl_repo / "uavcan",
            dsdl_repo / "reg",
            user_dsdl_repo / "dinosaurs",
        ],
        output_directory=output_dir,
    )
    print(f"[SUCCESS] DSDL编译完成! 输出目录: {output_dir}")

if __name__ == "__main__":
    compile_dsdl()
