#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Finite_vertices_iterator Vertex_iterator;
typedef Triangulation::Edge_iterator  Edge_iterator;

long floor_to_long(const K::FT& x)
{
	double a = std::floor(CGAL::to_double(x));
	while (a > x) a -= 1;
	while (a+1 <= x) a += 1;
	return a;
}

void testcase() {
	int n; std::cin >> n; // n=#snow_cannons
	
	std::vector<K::Point_2> pts;
	for (int i = 0; i < n; i++) {
		K::Point_2 p;
		std::cin >> p;
		pts.push_back(p);
	}
	// construct triangulation
	Triangulation t;
	t.insert(pts.begin(), pts.end());
	
	K::FT d_min; bool first = true;
	// find smallest distance between 2 snow cannons (i,e find the distance between the 2 closest snow cannons)
	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
		Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
		K::FT edge_length = CGAL::squared_distance(v1->point(), v2->point());
		if(first) {
			d_min = edge_length;
			first = false;
		} else {
			d_min = CGAL::min(d_min, edge_length);
		}
	}
	
	std::cout << floor_to_long(CGAL::sqrt(d_min/4.0)) << std::endl;
}

int main()
{
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
