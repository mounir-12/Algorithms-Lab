#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
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

struct Block {
	int left;
	int right;
	Block(int l, int r): left(l), right(r) {
		if(r < l) {
			left = r; right = l;
		}
	}
};

bool operator<(Block b1, Block b2) {
	return b1.left < b2.left;
}

void testcase() {
	int w, n; std::cin >> w >> n; // w = width of the wall, n = #blocks
	
	std::vector<Block> blocks;
	for(int i=0; i<n; i++) {
		int l, r; std::cin >> l >> r; // left and right coordinates of the block i
		blocks.push_back(Block(l,r));
	}
	
	Graph G(2*(w+1)); // we represent each of the coordinates 0...w by 2 vertices i and i+1 linked by an edge of capacity 1
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	//ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

	for(int i=0; i<=w; i++) { // link 2 vertices of each coordinate position by edge of capacity 1
		eaG.addEdge(2*i, 2*i+1, 1); // e,g coordinate 0 has vertices 0 and 1, coord 1 has vertices 2 and 3 ...
		// this is to make sure that no coordinate position is used up more than once (to avoid having 2 crack vertically aligned)
		// vertex 1 is the right vertex of coord 0, vertex 0 is its left vertex
	}
	
	for(int i=0; i<n; i++) {
		Block b = blocks[i];
		// each block is represented by an edge from its left to right coord
		// each block can move the flow from the right vertex of its left coord to the left vertex of its right coord
		eaG.addEdge(2*b.left+1, 2*b.right, 1);
	}
	
	// max_flow finding
	// the source is the right vertex of the coord 0, the sink is the left vertex of coord w
	// only the coordinates 1 ... w-1 are limited by 1 crack (i,e 1 flow from left to right vertex)
	// that's why we want the flow that starts at the right vertex of coord 0 and stops at the left vertex of coord w
	long max_flow = boost::push_relabel_max_flow(G, 1, 2*w);
	
	std::cout << max_flow << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
