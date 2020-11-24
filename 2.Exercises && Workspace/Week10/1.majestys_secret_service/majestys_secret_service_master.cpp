#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
using namespace boost;
using namespace std;
typedef adjacency_list<vecS, vecS, undirectedS, no_property> Graph;
typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_weight_t,int>> DiGraph; // An edge-weighted digraph.
typedef property_map<DiGraph, edge_weight_t >::type WeightMap;
typedef graph_traits<DiGraph>::vertex_descriptor Vertex;
typedef graph_traits<DiGraph>::edge_descriptor Edge;

void testcase() {
// Read the input and build graph G.
	int N, M, A, S, C, D; // Capital letters used for the input sizes.
	cin >> N >> M >> A >> S >> C >> D;
	DiGraph G(N);
	WeightMap weight_map = get(edge_weight, G);
	for (int m = 0; m < M; ++m) { // Lower case letters used for the counters.
		char w; int x, y, z;
		cin >> w >> x >> y >> z;
		Edge e;
		tie(e, tuples::ignore) = add_edge(x,y,G);
		weight_map[e] = z;
		if (w == 'L') { // Add the reverse edge only for ski lifts.
			tie(e, tuples::ignore) = add_edge(y,x,G);
			weight_map[e] = z;
		}
	}
	// Compute one distance map per agent.
	vector<vector<int> > distmap(A, vector<int>(N));
	for (int a = 0; a < A; ++a) {
		int i; cin >> i;
		dijkstra_shortest_paths(G, i, distance_map(make_iterator_property_map(distmap[a].begin(), get(vertex_index, G))));
	}
	// Represent G’ as the pairwise distance matrix T from agents to shelters.
	vector<vector<int> > T(A, vector<int>(S));
	for (int s = 0; s < S; s++) {
		int j; cin >> j;
		for(int a = 0; a < A; a++) {
			T[a][s] = distmap[a][j]; // Unreachable pairs are at distance INT_MAX.
		}
	}
	// Binary search for the smallest t.
	int low = 0, high = INT_MAX;
	while (low < high) {
		int mid = low + (high-low)/2;
		// Build the bipartite matching graph G’’_t.
		Graph GG(A + C * S);
		for (int a = 0; a < A; ++a) {
			for (int s = 0; s < S; ++s) {
				if (T[a][s] == INT_MAX) continue; // Ignore unreachable shelters.
					for (int c = 0; c < C; ++c) { // c agents will still follow.
						if (T[a][s] + (c + 1) * D <= mid) { // Agent can enter in time.
							add_edge(a, A + c * S + s, GG); // Add the edge to G’’_t.
						}
					}
			}
		}
		// Compute the size of the maximum cardinality matching.
		vector<Vertex> matemap(A + C * S);
		edmonds_maximum_cardinality_matching(GG, make_iterator_property_map(matemap.begin(), get(vertex_index, GG)));
		const Vertex NULL_VERTEX = graph_traits<Graph>::null_vertex();
		int matching_size = 0;
		for (int a = 0; a < A; ++a) { // Count the number of matched agents.
			matching_size += (matemap[a] != NULL_VERTEX);
		}
		if (matching_size == A) { // The agents all make it in time.
			high = mid;
		} else { // Some agents do not make it, so we need more time.
			low = mid+1;
		}
	}
	cout << low << endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; cin >> t;
	while (t--) testcase();
	return 0;
}
