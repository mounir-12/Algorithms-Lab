#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

int ceil_to_int(const EK::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}
int testcase() {
	// we know that Delaunay Triangulation contains the nearest neighbor graph
	// so for each graype (each position), its nearest neighbor is linked to it by
	// a Delaunay edge. We find the smallest edge which gives us the smallest
	// distance to travel. The 2 graypes at the end points of this edge would meet at
	// the center, so each one travels half the distance 
	int n; std::cin >> n; // n=#graypes
	if(n == 0) return 0;
	
	std::vector<IK::Point_2> pts;
	for (int i = 0; i < n; i++) {
		IK::Point_2 p; std::cin >> p;
		pts.push_back(p);
	}
	// construct triangulation
	Triangulation t;
	t.insert(pts.begin(), pts.end());
	
	Edge_iterator e = t.finite_edges_begin();
	IK::FT min_squared_dist = t.segment(e).squared_length(); // first edge distance as min_distance
	e++;
	// Find the smallest edge
	while(e != t.finite_edges_end()) {
		IK::FT squared_dist = t.segment(e).squared_length(); // distance in meter
		if(squared_dist < min_squared_dist) {
			min_squared_dist = squared_dist;
		}
		e++;
	}
	// convert to exact type then divide by 4 and compute sqrt
	EK::FT traveled_dist = CGAL::sqrt(EK::FT(min_squared_dist) / 4.0); // traveled distance with speed of 1m/s so this is also travel time
	std::cout << ceil_to_int(traveled_dist*100) << std::endl; // print traveled distance per 100 sec
	return 1;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	while(testcase());
	return 0;
}
