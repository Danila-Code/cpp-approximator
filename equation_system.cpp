#include "equation_system.h"

namespace {
struct Index {
    size_t row;
    size_t col;
};

Index GetNextIndex(Index index, size_t size, int direct = 1) {
    index.col = (index.col + size + direct) % size;
    index.row = ++index.row < size ? index.row : 0;
    return index;
}

double GetDiagonalMultiply(const Matrix& matrix, Index start_index, int direct) {
    double res = 1;
    Index index = start_index;
    for (size_t i = 0; i < matrix.size(); ++i) {
        index = GetNextIndex(index, matrix.size(), direct);
        res *= matrix[index.col][index.row];
    }
    return res;
}

// Calc determinant for square matrix 2*2
double Get2x2Determinant(const Matrix& matrix) {
    return matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
}

}  // namespace

// calc determinant of matrix
double GetDeterminant(const Matrix& matrix) {
    if (matrix.size() == 2) {
        return Get2x2Determinant(matrix);
    }

    double res = 0;
    Index main_index{0, 0};
    Index side_index{0, matrix.size() - 1};

    for (size_t i = 0; i < matrix.size(); ++i) {
        res += GetDiagonalMultiply(matrix, main_index, 1);
        res -= GetDiagonalMultiply(matrix, side_index, -1);
        ++main_index.row;
        ++side_index.row;
    }
    return res;
}

double CalcAlgebraicAddition(size_t col, size_t row, const Matrix& matrix) {
    size_t size = matrix.size() - 1;
    Matrix minor_matrix(size);

    size_t m_i = 0;
    for (size_t i = 0; i < matrix.size(); ++i) {
        if (i == col) {
            continue;
        }
        minor_matrix[m_i].reserve(size);
        for (size_t j = 0; j < matrix.size(); ++j) {
            if (j == row) {
                continue;
            }
            minor_matrix[m_i].emplace_back(matrix[i][j]);
        }
        ++m_i;
    }
    int coef = (col + row) % 2 == 0 ? 1 : -1;
    return coef * GetDeterminant(minor_matrix);
}

// return inverse matrix
Matrix GetInverseMatrix(const Matrix& matrix, double det) {
    size_t size = matrix.size();
    Matrix inverse_matrix(size, std::vector<double>(size));

    for (size_t i = 0; i < size; ++i) { // change col
        for (size_t j = 0; j < size; ++j) { // change row
            inverse_matrix[j][i] = CalcAlgebraicAddition(i, j, matrix) / det;
        }
    }

    return inverse_matrix;
}

// multiply square matrix n*n and vector with n elements
std::vector<double> MultiplyMatrix(const Matrix& matrix, const std::vector<double>& vec) {
    size_t size = vec.size();
    std::vector<double> res(size);

    for (size_t i = 0; i < size; ++i) {
        double t = 0;
        for (size_t j = 0; j < size; ++j) {
            t += matrix[j][i] * vec[j];
        }
        res[i] = t;
    }
    return res;
}

// ********** methods of class EquationSystem  ************
// calc system of equations and return solution
std::optional<std::vector<double>> EquationSystem::GetSolve() const {
    std::vector<double> res;
    res.reserve(matrix_.size());

    const double det = GetDeterminant(matrix_);
    if (!det) {
        return std::nullopt;
    }

    
    /*Matrix inverse_matrix = GetInverseMatrix(matrix_, det);
    res = MultiplyMatrix(inverse_matrix, right_part_);*/
    

    for (size_t i = 0; i < matrix_.size(); ++i) {
        std::vector<double> temp_col = std::move(matrix_[i]);
        matrix_[i] = std::move(right_part_);

        double i_det = GetDeterminant(matrix_);
        res.emplace_back(i_det / det);

        right_part_ = std::move(matrix_[i]);
        matrix_[i] = std::move(temp_col);
    }

    return res;
}