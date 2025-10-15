#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>

#include "approximator.h"

using namespace std::literals;

constexpr double EPSILON = 1e-6;
std::ostream& operator<<(std::ostream& out, Data point) {
    out << "x = "s << point.x << " y = "s << point.y;
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec) {
    for (auto item : vec) {
        out << item << ' ';
    } 
    return out;
}

double SquareFunc(double x) {
    return 2.1 * x*x;
}

bool IsEqual(double lhs, double rhs) {
    return fabs(lhs - rhs) < EPSILON;
}

double CalcY(const std::vector<double>& coeffs, double x) {
    int exp = 0;
    return std::accumulate(coeffs.begin(), coeffs.end(), 0.0,
        [&x, &exp](double init, double value) {
            double res = init + value * pow(x, exp);
            ++exp;
            return res;
    });
}

// Testing struct
struct RandomPolynomial {
    explicit RandomPolynomial(int max_power) {
        GenerateVector(max_power);
    }

    double operator()(double x) const {
        return CalcY(polynom_coeff, x);
    }

    std::vector<double> polynom_coeff;

private:
    void GenerateVector(size_t size) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(1.0, 5.0);
        std::vector<double> vec(size + 1);
        std::for_each(vec.begin(), vec.end(),
            [&dis, &gen](double& item) {
                item = static_cast<int>(dis(gen));
                //item = dis(gen);
        });
        polynom_coeff = std::move(vec);
    }
};

bool operator==(const RandomPolynomial& polynom, std::vector<double> coeffs) {
    if (polynom.polynom_coeff.size() != coeffs.size()) {
        return false;
    }
    for (size_t i = 0; i < coeffs.size(); ++i) {
        if(!IsEqual(polynom.polynom_coeff[i], coeffs[i])) {
            return false;
        }
    }
    return true;
}

std::vector<double> GetYFromPolynomial(const std::vector<Data>& data, const std::vector<double>& coeffs) {
    std::vector<double> y;
    y.reserve(data.size());
    for (auto point : data) {
        y.emplace_back(CalcY(coeffs, point.x));
    }
    return y;
}

void TestGetSolve() {
    size_t max_power = 2;
    std::vector<Data> data(5);
    RandomPolynomial polynom(max_power);

    for(size_t i = 0; i < data.size(); ++i) {
        data[i].x = i + 1;
        data[i].y = polynom(data[i].x);
    }

    Approximator app;
    app.SetData(data);
    auto res = app.GetPolynomCoeffs(max_power);

    if (res) {
        std::cout << "Result coefficients: "s << res.value() << std::endl;
    } else {
        std::cout << "Solve not found:(";
    }

    data = app.GetData();
    std::cout << "Source coefficients: "s << polynom.polynom_coeff << std::endl;
    //assert(polynom == res.value());
    std::cout << "Source data:\n"s << data << std::endl;
    std::cout << "Calc data:\n"s << GetYFromPolynomial(data, res.value()) << std::endl;

    std::cout << "SSE = "s << app.GetSumSquaredErrors() << std::endl;
}

void TestGetSolve2() {
    std::vector<Data> data(4);

    for(size_t i = 0; i < data.size(); ++i) {
        data[i].x = 1 + i;
        data[i].y = SquareFunc(data[i].x);
    }

    Approximator app;
    app.SetData(data);
    auto res = app.GetPolynomCoeffs(2);

    if (res) {
        std::cout << res.value() << std::endl;
    } else {
        std::cout << "Solve not found:(";
    }

    data = app.GetData();
    std::cout << "Source data:\n"s << data << std::endl;
    std::cout << "Calc data:\n"s << GetYFromPolynomial(data, res.value()) << std::endl;
    
}

int main() {
    TestGetSolve();
    //TestGetSolve2();

}