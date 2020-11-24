#include <iostream>
#include <cstdlib>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL typedefs
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
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
	// We have n train stations and n-1 directed edges where edge i is between station i and station i+1
	// A train goes from station 0 to station n-1. Each edge i represents a trip i of the train from 
	// station i to i+1. Each edge has capacity l so the train can have at most l passengers (i,e agents)
	// in each trip. We have m agents, each agent j want to travel from some station x_j to station y_j 
	// and moving this agents gives us a profit q_j. Given the limited capacity of the train, we want to
	// choose a set of agents that will get to travel in the train such that we maximize the profit.
	// We add to our graph the edges (x_j, y_j) with profit q_j and capacity 1 for each agent j.
	// Instead of having only 1 train that transports at most l passengers, we create l small trains that
	// transport 1 passenger. Each train starts at station 0 and finishes at station n-1 and they
	// all reach each station at the same moment. Now edge capacities represent how many train
	// can take that edge at the same time. Let us call the edges between the the stations "normal edges"
	// and the added edges (x_j, y_j) the "agent edge". As long as a train moves along the normal edges, then
	// it is seen as empty and gets no profit. If a train decides to take the agent edge (x_j, y_j) then 
	// this means the train transports agent j from x_j to y_j (skipping all stations in between) and gets 
	// profit q_j for that.
	// The problem now is just to decide a route for each train, such as to maximize the total profit of
	// all trains. This is naturally a max-flow max-profit problem where each flow is a train.
	// We could solve the min-cost max-flow problem where each edge's cost is -profit, but since every train
	// goes from station 0 to station n-1, then all trains traverse the same distance with "length of normal
	// edge" = 1 and "length of agent j's edge" = "y_j - x_j" >=1. We offset each unit length by the same amount
	// such as to get positive new_costs only. To be sure that each edge's new_cost > 0, we offset each
	// unit length by max_profit = 2^7 = 128 to account for the "worst case"  where an agent edge has unit length
	// and cost= - max_profit.
	int n, m, l; std::cin >> n >> m >> l; // n=#stations, m=#agents, l=#small_train=capacity_of_the_big_train
	
	Graph G(n+1); // the source is vertex n which pushes l trains to the start station 0
				  // the sink is the vertex n-1 (i,e station n-1 since all trains finish here)
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    //ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    Vertex source = n;
    Vertex target = n-1;
    
    int max_profit = 128; // the offset value we add to each unit length
    
    eaG.addEdge(source, 0, l, 0); // add edge from source to station 0 to limit nb of trains starting at this station to l trains
								  // this actions is just to prepare the train for the start of their journey and has 0 cost
    
    for(int i=0; i<n-1; i++) { // build the "normal edges"
		eaG.addEdge(i, i+1, l, max_profit - 0); // at most l train and no profit (we also add the unit length offset)
	}
	
	for(int j=0; j<m; j++) {
		int x, y, q; std::cin >> x >> y >> q;
		int dist = y-x; // length of the edge (x, y)
		eaG.addEdge(x, y, 1, dist*max_profit - q); // at most 1 train moves the agent j and gets profit q (add max_profit per unit length)
	}
	
	// find min cost
	boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int cost = boost::find_flow_cost(G);
    
    // we know the max-flow is l, so we substract the added offset per unit length to get the profit
    int profit = l * (n-1) * max_profit - cost; // each of the l trains traverses distance (n-1)
	std::cout << profit << std::endl;	
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
