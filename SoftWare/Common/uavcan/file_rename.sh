#!/bin/bash

# 需要查找 .uavcan 文件的目录
BASE_DIR="./custom_data_types"

# 创建目标目录
DEST_DIR="./custom_data_types_dsdl"
mkdir -p "$DEST_DIR"

# 遍历所有 .uavcan 文件
find "$BASE_DIR" -type f -name "*.uavcan" | while read -r file; do
    # 生成目标文件路径
    relative_path="${file#$BASE_DIR/}"
    dest_file="$DEST_DIR/${relative_path%.uavcan}.dsdl"

    # 创建目标文件夹结构
    mkdir -p "$(dirname "$dest_file")"

    # 复制文件并重命名
    cp "$file" "$dest_file"
    echo "Renamed $file to $dest_file"
done

# 提示完成
echo "All .uavcan files have been copied and renamed to .dsdl in $DEST_DIR"
