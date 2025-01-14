#include "../exercise.h"
#include <vector>
#include <cstring>

// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4]{};
    std::vector<T> *data;

    explicit Tensor4D(unsigned int const shape_[4]) {
        unsigned int size = 1;
        for (int i = 0; i < 4; ++i) {
            shape[i] = shape_[i];
            size *= shape_[i];  // 计算总的元素数目
        }
        data = new std::vector<T>(size);
    }

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        for (int i = 0; i < 4; ++i) {
            shape[i] = shape_[i];
            size *= shape_[i];  // 计算总的元素数目
        }
        data = new std::vector<T>(size);
        for (unsigned int i = 0; i < size; i++) {
            (*data)[i] = data_[i];
        }
        //memcpy(data, data_, size * sizeof(T));  // 拷贝数据
    }
    ~Tensor4D() = default;

    // 获取张量在一维数据数组中的偏移索引
    [[nodiscard]] unsigned int getIndex(unsigned int i, unsigned int j, unsigned int k, unsigned int l) const {
        return ((i * shape[1] + j) * shape[2] + k) * shape[3] + l;
    }

    // 获取指定索引处的值
    [[nodiscard]] T get(unsigned int i, unsigned int j, unsigned int k, unsigned int l) const {
        return (*data)[getIndex(i, j, k, l)];
    }

    // 设置指定索引处的值
    void set(unsigned int i, unsigned int j, unsigned int k, unsigned int l, T value) {
        (*data)[getIndex(i, j, k, l)] = value;
    }

    // 打印张量的数据（用于调试）
    void print() const {
        for (unsigned int i = 0; i < shape[0]; ++i) {
            for (unsigned int j = 0; j < shape[1]; ++j) {
                for (unsigned int k = 0; k < shape[2]; ++k) {
                    for (unsigned int l = 0; l < shape[3]; ++l) {
                        std::cout << get(i, j, k, l) << " ";
                    }
                    std::cout << "\n";
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。
    Tensor4D &operator+=(Tensor4D const &others) {
        // 检查形状是否兼容（单向广播规则）
        for (int dim = 0; dim < 4; ++dim) {
            if (others.shape[dim] != 1 && this->shape[dim] != others.shape[dim]) {
                throw std::invalid_argument("Tensors have incompatible shapes for broadcasting.");
            }
        }

        // 创建结果张量，形状与 a 相同
        Tensor4D<T> result(this->shape);

        // 进行广播计算
        for (unsigned int i = 0; i < this->shape[0]; ++i) {
            for (unsigned int j = 0; j < this->shape[1]; ++j) {
                for (unsigned int k = 0; k < this->shape[2]; ++k) {
                    for (unsigned int l = 0; l < this->shape[3]; ++l) {
                        // 根据广播规则计算 b 的索引（如果对应维度大小为 1，则使用索引 0）
                        unsigned int bi = (others.shape[0] == 1) ? 0 : i;
                        unsigned int bj = (others.shape[1] == 1) ? 0 : j;
                        unsigned int bk = (others.shape[2] == 1) ? 0 : k;
                        unsigned int bl = (others.shape[3] == 1) ? 0 : l;

                        // 相加并存储结果
                        result.set(i, j, k, l, this->get(i, j, k, l) + others.get(bi, bj, bk, bl));
                    }
                }
            }
        }

        this->data = result.data;
        return *this;
    }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT((*t0.data)[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            //std::cout << t0.data[i] << " ";
            ASSERT((*t0.data)[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT((*t0.data)[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
