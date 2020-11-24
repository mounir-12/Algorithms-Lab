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
	// every day, we have some menus produced (supply) and some students to feed (demand). We can have 1 vertex per day
	// each one having a supply and a demand. Since we could have leftover on day i, those can be supplied to day i+1 with
	// a certain limit on nb of supplied menus from day i to day i+1
	// So we have vertices with supplies and demands, thus we add an edges from the source s to each day i with capacity = 
	// "#menus produced on day i" and with cost = "production price of a menu on day i". We also add an edge from each day i 
	// to the target t with capacity = "#students to be served on day i" and with cost = "-(selling price of a menu on day i)"
	// we add an edge from each day i to day i+1 (so as to supply leftovers) with capacity = "#menus that can be stored overnight
	// from day i to day i+1" and with cost = "energy cost per menu from day i to day i+1"
	// Running min cost max flow returns a min cost c, with -c the target value, -c >0 => profit and -c<0 => loss
	// Each flow f from s to t (i,e 1 flow) will have a production cost C(f) >= 0 "because of the edge from s to day i that f follows"
	// a price return P(f) <= 0 "because of the edge from day j to t that f follows" and energy cost E(f) "because of the edge from
	// day i to day j that f follows" (E(f) = 0 if "day i = day j"). We minimize P(f)+C(f)+E(f) over all found f.
	// Instead of having a negative cost P(f), we assign to each edge (day i, t) the cost "max_price - price_i" so we can use faster 
	// algorithm, so new_P(f) = max_price - P(f) >=0 and we minimize new_P(f)+C(f)+E(f) over all found flows then subtract the added
	// max_price for each found flow
	
	int n; std::cin >>n; // n=#days
	Graph G(n);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    Vertex source = boost::add_vertex(G);
    Vertex target = boost::add_vertex(G);
    
    for(int i=0; i<n; i++) { // connect source to each day i
		int a, c; std::cin >> a >> c; // read nb of produced menus and cost per menu
		eaG.addEdge(source, i, a, c); // add edge from source to day i with capacity a and cost c
	}
	
	int students_to_serve = 0;
	std::vector<std::pair<int, int>> prices_students(n); // price of menu and nb students for each day
	int max_price = INT_MIN;
	for(int i=0; i<n; i++) { // read all prices and find max_price
		int s, p; std::cin >> s >> p; // read nb of students to be served and price per menu on day i
		students_to_serve += s;
		prices_students[i] = std::make_pair(p, s);
		max_price = std::max(max_price, p);
	}
	
	for(int i=0; i<n; i++) { // connect each day i to target
		int p = prices_students[i].first;
		int s = prices_students[i].second;
		eaG.addEdge(i, target, s, max_price-p); // add edge from day i to target with capacity s and cost max_price-p
	}
	
	for(int i=0; i<n-1; i++) { // connect each day i to day i+1
		int v, e; std::cin >> v >> e; // read max of menus transferable from day i to day i+1 and cost of transfer of 1 menu
		eaG.addEdge(i, i+1, v, e); // add edge from day i to day i+1 with capacity v and cost e
	}
	
	// run min cost max flow algorithm with non-negative weights
	boost::successive_shortest_path_nonnegative_weights(G, source, target);
    int min_cost = boost::find_flow_cost(G);
    int max_nb_students = 0;
    // Iterate over all edges leaving the source to sum up the flow values.
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G); e != eend; ++e) {
        max_nb_students += capacitymap[*e] - rescapacitymap[*e];
    }
    
    int profit_loss = -(min_cost - max_nb_students*max_price); // subtract the added max_price per flow
    if(max_nb_students == students_to_serve) {
		std::cout << "possible ";
	} else {
		std::cout << "impossible ";
	}
    std::cout << max_nb_students << " " << profit_loss << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
