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

// add lines of coordinates axis
void GraphRenderer::AddAxis(svg::Document& doc,
                            const ScreenProjector& proj,
                            const std::vector<Data>& points) const {
    using namespace std::literals;
    svg::Point left(settings_.padding, settings_.height / 2);
    svg::Point right(settings_.width - settings_.padding, settings_.height / 2);

    doc.Add(svg::Line()
                       .SetPoint1(left)
                       .SetPoint2(right)
                       .SetStrokeColor("Black"s)
    );
    svg::Point up(settings_.width / 2, settings_.padding);
    svg::Point down(settings_.width / 2, settings_.height - settings_.padding);

    doc.Add(svg::Line()
                       .SetPoint1(up)
                       .SetPoint2(down)
                       .SetStrokeColor("Black"s)
    );

    // left border
    doc.Add(svg::Line()
                       .SetPoint1({settings_.padding,
                                   settings_.padding})
                       .SetPoint2({settings_.padding,
                                   settings_.height - settings_.padding})
                       .SetStrokeColor("Black"s)
    );
    // right border
    doc.Add(svg::Line()
                       .SetPoint1({settings_.width - settings_.padding,
                                   settings_.padding})
                       .SetPoint2({settings_.width - settings_.padding,
                                   settings_.height - settings_.padding})
                       .SetStrokeColor("Black"s)
    );
    // top border
    doc.Add(svg::Line()
                       .SetPoint1({settings_.padding,
                                   settings_.padding})
                       .SetPoint2({settings_.width - settings_.padding,
                                   settings_.padding})
                       .SetStrokeColor("Black"s)
    );
    // bottom border
    doc.Add(svg::Line()
                       .SetPoint1({settings_.padding,
                                   settings_.height - settings_.padding})
                       .SetPoint2({settings_.width - settings_.padding,
                                   settings_.height - settings_.padding})
                       .SetStrokeColor("Black"s)
    );

}

svg::Document GraphRenderer::Render(const std::vector<Data>& source_points,
                                    const std::vector<Data>& result_points) const {
    ScreenProjector proj(result_points, settings_);

    svg::Document doc;

    if (settings_.draw_axis) {
        AddAxis(doc, proj, result_points);
    }
    AddGraphPolyline(doc, proj, result_points);
    AddSourcePoints(doc, proj, source_points);

    return doc;
}
}  // namespace renderer