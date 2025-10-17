#include "approximator.h"

#include <algorithm>
#include <cassert>

// Least squares approximation of max_power degree polynomials

/*
a0*m + a1*sum(x) + ... + an-1*sum(x^n-1) + an*sum(x^n) = sum(y)
a0*sum(x) + a1*sum(x^2) + ... + an-1*sum(x^n) + an*sum(x^(n+1)) = sum(x*y)
 ...
a0*sum(x^(n-1)) + a1*sum(x^n) + ... + an-1*sum(x^(2n-2)) + an*sum(x^(2n-1)) = sum(x^n-1*y)
a0*sum(x^n) + a1*sum(x^(n+1)) + ... + an-1*sum(x^(2n-1)) + an*sum(x^2n) = sum(x^n*y)
*/

namespace {
// return matrix (with size m*m) with powers of each x from 1 to 2*max_power
// x[0] x[1] ... x[m-1]
// x[0]^2 x[1]^2 ... x[m-1]^2
// ...
// x[0]^2n x[1]^2n ... x[m-1]^2*max_power
// m - numbers of x
// n - max power
Matrix GetXPowers(const std::vector<Data>& data, int max_power) {
    // matrix with x powers
    const size_t size = 2 * max_power; // powers from 1 to 2*max_power
    std::vector<std::vector<double>> x_powers(size);
    
    for(size_t i = 0; i < size; ++i) {
        x_powers[i].reserve(data.size()); // x[0] to x[m-1]

        for(size_t j = 0; j < data.size(); ++j) {
            double prev_x_power = i > 0 ? x_powers[i-1][j] : 1;
            x_powers[i].emplace_back(prev_x_power * data[j].x);
        }
    }
    return x_powers;
}

// return vector with sum of powers of each x[i]:
// m sum(x[i]) sum(x[i]^2) ... sum(x[i]^2n)
std::vector<double> GetSumOfXPowers(const Matrix& x_powers, int max_power) {
    if (x_powers.empty()) {
        return {};
    }
    std::vector<double> sum_x_powers;
    sum_x_powers.reserve(2 * max_power + 1);

    sum_x_powers.emplace_back(x_powers[0].size());

    std::for_each(x_powers.begin(), x_powers.end(),
        [&sum_x_powers](const std::vector<double>& vec) {
            sum_x_powers.emplace_back(std::accumulate(vec.begin(), vec.end(), 0.0));
    });
    return sum_x_powers;
}

// Fill matrix with source data (x and y)
/*
----------------------matrix---------------------------------------------------
| m            sum(x)   ... sum(x^(n-1))  sum(x^n)      |   | sum(y)       |
| sum(x)       sum(x^2) ... sum(x^n)      sum(x^(n+1))  |   | sum(x*y)     |
| ...                                                   | = | ...          |
| sum(x^(n-1)) sum(x^n) ... sum(x^2n-2)   sum(x^(2n-1)) |   | sum(x^n-1*y) |
| sum(x^n) sum(x^(n+1)) ... sum(x^(2n-1)) sum(x^2n)     |   | sum(x^n*y)   |
*/
Matrix GetMatrix(const Matrix& x_powers, int max_power) {
    std::vector<double> sum_x_powers = GetSumOfXPowers(x_powers, max_power);
    const size_t size = max_power + 1;
    Matrix matrix(size); // matrix

    for(size_t i = 0; i < size; ++i) {
        matrix[i] = std::vector<double>(size);
        for(size_t j = 0; j < size; ++j) { // fill cols
            matrix[i][j] = sum_x_powers[i + j];
        }
    }
    return matrix;
}

std::vector<double> GetRightPart(const Matrix& x_powers, const std::vector<Data>& data, int max_power) {
    std::vector<double> right_part(max_power + 1);

    right_part[0] = std::accumulate(data.begin(), data.end(), 0,
        [](double init, Data data) {
            return init + data.y;
    });
    
    for (size_t i = 1; i < right_part.size(); ++i) {
        double res = 0;
        for (size_t j = 0; j < data.size(); ++j) {
            res += x_powers[i - 1][j] * data[j].y;
        }
        right_part[i] = res;
    }
    return right_part;
}

EquationSystem GetEquationSystem(const std::vector<Data>& data, int max_power) {
    const auto x_powers = GetXPowers(data, max_power);
    return EquationSystem(GetMatrix(x_powers, max_power), GetRightPart(x_powers, data, max_power));
}

}  // namespace

// sets the data to be approximated
void Approximator::SetData(std::vector<Data>& data) {
    data_ = std::move(data);
}

// returns coefficients of the polynomial
Polynomial Approximator::GetPolynom() const {
    return polynom_.value();
}
// returns coefficients of the polynomial
std::optional<Polynomial> Approximator::GetPolynom(size_t polynom_degree) {
    if (polynom_ && polynom_degree_ == polynom_degree) {
        return polynom_;
    }
    polynom_degree_ = polynom_degree;
    CalcPolynomCoeffs();

    return polynom_;
}

// method calculate polynomial coefficient for data_ and set polynom_coeff_
void Approximator::CalcPolynomCoeffs() {
    auto res = GetEquationSystem(data_, polynom_degree_).GetSolve();
    if (res) {
        polynom_ = Polynomial(res.value());
    }
}

// return sum of squared errors
double Approximator::GetSumSquaredErrors() const {
    if (!polynom_) {
        return 0;
    }
    return std::accumulate(data_.begin(), data_.end(), 0,
        [this](double init, Data point) {
            return init + pow(point.y - (*polynom_)(point.x), 2);
    });
}

std::vector<Data> Approximator::GetData() const {
    return data_;
}