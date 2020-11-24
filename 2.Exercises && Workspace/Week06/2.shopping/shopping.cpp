#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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
	int n, m, s; std::cin >> n >> m >> s; // n=#intersections, m=#streets, s=#stores
	std::vector<int> stores(s);
	for(int i=0; i<s; i++) {
		std::cin >> stores[i]; // save the index of vertex representing the store
	}
	
	Graph G(n+2*m); // n intersection + 2 vertices per street (since we convert an undirected edge (street) to a diamond)
	// i,e for each undirected edge {u,v}, we create 2 vertices w and x, remove the edge {u,v} and add the
	// directed edges (u, w) , (w, x), (x, v), (v, w) and (x, u)
	/*    w
	//   /|\
	//  u | v
	//   \|/
	//    x
	// only 1 flow is allowed to use the edge w->x, once used (by either a flow going u->v or v->u) the passage u->v and v->u is 
	// blocked
	*/
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	//ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
	
	for(int i=0; i<m; i++) {
		int u, v; std::cin >> u >> v;
		int w = m + 2*i;
		int x = m + 2*i +1;
		eaG.addEdge(u, w, 1);
		eaG.addEdge(w, x, 1);
		eaG.addEdge(x, v, 1);
		eaG.addEdge(v, w, 1);
		eaG.addEdge(x, u, 1);
	}
	
	Vertex source = 0;
	Vertex target = boost::add_vertex(G);
	
	for(int i=0; i<s; i++) {
		eaG.addEdge(stores[i], target, 1); // add edge from each store to the target
	}
	
	long max_flow = boost::push_relabel_max_flow(G, source, target);
	
	if(max_flow == s) { // found s edge dijoint paths (1 per store), then "yes"
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
