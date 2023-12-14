#include <iostream>

// 计算 Zp 中给定非零元素 a 的逆元
int modInverse(int a, int p) {
    int originalP = p;
    int x = 0, y = 1, lastX = 1, lastY = 0, temp;
    while (p != 0) {
        int quotient = a / p;
        int remainder = a % p;

        a = p;
        p = remainder;

        temp = x;
        x = lastX - quotient * x;
        lastX = temp;

        temp = y;
        y = lastY - quotient * y;
        lastY = temp;
    }

    // 如果 a 和 p 不互质（最大公约数不为1），不存在逆元
    if (a != 1) {
        return -1;
    }

    // 确保逆元为正数
    int inverse = lastX % originalP;
    if (inverse < 0) {
        inverse += originalP;
    }

    return inverse;
}

int main() {
    int a, p;
    std::cin >> a >> p;

    int inverse = modInverse(a, p);
    if (inverse != -1) {
        std::cout << inverse << std::endl;
    }

    return 0;
}
