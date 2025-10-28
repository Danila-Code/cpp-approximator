#pragma once

#include "approximator.h"
#include "graph_renderer.h"

#include <iostream>

class ApproximatorManager {
public:
    explicit ApproximatorManager(Approximator& app, renderer::GraphRenderer& renderer)
        : app_{app}, renderer_{renderer} {
    }
    // output graph of polynomial in svg format to out stream
    void RenderGraph(std::ostream& out) const;
    // output polynomial in string format to out stream
    std::string GetPolynomial() const;

private:

    std::vector<Data> GenerateData(double min_x, double max_x, size_t count) const;

    Approximator& app_;
    renderer::GraphRenderer& renderer_;
};