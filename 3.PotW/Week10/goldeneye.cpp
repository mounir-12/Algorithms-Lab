#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/incremental_components.hpp>
#include <boost/pending/disjoint_sets.hpp>

// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

// CGAL typdefs
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

// BGL typedefs
typedef boost::disjoint_sets_with_storage<> union_find; // disjoint_sets aka union_find

struct myEdge { // store for each delaunay edge (between jammers) its squared length and the corresponding graph vertices 
	int u;
	int v;
	K::FT squared_length;
	myEdge(int u, int v, K::FT squared_length): u(u), v(v), squared_length(squared_length) {}
};

bool operator<(myEdge e1, myEdge e2) { // for sorting edges
	return e1.squared_length < e2.squared_length;
}

void testcase() {
	// We are on the plane R^2 and we have n points (aka jammers). Each jammer is the center of a circle with radius r. All circles
	// have the same radius r and any position within a circle is protected (safe position). The power consumed is p = (2*r)^2 so it
	// depends on the radius. The missions consist of a start and a target positions s and t, and we would like to move from s to t 
	// through safe positions only and find the minimum radius of protection to allow us to perform all missions.
	// If we only have 2 jammers (with centers) u and v, it only becomes safe to go from u to v if the radius of both circles is at least 
	// r_min = dist(u,v)/2, in case r = r_min then the circles are touching in the center of the segment [u,v] and any point s inside 
	// the circle of u can be moved safely to another point t inside the circle of v following the edges (s,u) -> (u,v) -> (v,t).
	// Given the n jammers, suppose we connect every jammer to all other jammers (by adding an edge), call it graph G1. Now take a 
	// new graph G with only the jammers (no edges). Then to go from some jammer u to another jammer v in G, we must build a path between
	// them. The best we can do to keep the radius small is that:
	// Given we are at jammer u and there is no path between u and v in G, we keep adding unadded edges e from G1 in G from smallest to
	// largest untill a path is created between u and v in G (we add an edge e=(w,x) only if there is currently no path between them).
	// This is basically the same way the Euclidian Minimum Spanning Tree (EMST) is built.
	// So since we need the edges of the EMST, we use Delaunay Triangulation to find those (since EMST edges are part of the Delaunay 
	// edges)
	// Our goal is just to know, for each mission, the path s to t with the smallest radius. The idea is to use an algorithm similar to 
	// a partial run of Kruskal's algorithm (so we use a union-find data structure aka disjoint sets). We start by having each of the 
	// n vertices being a singleton MST, so the disjoint sets have 1 vertex each. If there is no path from s to t (i,e s and t are on 
	// different sets in the union-find sets) we go through Delaunay edges e={u,v} from smallest to largest, find the set of u and set 
	// of v and union them (of course if u and v are on the same set, no need to union). We keep doing this till s and t become in the
	// same union-find set (i,e we build a path between them while keeping radius small), so we pause at the current added edge and 
	// record the largest radius found till now (i,e length of largest added edge). Then move to the next mission, if the current added
	// edges form a path to perform it safely then ok, otherwise, we continue from the lastly added edge by adding the next edges 
	// smallest to largest till we form a safe path from the mission's start position to its target position.
	// Another way to see it is to say we start with a graph with n vertices and no edges and for each mission, we check if the start 
	// and end vertices s and t are on the same connected component, if so then ok, else, keep adding edges smallest to largest (checking
	// every time an edge is added if s and t became in the same component). This is what is known as dynamic/incremental connected 
	// component, see : http://www.cs.au.dk/~gudmund/dynamicF10/dynanote.pdf
	// no need to use boost's incremental connected component implementation since we don't need to build the graph, only need to know 
	// if a path exists
	
	int n, m; double p; std::cin >> n >> m >> p; // n=#jammers, m=#missions, p=initial_power (= squared diameter of each jammer circle initially)
	
	union_find ds_p(n); // initial graph G1 for p-umbrellas, has n components, one per vertex (1 vertex per jammer)
	
	std::vector<K::Point_2> jammers;
	std::map<K::Point_2, int> jammer_index; // map each jammer point to a vertex index
	for(int i=0; i<n; i++) {
		K::Point_2 j; std::cin >> j; // read jammer location
		jammers.push_back(j);
		jammer_index[j] = i; // assign to vertex i 
	}
	
	// construct triangulation
	Triangulation tr;
	tr.insert(jammers.begin(), jammers.end());
	
	std::vector<myEdge> graph_edges; // store for each finite delaunay edge the corresponding graph edge
	for (Edge_iterator e = tr.finite_edges_begin(); e != tr.finite_edges_end(); e++) {
		K::Point_2 j1 = e->first->vertex((e->second + 1) % 3)->point(); // 1st jammer (1st end point of current edge)
		K::Point_2 j2 = e->first->vertex((e->second + 2) % 3)->point(); // 2nd jammer (2nd end point of current edge)
		int u = jammer_index[j1], v = jammer_index[j2];
		K::FT squared_length = CGAL::squared_distance(j1, j2);
		graph_edges.push_back(myEdge(u, v, squared_length));
		
		if(squared_length <= p) { // u and v linked by an edge of size <=p, so the path along this edge is safe under the p-umbrellas
			ds_p.union_set(u,v); // union the components of u and v (by adding edge {u,v}) so that u and v are in the same component (reachable from each other)
		}
	}
	
	std::sort(graph_edges.begin(), graph_edges.end()); // sort edge according to "delaunay edge's squared length"
	
	union_find ds_b(n); // initial graph G2 for b-umbrellas, has n components, one per vertex (1 vertex per jammer)
	union_find ds_a(n); // initial graph G3 for a-umbrellas, has n components, one per vertex (1 vertex per jammer)

	int index_b = 0; // index of the (pending) edge unadded to G2
	int index_a = 0; // index of the (pending) edge unadded to G3
	double a = 0, b = 0;
	
	for(int i=0; i<m; i++) { // loop over missions
		K::Point_2 s, t; std::cin >> s >> t; // read mission's start and target location
		K::Point_2 s_closest = tr.nearest_vertex(s)->point(); // nearest jammer to start location
		K::Point_2 t_closest = tr.nearest_vertex(t)->point(); // nearest jammer to target location
		int u = jammer_index[s_closest], v = jammer_index[t_closest]; // jammers vertices
		// energy needed to go "from s to s_closest" and "from t to t_closest" safely
		K::FT safe_energy = 4*CGAL::max(CGAL::squared_distance(s, s_closest), CGAL::squared_distance(t, t_closest));
		if(safe_energy <= p and ds_p.find_set(u) == ds_p.find_set(v)) {
			// there exits a safe path under p-umbrellas s->s_closest->t_closest->t
			std::cout << "y";
			// mission safe under p-umbrellas => must be the safe under b-umbrellas
			b = CGAL::max(b, safe_energy); // b must at least safe_energy
			// keep adding edges to G2, smallest to largest, till u and v are in the same component (i,e there exists a path between them) in G2
			while(index_b < graph_edges.size() and ds_b.find_set(u) != ds_b.find_set(v)) {
				myEdge& e(graph_edges[index_b]);
				ds_b.union_set(e.u, e.v); // union the 2 components of the 2 edge endpoints (i,e add edge)
				b = CGAL::max(b, e.squared_length); // b must be at least the energy incured by the added edge
				index_b++; // next edge to be added
			}
		} else {
			std::cout << "n";
		}
		
		// we must in any case ensure there is a safe path under a-umbrellas for this mission
		a = CGAL::max(a, safe_energy); // a must at least safe_energy
		// keep adding edges to G3, smallest to largest, till u and v are in the same component (i,e there exists a path between them) in G3
		while(index_a < graph_edges.size() and ds_a.find_set(u) != ds_a.find_set(v)) {
			myEdge& e(graph_edges[index_a]);
			ds_a.union_set(e.u, e.v); // union the 2 components of the 2 edge endpoints (i,e add edge)
			a = CGAL::max(a, e.squared_length); // a must be at least the energy incured by the added edge
			index_a++; // next edge to be added
		}
	}
	std::cout << "\n" << a << "\n" << b << std::endl;
	
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	std::cout << std::fixed << std::setprecision(0); // print all digits of doubles and no digit after the dot "."
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
