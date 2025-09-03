#include <algorithm>

template <const unsigned int N>
class Vector
{
private:
    float data[N];

public:
    Vector(const float (&input)[N]) { std::copy(input, input + N, data); }
    Vector() { std::fill(data, data + N, 0.0f); }
    ~Vector() = default;
    float &operator[](unsigned int index) { return data[index]; }
    const float &operator[](unsigned int index) const { return data[index]; }
    constexpr unsigned int size() const { return N; }
};
