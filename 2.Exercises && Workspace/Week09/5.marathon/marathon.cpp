#include <iostream>
#include <cstdlib>
#include <map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

//BGL typedefs
// Weighted undirected Graph
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,	
		boost::property<boost::edge_weight_t, int>> WGraph;
typedef boost::graph_traits<WGraph>::edge_descriptor WEdge;
typedef boost::graph_traits<WGraph>::vertex_descriptor WVertex;
typedef boost::graph_traits<WGraph>::edge_iterator WEdgeIt;
typedef boost::property_map<WGraph, boost::edge_weight_t>::type	WeightMap;
// Capacity Graph
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > > CGraph;
typedef	boost::graph_traits<CGraph>::vertex_descriptor CVertex;
typedef	boost::graph_traits<CGraph>::edge_descriptor CEdge;
typedef	boost::graph_traits<CGraph>::edge_iterator CEdgeIt;
typedef	boost::property_map<CGraph, boost::edge_capacity_t>::type EdgeCapacityMap;
typedef	boost::property_map<CGraph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef	boost::property_map<CGraph, boost::edge_reverse_t>::type ReverseEdgeMap;

class EdgeAdder {
	CGraph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	EdgeAdder(CGraph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	void addEdge(int from, int to, long capacity) {
		CEdge e, rev_e;
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
	// we have some itersections (vertices) and bi-directional streets (edges) between intersections
	// each street has a width (capacity) and a length. We have a start vertex s and a finish vertex f
	// each runner is a flow (i,e of size 1) from s to f and we try to maximize the number of flows f1, .., fm
	// let d_min be the min distance from s to f. There might be multiple paths p1, p2, ..., pk from s to f 
	// such that distance(p_i) = d_min (i,e multiple paths with min distance).
	// Since all runners take only shortest paths (no detours), we must ensure that flows only take paths that are
	// shortest paths from s to f. To do so, we first create an undirected weighted graph G1 to run dijkstra from s
	// then from f. This gives us shortest distances from s to every vertex and same from f to every vertex
	// Then we need to find all edges e={u,v} in G1 that are part of a shortest path, i,e D1 := dist_s_to[u] + length(e) + 
	// dist_f_to[v] == d_min or D2 := dist_s_to[v] + length(e) + dist_f_to[u] == d_min. Those 2 cases imply we can have a shortest path 
	// where we "go from s to u then take edge e in the direction u->v then go from v to f" or "go from s to v then take edge e in 
	// the direction v->u then go from u to f"
	// So by computing the distances D1 and D2 above we can find in which direction the edge e={u,v} is used in the shortest path
	// So we create a directed graph G2 to which we add only the edges e (in the correct direction) that participate in a shortest path
	// from s to f. So for each edge e={u,v}, we check if D1 == d_min (i,e there is a directed shortest path s->u->v->f). If so we add 
	// the edge u->v to G2
	// We do the same with other direction v->u (i,e check if D2 == d_min and add v->u to G2 if true)
	// Then run a max-flow on G2 from s to f
	// Note: we cannot have undirected edge e={u,v} participate in 2 different shortest paths P1 and P2 from s to f once as u->v in P1
	// and again as v->u in P2. The proof is as follows:
	// Suppose this is the case then P1 (s->u->v->f) would have length (I) d_min = "dist_s_to[u] + length(e) + dist_f_to[v]"
	// and P2 (s->v->u->f) would have length (II) d_min = "dist_s_to[v] + length(e) + dist_f_to[u]" (same length as P1)
	// (I) = (II) <=> "dist_s_to[u] + dist_f_to[v]" = "dist_s_to[v] + dist_f_to[u]" (III)
	// <=> "dist_s_to[u] + dist_f_to[u] + dist_f_to[v] + dist_s_to[v]" = "2*(dist_s_to[v] + dist_f_to[u])"
	// where we added "dist_f_to[u] + dist_s_to[v]" to both sides of (III)
	// we know dist_s_to[u] + dist_f_to[u] >= d_min since it is the distance of a path s->u->f from s to f (might or might not a 
	// shortest path from s to f)
	// likewise dist_f_to[v] + dist_s_to[v] >= d_min since it is the distance of a path s->v->f from s to f
	// so 2*(dist_s_to[v] + dist_f_to[u]) >= 2*d_min <=> "dist_s_to[v] + dist_f_to[u]" >= d_min
	// <=> "d_min - length(e)" >= d_min (from (II))
	// <=> "length(e) <= 0" which is a contradiction since we have length(e) >= 1 for all edges e
	// We then only need to run max_flow algorithm from s to f to find the max number of runners possible
	
	int n, m, s, f; std::cin >> n >> m >> s >> f; // n=#intersections, m=#streets, s=start_intersection, f=#finish_intersection
	WGraph G1(n); // 1st graph: weighted undirected graph to run dijkstra
	WeightMap weightmap = boost::get(boost::edge_weight, G1);
	
	std::map<WEdge, int> capacities;
	for(int i=0; i<m; i++) {
		int a, b, c, d; std::cin >> a >> b >> c >> d; // read (a,b) edge capacity c and length l
		if(a == b) continue; // no need to add edge from vertex to itself
		WEdge e; bool success;
		boost::tie(e, success) = boost::add_edge(a, b, G1); // add undirected edge between a and b
		weightmap[e] = d;
		capacities[e] = c; // store capacity for edge e in std::map
	}
	
	// run dijkstra's algorithm twice
	std::vector<int> dist_s_to(n); // shortest distance from s to every vertex
	boost::dijkstra_shortest_paths(G1, s,
		boost::distance_map(boost::make_iterator_property_map(dist_s_to.begin(), boost::get(boost::vertex_index, G1))));
		
	std::vector<int> dist_f_to(n); // shortest distance from f to every vertex
	boost::dijkstra_shortest_paths(G1, f,
		boost::distance_map(boost::make_iterator_property_map(dist_f_to.begin(), boost::get(boost::vertex_index, G1))));
		
	int dist_s_to_f = dist_s_to[f];
	if(dist_s_to_f == 0 or dist_s_to_f == INT_MAX) { // corner cases
		std::cout << 0 << std::endl;
		return;
	}
	
	CGraph G2(n); // 2nd graph: graph for max flow
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G2);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G2);
	//ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G2);
	EdgeAdder eaG(G2, capacitymap, revedgemap);
	
	WEdgeIt e, eend;
	for (boost::tie(e, eend) = boost::edges(G1); e != eend; ++e) { // iterate over all undirected edges of G1
		// for each edge e={u,v}, find if there is a shortest path that uses e in direction u->v, same for direction v->u
		WVertex u = boost::source(*e, G1);
		WVertex v = boost::target(*e, G1);
		
		if(dist_s_to[u] + weightmap[*e] + dist_f_to[v] == dist_s_to_f) { // edge u->v part of a shortest path s->f
			eaG.addEdge(u, v, capacities[*e]); // add edge in direction u->v
		}
		if(dist_s_to[v] + weightmap[*e] + dist_f_to[u] == dist_s_to_f) { // edge v->u part of a shortest path s->f
			eaG.addEdge(v, u, capacities[*e]); // add edge in direction v->u
		}
	}
	
	// compute max number of runners through max_flow
	long max_participants = boost::push_relabel_max_flow(G2, s, f);
	
	std::cout << max_participants << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
