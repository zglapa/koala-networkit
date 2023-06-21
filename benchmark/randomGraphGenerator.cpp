#include <cassert>
#include <iostream>
#include <map>

#include <coloring/EnumerationVertexColoring.hpp>
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
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <nodes>"
                  << " <probability>" << std::endl;
        return 1;
    }
    int n = std::stoi(argv[1]);
    double p = std::stod(argv[2]);
    NetworKit::Graph G = generateRandomGraph(n, p);
    std::string graph = Koala::G6GraphWriter().writeline(G);
    std::cout << graph << std::endl;
    // print_graph_in_human_readable(G);
    return 0;
}
