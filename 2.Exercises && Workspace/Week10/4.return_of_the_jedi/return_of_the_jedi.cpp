#include <iostream>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#define FOR(i, n) for(int i=0; i<(n); i++)

// BGL typedefs
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
		boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;	

void testcase() {
	// we have n planets, we consider each one a vertex with 0-based index. Each vertex can be connected to every other vertex
	// we are given for each vertex i = 0, ..., n-1, the costs of the edges from i to i+1, i+2, ..., n-1
	// so if we consider the "adjacency matrix G" where at G[u][v] = G[v][u] = "cost of connecting u to v" then the given
	// input is the values above the diagonal of this matrix (the diagonal not included). These costs are our edge weights.
	// The way Leia creates the network is similar to Prim's algorithm, the difference is that the ties are broken in a different
	// manner (Prim's algorithm has no preference between 2 edges of the same weight). So Leia builds an MST (might be the same 
	// one built by Prim in some specific cases).
	// We want to build a graph that connects all vertices too but is different from Leia's graph. We can do this by removing some
	// edge from Leia's MST and replacing it by another different edge such that the resulting graph G' is connected and the 
	// difference between weight(G') and weight(Leia's MST) is minimized.
	// The way to find this edge is as follows: We find any MST (using Kruskal's or Prim's algorithm), call it T
	// Then for each possible edge e=(u,v) if e not in T, we add the edge e to T, this creates a cycle since T already contains a 
	// path from u to v in T, call this path p. To get a tree T' back, we need to break the cycle by removing some edge in the path p.
	// The best thing to do to minimize the difference between weight(T) and weight(T') is to remove the largest edge in p.
	// Clearly, each T' we build is different from T (since we remove an edge of T and add an edge not in T)
	// Also, the difference between the added and the removed edge is at least 0 (it can't be negative, otherwise T' has weight
	// less than T i,e T is not an MST --> contradiction). So weight(e_removed) <= weight(e_added)
	// Let T_min be the tree amongst all created trees T' that has the closest weight to T, then the solution is weight(T_min).
	// The question now is that: how do we know that T is the same as Leia's Tree?
	// The answer to this is that we don't need to build Leia's Tree explicitely. That's because we have 2 cases:
	// 		1-The found Tree T is exactly Leia's Tree, then the found T_min is different from T and the answer is correct
	// 		2-The found Tree T is different than Leia's Tree so it has the same weight as Leia's Tree (since both are MSTs, we could 
	//		   directly output weight(T) as a solution), we know that there exists an edge e in Leia's Tree not in T, so this is
	//		   one of the candidate edges we will try. So adding e to T creates a cycle C. We remove the largest edge in C and as 
	//		   seen before weight(e_removed) <= weight(e). The insight is that weight(e) cannot be strictly larger than weight(e_removed)
	//		   that's because of the "Cycle Property" which says: For any cycle C in the graph, if the weight of an edge e of C is larger 
	//		   than the individual weights of all other edges of C, then this edge cannot belong to an MST.
	//		   So by the cycle property, if weight(e) > weight(e_removed) then e cannot be part of an MST (which is a contradiction
	//		   since e is part of Leia's MST. So weight(e) = weight(e_removed)
	//		   So the found Tree T' (after adding e and removing the largest edge) has the same weight as T, so weight(T_min) = weight(T')
	// Another way would be to build Leia's tree (by a slight modification to Prim's algorithm) then we know we are in case 1 above
	
	int n, t; std::cin >> n >> t; // n=#planets, t=#index_of_planet_tatooine
	t--; // 0-based index, t is not used here, but may be used with prim's algorithm
	
	Graph G(n);
	WeightMap Gweightmap = boost::get(boost::edge_weight, G);
	
	for(int i=0; i<n; i++) {
		for(int j=i+1; j<n; j++) {
			int cost; std::cin >> cost;
			Edge e; bool success; boost::tie(e, success) = boost::add_edge(i, j, G); // add edge
			Gweightmap[e] = cost;
		}
	}
	
	std::vector<Edge> mst_edges;
	boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst_edges));
	
	Graph MST(n);
	WeightMap MSTweightmap = boost::get(boost::edge_weight, MST);
	int totalWeight = 0; // weight of the found MST
	for(Edge e: mst_edges) {
		Vertex u = boost::source(e, G); Vertex v = boost::target(e, G);
		Edge e_mst; bool success; 
		boost::tie(e_mst, success) = boost::add_edge(u, v, MST); // add edge to MST
		MSTweightmap[e_mst] = Gweightmap[e];
		totalWeight += Gweightmap[e];
	}
	
	std::vector<std::vector<int>> largest_edge(n, std::vector<int>(n, 0));	// largest_edge[i][j] = weight of largest edge in the MST path 
																		// from vertex i to j
	
	// BFS on the MST starting from each vertex w to find largest edge from w to every other vertex
	FOR(w, n) { // w is start vertex
		std::queue<int> Q;
		std::vector<bool> visited(n, false);
		Q.push(w);
		while(!Q.empty()) {
			int u = Q.front(); Q.pop(); // pop the vertex from the queue
			visited[u] = true;
			OutEdgeIt oebeg, oeend;
			for (boost::tie(oebeg, oeend) = boost::out_edges(u, MST); oebeg != oeend; ++oebeg) { // add neighbors of u in the MST
				Edge e = *oebeg; // edge (u,v) of MST
				Vertex v = boost::target(e, MST); // neighbor v of u
				if(!visited[v]) { // add only unvisited edges, MST graph is undirected so avoid having v add u then u add v to MST
					Q.push(v);
					 // largest edge from w to v is either largest edge from w to u or the edge (u,v)
					largest_edge[w][v] = std::max(largest_edge[w][u], MSTweightmap[e]);
				}
			}
		}
	}
		
	// loop over every edge (u,v) in G
	int smallest_diff = INT_MAX; // smallest difference
	EdgeIt ebeg, eend;
	for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
		Vertex u = source(*ebeg, G); Vertex v = target(*ebeg, G);
		Edge e; bool success; boost::tie(e, success) = boost::edge(u, v, MST); // check if edge in MST
		if(!success) { // edge not in MST
			int diff = Gweightmap[*ebeg] - largest_edge[u][v]; // weight added when removing largest edge from u to v in MST and adding *ebeg
			smallest_diff = std::min(smallest_diff, diff); // we want the smallest difference
		}
	}
	
	int newTotalWeight = totalWeight + smallest_diff;
	std::cout << newTotalWeight << std::endl;
	
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
