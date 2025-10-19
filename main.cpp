#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>

#include "approximator_manager.h"
#include "graph_renderer.h"

using namespace std::literals;

std::ostream& operator<<(std::ostream& out, Data point) {
    out << "( "s << point.x << ", "s << point.y << " )"s;
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec) {
    for (auto item : vec) {
        out << item << std::endl;
    } 
    return out;
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

    std::vector<Data> GenerateData(double min, double max, size_t count) const {
        std::vector<Data> data(count);
        double step = (max - min) / (count - 1);
        double next = min;
        for (auto& point : data) {
            point.x = next;
            point.y = (*this)(point.x);
            next += step;
        }
        return data;
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
                //item = static_cast<int>(dis(gen));
                item = dis(gen);
        });
        polynom_coeff = std::move(vec);
    }
};


void TestGetSolve() {
    size_t max_power = 2;

    double min_x = -10;
    double max_x = 10;
    size_t count = 10;

    RandomPolynomial polynom(max_power);
    std::vector<Data> data = polynom.GenerateData(min_x, max_x, count);

    Approximator app;
    app.SetData(data);
    auto res = app.GetPolynom(max_power);

    if (res) {
        std::cout << "Result coefficients:\n"s << res.value().coeffs << std::endl;
    } else {
        std::cout << "Solve not found:(";
    }

    data = app.GetData();
    std::cout << "Source coefficients:\n"s << polynom.polynom_coeff << std::endl;
    std::cout << "Source data:\n"s << data << std::endl;

    std::cout << "SSE = "s << app.GetSumSquaredErrors() << std::endl;
}

void TestRendering() {
    size_t max_power = 2;

    double min_x = -5;
    double max_x = 5;
    size_t count = 10;

    RandomPolynomial polynom(max_power);
    std::vector<Data> data = polynom.GenerateData(min_x, max_x, count);

    Approximator app;
    app.SetData(data);
    auto res = app.GetPolynom(max_power);

    RenderSettings settings{
        .width = 1000,
        .height = 1000,
        .padding = 10,
        .line_width = 1,
        .line_color = svg::Color("Black"s)
    };
    GraphRenderer renderer(settings);
    ApproximatorManager app_manager(app, renderer);

    std::ofstream out("graph_1.svg");
    app_manager.RenderGraph(out);

    if (res) {
        std::cout << "Result coefficients:\n"s << res.value().coeffs << std::endl;
    } else {
        std::cout << "Solve not found:(";
    }

    data = app.GetData();
    std::cout << "Source coefficients:\n"s << polynom.polynom_coeff << std::endl;
    std::cout << "Source data:\n"s << data << std::endl;

    std::cout << "SSE = "s << app.GetSumSquaredErrors() << std::endl;
}

int main() {
    //TestGetSolve();
    TestRendering();

}