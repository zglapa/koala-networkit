#include <cassert>
#include <iostream>
#include <map>

#include <coloring/EnumerationVertexColoring.hpp>
#include <io/DimacsGraphWriter.hpp>
#include <io/G6GraphWriter.hpp>
#include <networkit/generators/ErdosRenyiGenerator.hpp>

NetworKit::Graph generateRandomGraph(int n, double p) {
    NetworKit::ErdosRenyiGenerator generator(n, p);
    return generator.generate();
}

void print_graph_in_human_readable(NetworKit::Graph G) {
    std::cerr << "n = " << G.numberOfNodes() << std::endl;
    std::cerr << "m = " << G.numberOfEdges() << std::endl;
    std::cerr << "edges list:" << std::endl;
    G.forEdges(
    [&](NetworKit::node u, NetworKit::node v) { std::cerr << u << " " << v << std::endl; });
}


int main(int argc, const char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <nodes>"
                  << " <probability>"
                  << "<output file>" << std::endl;
        return 1;
    }
    int n = std::stoi(argv[1]);
    double p = std::stod(argv[2]);
    std::string output_file = argv[3];
    NetworKit::Graph G = generateRandomGraph(n, p);
    Koala::DimacsGraphWriter().write(G, output_file);
    // print_graph_in_human_readable(G);
    return 0;
}
