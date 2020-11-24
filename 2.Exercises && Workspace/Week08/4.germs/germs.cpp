#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Finite_vertices_iterator Vertex_iterator;
typedef Triangulation::Edge_iterator  Edge_iterator;

int ceil_to_int(const K::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a-1 >= x) a -= 1;
  while (a < x) a += 1;
  return a;
}

K::FT hour_of_death(std::vector<K::FT>& squared_radiuses, int i) {
	K::FT radius_at_death = CGAL::sqrt(squared_radiuses[i]);
	return CGAL::sqrt(CGAL::max(0.0, radius_at_death - 0.5));
}

int testcase() {
	// we know each bacteria keeps growing indefinitely, each bacteria is a disk with growing radius
	// for each bacteria b, it will touch the closest neighbor bacteria n before any other bacteria at the middle
	// of the edge [b, n]. The edge [b, n] must be a Delaunay edge since Delaunay graph contains nearest neighbor graph
	// So a bacteria dies either because it first touched the closest neighbor or the dish (bounding box) border, whichever is closest.
	// We just find the (squared) radius of each bacteria at the moment of death and sort bacteria by moment of death
	// (i,e by radius at the moment of death since all bacteria grow the same way) which gives us the 1st, median and last bacteria
	// by radius size then use this to compute f, m and l
	int n; std::cin >> n; // n=#bacteria
	if(n == 0) return 0;
	
	// all computed distances below are squared
	// bouding box rectangle corner points
	K::Point_2 low_left; std::cin >> low_left;
	K::Point_2 up_right; std::cin >> up_right;
	K::Point_2 low_right(up_right.x(), low_left.y());
	K::Point_2 up_left(low_left.x(), up_right.y());
	// bounding box lines
	K::Line_2 left_border(low_left, up_left);
	K::Line_2 right_border(low_right, up_right);
	K::Line_2 up_border(up_left, up_right);
	K::Line_2 low_border(low_left, low_right);
	std::vector<K::Point_2> pts;
	for (int i = 0; i < n; i++) {
		K::Point_2 p;
		std::cin >> p;
		pts.push_back(p);
	}
	// construct triangulation
	Triangulation t;
	t.insert(pts.begin(), pts.end());
	
	// we suppose at first that each bacteria dies by touching a border, so find closest border
	std::map<Vertex_handle, K::FT> radius_at_death;
	for(Vertex_iterator v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++) {
		K::FT dist_to_left = CGAL::squared_distance(v->point(), left_border);
		K::FT dist_to_right = CGAL::squared_distance(v->point(), right_border);
		K::FT dist_to_up = CGAL::squared_distance(v->point(), up_border);
		K::FT dist_to_low = CGAL::squared_distance(v->point(), low_border);
		radius_at_death[v] = CGAL::min(dist_to_left, CGAL::min(dist_to_right, CGAL::min(dist_to_up, dist_to_low)));
	}
	// find closest neighbor and update radius at death if nedded
	for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Vertex_handle v1 = e->first->vertex((e->second + 1) % 3);
		Vertex_handle v2 = e->first->vertex((e->second + 2) % 3);
		K::FT edge_length_halfed = CGAL::squared_distance(v1->point(), v2->point()) / 4.0;
		radius_at_death[v1] = CGAL::min(edge_length_halfed, radius_at_death[v1]);
		radius_at_death[v2] = CGAL::min(edge_length_halfed, radius_at_death[v2]);
	}
	
	// sort radiuses at death of bacterias
	std::vector<K::FT> sorted_radiuses;
	for(auto it = radius_at_death.begin(); it != radius_at_death.end(); it++) {
		sorted_radiuses.push_back(it->second);
	}
	std::sort(sorted_radiuses.begin(), sorted_radiuses.end());
	
	int f, m, l;
	f = ceil_to_int(hour_of_death(sorted_radiuses, 0));
	m = ceil_to_int(hour_of_death(sorted_radiuses, n/2));
	l = ceil_to_int(hour_of_death(sorted_radiuses, sorted_radiuses.size()-1));
	
	std::cout << f << " " << m << " " << l << std::endl;
	return 1;
}

int main()
{
	std::ios_base::sync_with_stdio(false);
	
	while(testcase());
	return 0;
}
