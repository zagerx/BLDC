#include "comment.h"
std::vector<unsigned char> stringToUCharVectorOptimized(const QString &str)
{
    QStringList floatsStr = str.split(',');
    std::vector<float> floatVector;

    // 提取所有浮点数到float向量中
    for (const QString &floatStr : floatsStr) {
        bool ok;
        float value = floatStr.toFloat(&ok);
        if (!ok) {
            // 如果转换失败，可以选择跳过或抛出异常
            // 这里我们选择跳过
            continue;
        }
        floatVector.push_back(value);
    }

    // 将float向量中的每个浮点数的字节表示添加到std::vector<unsigned char>中
    std::vector<unsigned char> result;
    result.reserve(floatVector.size() * sizeof(float));
    for (const float &f : floatVector) {
        result.insert(result.end(), reinterpret_cast<const unsigned char*>(&f), reinterpret_cast<const unsigned char*>(&f) + sizeof(float));
    }
    return result;
}





#include <iostream>
#include <vector>
#include <iomanip> // 用于 std::hex 和 std::setw
void PrintBytes(const std::vector<unsigned char>& bytes) {
    for (auto byte : bytes) {
        // 打印每个字节，以十六进制形式，前面补0以保持两位宽度
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte << ' ';
    }
    std::cout << std::endl; // 打印完毕后换行
}

void printFloatsFromBytes(const std::vector<unsigned char>& data) {
    // 检查数据长度是否是4的倍数，因为每个float需要4个字节
    if (data.size() % 4 != 0) {
        qDebug() << "Warning: Data size is not a multiple of 4, which is required for floats.";
        return;
    }

    // 使用memcpy将字节复制到float数组中
    // 注意：这里假设了系统是小端字节序
    size_t numFloats = data.size() / 4;
    std::vector<float> floats(numFloats);
    for (size_t i = 0; i < numFloats; ++i) {
        // 注意：这里使用了&floats[i]的地址来接收复制的字节
        // memcpy的参数是目标地址、源地址和要复制的字节数
        memcpy(&floats[i], &data[i * 4], 4);
    }

    // 打印每个float的值
    for (float f : floats) {
        qDebug() << f;
    }
}