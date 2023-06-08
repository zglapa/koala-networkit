/*
 * EnumerationVertexColoring.hpp
 *
 * Created on: 07.06.2023
 *   Author: Zofia Glapa (zofia.glapa@student.uj.edu.pl)
 */

#pragma once

#include <map>
#include <networkit/base/Algorithm.hpp>
#include <networkit/graph/Graph.hpp>
#include <optional>
#include <vector>


namespace Koala {

class IndependentSetChecker {

public:
    IndependentSetChecker(const NetworKit::Graph& graph);
    bool isIndependentSet(std::vector<NetworKit::node>& nodes);
    long long int numberOfIndependentSetsNotIntersectingWith(std::vector<NetworKit::node>& nodes);

private:
    const std::optional<NetworKit::Graph> graph;
};

class InclusionExclusionVertexColoring : public NetworKit::Algorithm {
public:
    /**
     * Given an input graph, set up the enumeration vertex coloring procedure.
     *
     * @param graph The input graph.
     */
    InclusionExclusionVertexColoring(const NetworKit::Graph& graph);

    /**
     * Return the coloring found by the algorithm.
     *
     * @return a map from nodes to colors.
     */
    const std::map<NetworKit::node, int> getColoring() const;
    int getChromaticNumber() const;
    void run();

protected:
    const std::optional<NetworKit::Graph> graph;
    std::vector<int> best_solution;
    bool is_k_colorable(int k, const NetworKit::Graph& G);
    int calculateChromaticNumber(const NetworKit::Graph& G);
    int chromatic_number;
    void contractNodes(std::map<int, std::vector<int>>& contractedNodes, int u, int v);
};

} /* namespace Koala */