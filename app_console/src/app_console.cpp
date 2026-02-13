#include <fstream>
#include <iostream>

#include "approximator_manager.h"
#include "graph_renderer.h"
#include "json_reader.h"

using namespace std::literals;

int main() {
    Approximator app;

    renderer::RenderSettings settings{.width = 500,
                                      .height = 500,
                                      .padding = 10,
                                      .line_width = 1,
                                      .radius = 3,
                                      .line_color = svg::Color("Black"s),
                                      .circle_color = svg::Color("Red"s)};
    renderer::GraphRenderer renderer(settings);

    ApproximatorManager manager(app, renderer);

    while (true) {
        std::cout << "Enter input file with data in JSON-format or exit" << std::endl;

        std::string input_file_name;
        std::cin >> input_file_name;

        if (input_file_name == "exit"s) {
            break;
        }

        std::ifstream input_file(input_file_name);

        if (!input_file.is_open()) {
            std::cout << "failed to open " << input_file_name << std::endl;
            continue;
        }

        JsonReader reader(input_file);
        reader.SetData(app);
        app.ApproximateData();
        std::cout << app.GetPolynom().ToString() << std::endl;
    }
}
