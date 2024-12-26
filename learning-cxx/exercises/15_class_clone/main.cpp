#include "../exercise.h"

// READ: 复制构造函数 <https://zh.cppreference.com/w/cpp/language/copy_constructor>
// READ: 函数定义（显式弃置）<https://zh.cppreference.com/w/cpp/language/function>


class DynFibonacci {
    size_t *cache;
    int cached;

public:
    // TODO: 实现复制构造器
    DynFibonacci(DynFibonacci const &a) : cache(new size_t[a.cached]), cached(a.cached) {
        for (int i = 0; i < cached; ++i) {
            cache[i] = a.cache[i];
        }
    }

    // TODO: 实现动态设置容量的构造器
    DynFibonacci(int capacity): cache(new size_t[capacity]), cached(2) {
        if (capacity >= 2) {
            cache[0] = 0;
            cache[1] = 1;
        }
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci() {
        delete[] cache;
    }

    // TODO: 实现正确的缓存优化斐波那契计算
    size_t get(int i) {
        // 如果请求的索引超出了缓存大小，则返回 0 或者处理错误
        if (i >= 16) return 0;

        // 如果索引已经在缓存中，则直接返回缓存的结果
        if (i < cached) {
            return cache[i];
        }

        // 否则，从已缓存的最大索引开始计算直到 i
        for (int n = cached; n <= i; ++n) {
            if (n == 0) {
                cache[n] = 0;
            } else if (n == 1) {
                cache[n] = 1;
            } else {
                cache[n] = cache[n - 1] + cache[n - 2];
            }
            cached = n + 1; // 更新已缓存的数量
        }

        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    // NOTICE: 名字相同参数也相同，但 const 修饰不同的方法是一对重载方法，可以同时存在
    //         本质上，方法是隐藏了 this 参数的函数
    //         const 修饰作用在 this 上，因此它们实际上参数不同
    size_t get(int i) const {
        if (i <= cached) {
            return cache[i];
        }
        ASSERT(false, "i out of range");
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    DynFibonacci const fib_ = fib;
    ASSERT(fib_.get(10) == fib.get(10), "Object cloned");
    return 0;
}
