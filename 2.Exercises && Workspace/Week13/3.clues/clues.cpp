#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/bipartite.hpp>
// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

// BGL typedefs
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef std::vector <boost::default_color_type> partition_t;
typedef typename boost::property_map<Graph, boost::vertex_index_t>::type index_map_t;
typedef boost::iterator_property_map <partition_t::iterator, index_map_t> partition_map_t;
typedef boost::color_traits<boost::default_color_type> Color;

// CGAL typedefs
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;

bool fine_coloring(Triangulation& t, long rr) { // check if the stations with same color are distant by d > r
	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
		if(t.segment(*e).squared_length() <= rr) return false;
	return true;
}

void print_n(int m) { // print the character "n" m times
	for(int i=0; i<m; i++) {
			std::cout << "n";
	}
	std::cout << std::endl;
}

void testcase() {
	// We have n vertices (stations) each one represents the center of disk of radius r. We would like to find if we can assign
	// each vertex 1 of 2 numbers (0 or 1 -> the frequencies) such that no other center in range (i,e no other center that 
	// is away by at most r) has the same number assigned. If we consider the graph G with an edge between every pair of vertices
	// (v_i, v_j) whose distance ||v_i - v_j|| <= r, then the problem is a graph coloring problem with 2 colors: color 0 and color 1.
	// Let G' be some "connected graph" that we want to 2-color. Because of the limited choice of colors, we must have that when we
	// color a vertex v in G' with color 0 then we need to color all adjacent vertices with color 1 and vice versa. Also, if we
	// find a 2-coloring of G', then changing the color of 1 vertex from 0 to 1 will propagate the change to neighbor vertices (i,e
	// all neighbors will have to change from 1 to 0) and so on to the whole graph. Thus we only have 2 ways of coloring the graph
	// which are equivalent (i,e we can go from one to the other by inverting the color of every vertex)
	// Now for the graph G built above, it may be disconnected. Suppose G has k components and let G1, ..., Gk be the subgraphs
	// of G that are connected (i,e G_i has all vertices and edges of component i). Any vertex v in G_i is away by more than r
	// from any vertex w in G_j for all i != j (otherwise we would have an edge (v, w) thus v and w would be in the same component)
	// so these G_i can be considered independently from one another when coloring G
	// Consider now "any Spanning Tree" T_i of G_i. If G_i is 2-colorable, then so is T_i because every edge in T_i is in G_i
	// but if T_i is 2-colorable, then G_i not necessarily 2-colorable (because not all edges of G_i are in T_i).
	// But if T_i is 2-colorable and no 2 vertices colored the same are adjacent (recall that 2 vertices are adjacent if the distance
	// between them is at most r) then G_i is 2 colorable. So we can find a 2-coloring of T_i then check the smallest distance
	// between 2 vertices colored the same (if > r then G_i 2-colorable)
	// But How do we find the Spanning Trees T_i? The answer is Delaunay for the EMST
	// Now we build the graph G_full with the same vertices as G an edge between every pair of vertices (u,v) with weight(u,v) = distance 
	// between u and v then build the MST T of G_full (which is actually the EMST of G_full) by sorting all edges in increasing weight 
	// and taking edges (u,v) from smallest to largest if u and v are not connected (Kruskal-like algorithm). We have 2 cases:
	// 	1- All edges in T have weight <= r. In this case, the graph G contains only k=1 component and T is a spanning Tree of G
	// 	2- Some edges in T have weight > r. This means that while building T, we went throught all edges with weight <= r, added
	// 	   some of them (we added an edge if it connects 2 unconnected components, so edges (u,v) of length <= r with u and v connected
	// 	   are not added) then after having exhausted all edges of length <= r moved to edges of length > r to connect unconnected
	// 	   components. We see here that if we stop this algorithm after having exhausted all edges of length <= r, then we would
	// 	   be left with our k components of G and that the edges in each component are spanning of the component, thus we found
	//	   our spanning Trees T_i. Instead of stopping the algorithm, just build the full EMST then remove edges of length > r
	// Now having found our T_i, we take every T_i and 2-color it, if not possible, then can't 2-color G, if possible, then we
	// split T_i to T_i,0 for vertices colored 0 and T_i,1 for vertices colored 1 then we find the closest pair (u,v) in T_i,0:
	// if the distance(u,v) <= r then G_i is not 2 colorable (since with 2 colors, we only have 2 ways to color T_i as explained
	// above, thus u and v would always have the same color 0 or 1 in both ways of coloring T_i). We do the same for T_i,1.
	// If in both cases (i,e with T_i,0 and T_i,1) we have that distance(closest_pair) > r, then G_i is 2-colorable.
	// If all subgraphs G_i are 2-colorable then G is 2-colorable
	// To find the closest pair in T_i,0, we triangulate points in T_i,0 and find the smallest edge (since the Delaunay Triangulation 
	// contains the nearest neighbor graph)
	// Note here that instead of Triangulating each T_i,0 separately, we can merge the T_i,0 for all i so T_0 = "union of T_i,0" then
	// triangulate them all. We know any pair (u, v) with "u in T_i,0 and v in T_j,0 i != j" will have distance(u,v) > r since u in G_i
	// and v in G_j thus if there is a pair (u, v) in T_0 with distance(u, v) <= r then (u, v) are necessarily in the same T_i,0
	// thus we just find the closest pair (u,v) in T_0 and if distance(u, v) <= r then one of the T_i,0 is not 2-colorable thus the
	// corresponding G_i is not 2-colorable => G not 2 colorable
	// Then the question of finding if Holmes (H) and Watson (W) can communicate is just about finding if H and W are in range of each
	// other and if not, we need to check if the closest vertex v_H of H is in the same component of G as the closest vertex v_W of W
	// i,e v_H and v_W are in the same T_i
	// Note that in the code below we add all edges with length <= r. This is not a problem because these are just some edges of the G_i
	// added to the T_i (the analysis stays the same)
	// Also, we use the fact that to decide whether a graph is 2 colorable is equivalent to deciding if the graph is bipartite (check:
	// https://en.wikipedia.org/wiki/Graph_coloring#Polynomial_time) so we use the is_bipartite function of boost which will check if
	// the graph is bipartite and if so assign a color to each vertex (black or white)
	
	int n, m; long rr; std::cin >> n >> m >> rr; // n=#stations, m=#clues, rr=squared_radius
	rr *= rr;
	
	std::vector<std::pair<K::Point_2, int>> stations(n); // each pair is a vertex and the index of the vertex stored as info()
	for(int i=0; i<n; i++) {
		int x, y; std::cin >> x >> y;
		stations[i] = std::make_pair(K::Point_2(x, y), i);
	}
	
	std::vector<K::Point_2> holmes;
	std::vector<K::Point_2> watson;
	for(int i=0; i<m; i++) { // read the different positions of holmes and watson
		int x1, y1, x2, y2; std::cin >> x1 >> y1 >> x2 >> y2;
		holmes.push_back(K::Point_2(x1, y1));
		watson.push_back(K::Point_2(x2, y2));
	}
	// construct the triangulation of the stations
	Triangulation t;
	t.insert(stations.begin(), stations.end());
	
	Graph G_r(n); // graph G_r containing all edges in the triangulation with squared_length <= r*r
	
	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) { // go over finite edges
		Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
		Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
		if(CGAL::squared_distance(v1->point(), v2->point()) <= rr) {
			boost::add_edge(v1->info(), v2->info(), G_r); // add edge
		}
	}
	// code from boost doc to check if bipartite (i,e 2-colorable) and assign colors accordingly
	partition_t partition(n);
    partition_map_t partition_map (partition.begin(), boost::get(boost::vertex_index, G_r));
    bool colorable = is_bipartite(G_r, boost::get(boost::vertex_index, G_r), partition_map);
    
    if(!colorable) { // not 2-colorable
		print_n(m); return; // print m times the character "n" and return
	}
	
	bool no_interference = false; // we check if the found coloring is a fine coloring
	std::vector<std::pair<K::Point_2, int>> white_stations; // store vertices colored white
	std::vector<std::pair<K::Point_2, int>> black_stations; // store vertices colored black
	for(int i=0; i<n; i++) { // split the stations to white and black stations
		if(partition_map[i] == Color::white()) {
			white_stations.push_back(stations[i]);
		} else {
			black_stations.push_back(stations[i]);
		}
	}
	Triangulation t_white, t_black;
	t_white.insert(white_stations.begin(), white_stations.end());
	t_black.insert(black_stations.begin(), black_stations.end());
	// check if the coloring found is fine for graph G (see explanation above)
	no_interference = fine_coloring(t_white, rr) and fine_coloring(t_black, rr);
	if(!no_interference) { // G not 2-colorable
		print_n(m); return; // print m times the character "n" and return
	}
	// find the connected components in G_r
	std::vector<int> componentmap(n);
	boost::connected_components(G_r, boost::make_iterator_property_map(componentmap.begin(), boost::get(boost::vertex_index, G_r)));
	
	for(int i=0; i<m; i++) {
		if(CGAL::squared_distance(holmes[i], watson[i]) <= rr) { // holmes and watson whithin range => can communicate
			std::cout << "y"; continue;
		}
		Vertex_handle s_h = t.nearest_vertex(holmes[i]); // the nearest station to holmes
		Vertex_handle s_w = t.nearest_vertex(watson[i]); // the nearest station to watson
		if(CGAL::squared_distance(holmes[i], s_h->point()) <= rr and CGAL::squared_distance(watson[i], s_w->point()) <= rr
			and componentmap[s_h->info()] == componentmap[s_w->info()]) { // the closest stations are whithin range and connected
				std::cout << "y";		
		} else 
			std::cout << "n";
	}
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
