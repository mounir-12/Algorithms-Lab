#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;
typedef std::pair<int, int> PII;
typedef std::vector<PII> VPII;
// BGL typedefs
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
		boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;

VPII get_neighbors(int i, int j, int k) {
	int i_up = i-1, i_down = i+1;
	int j_left = j-1, j_right = j+1;
	VPII neighbors;
	if(i_up >= 0) { // add the up neighbors
		if(j_left >= 0) neighbors.push_back(PII(i_up, j_left)); // add up-left neighbor
		if(j < i) neighbors.push_back(PII(i_up, j)); // add up-right neighbor
	}
	if(j_left >= 0) neighbors.push_back(PII(i, j_left)); // add the left neighbor
	if(j < i) neighbors.push_back(PII(i, j_right)); // add the right neighbor
	if(i_down < k) { // every ball, except the ones on the ground, has 2 down neighbors so add them
		neighbors.push_back(PII(i_down, j)); // add the down-left neighbor
		neighbors.push_back(PII(i_down, j_right)); // add the down-right neighbor
	}
	return neighbors;
}

int get_vertex_index(int i, int j) {
	return (i*(i+1))/2 + j; // 0+1+...+i = (i*(i+1))/2 is the number of balls in the lines 0...(i-1) and is thus the index of the 1st
							// in line i (given the 0-based indexing)
}

VI get_shortest_distance(Graph &G, int i, int j, int V) {
	std::vector<int> distmap(V);
	Vertex start = get_vertex_index(i, j);
	boost::dijkstra_shortest_paths(G, start,
		boost::distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));
	return distmap; 
}

void testcases() {
	int k; std::cin >> k; // k=#balls
	
	VVI weights(k);
	for(int i=0; i<k; i++) { // read in the (low-half) matrix of weights
		for(int j=0; j<=i; j++) {
			int w; std::cin >> w;
			weights[i].push_back(w);
		}
	}
	
	int V = (k*(k+1))/2; // number of vertices, 1 vertex per ball
	Graph G(V);
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	for(int i=0; i<k; i++) {
		for(int j=0; j<=i; j++) {
			Edge e; bool success;
			int v = get_vertex_index(i, j); // index of the vertex representing the ball at position (i, j)
			VPII neighbors = get_neighbors(i, j, k);
			for(PII p: neighbors) {
				int i_n = p.first; // line index of the neighbor in the weights vector<>
				int j_n = p.second; // column index of the neighbor in the weights vector<>
				int u = get_vertex_index(i_n, j_n); // index of the vertex representing the ball at position (i_n, j_n)
				boost::tie(e, success) = boost::add_edge(v, u, G);
				weightmap[e] = weights[i_n][j_n];
			}
		}
	}
	
	std::vector<int> distmapTop = get_shortest_distance(G, 0, 0, V); // get distance from ball at index (0, 0) to every other ball
	std::vector<int> distmapLowLeft = get_shortest_distance(G, k-1, 0, V); // same for ball at index (k-1, 0)
	std::vector<int> distmapLowRight = get_shortest_distance(G, k-1, k-1, V); // same for ball at index (k-1, k-1)
	
	int min_total_weight = INT_MAX;
	for(int i=0; i<k; i++) { // find the index (i, j) of the ball where the paths should meet
		for(int j=0; j<=i; j++) {
			int v = get_vertex_index(i, j); // index of the vertex representing the ball at position (i, j)
			// the sum of distances from each corner ball to ball (i, j) has 2*weights[i][j] more weight than the total weight wanted
			int sum_weights = distmapTop[v] + distmapLowLeft[v] + distmapLowRight[v] - 2*weights[i][j]; // subtract 2*weight[i][j]
			min_total_weight = std::min(min_total_weight, sum_weights);
		}
	}
	std::cout << min_total_weight << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcases();
	return 0;
}
