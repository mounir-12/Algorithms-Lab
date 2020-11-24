#include <iostream>
#include <vector>
#include <set>
#include <map>
// CGAL includes
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


// CGAL typedefs
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
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

void add_edge(Vertex u, Vertex v, Graph& G, WeightMap& weightmap, K::FT& squared_length) {
	Edge e;	bool success;
	boost::tie(e, success) = boost::add_edge(u, v, G);
	weightmap[e] = squared_length;
}

K::FT combine(K::FT val1, K::FT val2) {
	return CGAL::min(val1, val2);
}

bool compare(K::FT val1, K::FT val2) {
	return val1 > val2;
}

void testcase() {
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
	
	int n, m; long r; std::cin >> n >> m >> r; // n=#trees, m=#balloons, r=tree_radius

	std::vector<K::Point_2> pts;
	for (int i = 0; i<n; i++) {
		long x, y; std::cin >> x >> y;
		pts.push_back(K::Point_2(x, y));
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
	
	std::vector<K::FT> squared_circumradiuses(V-1); // compute the circumradius (radius of the circumcircle) for each finite face
	for(Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) { // loop over finite faces
		Vertex u = face_to_index[f]; // vertex number of current face
		K::Point_2 circumcenter = t.dual(f); // circumcenter coordinates
		K::Point_2 q0 = f->vertex(0)->point(); // get some vertex of the face
		squared_circumradiuses[u] = CGAL::squared_distance(circumcenter, q0);
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
	
	// run modified dijkstra (for widest path)
	boost::dijkstra_shortest_paths(G, infinite,boost::distance_map(&widthmap[0]).distance_zero(DBL_MAX)
	.distance_inf(0).distance_combine(&combine).distance_compare(&compare));
	
	for(int i=0; i<m; i++) {
		 long x, y, s; std::cin >> x >> y >> s; // read the balloon's coordinates and radius
		 K::Point_2 p(x, y); // the disk's center
		 K::FT disk_r = s+r; // the disk's radius
		 K::FT disk_squared_r = disk_r*disk_r; // the disk's squared radius
		 K::Point_2 q = t.nearest_vertex(p)->point(); // find the closest tree
		 if(CGAL::squared_distance(p, q) >= disk_squared_r) { // not touching any tree, we can move disk to circumcenter
			 Face_handle f = t.locate(p); // find the face f where the disk is
			 if(t.is_infinite(f)) { // on infinite face, can move the disk as far as we want from trees
				 std::cout << "y";
				 continue;
			 }
			 // otherwise, the face is finite, we move disk to circumcenter
			 Vertex u = face_to_index[f]; // get the finite face vertex index
			 if(squared_circumradiuses[u] >= 4*disk_squared_r) { // moving to the circumcenter is enough
				 std::cout << "y";
				 continue;
			 }
			 if(widthmap[u] >= 4*disk_squared_r) { // there exists a "wide enough" path from f to infinite face  
				 std::cout << "y";
				 continue;
			 }
		 }
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
