#include "../exercise.h"

// C++ 中，`class` 和 `struct` 之间的**唯一区别**是
// `class` 默认访问控制符是 `private`，
// `struct` 默认访问控制符是 `public`。
// READ: 访问说明符 <https://zh.cppreference.com/w/cpp/language/access>

// 这个 class 中的字段被 private 修饰，只能在 class 内部访问。
// 因此必须提供构造器来初始化字段。
// READ: 构造器 <https://zh.cppreference.com/w/cpp/language/constructor>
class Fibonacci {
    size_t cache[16];
    int cached;

public:
    // TODO: 实现构造器
     Fibonacci(){
         cache[0] = 0;
         cache[1] = 1;
         cached = 2;
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
};

int main(int argc, char **argv) {
    // 现在类型拥有无参构造器，声明时会直接调用。
    // 这个写法不再是未定义行为了。
    Fibonacci fib;
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}
