#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef K::Point_2 Point;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

long field_floor(const K::FT& x) {
	double a = std::floor(CGAL::to_double(x));
	while (a > x) a -= 1;
	while (a+1 <= x) a += 1;
	return a;
}

long field_ceil(const K::FT& x) {
	double a = std::ceil(CGAL::to_double(x));
	while (a-1 >= x) a -= 1;
	while (a < x) a += 1;
	return a;
}

int testcase() {
	int n; std::cin >> n; // n=#homes
	if(n == 0) return 0;
	
	std::vector<Point> homes; // homes to be covered
	for(int i=0; i<n; i++) {
		long x, y; std::cin >> x >> y;
		homes.push_back(Point(x,y));
	}
	
	Min_circle mc(homes.begin(), homes.end(), true); // object representing the unique circle of smallest area enclosing all homes
	Traits::Circle c = mc.circle(); // get the circle
	K::FT min_squared_radius = c.squared_radius();
	
	for(auto it = mc.support_points_begin(); it != mc.support_points_end(); it++) { // the outlier is 1 of the support points
		std::vector<Point> new_homes;
		for(int i=0; i<n; i++) {
			Point home = homes[i];
			if(home != *it) {
				new_homes.push_back(home);
			}
		}
		
		Min_circle mc(new_homes.begin(), new_homes.end(), true); // enclose all except the outlier
		Traits::Circle c = mc.circle(); // get the circle
		min_squared_radius = CGAL::min(min_squared_radius, c.squared_radius()); // min radius that encloses all but outlier
	}
	std::cout << field_ceil(CGAL::sqrt(min_squared_radius)) << std::endl;
	return 1;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	while(testcase());
	return 0;
}
