#pragma once

#include <optional>
#include <vector>

using Matrix = std::vector<std::vector<double>>;

// return algebraic addition for element in matrix[row][col]
double CalcAlgebraicAddition(size_t row, size_t col, const Matrix& matrix);
// calc determinant of matrix
double GetDeterminant(const Matrix& matrix);
// return inverse matrix
Matrix GetInverseMatrix(const Matrix& matrix, double det);
// multiply square matrix n*n and vector with n elements
std::vector<double> MultiplyMatrix(const Matrix& matrix, const std::vector<double>& vec);

// system of equations
class EquationSystem {
public:
    explicit EquationSystem(Matrix matrix, std::vector<double> right_part) 
        : matrix_{std::move(matrix)},
          right_part_{std::move(right_part)} {
    }

    // calc system of equations and return solution
    std::optional<std::vector<double>> GetSolve() const;

private:
    // solve matrix equation by the Gauss method
    void SolveByTheGauss() const;

    // matrix_ stores vectors with raws elements
    // matrix_[0] match to the first raw
    mutable Matrix matrix_;
    mutable std::vector<double> right_part_;
};