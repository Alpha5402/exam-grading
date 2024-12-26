#include "../exercise.h"

struct Fibonacci {
    unsigned long long cache[128];
    int cached;

    // TODO: 实现正确的缓存优化斐波那契计算
    unsigned long long get(int i) {
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
};

int main(int argc, char **argv) {
    // TODO: 初始化缓存结构体，使计算正确
    Fibonacci fib = { {0, 1}, 2 };
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}
