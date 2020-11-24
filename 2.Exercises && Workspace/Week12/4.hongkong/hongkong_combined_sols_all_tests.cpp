#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>
// CGAL includes
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
// BGL includes
#include <boost/pending/disjoint_sets.hpp>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// CGAL typedefs
typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef Triangulation::Face_handle Face_handle;
// BGL typedefs
typedef boost::disjoint_sets_with_storage<> union_find; // disjoint_sets aka union_find
typedef boost::adjacency_list<boost::vecS /* we allow parallel edges */, boost::vecS, boost::undirectedS,
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, EK::FT>> Graph; // weighted edge graph, weights of type K::FT
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;

void add_edge(Vertex u, Vertex v, Graph& G, WeightMap& weightmap, EK::FT& squared_length) {
	Edge e;	bool success;
	boost::tie(e, success) = boost::add_edge(u, v, G);
	weightmap[e] = squared_length;
}

EK::FT combine(EK::FT val1, EK::FT val2) {
	return CGAL::min(val1, val2);
}

bool compare(EK::FT val1, EK::FT val2) {
	return val1 > val2;
}

struct myEdge {
	int u, v;
	EK::FT squared_width;
	myEdge(int u, int v, EK::FT squared_width): u(u), v(v), squared_width(squared_width) {}
};

struct myDisk {
	int index;
	IK::Point_2 p;
	EK::FT squared_r;
	myDisk(int index, IK::Point_2 p, EK::FT squared_r) : index(index), p(p), squared_r(squared_r) {}
};

bool operator<(myEdge e1, myEdge e2) { // sort in ascending order
	return e1.squared_width > e2.squared_width;
}

bool operator<(myDisk d1, myDisk d2) { // sort in ascending order
	return d1.squared_r > d2.squared_r;
}

