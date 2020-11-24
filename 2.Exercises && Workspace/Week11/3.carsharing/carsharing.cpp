#include <iostream>
#include <vector>
#include <set>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#define FOR(i, n) for(int i=0; i<(n); i++)

typedef std::vector<int> VI;
typedef std::pair<int, int> PII;

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

// Custom Edge Adder Class
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

struct Request {
	int s, t, d, a, p;
	Request(int s, int t, int d, int a, int p): s(s), t(t), d(d), a(a), p(p) {}
};

void testcase() {
	// we have N requests and S stations, each request has a start and end station "s and t", a departure and arrival
	// time "d and a" and a profit "p". Each station has initially l_i cars. To fullfill a request, a car must be available
	// at start station "s" at time "d" of that request. We want to move the cars around to fullfill some requests such that
	// the total profit (of fulfilling those requests) is maximized. Each request defines some key events which are "car started
	// from s at time d" and "car reached t at time a", other events like "car is moving from s to t" are not important for us
	// because we only care about when a car arrives to a certain station so we can assign it to next requests and when a car departs
	// from a station (i,e can't assign it anymore till it arrives to destination)
	// We represent each of the important events of a request by a vertex, i,e we add for each request 1 vertex for representing 
	// the pair (start_station, departure_time) and another vertex for representing the pair (end_station, arrival_time)
	// We represent each request by an edge from the vertex of the pair (s, d) to the vertex of the pair (t, a) with capacity 1 (since
	// each request needs only 1 car to be fullfilled). This represents the movement of a car in time and space (i,e from s to t in the
	// time interval [d,a]). But if we don't want to fullfill some request (even though we have enough cars to do so), we must be able 
	// to "transfer the unused cars in time only" i,e the car moves from (station_i, t1) to (station_i, t2) with t1 < t2 so it stays in
	// the same station but moves in time so that it becomes available for later requests at that station. To do so, we can sort the 
	// pairs of the same station to get (station_i, t1), (station_i, t2) ..., (station_i, t_n) where t1 <= t2 <= ... <= t_n (i,e we sort 
	// them chronologically only) and add an edge between succesive pairs (station_i, t1) -> (station_i, t2) -> ... -> (station_i, t_n)
	// The source vertex can be seen as time t = 0 and holds all cars. It is used to send cars initially at their initial station so 
	// that they are available at the depart time of the 1st request of each station. So we add an edge from the source to 
	// (station_i, t1) with capacity l_i for all i=1...S. For the edges that represent a car staying at the same station, the 
	// capacity is L = "total number of cars" since each station can hold all cars (no storage limit). The target vertex can be
	// seen as the time where every car reached its final destination, call this time t_max which is the max arrival time
	// over all requests. We add an edge from (station_i, t_n) to target to represent the fact that a car stays at station_i
	// in the time [t_n, t_max]. Since we want to maximize the total_profit, we need to minimize "-total_profit", so for each
	// edge added the cost is 0 except edges representing the requests i whose cost would be -p_i (-profit of request i). This solution
	// works with cycle_canceling algorithm but is still too slow because we have O(N) vertices (we added 2 vertices per request)
	// and O(N) edges (we added 1 edge per request, N edges representing transfer in time only (i,e stay at same station) at most N edges 
	// from source and to target).
	// Since we have that the "temporal distance" from source to target is fixed for any source-target path (because source represents t=0 
	// and target represents t=t_max, so "temporal distance" between them is t_max-0), we can apply the trick to get non-negative costs
	// where we shift the cost of each unit of time (i,e 1 min) in each edge by max_profit (this is because we need to ensure that each
	// flow (i,e each source-target path) is shifted by the same amount and each edge has positive cost (since the smallest edge we could
	// have might move us only 1 min forward in time and it might have the max_profit as its profit).
	// Thus, each flow is shifted here by t_max*max_profit so the "cost of 1 flow" would be "t_max*max_profit - flow_profit" where 
	// flow_profit=(profit of edges in the source-target path taken by this unit flow) so the total_cost = "sum of costs of all flows" = 
	// "sum(t_max*max_profit - flow_profit) for all flows" = "nb_flow*t_max*max_profit - total_profit" where nb_flow = max_flow since
	// each flow represents a car moving from s to t (in time and possibly in space too) and we want to maximize the number of requests
	// served (so maximize the number of cars moving). Finally, we find "total_profit = nb_flow*t_max*max_profit - total_cost"

	int N, S; std::cin >> N >> S; // N=#requests, S=#stations
	
	VI inital_cars(S); // store at index i the number of cars initially at station i
	int L = 0; // total number of cars
	FOR(i, S) {
		std::cin >> inital_cars[i];
		L += inital_cars[i];
	}
	
	std::set<PII> station_time_pairs; // set of station_time_pairs sorted (using stations nb first then using time)
	std::vector<Request> requests;
	int max_profit = 0;
	int max_t = 0;
	FOR(i, N) {
		int s, t, d, a, p; std::cin >> s >> t >> d >> a >> p;
		s--; t--; // 0-based index
		max_profit = std::max(max_profit, p);
		max_t = std::max(max_t, a);
		requests.push_back(Request(s, t, d, a, p));
		station_time_pairs.insert(PII(s,d));
		station_time_pairs.insert(PII(t,a));
	}
	int V = station_time_pairs.size();
	
	// Create Graph and Maps
    Graph G(V);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    Vertex source = boost::add_vertex(G);
    Vertex target = boost::add_vertex(G);
    
	std::map<PII, int> map; // map each (station, time) pair to a vertex index
	PII prev; bool no_prev = true;
	int v = 0;
	for(auto it = station_time_pairs.begin(); it != station_time_pairs.end(); it++) {
		PII curr = *it;
		map[curr] = v; // map current pair to v
		if(no_prev) { // first elem of the set
			eaG.addEdge(source, v, inital_cars[curr.first], (curr.second-0)*max_profit); // add edge from source to new found station
			no_prev = false; // we now have a prev
		} else if(prev.first == curr.first) { // same station
			eaG.addEdge(v-1, v, L, (curr.second - prev.second) * max_profit); // edge from the previous vertex to curr vertex
																			 // (for when cars stay at the same station)
		} else if(prev.first != curr.first) { // new station found
			eaG.addEdge(v-1, target, L, (max_t - prev.second)*max_profit); // edge from the previous vertex to target 
																		  // (for when cars stay at the same station)
			eaG.addEdge(source, v, inital_cars[curr.first], (curr.second-0)*max_profit); // add edge from source to new found station
		}
		prev = curr; 
		v++;
	}
	eaG.addEdge(v-1, target, L, (max_t - prev.second)*max_profit); // edge from the last vertex of last station to target
	
	FOR(i, N) { // loop over requests
		PII start = std::make_pair(requests[i].s, requests[i].d);
		PII end = std::make_pair(requests[i].t, requests[i].a);
		int u = map[start];
		int v = map[end];
		eaG.addEdge(u, v, 1, (end.second - start.second) * max_profit - requests[i].p); // edge for the 1 car assigned to request i (if any)
	}
	// min-cost max-flow
	boost::successive_shortest_path_nonnegative_weights(G, source, target);
	int cost = boost::find_flow_cost(G);
	int flow = 0;
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(source, G); e != eend; ++e) { // find the max flow value
        flow += capacitymap[*e] - rescapacitymap[*e];
    }
    
    int profit = flow*max_profit*max_t - cost;
    std::cout << profit << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
