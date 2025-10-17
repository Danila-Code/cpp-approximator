#include "approximator_manager.h"

void ApproximatorManager::RenderGraph(std::ostream& out) const {
    auto source_points = app_.GetData();
    renderer_.Render(source_points).Render(out);
}

std::vector<Data> ApproximatorManager::GenerateData() const {
    std::vector<Data> res;

    return res;
}