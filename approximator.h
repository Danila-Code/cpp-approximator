#pragma once

#include "equation_system.h"

#include <cmath>
#include <numeric>
#include <optional>

using Coeffs = std::vector<double>;

// pairs of x and y(x) that needs to approximate
struct Data {
    double x;
    double y;
};

struct Polynomial {
public:
    explicit Polynomial(std::vector<double> vec) : coeffs{std::move(vec)} {}
    // calc polynomial func value y(x)
    // coeffs_ must contain values
    double operator()(double x) const {
        int exp = 0;
        return std::accumulate(coeffs.begin(), coeffs.end(), 0.0,
        [&x, &exp](double init, double value) {
            double res = init + value * pow(x, exp);
            ++exp;
            return res;
    });
    }

    // coefficients in a polynomial, starts from the free member and ends on biggest degree member
    std::vector<double> coeffs;
};

class Approximator {
public:
    Approximator() = default;

    // sets the data to be approximated
    void SetData(std::vector<Data>& data);

    // returns coefficients of the polynomial if the approximation is successful
    // the coefficients follow starting from a0 to an
    std::optional<Polynomial> GetPolynom(size_t polynom_degree);
    
    // return sum of squared errors
    double GetSumSquaredErrors() const;

    std::vector<Data> GetData() const;

private:
    // method calculate polynomial coefficient for data_ and set polynom_coeff_
    void CalcPolynomCoeffs();

    // data that needs to be approximated
    std::vector<Data> data_{};
    // degree of polynomial
    size_t polynom_degree_ = 2;
    // polynomial
    std::optional<Polynomial> polynom_;
};
