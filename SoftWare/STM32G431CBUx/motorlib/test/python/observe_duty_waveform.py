import os
import matplotlib.pyplot as plt


def observe_duty_waveform(
    data,
    output_dir="output",
    filename="duty_waveform.png",
    step_threshold=0.2,
):
    """
    Duty 波形观测模块（非验证）

    职责：
    1. 绘制 duty_a / duty_b / duty_c
    2. 标注可能的跳变点（不判错）
    3. 统计钳位行为
    4. 保存为图片文件

    说明：
    - 所有“规则”仅用于观测提示
    - 不 raise RuntimeError
    """

    da = data.get("duty_a", [])
    db = data.get("duty_b", [])
    dc = data.get("duty_c", [])

    length = min(len(da), len(db), len(dc))
    indices = list(range(length))

    # ----------- 连续性观测（不判错）-----------
    jump_indices = []

    for i in range(1, length):
        if (
            abs(da[i] - da[i - 1]) > step_threshold or
            abs(db[i] - db[i - 1]) > step_threshold or
            abs(dc[i] - dc[i - 1]) > step_threshold
        ):
            jump_indices.append(i)

    if jump_indices:
        print(
            f"[Duty] Notice: {len(jump_indices)} large steps detected "
            f"(e.g. index {jump_indices[0]})"
        )

    # ----------- 钳位统计（观测信息）-----------
    clamp_count = 0
    eps = 1e-6

    for i in range(length):
        clamps = 0
        for d in (da[i], db[i], dc[i]):
            if abs(d - 0.0) < eps or abs(d - 1.0) < eps:
                clamps += 1
        if clamps > 1:
            clamp_count += 1

    if clamp_count:
        print(f"[Duty] Notice: {clamp_count} samples with multi-phase clamp")

    # ----------- 绘图 -----------
    os.makedirs(output_dir, exist_ok=True)
    output_path = os.path.join(output_dir, filename)

    plt.figure(figsize=(10, 5))
    plt.title("SVPWM Duty Waveforms")

    plt.plot(indices, da[:length], label="Duty A")
    plt.plot(indices, db[:length], label="Duty B")
    plt.plot(indices, dc[:length], label="Duty C")

    # 跳变点标注
    for idx in jump_indices:
        plt.axvline(idx, color="red", alpha=0.1)

    plt.xlabel("Sample Index")
    plt.ylabel("Duty")
    plt.ylim(-0.05, 1.05)
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(output_path, dpi=150)
    plt.close()

    print(f"[Duty] Plot saved to: {output_path}")
