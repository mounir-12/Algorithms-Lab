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

K::FT combine(K::FT val1, K::FT val2) { // 1st arg is max width to get to current vertex, 2nd arg is width of edge from curr to neighbor vertex
	return CGAL::min(val1, val2);
}

bool compare(K::FT val1, K::FT val2) { // return true if val1 > val2 (since we want to maximize)
	return val1 > val2;
}

void add_edge(Vertex& u, Vertex& v, Graph& G, WeightMap& weightmap, K::FT& squared_length) {
	Edge e;	bool success;
	boost::tie(e, success) = boost::add_edge(u, v, G);
	weightmap[e] = squared_length;
}
void testcase() {
	
	// The disk D is centered at the entrance and has diameter 2p. We want to move it through voronoi edges from entrance to exit
	// such as the path is "wide enough" for the disk to move from face to face (i,e each delaunay edge crossed by moving from
	// 1 face to another must be at least 2p in length)
	// Since we consider each snake square (by adding 4 delaunay points for each of the 4 corners of a snake square, then the disk cannot
	// enter any of the snake faces (i,e triangle splitting a 1x1 snake square in half) since edges of these faces connected to 
	// a free square have length 1, so these "entry edges" to a snake from a free face are too small for a disk of diameter 2p >= 2
	// to cross them
	// So we can connect every finite face to all its neighboring finite faces and not worry about the possibility that the face is 
	// a snake face
	// We consider 2 axis, x-axis points right and y axis points up, the grid is axis aligned and the lower-left point of its lower-left
	// square is at the origin (0, 0). Likewise, the upper-left corner of its upper-left square is at (0, h), the upper-right
	// corner of its upper-right square is at (w, h) and the lower-right corner of its lower right square is at (0, w)
	// each square being indexed by (j, i) where (j, i) is the coordinates of the square's lower-left corner
	
	int h, w, p; std::cin >> h >> w >> p;
	
	int q, r, s, t; std::cin >> q >> r >> s >> t;
	r--; t--; // 0 based indices
	q = h-q; s = h-s; // y axis points up 
	K::Point_2 entrance(q+0.5, r+0.5); // entrance at the middle of the entrance square
	K::Point_2 exit(s+0.5, t+0.5); // exit at the middle of the exit square
	
	//std::cout << "entrance: " << entrance << ", exit: " << exit << std::endl;
	
	std::vector<K::Point_2> pts;
	for(int i=h-1; i>=0; i--) { // represents the y axis (points up)
		for(int j=0; j<w; j++) { // represents the x axis (points right)
			char c; std::cin >> c;
			if(c == '0') { // this is a snake square get its corner points
				K::Point_2 p1(j, i+1); pts.push_back(p1); // upper left corner of snake sqaure
				K::Point_2 p2(j+1, i+1); pts.push_back(p2); // upper right corner of snake sqaure
				K::Point_2 p3(j+1, i); pts.push_back(p3); // lower right corner of snake sqaure
				K::Point_2 p4(j, i); pts.push_back(p4); // lower left corner of snake sqaure
			}
		}
	}
	// construct triangulation
	Triangulation tr;
	tr.insert(pts.begin(), pts.end());
	
	std::map<Face_handle, int> face_to_index; // map each finite face to a vertex index
	
	int i = 0;
	for(Face_iterator f = tr.finite_faces_begin(); f != tr.finite_faces_end(); ++f) {
		face_to_index[f] = i; i++; // assign a vertex number to the finite face
	}
	
	int V = tr.number_of_faces(); // number of finite faces
	Graph G(V); // add a vertex for each finite face
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	for(Face_iterator f = tr.finite_faces_begin(); f != tr.finite_faces_end(); ++f) { // loop over finite faces
		Vertex u = face_to_index[f]; // vertex number of current face
		for(int i=0; i<3; i++) {
			Face_handle neighbor = f->neighbor(i);
			K::Point_2 p1 = f->vertex((i+1)%3)->point(); // 1st vertex of the common edge between f and neighbor
			K::Point_2 p2 = f->vertex((i+2)%3)->point(); // 2nd vertex of the common edge between f and neighbor
			K::FT squared_length = CGAL::squared_distance(p1, p2); //  triangle edge squared length
			if(!tr.is_infinite(neighbor)) { // infinite faces are outside the grid, we need to stay inside the grid, so no edge to infinite face
				Vertex v = face_to_index[neighbor];
				add_edge(u, v, G, weightmap, squared_length);
			}
		}
	}
	
	Face_handle start = tr.locate(entrance); Vertex source = face_to_index[start]; // initial face of the disk
	Face_handle finish = tr.locate(exit); Vertex target = face_to_index[finish]; // target face
	
	std::vector<K::FT> widthmap(V);
	
	// find widest path
	boost::dijkstra_shortest_paths(G, source, boost::distance_map(&widthmap[0]).distance_zero(DBL_MAX)
	.distance_inf(0).distance_combine(&combine).distance_compare(&compare));
	
	//std::cout << "bottleneck width: " << widthmap[target] << std::endl;
	if(widthmap[target] >= 4*p*p) {
		std::cout << "yes" << std::endl;
	} else {
		std::cout << "no" << std::endl;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
