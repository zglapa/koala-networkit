#include <cassert>
#include <chrono>
#include <iostream>
#include <map>

#include <coloring/EnumerationVertexColoring.hpp>
#include <io/DimacsGraphReader.hpp>
template <typename T> void benchmark(std::string path) {


    NetworKit::Graph G = Koala::DimacsGraphReader().read(path);
    auto algorithm = T(G);
    algorithm.run();
    auto colors = algorithm.getColoring();
    int max_color = 0;
    G.forEdges([&](NetworKit::node u, NetworKit::node v) { assert(colors[u] != colors[v]); });
    // for (const auto& [v, c] : colors) {
    //     max_color = std::max(max_color, c);
    // }
    // std::cout << "max_color = " << max_color << std::endl;
}

int main(int argc, const char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <algorithm>"
                  << " <input file> " << std::endl;
        return 1;
    }

    std::string path = argv[2];
    switch (std::stoi(argv[1])) {
    case 0: benchmark<Koala::BrownsOrdinaryEnumerationVertexColoring>(path); break;
    case 1: benchmark<Koala::ChristofidesEnumerationVertexColoring>(path); break;
    case 2: benchmark<Koala::BrelazEnumerationVertexColoring>(path); break;
    case 3: benchmark<Koala::KormanEnumerationVertexColoring>(path); break;
    }

    return 0;
}
