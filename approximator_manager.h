#pragma once

#include "approximator.h"
#include "graph_renderer.h"

#include <iostream>



class ApproximatorManager {
public:
    explicit ApproximatorManager(Approximator& app, renderer::GraphRenderer& renderer)
        : app_{app}, renderer_{renderer} {
    }

    void RenderGraph(std::ostream& out) const;
private:

    std::vector<Data> GenerateData(double min_x, double max_x, size_t count) const;

    Approximator& app_;
    renderer::GraphRenderer& renderer_;
};