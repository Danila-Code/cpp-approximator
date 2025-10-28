#pragma once

#include "approximator_manager.h"
#include "json.h"

class JsonReader {
public:
    explicit JsonReader(std::istream& input);
    // set source data to Approximator
    void SetData(Approximator& app) const;
    // output result of approximation to output stream
    void ReturnResult(ApproximatorManager& manager, std::ostream& output) const;

private:
    // return vector with data from json ddictionary
    std::vector<Data> GetSourceData() const;
    // return polynomial degree from json dictionary
    double GetPolynomDegree() const;


    json::Dict dict_;

};