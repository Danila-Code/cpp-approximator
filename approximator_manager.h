#pragma once

#include "approximator.h"
#include "graph_renderer.h"

#include <iostream>


class ApproximatorManager {
public:
    explicit ApproximatorManager(Approximator& app, GraphRenderer& renderer)
        : app_{app}, renderer_{renderer} {
    }

    void Render(std::ostream& out) const;
private:
    Approximator& app_;
    GraphRenderer& renderer_;
};