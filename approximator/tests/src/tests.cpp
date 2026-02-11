#include <gtest/gtest.h>

#include "approximator.h"

TEST(TestApproximator, PolynomCoeffs) {
    double min_x = -5;
    double max_x = 5;
    size_t count = 10;

    Polynomial polynom({1, 1, 1});

    auto generate_data = [&polynom](double min, double max, size_t count) {
        std::vector<Data> data(count);
        double step = (max - min) / count;
        double next = min;
        for (auto& point : data) {
            point.x = next;
            point.y = polynom(point.x);
            next += step;
        }
        return data;
    };

    std::vector<Data> data = generate_data(min_x, max_x, count);

    Approximator app;
    app.SetData(data);
    app.ApproximateData();

    auto res = app.GetPolynom();

    EXPECT_EQ(polynom.coeffs, res.coeffs);
}
