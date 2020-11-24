#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

int testcase() {
	int n; std::cin >> n; // n=#segments
	if(n == 0) return 0; 
	
	long x, y, a, b; std::cin >> x >> y >> a >> b; // read the coordinates of the ray points
	K::Ray_2 r(K::Point_2(x,y), K::Point_2(a,b));
	
	std::vector<K::Segment_2> segs; // to store the n segments
	for(int i=0; i<n; i++) {
		long r, s, t, u; std::cin >> r >> s >> t >> u;
		K::Segment_2 seg(K::Point_2(r,s), K::Point_2(t,u));
		segs.push_back(seg);
	}
	
	bool intersect = false;
	for(int i=0; i<n; i++) { // find if ray intersects any segment
		intersect = intersect || CGAL::do_intersect(r, segs[i]);
		if(intersect) break;
	}
	if(intersect) {
		std::cout << "yes" << std::endl;
	} else {
		std::cout << "no" << std::endl;
	}
	return 1;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	while(testcase());
	return 0;
}