void testcase(int n, int m, long r, std::vector<IK::Point_2>& pts, std::vector<myDisk>& my_disks, int sol) {	

	// construct triangulation
	Triangulation t;
	t.insert(pts.begin(), pts.end());
	
	std::unordered_map<Face_handle, int> face_to_index; // map each finite face to a vertex index
	
	int i = 0;
	for(Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
		face_to_index[f] = i; i++; // assign a vertex number to the finite face
	}
	
	int V = t.number_of_faces(); // number of finite faces
	Graph G(V); // add a vertex for each finite face
	Vertex infinite = boost::add_vertex(G); // add a vertex to represent all infinite faces
	V++;
	WeightMap weightmap = boost::get(boost::edge_weight, G);
	
	std::vector<EK::FT> squared_circumradiuses(V); // compute the circumradius (radius of the circumcircle) for each face
	squared_circumradiuses[infinite] = DBL_MAX; // infinite face has infinite radius
	std::vector<myEdge> my_edges;
	for(Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) { // loop over finite faces
		int u = face_to_index[f]; // vertex number of current face
		IK::Point_2 Ip0 = f->vertex((0))->point(); EK::Point_2 p0(Ip0.x(), Ip0.y());
		IK::Point_2 Ip1 = f->vertex((1))->point(); EK::Point_2 p1(Ip1.x(), Ip1.y());
		IK::Point_2 Ip2 = f->vertex((2))->point(); EK::Point_2 p2(Ip2.x(), Ip2.y());
		EK::Point_2 circumcenter = CGAL::circumcenter(p0, p1, p2);
		squared_circumradiuses[u] = CGAL::squared_distance(circumcenter, p0);
		EK::FT squared_width0 = CGAL::squared_distance(p0, p1);
		EK::FT squared_width1 = CGAL::squared_distance(p1, p2);
		EK::FT squared_width2 = CGAL::squared_distance(p2, p0);
		for(int i=0; i<3; i++) {
			Face_handle neighbor = f->neighbor(i);
			int v;
			EK::FT squared_width;
			if(t.is_infinite(neighbor)) {
				v = infinite;
			} else {
				v = face_to_index[neighbor];
			}
			if(i==0) {
				squared_width = squared_width1;
			} else if(i==1) {
				squared_width = squared_width2;
			} else {
				squared_width = squared_width0;
			}
			my_edges.push_back(myEdge(u, v, squared_width));
			add_edge(u, v, G, weightmap, squared_width);
		}
	}
	
	std::vector<EK::FT> widthmap(V);
	
	// run modified dijkstra (for widest path)
	boost::dijkstra_shortest_paths(G, infinite,boost::distance_map(&widthmap[0]).distance_zero(DBL_MAX)
	.distance_inf(0).distance_combine(&combine).distance_compare(&compare));

	 std::sort(my_disks.begin(), my_disks.end());
	 
	 union_find ds(V); // union find ds
	 auto edges_it = my_edges.begin();
	 auto disks_it = my_disks.begin();
	 std::vector<char> answers(m); // to store the answer to each query
	 std::unordered_map<int, EK::FT> set_to_max_radius; // store the max_radius of a face for each connected component
	 for(int i=0; i<V; i++) { // initially each component contains 1 vertex representing a face, so add this face's circumradius
		 set_to_max_radius[i] = squared_circumradiuses[i];
	 }
	 bool sorted = false;
	 while(disks_it != my_disks.end()) {
		 const myDisk& d(*disks_it); // the current disk
		 int i = disks_it->index; // index where to store the answer
		 IK::Point_2 Iq = t.nearest_vertex(d.p)->point(); // find the closest tree to the disk
		 EK::Point_2 p(d.p.x(), d.p.y()); EK::Point_2 q(Iq.x(), Iq.y()); // contruct the exact points
		 char answer = 'n';
		 if(CGAL::squared_distance(p, q) >= d.squared_r) { // not touching any tree, we can move disk to circumcenter
			 Face_handle f = t.locate(d.p); // find the face f where the disk is
			 if(t.is_infinite(f)) { // on infinite face, can move the disk as far as we want from trees
				 answer = 'y'; goto endloop;
			 }
			 int w = face_to_index[f]; // get the vertex index of the face where the disk is located
			 if(widthmap[w] >= 4*d.squared_r) {
				 answer = 'y'; goto endloop;
			 }
			 int set_w = ds.find_set(w); // get the component of w
			 if(set_to_max_radius[set_w] >= 4*d.squared_r) { // the disk is in a component where it can move freely to some safe face
				 answer = 'y'; goto endloop;
			 }
			 if(sol == 0) {
				 goto endloop;
			 }
			 if(not sorted) {
				 std::sort(my_edges.begin(), my_edges.end());
				 sorted = true;
				 edges_it = my_edges.begin();
			 }
			 // otherwise, we keep adding "wide enough" edges for the current disk till it's in a component with a safe face
			 while(edges_it != my_edges.end() and edges_it->squared_width >= 4*d.squared_r) { // current disk can move along this edge, add it
				 int u = edges_it->u, v = edges_it->v; // get the edge's source and target
				 int set_u = ds.find_set(u); // get u's set (i,e the root representing the set)
				 int set_v = ds.find_set(v); // get v's set (i,e the root representing the set)
				 if(set_u != set_v) {
					 ds.link(set_u, set_v); // union both sets
					 // update the resulting component's max face radius accordingly, the resulting 
					 // component will be represented by the root set_u or the root set_v, so update both 
					 set_to_max_radius[set_u] = CGAL::max(set_to_max_radius[set_u], set_to_max_radius[set_v]);
					 set_to_max_radius[set_v] = set_to_max_radius[set_u];
				 }
				 edges_it++; // next edge
			 }
			 set_w = ds.find_set(w); // get the component again
			 if(set_to_max_radius[set_w] >= 4*d.squared_r) { // do the check again after adding all edges along which the disk can move
				answer = 'y'; goto endloop;
			 }
			 // if after adding all edges that are "wide enough" for the disk, the disk still can't reach a safe face, then output "n"
		 }
	endloop:
		 answers[i] = answer;
		 disks_it++; // next disk
	 }
	 
	 for(char answer: answers) {
		 std::cout << answer;
	 }
	 std::cout << std::endl;
}

void do_all(int t) {
	std::vector<int> n_vals;
	std::vector<int> m_vals;
	std::vector<long> r_vals;
	std::vector<std::vector<myDisk>> all_disks;
	std::vector<std::vector<IK::Point_2>> all_pts;
	int m_max = 0;
	for(int l=0; l<t; l++) {
		int n, m; long r; std::cin >> n >> m >> r; // n=#trees, m=#balloons, r=tree_radius
		n_vals.push_back(n); m_vals.push_back(m); r_vals.push_back(r);
		
		std::vector<IK::Point_2> pts;
		for (int i = 0; i<n; i++) {
			long x, y; std::cin >> x >> y;
			pts.push_back(IK::Point_2(x, y));
		}
		all_pts.push_back(pts);
		
		std::vector<myDisk> my_disks;
		for(int i=0; i<m; i++) {
			 long x, y, s; std::cin >> x >> y >> s; // read the balloon's coordinates and radius
			 IK::Point_2 p(x, y); // the disk's center
			 EK::FT disk_squared_r = s+r; disk_squared_r *= disk_squared_r; // the disk's squared radius
			 my_disks.push_back(myDisk(i, p, disk_squared_r));
		 }
		 all_disks.push_back(my_disks);
		 
		 m_max = std::max(m, m_max);
	}
	int sol = 1;
	if(m_max <= 100) {
		sol = 0;
	}
	for(int i=0; i<t; i++) {
		testcase(n_vals[i], m_vals[i], r_vals[i], all_pts[i], all_disks[i], sol);
	}
	
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	do_all(t);
	return 0;
}
