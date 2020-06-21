#include <iostream>
#include <vector>

//P1009 阶乘之和
/*
用高精度计算出S=1!+2!+3!+…+n! (n≤50)S=1!+2!+3!+…+n!(n≤50)
其中“!”表示阶乘，例如：5!=5×4×3×2×1。
*/

using namespace std;

//定义一个大数类
class BigData
{
public:
    //构造函数：给一个普通整数，构造一个大数对象
    BigData(int v=0)
    {
        int r;
        //把v每一位都存到一个vector容器中，个位数字在数组的第0位，其它按序
        while(v)
        {
            r = v%10;
            v = v/10;
            data.push_back(r);
        }
    }
    //利用一个vector数组构造一个大数对象，其中d中每个数字必须是0-9之间
    //高位不能有0，如果高位有0，此函数需要过滤一下
    BigData(const std::vector<int>& d)
    {
        data = d;
    }
    //返回大数的位数
    int Num() const {return data.size();}
    //左移大数，左移一位就是乘以10
    const BigData& LeftShift(int n)
    {
        //准备n个0
        std::vector<int> zeros(n, 0);
        //把n个0插入到数组前端，相当于乘以n个10
        data.insert(data.begin(), zeros.begin(), zeros.end());
        return *this;
    }
    //大数乘以一个[0,9]之间的数字
    BigData operator *(int digit) const
    {
        if(digit==0)
            return BigData();
        std::vector<int> b;
        int i, extra=0, s;
        //大数乘以一个单个数字的方法
        for(i=0; i<(int)data.size(); ++i)
        {
            s = data[i]*digit+extra;
            b.push_back(s%10);
            extra = s/10;
        }
        //如果剩余还有进位，记得加上去
        if(extra)
            b.push_back(extra);
        return BigData(b);
    }
    //定义两个友元函数处理大数加法和乘法
    friend BigData operator +(const BigData& a, const BigData& b);
    friend BigData operator *(const BigData& a, const BigData& b);

    //打印函数，理论上还要处理一下高位的0
    void Print()
    {
        //特殊处理一下0
        if(data.empty())
        {
            std::cout<<0;
            return;
        }
        int i;
        for(i=(int)data.size()-1; i>=0; --i)
        {
            std::cout<<data[i];
        }
    }
    //得到某一位上的数字
    int& operator[](int i) {return data[i];}
    const int& operator[](int i) const {return data[i];}
private:
    //大数用一个vector容器表示
    vector<int> data;
};

//友元函数，大数加法
BigData operator +(const BigData& a, const BigData& b)
{
    int i, j, extra=0,s;
    std::vector<int> c;
    //extra是进位，初始为0，从低位开始加起
    for(i=0, j=0; i<a.Num() && j<b.Num(); ++i, ++j)
    {
        s = a[i]+b[j]+extra;
        c.push_back(s%10);
        extra = s/10;
    }
    //如果a高位上的数字还没有访问完，则继续累加进位
    for(;i<a.Num(); ++i)
    {
        s = a[i]+extra;
        c.push_back(s%10);
        extra = s/10;
    }
    //如果b高位上的数字还没有访问完，则继续累加进位
    for(;j<b.Num(); ++j)
    {
        s = b[j]+extra;
        c.push_back(s%10);
        extra = s/10;
    }
    //如果还有进位
    if(extra)
        c.push_back(extra);
    return c;
}

//友元函数，大数乘法
BigData operator *(const BigData& a, const BigData& b)
{
    int i;
    BigData sum;

    for(i=0; i<b.Num(); ++i)
    {
        //(a*b[i]).LeftShift(i) 是关键
        //它用数字乘以一个大数后，还需要左移，也就是在低位上填上若干个0
        sum = sum + (a*b[i]).LeftShift(i);
    }
    return sum;
}

int main()
{
    int N, i;

    cin>>N;

    BigData S, d(1);
    //初始化S = 1！
    S = d;
    //计算阶乘和，d每次乘以一个i，从而节约计算时间
    for(i=2; i<=N; ++i)
    {
        //d = d*i, i一定要用一个大数对象，因为i会大于9
        d = d*BigData(i);
        //累加和
        S = S+d;
    }
    //打印结果
    S.Print();
    std::cout<<"\n";
    return 0;
}