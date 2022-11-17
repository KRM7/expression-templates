#include "matrix.hpp"
#include "operators.hpp"

#include <iostream>

using namespace matexpr;

int main()
{
    Matrix m1 = { { 1.0, 2.0 }, { 3.0, 4.0 } };
    Matrix m2 = { { 0.0, 1.0 }, { 1.0, 2.0 } };

    auto r = (2.0 * -m1 + m2) / 2.0 + m2 - 3 * Matrix{ { 5.0, 2.0 }, { 4.0, 1.0 } };

    Matrix<double> mr = r;

    std::cout << mr(0, 1) << "\n"; // -6.5
}