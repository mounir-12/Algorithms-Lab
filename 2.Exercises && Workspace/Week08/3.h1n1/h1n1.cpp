#include <iostream>
#include <vector>
#include <set>
#include <map>
// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// CGAL typedefs
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef Triangulation::Face_handle Face_handle;
//BGL typedefs
typedef boost::adjacency_list<boost::vecS /* we allow parallel edges */, boost::vecS, boost::undirectedS,
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, K::FT>> Graph; // weighted edge graph, weights of type K::FT
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;

void add_edge(Vertex& u, Vertex& v, Graph& G, WeightMap& weightmap, K::FT& squared_length) {
	Edge e;	bool success;
	boost::tie(e, success) = boost::add_edge(u, v, G);
	weightmap[e] = squared_length;
}

K::FT combine(K::FT val1, K::FT val2) { // 1st arg is max width to get to current vertex, 2nd arg is width of edge from curr to neighbor vertex
	return CGAL::min(val1, val2);
}

bool compare(K::FT val1, K::FT val2) { // return true if val1 > val2 (since we want to maximize)
	return val1 > val2;
}

int testcase() {
	// We model each healhy person at initial position I=(x,y) and its safety distance r=sqrt(d)
	// by a disk D centered at I with radius r
	// This is a path planning problem, the best strategy is to move D along Voronoi edges
	// We construct the Delaunay Triangulation of the positions of sick people
	// We first check if any sick person is inside the disk D at the initial position
	// if not, let the triangle ABC be the face containing I initially and O the circumcenter.
	// We have r <= OA = R = radius of circumcircle of ABC (since distance from I 
	// to closest sick person <= d), so we can move the center I of D to O (O is a Voronoi vertex).
	// The idea is to keep moving D from circumcenter of its current face to the circumcenter
	// of a neighboring face till D is free (i,e reach an infinite face).
	// So we need to find if there is such a path from O to an infinite face "wide enough" for D to pass
	// through all the faces in this path. This is a "widest path problem" where we have a graph G that
	// has a vertex v_i for each circumcenter c_i of each finite face f_i and we add an edge e=(v_i, v_j)
	// (which represents a Voronoi edge) if f_i adjacent to f_j and the "width" of e is the length of the
	// common edge between f_i and f_j
	// we then run a modifed Dijkstra algorithm wich solves the "widest path problem" aka " Maximum-bottleneck path"
	// check: http://www.cs.cmu.edu/~avrim/451f08/lectures/lect1007.pdf
	
	int n; // n=#infected_people
	std::cin >> n;
	if(n == 0) return 0;

	std::vector<K::Point_2> pts;
	for (int i = 0; i<n; i++) {
		K::Point_2 p;
		std::cin >> p;
		pts.push_back(p);
	}
	// construct triangulation
	Triangulation t;
	t.insert(pts.begin(), pts.end());
	
	std::map<Face_handle, int> face_to_index; // map each finite face to a vertex index
	
	int i = 0;
	for(Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
		face_to_index[f] = i; i++; // assign a vertex number to the finite face
	}
	
	int V = t.number_of_faces(); // number of finite faces
	Graph G(V); // add a vertex for each finite face
	Vertex infinite = boost::add_vertex(G); // add a vertex to represent all infinite faces
	V++;
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	for(Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) { // loop over finite faces
		Vertex u = face_to_index[f]; // vertex number of current face
		for(int i=0; i<3; i++) {
			Face_handle neighbor = f->neighbor(i);
			K::Point_2 p1 = f->vertex((i+1)%3)->point(); // 1st vertex of the common edge between f and neighbor
			K::Point_2 p2 = f->vertex((i+2)%3)->point(); // 2nd vertex of the common edge between f and neighbor
			K::FT squared_length = CGAL::squared_distance(p1, p2); //  triangle edge squared length
			if(t.is_infinite(neighbor)) {
				add_edge(u, infinite, G, weightmap, squared_length);
			} else {
				Vertex v = face_to_index[neighbor];
				add_edge(u, v, G, weightmap, squared_length);
			}
		}
	}
	std::vector<K::FT> widthmap(V);
	
	// run modified dijkstra where the "distance to u" = "max width to u"
	// distance_zero assigned to source vertex at init must be infinity (since disk of any width accepted when at infinite face)
	// distance_inf assigned to other vertices at init must 0
	boost::dijkstra_shortest_paths(G, infinite,boost::distance_map(&widthmap[0]).distance_zero(DBL_MAX)
	.distance_inf(0).distance_combine(&combine).distance_compare(&compare));
	
	int m; std::cin >> m; // m=#healthy_people
	
	for(int i=0; i<m; i++) {
		 K::Point_2 p; std::cin >> p; // read the healthy person's position
		 double d; std::cin >> d; // read safety squared distance
		 K::Point_2 q = t.nearest_vertex(p)->point(); // find the closest sick person
		 if(CGAL::squared_distance(p, q) >= d) {
			 Face_handle f = t.locate(p); // find the face f where the healthy person is
			 if(t.is_infinite(f) || widthmap[face_to_index[f]] >= 4*d) { // f is infinite or there exists a "wide enough" path from f to infinite face  
				 std::cout << "y";
				 continue;
			}
		 }
		 std::cout << "n";
	}
	std::cout << std::endl;
	return 1;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	while(testcase());
	return 0;
}
