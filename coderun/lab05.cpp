#include <iostream>
using namespace std;

// 点类
class Point
{
public:
    Point() : x(0),y(0),isInfinity(false) {};
    Point(long long x,long long y) : x(x),y(y),isInfinity(false) {}; 
    long long x;
    long long y;
    bool isInfinity; // 标志位，记录该点是否为无穷远点，初始设置为false
    Point operator=(Point point); // 重载=
    bool operator==(Point point); // 重载==
};

// 椭圆曲线类
class Eccs
{
private:
    long long a;
    long long b;
    long long p;
public:
    Eccs() : a(0),b(0),p(0) {};
    Eccs(long long a,long long b,long long p) : a(a),b(b),p(p) {};
    Point Pointadd(Point endpoint,Point beginpoint); // 点加
};

// 扩展欧几里得算法计算最大公约数
long long ex_gcd(long long a,long long b,long long &x,long long &y);

// 求逆元
long long getInv(long long a,long long mod);

int main()
{
    long long a,b,p,x,y,k;
    cin >> a >> b >> p;
    Eccs e(a,b,p);
    cin >> x >> y >> k;
    Point point(x,y);
    Point answer;
    answer.operator=(point);

    for(int i = 1;i < k;i++)
    {
        answer.operator=(e.Pointadd(answer,point));
    }

    // 发现是无穷远点或k为0返回（-1，-1）
    if(answer.isInfinity || k == 0)
    {
        cout << -1 << " " << -1;
    }
    else
    {
        if(answer.x < 0)
        {
            answer.x += p;
        }
        if(answer.y < 0)
        {
            answer.y += p;
        }
        cout << answer.x << " " << answer.y;
    }
}

// 重载=运算符
Point Point::operator=(Point point)
{
    this->x = point.x;
    this->y = point.y;
    this->isInfinity = point.isInfinity;
    return *this;
}

// 重载==运算符
bool Point::operator==(Point point)
{
    return this->x == point.x && this->y == point.y;
}

// 点加
Point Eccs::Pointadd(Point endpoint,Point beginpoint)
{
    // endpoint代表的是每次倍加后的结果
    // beginpoint代表输入点
    long long t;
    Point answer;
    // 当endpoint是无穷远点时，直接返回beginpoint
    if(endpoint.isInfinity)
    {
        return beginpoint;
    }
    // 两点相同
    if(endpoint.operator==(beginpoint))
    {
        long long t1 = getInv(2*endpoint.y,p); // 模逆
        t = ((3 * (endpoint.x * endpoint.x) + a) * t1) % p; 
    }
    // 两点不同
    else
    {
        long long t2 = beginpoint.x-endpoint.x;
        if(t2 == 0)
        {
            answer.isInfinity = true;
            return answer;
        }
        if(t2 < 0)
        {
            t2 += p;
        }
        long long t1 = getInv(t2,p);
        t = ((beginpoint.y-endpoint.y) * t1) % p;
        if(t < 0)
        {
            t += p;
        }
    }
    answer.x = (t*t - endpoint.x - beginpoint.x) % p;
    if(answer.x < 0)
    {
        answer.x += p;
    }
    answer.y = (t * (endpoint.x - answer.x) - endpoint.y) % p;
    if(answer.y < 0)
    {
        answer.y += p;
    }
    return answer;
}

long long ex_gcd(long long a,long long b,long long &x,long long &y)
{
	if(b==0)
	{
		x=1,y=0;
		return a;
	}
	long long ret=ex_gcd(b,a%b,y,x);
	y-=a/b*x;
	return ret;
}

long long getInv(long long a,long long mod)
{
	long long x,y;
	long long d=ex_gcd(a,mod,x,y);
	return d==1 ? (x % mod + mod) % mod : -1;
}