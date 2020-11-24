#include <iostream>
#include <cstdlib>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL typedefs
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    }
};

void testcase() {
	// each buyer is a vertex and each site is a vertex, each buyer vertex connects to every site vertex
	// we want to assign each buyer to 1 site (so the edge capacities from buyers to sites are 1) such that
	// we maximize the total profit since every assignement comes with some profit
	// So this is a min cost max flow problem with the cost of an edge from buyer i to site j is -(bid_of_i_on_j)
	// To avoid having negative costs, we could find the max_bid and replace -(bid_of_i_on_j) by (max_bid - bid_of_i_on_j)
	// To limit the number of sites sold for a certain state, we add a vertex from every site to the corresponding state
	// with capacity 1 and cost 0 (i,e doesn't contribute to the min cost) then add a vertex from the source s to every buyer
	// with capacity 1 and cost 0 and from every state k to the the target t with capacity l_k = "limit for state k" and cost 0
	
	int N, M, S; std::cin >> N >> M >> S; // N=#buyers, M=#sites, S=#states
	Graph G(N+M+S); // vertices 0...N-1 for buyers, vertices N...N+M-1 for sites and vertices N+M...N+M+S-1 for states
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    Vertex source = boost::add_vertex(G);
    Vertex target = boost::add_vertex(G);
    
    for(int k=M+N; k<M+N+S; k++) { // connect states to target
		int l; std::cin >> l; // read the limit for state i
		eaG.addEdge(k, target, l, 0); // add edge from state to target with capacity l and cost 0
	}
	
	for(int j=N; j<N+M; j++) { // assign each site to its state
		int s; std::cin >> s; s--; // s-- for 0 based index
		int k = N+M+s; // state vertex index
		eaG.addEdge(j, k, 1, 0); // add edge from site to state with capacity 1 and cost 0
	}
	
	int bids[N][M]; // bids matrix
	int max_bid = INT_MIN;
	
	for(int i=0; i<N; i++) { // read all bids and find max_bid
		for(int j=0; j<M; j++) {
			int b; std::cin >> b; bids[i][j] = b; // bid of buyer i for site j
			max_bid = std::max(max_bid, b);
		}
	}
	
	for(int i=0; i<N; i++) { // connect source to buyers and buyers to sites
		eaG.addEdge(source, i, 1, 0); // add edge from source to buyer i of capacity 1 and cost 0
		for(int j=0; j<M; j++) {
			eaG.addEdge(i, N+j, 1, max_bid - bids[i][j]); // add edge from buyer i to site j of capacity 1 and cost "max_bid - bid_of_i_on_j"
		}
	}
	
	// run min cost max flow algorithm with non-negative weights
	boost::successive_shortest_path_nonnegative_weights(G, source, target);
	
    int cost = boost::find_flow_cost(G);
    int flow = 0; // max flow = number of successful assignements
    
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(source, G); e != eend; ++e) {
        flow += capacitymap[*e] - rescapacitymap[*e];
    }
    
    int profit = flow * max_bid - cost; // maximum profit (subtract the added max_bid for each assignement performed by the max_flow)
    std::cout << flow << " " << profit << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
