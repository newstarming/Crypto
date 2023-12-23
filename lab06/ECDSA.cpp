#include<iostream>
using namespace std;

void rule(int& x,int k) {
	for (; x < 0;) {
		x += k;
	}
	for(; x >= k;) {
		x -= k;
	}
}

void pointadd(int& x,int& y, int n) {
	int p = 11;
	n -= 2;
	int child1 = 3 * x * x  +1;
	rule(child1,p);
	int parent1 = 2 * y;
	rule(parent1,p);
	for (; child1 % parent1 != 0;) {
		child1 += 11;
	}
	int k = child1 / parent1 % 11;
	int x1 = x, y1 = y;
	x = (k * k - 2 * x1);
	rule(x,p);
	y = (k * (x1 - x) - y1);
	rule(y,p);
	for (; n > 0; n--) {
		int child2 = y - y1;
		rule(child2,p);
		int parent2 = x - x1;
		rule(parent2,p);
		if (parent2 == 0) {
			parent2 += 11;
			for (; child2 % parent2 != 0;) {
				child2 += 11;
			}
			int k = child2 / parent2 % 11;
			int x2 = x, y2 = y;
			x = (k * k - x2 - x1);
			rule(x, p);
			y = (k * (x2 - x) - y2);
			rule(y, p);
		}
		else {
			for (; child2 % parent2 != 0;) {
				child2 += 11;
			}
			int k = child2 / parent2 % 11;
			int x2 = x, y2 = y;
			x = (k * k - x2 - x1);
			rule(x, p);
			y = (k * (x2 - x) - y2);
			rule(y, p);
		}
	}
}

int inverse(int k) {
	int ci = 1;
	for (; k * ci % 13 != 1;) {
		ci++;
	}
	return ci;
}

int main() {
	int x0, y0;
	int m, x, k;
	cin >> x0 >> y0;
	cin >> m >> x >> k;
	pointadd(x0, y0, k);
	int r = x0 % 13;
	int hashx = 1;
	for (; x > 0; x--) {
		hashx *= 2;
	}
	hashx = hashx % 13;
	int inversek = inverse(k);
	int s = inversek * (hashx + m * r);
	rule(s, 13);
	cout << r << ' ' << s;
	return 0;
}