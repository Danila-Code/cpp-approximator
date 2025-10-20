#include "graph_renderer.h"

namespace renderer {
// add source data to the svg doc
void GraphRenderer::AddSourcePoints(svg::Document& doc, const ScreenProjector& proj,
        const std::vector<Data>& points) const {
    for (auto point : points) {
        doc.Add(svg::Circle()
            .SetCenter(proj(point))
            .SetRadius(settings_.radius)
            .SetFillColor(settings_.circle_color));
    }
}

// adds a polyline to the doc from the points of the polynomial
void GraphRenderer::AddGraphPolyline(svg::Document& doc, const ScreenProjector& proj,
        const std::vector<Data>& points) const {
    svg::Polyline graph;

    graph.SetStrokeWidth(settings_.line_width)
         .SetStrokeColor(settings_.line_color)
         .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
         .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
         .SetFillColor(svg::NoneColor);

    for (auto point : points) {
        graph.AddPoint(proj(point));
    }
    doc.Add(graph);
}

svg::Document GraphRenderer::Render(const std::vector<Data>& source_points,
                                    const std::vector<Data>& result_points) const {
    ScreenProjector proj(result_points, settings_);

    svg::Document doc;

    AddGraphPolyline(doc, proj, result_points);
    AddSourcePoints(doc, proj, source_points);

    return doc;
}
}  // namespace renderer