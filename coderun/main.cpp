#include <iostream>

struct Point {
    int x;
    int y;
};

// 椭圆曲线上的点加法运算
Point pointAddition(Point poin, Point q, int a, int p) {
    Point result;
    int lambda;

    if (poin.x == -1 && poin.y == -1) {
        return q;
    }
    if (q.x == -1 && q.y == -1) {
        return poin;
    }

    if (poin.x == q.x && poin.y == -q.y) {
        result.x = -1;
        result.y = -1;
        return result;
    }

    if (poin.x == q.x && poin.y == q.y) {
        lambda = (3 * poin.x * poin.x + a) % p;
        lambda = (lambda * inverseMod(2 * poin.y, p)) % p;
    } else {
        lambda = (q.y - poin.y) % p;
        lambda = (lambda * inverseMod(q.x - poin.x, p)) % p;
    }

    result.x = (lambda * lambda - poin.x - q.x) % p;
    result.y = (lambda * (poin.x - result.x) - poin.y) % p;

    return result;
}

// 椭圆曲线上的点倍乘运算
Point pointMultiplication(Point poin, int k, int a, int p) {
    Point result;
    Point q = poin;

    result.x = -1;
    result.y = -1;

    while (k > 0) {
        if (k % 2 == 1) {
            result = pointAddition(result, q, a, p);
        }
        q = pointAddition(q, q, a, p);
        k = k / 2;
    }

    return result;
}

int main() {
    int a, b, p;
    int x, y, k;

    std::cin >> a >> b >> p;
    std::cin >> x >> y >> k;

    Point poin = {x, y};
    Point result = pointMultiplication(poin, k, a, p);

    std::cout << result.x << " " << result.y << std::endl;

    return 0;
}