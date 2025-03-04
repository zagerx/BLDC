import os
import subprocess

output_dir = ".c_dsdl_generated"
std_dsdl_root = "public_regulated_data_types"

os.makedirs(output_dir, exist_ok=True)

for root, dirs, files in os.walk(std_dsdl_root):
    for file in files:
        # if file.endswith(".dsdl") and "." in file.split(".")[-2]:  # 校验文件名格式
        dsdl_path = os.path.join(root, file)
        # 提取完整命名空间（如 reg.udral.service.common）
        namespace = os.path.relpath(root, std_dsdl_root).replace(os.sep, ".")
        cmd = f"nnvg --target-language c \
                --outdir {output_dir} \
                --lookup-dir {std_dsdl_root} \
                {namespace} {dsdl_path}"  # 严格匹配命名空间层级‌:ml-citation{ref="1,2" data="citationList"}
        subprocess.run(cmd, shell=True, check=True)