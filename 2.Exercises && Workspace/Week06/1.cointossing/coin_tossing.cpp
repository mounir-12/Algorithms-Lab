#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
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
	int n, m; std::cin >> n >> m; //n = #players, m = #rounds
	
	std::vector<int> scores(n); // scores written down for each player
	std::vector<int> diff(n); // difference between scores and presumed_scores
	
	Graph G(m+n); // one vertex per round and per player, vertices 0...m-1 for round and rest for players
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
	
	Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);
	
	int points_to_assign = 0;
	for(int i=0; i<m; i++) {
		int a, b, c; std::cin >> a >> b >> c;
		if(c == 1) scores[a]++;
		if(c == 2) scores[b]++;
		if(c == 0) {
			eaG.addEdge(source, i, 1); // edge from source to the round i (1 point to be assigned per round)
			eaG.addEdge(i, m+a, 1); // edge from round i to player a
			eaG.addEdge(i, m+b, 1); // adge from round i to player b
			points_to_assign++; // 1 more point to assign
		}
	}
	
	int total_diff = 0; // total_diff to be filled, must be equal to the max flow
	for(int j=0; j<n; j++) {
		int s; std::cin >> s; // presumed score for player j
		diff[j] = s - scores[j]; // score remaining to get to the presumed score
		if(diff[j] < 0) { // impossible
			std::cout << "no" << std::endl;
			for(int k=j+1; k<n; k++) // finish reading 
				std::cin >> s;
			return;
		} else if(diff[j] > 0) {
			eaG.addEdge(m+j, target, diff[j]); // add capacity to be saturated
			total_diff += diff[j];
		}
	}
	
	if(points_to_assign != total_diff) { // can't assign all remaining points
		std::cout << "no" << std::endl;
		return;
	}
	
	// max_flow is bounded by the sum of capacities of outgoing edges from source s, so at most m
	long max_flow = boost::push_relabel_max_flow(G, source, target);

	if(max_flow == total_diff)
		std::cout << "yes" << std::endl;
	else
		std::cout << "no" << std::endl;
	
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
