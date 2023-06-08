#include <coloring/InclusionExclusionVertexColoring.hpp>
#include <iostream>
#include <map>
#include <optional>
#include <vector>
namespace Koala {
IndependentSetChecker::IndependentSetChecker(const NetworKit::Graph& graph)
: graph(std::make_optional(graph)) {
}
bool IndependentSetChecker::isIndependentSet(std::vector<NetworKit::node>& nodes) {
    if (!graph.has_value()) {
        throw std::runtime_error("Graph is not set.");
    }
    for (auto i = nodes.begin(); i != nodes.end(); ++i) {
        for (auto j = i + 1; j != nodes.end(); ++j) {
            if (graph->hasEdge(*i, *j)) {
                return false;
            }
        }
    }
    return true;
}
long long int IndependentSetChecker::numberOfIndependentSetsNotIntersectingWith(
std::vector<NetworKit::node>& nodes) {

    if (!graph.has_value()) {
        throw std::runtime_error("Graph is not set.");
    }
    std::unordered_set<int> nodesInX(nodes.begin(), nodes.end());
    std::vector<NetworKit::node> nodesWithoutX;
    for (int i = 0; i < graph->numberOfNodes(); i++) {
        if (nodesInX.find(i) == nodesInX.end())
            nodesWithoutX.push_back(i);
    }
    long long int result = 0;
    for (int i = 0; i < (1 << nodesWithoutX.size()); i++) {
        std::vector<NetworKit::node> subset;
        for (int j = 0; j < nodesWithoutX.size(); j++) {
            if (i & (1 << j)) {
                subset.push_back(nodesWithoutX[j]);
            }
        }
        if (isIndependentSet(subset)) {
            result++;
        }
    }
    return result;
}

InclusionExclusionVertexColoring::InclusionExclusionVertexColoring(const NetworKit::Graph& graph)
: graph(std::make_optional(graph)) {
}

const std::map<NetworKit::node, int> InclusionExclusionVertexColoring::getColoring() const {
    assureFinished();
    std::map<NetworKit::node, int> coloring;
    for (int i = 0; i < graph->numberOfNodes(); i++) {
        coloring[i] = best_solution[i];
    }
    return coloring;
}

bool InclusionExclusionVertexColoring::is_k_colorable(int k, const NetworKit::Graph& G) {
    long long int numberOfKCovers = 0;
    IndependentSetChecker independentSetChecker(G);
    std::vector<NetworKit::node> X;
    for (int i = 0; i < (1 << G.numberOfNodes()); i++) {
        X.clear();
        for (int j = 0; j < G.numberOfNodes(); j++) {
            if (i & (1 << j)) {
                X.push_back(j);
            }
        }
        long long int result = ((X.size() % 2) ? -1 : 1) *
        pow(independentSetChecker.numberOfIndependentSetsNotIntersectingWith(X), k);
        numberOfKCovers += result;
    }
    return numberOfKCovers > 0;
}

int InclusionExclusionVertexColoring::calculateChromaticNumber(const NetworKit::Graph& G) {
    int n = G.numberOfNodes();
    int l = 1;
    int r = n - 1;
    auto solution = std::vector<int>(n, 0);
    auto chromaticNumber = 0;
    if (is_k_colorable(l, G)) {
        solution = std::vector<int>(n, 1);
        chromaticNumber = 1;
    } else if (!is_k_colorable(r, G)) {
        for (int i = 0; i < n; i++) {
            solution[i] = i + 1;
        }
        chromaticNumber = n;
    } else {
        while (l + 1 < r) {
            int m = (l + r) / 2;
            if (is_k_colorable(m, G)) {
                r = m;
            } else {
                l = m + 1;
            }
        }
        if (is_k_colorable(l, G)) {
            solution = std::vector<int>(n, l);
            chromaticNumber = l;
        } else {
            solution = std::vector<int>(n, r);
            chromaticNumber = r;
        }
    }
    return chromaticNumber;
}

void InclusionExclusionVertexColoring::contractNodes(
std::map<int, std::vector<int>>& contractedNodes,
int u,
int v) {
    if (contractedNodes.find(u) == contractedNodes.end()) {
        contractedNodes[u] = std::vector<int>();
    }
    contractedNodes[u].push_back(v);
}

void InclusionExclusionVertexColoring::run() {
    int freeColor = 1;
    int numberOfNodes = graph->numberOfNodes();
    std::map<int, std::vector<int>> contractedNodes;
    best_solution = std::vector<int>(numberOfNodes, 0);
    auto G = *graph;
    while (numberOfNodes > 1) {
        NetworKit::node v = 0;
        G.forNodes([&](NetworKit::node u) {
            v = u;
            return;
        });
        std::vector<NetworKit::node> notNeighbours;
        NetworKit::Graph H(G);

        int k = 0;
        H.forNodes([&](NetworKit::node u) {
            if (u != v && !H.hasEdge(u, v)) {
                H.addEdge(u, v);
                notNeighbours.push_back(u);
                k++;
            }
        });
        int chromaticNumberG = calculateChromaticNumber(G);
        int chromaticNumberH = calculateChromaticNumber(H);
        if (chromaticNumberG == chromaticNumberH) {
            G.removeNode(v);
            best_solution[v] = freeColor++;
        } else {
            int l = 0;
            int r = k;
            int u1 = notNeighbours[0];
            G.addEdge(v, u1);
            if (calculateChromaticNumber(G) == chromaticNumberG + 1) {
                contractNodes(contractedNodes, u1, v);
                G.forNeighborsOf(v, [&](NetworKit::node u) {
                    if (u != u1) {
                        G.addEdge(u, u1, 1.0, true);
                    }
                });
                G.removeNode(v);
            } else {
                int prev_m = 0, m = 0, um = 0;
                while (l + 1 < r) {
                    m = (l + r) / 2;
                    um = notNeighbours[m];
                    if (m > prev_m) {
                        for (int i = prev_m + 1; i <= m; i++) {
                            G.addEdge(v, notNeighbours[i]);
                        }
                    } else {
                        for (int i = m + 1; i <= prev_m; i++) {
                            G.removeEdge(v, notNeighbours[i]);
                        }
                    }
                    prev_m = m;
                    if (calculateChromaticNumber(G) == chromaticNumberG) {
                        l = m;
                    } else {
                        r = m;
                    }
                }
                G.forNeighborsOf(v, [&](NetworKit::node u) {
                    if (u != um) {
                        G.addEdge(u, um, 1.0, true);
                    }
                });
                G.removeNode(v);
                contractNodes(contractedNodes, um, v);
            }
        }
        numberOfNodes--;
    }

    for (auto& pair : contractedNodes) {
        if (best_solution[pair.first] == 0)
            best_solution[pair.first] = freeColor++;
        for (auto& node : pair.second) {
            best_solution[node] = best_solution[pair.first];
        }
    }
    for (int i = 0; i < graph->numberOfNodes(); i++) {
        if (best_solution[i] == 0)
            best_solution[i] = freeColor++;
    }
    chromatic_number = calculateChromaticNumber(*graph);
    hasRun = true;
}

int InclusionExclusionVertexColoring::getChromaticNumber() const {
    assureFinished();
    return chromatic_number;
}

} // namespace Koala
