#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <unordered_map>
#include <algorithm>
// CGAL includes
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

// CGAL kernels
typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
// CGAL triangulation
typedef CGAL::Triangulation_vertex_base_2<IK> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<EK::FT, IK> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<IK, Tds> Triangulation;
typedef Triangulation::All_faces_iterator Face_iterator;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Vertex_handle Vertex_handle;

struct FaceComparator {
    bool operator()(Face_handle f1, Face_handle f2)
    {
        return f1->info() < f1->info();
    }
};

struct ExactLengths {
	std::vector<EK::FT> sl; // squared lengths
	ExactLengths() {}
	ExactLengths(Triangulation& t, Face_handle& f) {
		for(int i=0; i<3; i++) {
			Vertex_handle v = f->vertex((i+1)%3);
			Vertex_handle u = f->vertex((i+2)%3);
			if(t.is_infinite(v) || t.is_infinite(u)) {
				sl.push_back(DBL_MAX);
			} else {
				EK::Point_2 p(v->point().x(), v->point().y());
				EK::Point_2 q(u->point().x(), u->point().y());
				sl.push_back(CGAL::squared_distance(p, q));
			}
		}
	}
};

void find_widest_reachable_face(Triangulation& t) {
	std::priority_queue<std::pair<EK::FT, Face_handle>> pq; // priority queue
	std::unordered_map<Face_handle, ExactLengths> face_to_exact_lengths;
	
	for(Face_iterator f = t.all_faces_begin(); f != t.all_faces_end(); ++f) { // loop over all faces
		face_to_exact_lengths[f] = ExactLengths(t, f);
		if(t.is_infinite(f)) {
			f->info() = DBL_MAX;
			pq.push(std::make_pair(DBL_MAX, f));
		} else {
			IK::Point_2 Ip0 = f->vertex((0))->point(); EK::Point_2 p0(Ip0.x(), Ip0.y());
			IK::Point_2 Ip1 = f->vertex((1))->point(); EK::Point_2 p1(Ip1.x(), Ip1.y());
			IK::Point_2 Ip2 = f->vertex((2))->point(); EK::Point_2 p2(Ip2.x(), Ip2.y());
			EK::Point_2 circumcenter = CGAL::circumcenter(p0, p1, p2);
			EK::FT squared_circumradius = CGAL::squared_distance(circumcenter, p0);
			f->info() = squared_circumradius;
			pq.push(std::make_pair(squared_circumradius, f));
		}
		
	}

	while(!pq.empty()) {
		Face_handle f = pq.top().second; EK::FT clearance = pq.top().first; pq.pop();
		if(clearance < f->info()) { // clearance still holds the old version, so we popped an old version, just skip
			continue;
		}
		// update neightboring faces
		for(int i=0; i<3; i++) {
			Face_handle neighbor = f->neighbor(i);
			EK::FT common_edge_length = face_to_exact_lengths[f].sl[i];
			EK::FT new_clearance = CGAL::min(common_edge_length, f->info()); // moving from "neighbor to f" would allow us to reach a face
																		// a face (f or other) with clearance f->info(), but this movement
																		// can only be done by disks of (squared) diameter at most 
																		// "common_edge_length", so the smallest of the 3 values is the
																		// new clearance
			if(new_clearance > neighbor->info()) {
				neighbor->info() = new_clearance;
				pq.push(std::make_pair(new_clearance, neighbor));
			}
		}
	}
}

void testcase() {	
	int n, m; long r; std::cin >> n >> m >> r; // n=#trees, m=#balloons, r=tree_radius
	
	std::vector<IK::Point_2> pts;
	for (int i = 0; i<n; i++) {
		long x, y; std::cin >> x >> y;
		pts.push_back(IK::Point_2(x, y));
	}
	// construct triangulation
	Triangulation t;
	t.insert(pts.begin(), pts.end());
	
	find_widest_reachable_face(t);
	
	for(int i=0; i<m; i++) {
		 long x, y, s; std::cin >> x >> y >> s; // read the balloon's coordinates and radius
		 IK::Point_2 p(x, y); EK::Point_2 p_exact(x, y); // the disk's center (exact and non-exact versions)
		 EK::FT disk_squared_r = s+r; disk_squared_r *= disk_squared_r; // the disk's squared radius
		 IK::Point_2 nearest = t.nearest_vertex(p)->point(); // find closest tree (using non-exact version)
		 EK::Point_2 nearest_exact = EK::Point_2(nearest.x(), nearest.y());
		 
		 if(CGAL::squared_distance(p_exact, nearest_exact) < disk_squared_r) { // can't inflate balloon
			 std::cout << "n";
		 } else {
			 Face_handle f = t.locate(p); // find face containing disk
			 if (f->info() < 4*disk_squared_r) { // not enought clearance for balloon
				std::cout << "n";
			} else {
				std::cout << "y";
			}
		 }
	 }
	 std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
