#include "../exercise.h"

// READ: Trivial type <https://learn.microsoft.com/zh-cn/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170>

struct FibonacciCache {
    unsigned long long cache[16];
    int cached;
};

// TODO: 实现正确的缓存优化斐波那契计算
static unsigned long long fibonacci(FibonacciCache &cache, int i) {
    // 如果请求的索引超出了缓存大小，则返回 0 或者处理错误
    if (i >= 16) return 0;

    // 如果索引已经在缓存中，则直接返回缓存的结果
    if (i < cache.cached) {
        return cache.cache[i];
    }

    // 否则，从已缓存的最大索引开始计算直到 i
    for (int n = cache.cached; n <= i; ++n) {
        if (n == 0) {
            cache.cache[n] = 0;
        } else if (n == 1) {
            cache.cache[n] = 1;
        } else {
            cache.cache[n] = cache.cache[n - 1] + cache.cache[n - 2];
        }
        cache.cached = n + 1; // 更新已缓存的数量
    }

    return cache.cache[i];
}

int main(int argc, char **argv) {
    // TODO: 初始化缓存结构体，使计算正确
    FibonacciCache fib = { {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 2 };
    // NOTICE: C/C++ 中，读取未初始化的变量（包括结构体变量）是未定义行为
    // READ: 初始化的各种写法 <https://zh.cppreference.com/w/cpp/language/initialization>
    ASSERT(fibonacci(fib, 10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fibonacci(fib, 10) << std::endl;
    return 0;
}
