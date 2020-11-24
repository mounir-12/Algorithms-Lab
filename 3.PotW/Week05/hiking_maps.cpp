#include <iostream>
#include <vector>
// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

// check if triangle t (defined by 6 points) contains the point p (triangle edges included)
bool contains_p(std::vector<K::Point_2>& t, K::Point_2 p) {
	// we perform this check without constructing the triangles but only using the predicate right_turn (since faster)
	// the function supposes the points q0=t[0], q1=t[1], q2=t[2], q3=t[3], q4=t[4] and q5=t[5] of triangle t are given in ccw order
	// p is inside the triangle (triangle edges included) iff p is on or on the left of the line (q0, q1) (i,e
	// not on the right of the line (q0, q1)), and same thing for the line (q2, q3) and (q3, q4)
	return !CGAL::right_turn(t[0], t[1], p) 
		and !CGAL::right_turn(t[2], t[3], p) 
		and !CGAL::right_turn(t[4], t[5], p);
}

void testcase() {
	// We have m points p_1, .., p_m that define path segments S_i = [p_i, p_(i+1)]
	// we have n triangles indexed 0...(n-1) and we would like to find 2 indices b,e
	// 0<= b < e <= n-1 such that k=(b-e+1) is minimized and the triangles t_b ... t_e
	// cover all segements S_i. We say that a segment is covered by a triangle t if S_i
	// is completely inside t because we want that "every single leg (aka segment) of 
	// their hike should be contained on a single map part (aka triangle).
	// So a segment that is for e,g half inside triangle t_i and the other half is inside
	// triangle t_j is still considered as "not covered".
	// This problem can be solved with a sliding window problem where we start with left and right pointers
	// at 0, then increment right pointer as long as not all segements are covered. To find how
	// many triangles cover a segment S_i, we keep the count in an array bins where bins[i] is the
	// number of triangles between left and right pointer that cover segment S_i.
	// Once all segments are covered by the triangles between left and right pointer, we start moving
	// the left pointer. We keep moving it untill 1 or more segments are no longer covered.
	// Moving the right pointer adds triangles to the window, whereas moving left pointer removes them. 
	// Whenever we add a triangle, we increment bins[i] for all S_i covered by the added triangle.
	// Whenever we remove a triangle, we decrement bins[i] for all S_i covered by the removed triangle.
	// All segements are covered if bins[i] > 0 for all i. We compute the min k for all triangle windows
	// that cover all S_i. S_i is covered by t iff both endpoints of S_i are inside t.
	// We use the function contains_p() above to perform this check without constructing t, only using predicates
	
	int m, n; std::cin >> m >> n; // m=#points, n=#triangles
	std::vector<K::Point_2> points; // the points p_0, ..., p_(m-1)
	for(int i=0; i<m; i++) {
		K::Point_2 p; std::cin >> p;
		points.push_back(p);
	}
	std::vector<K::Segment_2> segs; // the m-1 segments
	for(int i=0; i<m-1; i++) {
		segs.push_back(K::Segment_2(points[i], points[i+1]));
	}
	
	std::vector<std::vector<int>> triangles_cover(n); // store index i the indices of the segements covered by triangle i
	for(int i=0; i<n; i++) { // read the triangles
		std::vector<K::Point_2> triangle(6); // ith triangle
		// read the points q0, q1, q2, q3, q4, q5
		std::cin >> triangle[0] >> triangle[1] >> triangle[2] >> triangle[3] >> triangle[4] >> triangle[5];
		// for the function contains_p() above to work, the points q0, q1, q2, q3, q4 and q5 must be in ccw order
		// so make sure that q2 is on the left of the line (q0, q1) and that q4 is on the left of the line (q2, q3) 
		// and that q0 is on the left of the line (q4, q5)
		for (int j = 0; j < 6; j+=2)
			if(CGAL::right_turn(triangle[j], triangle[j+1], triangle[(j+2)%6]))
				std::swap(triangle[j], triangle[j+1]);

		for(int j=0; j<m-1; j++) { // find the segments covered by ith triangle
			if(contains_p(triangle, segs[j].source()) and contains_p(triangle, segs[j].target())) { // segment j covered by triangle i
				triangles_cover[i].push_back(j);
			}
		}
	}
	
	int left = 0, right = -1;
	std::vector<int> bins(m-1, 0); // for each segment i, store the number of triangles that cover it
	int min_k = n; // the max cost k is n i,e when right = n-1 and left = 0
	int not_covered = m-1; // number of segments not covered by triangles in window
	while(right < n) {
		if(not_covered != 0) { // still some segments not covered
			right++; // add next triangle
			if(right == n) break; // no more triangle to add
			int l = triangles_cover[right].size(); // number of segments covered by newly added triangle
			for(int i=0; i<l; i++) {
				int j = triangles_cover[right][i]; // index of a segment covered by the newly added triangle
				bins[j]++; // segment j covered by triangle, increment its bin count
				if(bins[j] == 1) { // segment j newly covered
					not_covered--; // decrement the number of segments not covered
				}
			}
		} else { // all segements covered
			min_k = std::min(min_k, right-left+1); // keep the smallest interval length
			// remove left triangle
			int l = triangles_cover[left].size(); // number of segments covered by the triangle to be removed
			for(int i=0; i<l; i++) {
				int j = triangles_cover[left][i]; // index of a segment covered by the triangle to be removed
				bins[j]--; // segment j covered by triangle, decrement its bin count
				if(bins[j] == 0) { // segment j not covered anymore by any triangle
					not_covered++; // increment the number of segments not covered
				}
			}
			left++; // move pointer
		}
	}
	std::cout << min_k << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
