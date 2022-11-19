#include "matrix.hpp"
#include "operators.hpp"

#include <iostream>

using matexpr::Matrix;

template<typename T>
void printMatrix(const Matrix<T>& matrix)
{
    for (size_t row = 0; row < matrix.nrows(); row++)
    {
        for (size_t col = 0; col < matrix.ncols(); col++)
        {
            std::cout << matrix(row, col) << "\t";
        }
        std::cout << "\n";
    }
}

int main()
{
    Matrix m1 = { { 1.0, 2.0 }, { 3.0, 4.0 } };
    Matrix m2 = { { 0.0, 1.0 }, { 1.0, 2.0 } };

    auto r = (2.0 * -m1 + m2) / 2.0 + m2 - 3 * Matrix{ { 5.0, 2.0 }, { 4.0, 1.0 } };

    Matrix<double> mr = r;

    printMatrix(mr);
}