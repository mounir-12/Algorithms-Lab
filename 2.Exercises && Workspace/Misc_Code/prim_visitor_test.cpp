#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

// BGL typedefs
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
		boost::property<boost::edge_weight_t, std::pair<int, int>>> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

class myPrimVisitor {
public:
	std::vector<std::pair<int, int>>& distmap;
    myPrimVisitor(std::vector<std::pair<int, int>>& distmap): distmap(distmap) {}
    
    // called before storing initial distances
    void initialize_vertex(Vertex u, Graph g) {}
	
	// called when vertex poped from queue
    void examine_vertex(Vertex u, Graph g) {
		std::cout << "popped vertex: " << u << std::endl;
	}
	
	// called on every out edge of the popped vertex 
	void examine_edge(Edge e, Graph g) {
		std::cout << "examining edge: " << e << std::endl;
	}
	
	void discover_vertex(Vertex u, Graph g) {}
	
	// called on edge e=(u,v) if weight[e] < dist[v] (i,e edge e connect us to v with lower cost/distance/weight)
	void edge_relaxed(Edge e, Graph g) {
		//Vertex u = boost::source(e, g);
		//Vertex v = boost::target(e, g);
		std::cout << "relaxed edge: " << e << std::endl;
	}
	
	void edge_not_relaxed(Edge e, Graph g) {
		std::cout << "not relaxed edge: " << e << std::endl;
	}
	
	// called after all out edges of vertex u are examined
	void finish_vertex(Vertex u, Graph g) {}
	
};

std::pair<int, int> combine(std::pair<int, int> val1, std::pair<int, int> val2) {
	std::cout << "combining -- val1: (" << val1.first << ", " << val1.second << "), val2: (" << val2.first << ", " << val2.second << ")" << std::endl;
	return val2;
}

bool compare(std::pair<int, int> val1, std::pair<int, int> val2) {
	std::cout << "comparing -- val1: (" << val1.first << ", " << val1.second << "), val2: (" << val2.first << ", " << val2.second << ")" << std::endl;
		
	if(val1.first == val2.first) { // same weight
		return val1.second > val2.second; // the smallest is the most important
	}
	return val1.first < val1.second; // else, the smallest is the one with smaller edge weight
}

void print_infos(int V, Graph& G, WeightMap& weightmap, Vertex start, std::vector<Vertex>& primpredmap) {
	std::cout << "Prim builds a minimum spanning tree (of node " << start << "'s component) of total weight: \n";
	int totalweight = 0;
	for (int i = 0; i < V; ++i) {
		std::cout << "pred of: " << i << "is: " << primpredmap[i] << std::endl;
		/*if (primpredmap[i] != V) {
			Edge e; bool success;
			boost::tie(e, success) = boost::edge(i, primpredmap[i], G); // recall: graph is undirected, so we can swap i and primpredmap[i]
			totalweight += weightmap[e].first;
		}*/
	}
	
	std::cout << totalweight << "\n";
	std::cout << "Edges in Prim's minimum spanning:\n";	
	// iterate over all vertices
	for (int i = 0; i < V; ++i) {
		// OutEdgeIterators -- but recall, the graph is undirected, so outedge is in fact just an edge (no arrow)
		// ================
		Vertex u = i;
		OutEdgeIt oebeg, oeend;
		// check all neighbors of u which one is its predecessor
		for (boost::tie(oebeg, oeend) = boost::out_edges(u, G); oebeg != oeend; ++oebeg) {
			Vertex v = boost::target(*oebeg, G);	// source(*oebeg, G) is guaranteed to be u, even in an undirected graph.
			if (primpredmap[u] == v) {
				std::cout << u << " -- " << v << " (weight " << weightmap[*oebeg].first << ")\n";
			}
		}
	}
	std::cout << std::endl;
}

int main() {
	// Create Graph, Vertices and Edges (from BGL tutorial code)
	// =========================================================
	int V = 4;
	Graph G(V);	// creates an empty graph on n vertices
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	std::vector<std::vector<int> > graphedges = {{0,1,2}, {0,2,2}, {1,3,3}, {2,3,3}}; // undirected
	int E = graphedges.size();
	for (int i = 0; i < E; ++i) { // add graph edges
		Edge e;	bool success;
		Vertex u = graphedges[i][0];
		Vertex v = graphedges[i][1];
		int w = graphedges[i][2];
		// add undirected edge
		boost::tie(e, success) = boost::add_edge(u, v, G);
		weightmap[e] = std::make_pair(w, v);
	}
	
	// Prim minimum spanning tree
	// ==========================
	std::vector<Vertex> primpredmap(V);
	std::vector<std::pair<int, int>> distmap(V);
	Vertex start = 0;
	//myPrimVisitor vis(distmap);
	
	boost::dijkstra_shortest_paths(G, start ,boost::distance_map(&distmap[0]).distance_zero(std::make_pair(0, V))
	.distance_inf(std::make_pair(INT_MAX, -1)).distance_combine(&combine).distance_compare(&compare));
	std::cout << "done" << std::endl;
	print_infos(V, G, weightmap, start, primpredmap);
	return 0;	
}

