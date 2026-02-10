#include "json_reader.h"

JsonReader::JsonReader(std::istream& input) {
    dict_ = json::Load(input).GetRoot().AsMap();
}

// return vector with data from json ddictionary
std::vector<Data> JsonReader::GetSourceData() const {
    using namespace std::literals;

    if (!dict_.contains("data"s)) {
        return {};
    }
    auto& node = dict_.at("data"s);
    if (!node.IsArray()) {
        return {};
    }
    auto& array = node.AsArray();
    std::vector<Data> data;
    data.reserve(array.size());

    for (const auto& node : array) {
        // добавить проверки!!!!!!!!!!!!!!!!!!!!!!

        data.emplace_back(Data{node.AsArray().at(0).AsDouble(), node.AsArray().at(1).AsDouble()});
    }
    return data;
}
// return polynomial degree from json dictionary
double JsonReader::GetPolynomDegree() const {
    using namespace std::literals;
    if (!dict_.contains("degree"s)) {
        return 0;
    }
    auto& degree_node = dict_.at("degree"s);
    if (!degree_node.IsInt()) {
        return 0;
    }
    return degree_node.AsInt();
}

// set source data to Approximator
void JsonReader::SetData(Approximator& app) const {
    app.SetData(GetSourceData());
    app.SetPolynomDegree(GetPolynomDegree());
}

// output result of approximation to output stream
void JsonReader::ReturnResult(ApproximatorManager& manager, std::ostream& output) const {
    json::Dict res_dict;

    res_dict["polynomial"] = manager.GetPolynomial();

    std::ostringstream graph_stream;
    manager.RenderGraph(graph_stream);
    res_dict["graph"] = graph_stream.str();

    json::Print(json::Document(res_dict), output);
}
