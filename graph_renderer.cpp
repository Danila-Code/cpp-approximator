#include "graph_renderer.h"

svg::Document GraphRenderer::Render(const std::vector<Data>& source_points,
                                    const std::vector<Data>& result_points) const {
    ScreenProjector proj(result_points, settings_);

    svg::Document doc;
    svg::Polyline graph;

    graph.SetStrokeWidth(settings_.line_width)
         .SetStrokeColor(settings_.line_color)
         .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
         .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND)
         .SetFillColor(svg::NoneColor);

    for (auto point : result_points) {
        graph.AddPoint(proj(point));
    }
    doc.Add(graph);
    return doc;
}