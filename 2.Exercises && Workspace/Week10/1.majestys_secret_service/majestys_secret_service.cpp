#include <iostream>
#include <vector>
#include <climits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> UGraph; // unweighted undirected graph
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, 
		boost::property<boost::edge_weight_t, int>> DGraph; // weighted directed graph
typedef boost::graph_traits<DGraph>::edge_descriptor Edge;
typedef boost::graph_traits<DGraph>::vertex_descriptor Vertex;
typedef boost::property_map<DGraph, boost::edge_weight_t>::type	WeightMap;

void testcase() {
	// We have a graph G with n vertices and edges between them. We have "a" vertices that are initial locations of agents
	// "s" vertices which are locations of shelters. Each shelter has c places (seats) available.
	// Once an agent is at the entrance of a shelter, he needs to wait for k*d seconds to get the place k (k=1...c)
	// We want to find the shortest time for all agents to get a place in a reachable shelter. So we need to make each agent reach
	// the shelter as fast as possible, then wait for some time at the shelter entrance to get a place.
	// Thus we compute the shortest distance (here distance means temporal distance, i,e time) from each agent to each shelter with
	// Dijkstra's algorithm.
	// Suppose we decide that min_time must be l for some l in [0, INT_MAX]. Then we compute the min time t_i,j,k for the agent i to
	// get place k in shelter j: t_i,j,k = time_i_to[j] + k*d with time_i_to[j] the found min time for agent i to reach shelter j
	// (if reachable of course, if not reachable then skip). Then for all t_i,j,k <= l, we mark the agent i as matchable to place k
	// of shelter j.
	// The question now is to decide for the correct value l of min_time. The value l is correct if it is the smallest value such that
	// every agent can be matched to some place k in a shelter j. To decide if every agent is matchable, this is basically just
	// finding the size of a maximum cardinality matching MCM (where we agents to places in shelters) and this size should be the
	// number of agents "a". As for finding the smallest value l, we said that l in [0, INT_MAX], so we can search this range
	// for the smallest value that allows us to match every agent using binary search
	// Note here that in order order to find the size of the MCM, we can find the max-flow since the graph is bipartite or use
	// edmonds's max cardinality matching (used here).
	// Note here that the value l is in [0, INT_MAX] because the worst case would be if an agent would have to visit every vertex
	// in G before reaching the shelter (e,g agent at vertex 0 and shelter at vertex n-1 and G is a linked list and all edge weights 
	// z = 10^4), so to reach the shelter, the agent needs at most n_max*z_max time and to enter a shelter, he would need at most 
	// c_max*d_max, thus the worst time would be MAX = n_max*z_max + c_max*d_max = 10^3 * 10^4 + 2*10^3 < INT_MAX
	// So we might as well limit the search to [0, MAX] (see commented code where we find z_max then compute MAX' = n*z_max + c*d)
	int n, m, a, s, c, d; std::cin >> n >> m >> a >> s >> c >> d;
	
	DGraph G1(n);
	WeightMap weightmap1 = boost::get(boost::edge_weight, G1);
	
	//int z_max = 0;
	for(int i=0; i<m; i++) { // add directed edges
		char w; int x, y, z; std::cin >> w >> x >> y >> z;
		//z_max = std::max(z_max, z);
		Edge e; bool success;
		if(w == 'S') { // slope, so add edge x->y
			boost::tie(e, success) = boost::add_edge(x, y, G1);
			weightmap1[e] = z;
		} else if(w == 'L') { // lift, so add edge x->y and y->x
			boost::tie(e, success) = boost::add_edge(x, y, G1);
			weightmap1[e] = z;
			boost::tie(e, success) = boost::add_edge(y, x, G1);
			weightmap1[e] = z;
		}
	}
	
	std::vector<int> agents;
	for(int i=0; i<a; i++) { // read agents positions
		int j; std::cin >> j;
		agents.push_back(j);
	}
	
	std::vector<int> shelters;
	for(int i=0; i<s; i++) { // read shelters positions
		int j; std::cin >> j;
		shelters.push_back(j);
	}
	
	// find the shortest distance from each agent to every other vertex
    std::vector<std::vector<int>> distmaps(a, std::vector<int>(n));
	for(int i=0; i<a; i++) {
		std::vector<int>& distmap(distmaps[i]);
		Vertex start = agents[i];
		boost::dijkstra_shortest_paths(G1, start, 
			boost::distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G1))));
	}
	
	long left = 0, right = INT_MAX; // we perform our binary search in the range [0, INT_MAX]
									// we might as well set "right = n*z_max + c*d" which would be faster
	while(left != right) {
		long mid = (right+left)/2;
		UGraph G2(a+c*s); // vertices 0...(a-1) for agents and vertices a...a+c*s-1 for shelters, each shelter gets c consecutive indices
						 // 1 per place available in the shelter
		for(int i=0; i<a; i++) { // for each agent i
			for(int j=0; j<s; j++) { // for each shelter j
				int dist = distmaps[i][shelters[j]]; // distance from agent i to shelter j
				if(dist != INT_MAX) { // shelter j reachable from agent i
					for(int k=0; k<c; k++) { // for each place k in shelter j
						if(dist + (k+1)*d <= mid) { // if the agent can reach the shelter and take the place k in time <= mid
													// then place k is matchable to agent
													// in order to take place k, agent needs to get to shelter, then wait (k+1)*d seconds
							boost::add_edge(i, a+c*j+k, G2);
						}
					}
				}
			}
		}
		std::vector<Vertex> matemap(a+c*s);
		boost::edmonds_maximum_cardinality_matching(G2, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G2)));
		int matchingsize = matching_size(G2, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G2)));
		if(matchingsize == a) { // all agents matchable, then min time is <= mid
			right = mid;
		} else { // not all agents can reach and enter a shelter in time <= mid, then min time > mid
			left = mid+1;
		}
	}
    
    std::cout << right << std::endl; // output final result
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
