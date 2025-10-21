#include "approximator_manager.h"

void ApproximatorManager::RenderGraph(std::ostream& out) const {
    auto source_points = app_.GetData();
    auto [iter_min, iter_max] = std::minmax_element(source_points.begin(), source_points.end(),
        [](Data lhs, Data rhs) {
            return lhs.x < rhs.x;
    });

    double min_x = iter_min->x;
    double max_x = iter_max->x;
    double padding = (max_x - min_x) * 0.1;

    auto result_points = GenerateData(min_x - padding, max_x + padding, 1000);

    renderer_.Render(source_points, result_points).Render(out);
}

std::vector<Data> ApproximatorManager::GenerateData(double min_x, double max_x, size_t count) const {
    std::vector<Data> points(count);
    const Polynomial polynom = app_.GetPolynom();

    const double step = (max_x - min_x) / (count - 1);
    double next = min_x;

    for (Data& point : points) {
        point.x = next;
        point.y = polynom(point.x);
        next += step;
    }
    return points;
}