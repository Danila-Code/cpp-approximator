#pragma once

#include "approximator.h"
#include "svg.h"

#include <algorithm>
#include <vector>


struct RenderSettings {
    // picture size
    double width{};
    double height{};

    double padding{};  // padding from edges
    double line_width{};  // width of line

    svg::Color line_color{};  // color of line
};

namespace {
inline const double EPSILON = 1e-6;
inline bool IsZero(double value) {
    return std::abs(value) < EPSILON;
}

// class convert graph coordinates to screen coordinates
class ScreenProjector {
public:
    explicit ScreenProjector(const std::vector<Data>& points, const RenderSettings& settings)
            : padding_{settings.padding} {
        if (!points.size()) {
            return;
        }

        // find points with min and max x ccordinate
        const auto [iter_left, iter_right] = std::minmax_element(points.begin(), points.end(),
            [](Data lhs, Data rhs) {
                return lhs.x < rhs.x;
        });
        min_x_ = iter_left->x;
        const double max_x = iter_right->x; 

        // find points with min and max y ccordinate
        const auto [iter_bottom, iter_top] = std::minmax_element(points.begin(), points.end(),
            [](Data lhs, Data rhs) {
                return lhs.y < rhs.y;
        });
        min_y_ = iter_bottom->y;
        const double max_y = iter_top->y;

        std::optional<double> width_zoom_coef;
        if (!IsZero(max_x - min_x_)) {
            width_zoom_coef = (settings.width - 2 * settings.padding) / (max_x - min_x_);
        }

        std::optional<double> height_zoom_coef;
        if (!IsZero(max_y - min_y_)) {
            height_zoom_coef = (settings.height - 2 * settings.padding) / (max_y - min_y_);
        }

        if (width_zoom_coef && height_zoom_coef) {
            zoom_coef_ = std::min(*width_zoom_coef, *height_zoom_coef);
        } else if (width_zoom_coef) {
            zoom_coef_ = *width_zoom_coef;
        } else if (height_zoom_coef) {
            zoom_coef_ = *height_zoom_coef;
        }
    }

    // convert to screen coordinates
    svg::Point operator()(Data point) const {
        return {
            (point.x - min_x_) * zoom_coef_ + padding_,
            (point.y - min_y_) * zoom_coef_ + padding_
        };
    }

private:
    double min_x_;
    double min_y_;
    double padding_;
    double zoom_coef_;
};
}  // namespace

class GraphRenderer {
public:
    explicit GraphRenderer(const RenderSettings& settings) : settings_{settings} {
    }

    svg::Document Render(const std::vector<Data>& source_points,
                         const std::vector<Data>& result_points) const;

private:
    RenderSettings settings_;
};