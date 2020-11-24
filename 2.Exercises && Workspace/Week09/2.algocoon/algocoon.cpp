#include <iostream>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// BGL typedefs
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
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;

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
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

void testcase() {
	// we consider each figure a vertex and each limb a directed edge between 2 figures
	// the sculpture (i,e figures+limbs) is represented by a graph G=(V, E)
	// We want to cut the graph G into 2 parts such that the cut cost is minimum and each 
	// person gets at least 1 figure (i,e 1 vertex), so at least 1 figure is completely
	// split from the other figures
	// We thus need to find a min-cut (S,T) of the graph G such that the source s is some vertex u in V
	// and the target t is some other vertex in V. This works because we know that a cut (S, T) completely
	// separates all vertices in S from all vertices in T, and S contains at least s and T contains at least t
	// So we need to decide which vertex in V would be s and which one would be t such that the chosen pair (s,t)
	// would have the smallest cut cost (i,e the minimum "max flow from s to t" amongst all pairs (s,t)). We could try 
	// all pairs (s, t) but we can do better.
	// Let (S, T) be a global min-cut of graph G (i,e a cut of graph G with min cost). There might be multiple cuts (S1, T1),
	//  ...(Sn, Tn) with the same cost as (S, T). Take any vertex v in V, we have 2 cases here : v in S or v not in S (i,e v in T)
	// 1-	Suppose v in S, we know T is non-empty, let u be a vertex in T (we know u != v since v in S)
	// So (S, T) separates v from u with min-cost C (since it is the global min-cut). Now let (S', T') be
	// a min v-u cut (found through max flow with v as source and u as target), so (S', T') is the cut, amongst all available cuts 
	// of G "(S, T) included", that separates v from u with min-cost
	// this means that cost(S', T') <= cost(S, T) but since (S, T) is the global min-cut then cost(S, T) <= cost(S', T')
	// both inequalities imply cost(S, T) = cost(S', T'), so (S', T') is also a global min-cut
	// This means we can take any vertex u in T and find the min v-u cut and this would give us a global min-cut
	// but since we do not know which vertices u are in T, we can try all vertices u in V\{v} (since T is at most V\{v})
	// So we get multiple min v-u cuts, and the ones with smallest cost are the global min-cuts.
	// 2-	If v not in S, so v in T, then roles are exchanged here and we just find the smallest "min 'u-v' cut" for all u in V\{v}
	// Combining both cases (v in S and v in T) we get the global min-cut with no suposition on v.
	// v as noted above can be any vertex (I chose v = 0 below since if n = 2, then any v > 1 doesn't make sense, v = 1 also works)
 
	int n, m; std::cin >> n >> m; // n=#figures, m=#limbs
	Graph G(n);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
	
	for(int i=0; i<m; i++) { // construct graph
		int a, b, c; std::cin >> a >> b >> c; 
		eaG.addEdge(a, b, c);
	}
	
	// default values (also works for case n = 2)
	int min_cut = INT_MAX; // global min-cut
	int min_cut_s = 0; // some vertex in S of the global min-cut (S, T) that serves as source
	int min_cut_t = 1; // some vertex in T of the global min-cut (S, T) that serves as target
	for(int u=0; u<n; u++) {
		int v = 0;
		if(u == v) continue; // u and v must be different
		int flow1 = boost::push_relabel_max_flow(G, v, u);
		int flow2 = boost::push_relabel_max_flow(G, u, v);
		// update min_cut_s and min_cut_t if found a smaller flow
		if(flow1 < min_cut and flow1 < flow2) {
			min_cut = flow1;
			min_cut_s = v;
			min_cut_t = u;
		} else if(flow2 < min_cut and flow2 < flow1) {
			min_cut = flow2;
			min_cut_s = u;
			min_cut_t = v;
		}
	}
	
	// run push_relabel again to get correct values in rescapacitymap used in BFS
	min_cut = boost::push_relabel_max_flow(G, min_cut_s, min_cut_t);
	
	// BFS to find vertex set S
	std::vector<int> vis(n, false); // visited flags, one per vertex
	std::queue<int> Q;
	vis[min_cut_s] = true; // Mark the source as visited
	Q.push(min_cut_s);
	while (!Q.empty()) {
		const int u = Q.front();
		Q.pop();
		OutEdgeIt ebeg, eend;
		for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
			const int v = boost::target(*ebeg, G);
			// Only follow edges with spare capacity and skip visited vertices
			if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
			vis[v] = true;
			Q.push(v);
		}
	}
	
	std::vector<int> S; // S the set of figures taken by me, V\S are taken by my friend
	for(int i=0; i<n; i++) {
		if(vis[i]) {
			S.push_back(i);
		}
	}
	
	std::cout << min_cut;
	std::cout << "\n" << S.size() << " ";
	for(int i=0; i<S.size(); i++) {
		std::cout << S[i] << (i == S.size()-1 ? "" : " ");
	}
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
