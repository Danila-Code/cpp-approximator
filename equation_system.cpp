#include "equation_system.h"

#include <algorithm>
#include <cassert>
#include <deque>

// calc determinant of matrix recursevly
double GetDeterminantRec(size_t current_row, std::deque<size_t> ignore_cols, const Matrix& matrix) {
    size_t order = matrix.size() - current_row;  // order of matrix;
    if (order == 1) {
        return matrix[0][0];
    }

    if (order == 2) {
        size_t col1;
        for (col1 = 0; col1 < matrix.size(); ++col1) {
            if (ignore_cols.empty() || std::find(ignore_cols.begin(), ignore_cols.end(), col1) == ignore_cols.end()) {
                break;
            }
        }
        size_t col2;
        for (col2 = col1 + 1; col2 < matrix.size(); ++col2) {
            if (ignore_cols.empty() || std::find(ignore_cols.begin(), ignore_cols.end(), col2) == ignore_cols.end()) {
                break;
            }
        }
        return matrix[current_row][col1] * matrix[current_row + 1][col2] -
            matrix[current_row][col2] * matrix[current_row + 1][col1];
    }

    double det = 0;
    for (size_t i = 0; i < matrix.size(); ++i) {
        if (!ignore_cols.empty() && std::find(ignore_cols.begin(), ignore_cols.end(), i) != ignore_cols.end()) {
            continue;
        }
        int one_coef = (current_row + i) % 2 ? -1 : 1; //  -1 in pow of sum current row and current column
        auto new_ignore_cols(ignore_cols);
        new_ignore_cols.push_back(i);
        det += one_coef * matrix[current_row][i] * GetDeterminantRec(current_row + 1, new_ignore_cols, matrix);
    }
    return det;
}

// calc determinant of matrix
double GetDeterminant(const Matrix& matrix) {
    return GetDeterminantRec(0, {}, matrix);
}

// return algebraic addition for element in matrix[row][col]
double CalcAlgebraicAddition(size_t row, size_t col, const Matrix& matrix) {
    size_t size = matrix.size() - 1;
    Matrix minor_matrix(size);

    size_t m_i = 0;
    for (size_t i = 0; i < matrix.size(); ++i) {
        if (i == row) {
            continue;
        }
        minor_matrix[m_i].reserve(size);
        for (size_t j = 0; j < matrix.size(); ++j) {
            if (j == col) {
                continue;
            }
            minor_matrix[m_i].emplace_back(matrix[i][j]);
        }
        ++m_i;
    }
    int coef = (row + col) % 2 == 0 ? 1 : -1;
    return coef * GetDeterminant(minor_matrix);
}

// return inverse matrix
Matrix GetInverseMatrix(const Matrix& matrix, double det) {
    size_t size = matrix.size();
    Matrix inverse_matrix(size, std::vector<double>(size));

    for (size_t i = 0; i < size; ++i) { // change row
        for (size_t j = 0; j < size; ++j) { // change col
            inverse_matrix[i][j] = CalcAlgebraicAddition(i, j, matrix) / det;
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
            t += matrix[i][j] * vec[j];
        }
        res[i] = t;
    }
    return res;
}

// ********** methods of class EquationSystem  ************
    // solve matrix equation by the Gauss method
void EquationSystem::SolveByTheGauss() const {
    size_t size = matrix_.size();

    for (size_t i = 0; i < size; ++i) {
        double first_item = matrix_[i][i];
        assert(first_item);

        // we divide the equation by the first element to get one in the first element
        for (size_t j = i; j < size; ++j) {
            matrix_[i][j] /= first_item;
        }
        right_part_[i] /= first_item;
        assert(first_item);

        // we subtract the current one from each row multiplied by the first non-zero element
        for (size_t raw = 0; raw < size; ++raw) {
            if (raw == i) {
                continue;
            }
            first_item = matrix_[raw][i];
            for (size_t j = i;j < size; ++j) {
                matrix_[raw][j] -= matrix_[i][j] * first_item;
            }
            right_part_[raw] -= right_part_[i] * first_item;
        }
    }
}


// calc system of equations and return solution
std::optional<std::vector<double>> EquationSystem::GetSolve() const {
    std::vector<double> res;
    res.reserve(matrix_.size());

    const double det = GetDeterminant(matrix_);
    if (!det) {
        return std::nullopt;
    }

    //SolveByTheGauss();
    
    //res = right_part_;
    
    
    Matrix inverse_matrix = GetInverseMatrix(matrix_, det);
    res = MultiplyMatrix(inverse_matrix, right_part_);

    return res;
    //return std::move(right_part_);
}