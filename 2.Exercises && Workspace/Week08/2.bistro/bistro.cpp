#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

int testcase() {
	int n; // n=#existing_restaurants
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
	
	int m; std::cin >> m; // m=#candidate_positions
	
	for(int i=0; i<m; i++) {
		 K::Point_2 p; std::cin >> p; // read candidate position for new restaurant
		 K::Point_2 q = t.nearest_vertex(p)->point(); // find closest the closest restaurant
		 std::cout << CGAL::to_double(CGAL::squared_distance(p, q)) << std::endl;
	}
	return 1;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	std::cout << std::fixed << std::setprecision(0); // print all digits before the dot and no digit after the dot (no scientific notation)
	while(testcase());
	return 0;
}
