#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#define FOR(i, n) for(int i=0; i<(n); i++)

// BGL typedefs
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
	// Let J be the set of all jobs and Z be the set of all zones. Each job j in S gives some profit p_j and each zone z in Z has a 
	// ticket cost c_z.
	// Each job j is associated to some zones. Let Z_j be the set of zones associated with job j. In order to execute job j, we must have
	// bought all the tickets in Z_j. A ticket is only bought once for all jobs that need it.
	// So we have a bipartite graph G with jobs vertices to the left and zones vertices to the right. The vertex of job j has weight p_j
	// and the vertex of zone z has weight c_z. We have a connection from job j to each z in Z_j (no edge weights here)
	// Let J' be a subset in J, then the set zones associated with J' is " Z' = U Z_j for all j in J' " = "the union of the Z_j for all
	// j in J' ". The union here means that a zone doesn't appear more than once in Z' even if it appears in 2 or more Z_j
	// We would like to choose a subset J' in J such as to maximize the revenue "R = profit(J') - cost(Z')" with Z' the set of zones
	// associated to J', profit(J') = " sum of p_j for all j in J' " and cost(Z') = " sum of c_z for all z in Z' "
	// Let J'_c = J\J' be the complement of J' (i,e the set of jobs we didn't choose)
	// So R = profit(J') - cost(Z') = (profit(J) - profit(J'_c)) - cost(Z') = profit(J) - (profit(J'_c) + cost(Z'))
	// The value profit(J) is fixed with J the set of all jobs, so the maximize R, we need to minimize (profit(J'_c) + cost(Z'))
	// The insight here is to note that every edge in the graph G will necessarily have an endpoint in J'_c or in Z' (or both)
	// because if an edge=(j, z) has j not in J'_c then j in J' => z in Z' (since Z' is associated with J')
	// Thus we see that J'_c together with Z' represents a vertex cover. Let V' = J'_c U Z', then weight(V) = profit(J'_c) + cost(Z')
	// since J'_c and Z' are disjoint (one contains jobs and the other contains zones), so the revenue is:
	// R = total_profit - weight(V') => we want to minimize the weight of the vertex cover V' => we want to find the minimum-weight
	// vertex cover in a bipartite graph
	// This can be solved by creating a graph G' as G but without vertex weights then connect each vertex j in J to source with 
	// capacity p_j and each vertex z in Z to target with capacity c_z and the edges from job j to zone z has infinite capacity 
	// (or capacity p_j so as not to put a limit on the flows out of job j). So in G' we only have edge capacities (no vertex weights)
	// Then the max-flow = "min-weight of a vertex cover"
	// check: https://stackoverflow.com/questions/24204747/maximum-weighted-independent-set-in-bipartite-graph
	int Z, J; std::cin >> Z >> J; // Z=#zones, J=#jobs
	
	Graph G(J+Z); // vertices 0...J-1 are for jobs, vertices J...J+Z-1 are for zones
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	//ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
	
	Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);
	
	FOR(i, Z) { // add edges for zones
		int c; std::cin >> c;
		eaG.addEdge(J+i, target, c);
	}
	
	int total_profit = 0;
	FOR(i, J) { // add edges for jobs
		int p; std::cin >> p;
		eaG.addEdge(source, i, p);
		total_profit += p;
	}
	
	FOR(i, J) { // add edges from job to zones
		int n; std::cin >> n;
		FOR(k, n) {
			int z; std::cin >> z; // read the zone to connect to job i to
			eaG.addEdge(i, J+z, INT_MAX); // max capacity
		}
	}
	
	long flow = boost::push_relabel_max_flow(G, source, target); // find the min-weight vertex cover
	std::cout << total_profit - flow << std::endl; // output the result
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
