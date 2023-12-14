#include <iostream>
#include <random>

// 快速幂取模算法
long long powerMod(long long base, long long exponent, long long modulus) {
    long long result = 1;
    base %= modulus;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent /= 2;
    }

    return result;
}

// Miller-Rabin素性测试
bool millerRabinTest(long long n, int k) {
    if (n <= 1 || n == 4) {
        return false;
    }
    if (n <= 3) {
        return true;
    }

    long long d = n - 1;
    while (d % 2 == 0) {
        d /= 2;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dis(2, n - 2);

    for (int i = 0; i < k; ++i) {
        long long a = dis(gen);
        long long x = powerMod(a, d, n);
        if (x == 1 || x == n - 1) {
            continue;
        }

        bool isPrime = false;
        while (d != n - 1) {
            x = (x * x) % n;
            d *= 2;

            if (x == 1) {
                return false;
            }
            if (x == n - 1) {
                isPrime = true;
                break;
            }
        }

        if (!isPrime) {
            return false;
        }
    }

    return true;
}

int main() {
    long long n;
    std::cin >> n;

    int k = 10; // 进行 Miller-Rabin 测试的轮数

    if (millerRabinTest(n, k)) {
        std::cout << "Yes";
    } else {
        std::cout << "No";
    }

    return 0;
}
