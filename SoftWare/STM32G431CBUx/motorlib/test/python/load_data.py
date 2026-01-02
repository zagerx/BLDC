import csv


def load_csv_data(file_path):
    """
    固定接口：
    输入: CSV 文件路径
    输出: dict，字段名固定

    CSV 列顺序约定：
    vbus, eangle, ud, uq, ualpha, ubeta, duty_a, duty_b, duty_c
    """
    data = {
        "vbus": [],
        "eangle": [],
        "ud": [],
        "uq": [],
        "alpha": [],
        "beta": [],
        "duty_a": [],
        "duty_b": [],
        "duty_c": [],
    }

    with open(file_path, newline="") as f:
        reader = csv.reader(f)
        for row in reader:
            data["vbus"].append(float(row[0]))
            data["eangle"].append(float(row[1]))
            data["ud"].append(float(row[2]))
            data["uq"].append(float(row[3]))
            data["alpha"].append(float(row[4]))
            data["beta"].append(float(row[5]))
            data["duty_a"].append(float(row[6]))
            data["duty_b"].append(float(row[7]))
            data["duty_c"].append(float(row[8]))

    return data
