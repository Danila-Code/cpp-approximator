#pragma once

#include "equation_system.h"

#include <cmath>
#include <numeric>
#include <optional>
#include <sstream>
#include <string>


namespace {
// output monomial with coefficient coef and x in power of i
void PrintMonomial(std::ostream& out, double coef, size_t i) {
    if (i) {
        if (coef != 1) {
            out << std::abs(coef);
        }
        out << 'x';
        if (i > 1) {
            out << '^' << i;
        }
    } else {
        out << coef;
    }
}

}  // namespace

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

    // return string representation of Polynomial
    std::string ToString() const {
        using namespace std::literals;
        std::ostringstream res_stream;
        res_stream << "y(x) = "sv;
        
        bool first_coeff = true;
        
        for (size_t i = 0; i < coeffs.size(); ++i) {
            if (coeffs[i] == 0) {
                continue;
            }
            if (first_coeff) {
                first_coeff = false;
            } else {
                res_stream << (coeffs[i] < 0 ? " - "sv : " + "sv);
            }
            PrintMonomial(res_stream, coeffs[i], i);
        }
        return res_stream.str();
    }

    // coefficients in a polynomial, starts from the free member and ends on biggest degree member
    std::vector<double> coeffs;
};

class Approximator {
public:
    Approximator() = default;

    // sets the data to be approximated
    void SetData(std::vector<Data> data);

    Polynomial GetPolynom() const;
    // returns coefficients of the polynomial if the approximation is successful
    // the coefficients follow starting from a0 to an
    void ApproximateData();
    // set polenom degree
    void SetPolynomDegree(size_t degree);
    
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
