#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;

class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

void testcase() {
	// analogy to bank accounts:
	// each vertex v has a balance b_v, an amount to spend s_v on itself and each edge e=(v,u) imposes
	// a min_e and max_e amount to transfer from v to u
	// the balance could go negative (debt), vertices can spend and transfer money whatever the balance state is (negative or positive)
	// each vertex first finishes it spendings using its own balance, so b_v = b_v - s_v
	// for each edge e=(v,u), vertex v transfers to u min_e, so b_v = b_v - min_e and b_u = b_u + min_e
	// and max_e = max_e - min_e (take into account the transfered amount)
	// after all spendings and transfers, some vertices have positive balance (rich)
	// others have negative balance (poor). The rich vertices remove debt from poor ones through sending money (flow)
	// using the graph.
	// So we create a source "s" and a target "t", connect rich vertices to "s" and poor vertices to "t"
	// The capacity of added edges (s,v) and (v,t) are |b_v| (absolute value of b_v)
	// if the max_flow (i,e max amount transferable from rich to poor through the graph) is equal to
	// the total amount of debt then "yes" otherwise "no"
	int l, p; std::cin >> l >> p; // l=#locations, p=#paths
	
	Graph G(l); // vertices 0...l-1 for the l locations
	
	std::vector<std::pair<int, int>> demands_supplies(l); // each vertex has some demand and some supply of soldiers

	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
	
	std::vector<int> balances(l);
	
	for(int i=0; i<l; i++) {
		int g, d; std::cin >> g >> d; // g=#soldiers available in location i, d=#soldiers needed in location i
		balances[i] = g-d;
	}
	
	for(int i=0; i<p; i++) {
		int v, u, min, max; std::cin >> v >> u >> min >> max;
		balances[v] -= min; // v sends to u
		balances[u] += min; // u receives from v
		eaG.addEdge(v, u, max-min); // modify the max transferable amount from v to u 
	}
	
	Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);
	
	int total_demand = 0;
	for(int i=0; i<l; i++) {
		int balance = balances[i];
		if(balance >= 0) {
			eaG.addEdge(source, i, balance);
		} else {
			eaG.addEdge(i, target, -balance);
			total_demand += (-balance);
		}
	}
	
	// max amount supplied, might be greater than the total demand
	long max_flow = boost::push_relabel_max_flow(G, source, target);
	
	if(max_flow == total_demand) { // suppliers supply at least the total demanded amount
		std::cout << "yes" << std::endl;
	} else {
		std::cout << "no" << std::endl;
	}

}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
