#include "equation_system.h"

#include <optional>

using Coeffs = std::vector<double>;

// pairs of x and y(x) that needs to approximate
struct Data {
    double x;
    double y;
};

class Approximator {
public:
    Approximator() = default;

    // sets the data to be approximated
    void SetData(std::vector<Data>& data);

    // returns coefficients of the polynomial if the approximation is successful
    // the coefficients follow starting from a0 to an
    std::optional<Coeffs> GetPolynomCoeffs(size_t polynom_degree);
    
    // return sum of squared errors
    double GetSumSquaredErrors() const;


    std::vector<Data> GetData() const;

private:
    // method calculate polynomial coefficient for data_ and set polynom_coeff_
    void CalcPolynomCoeffs();

    // calc polynomial func value y(x)
    // polynom_coeffs_ must contain values
    double CalcPolynomialValue(double x) const;

    // data that needs to be approximated
    std::vector<Data> data_{};
    // degree of polynomial
    size_t polynom_degree_ = 2;
    // coefficients of polynomial
    std::optional<Coeffs> polynom_coeffs_;
};
