#include <iostream>
#include <vector>
// CGAL includes
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

long ceil_to_long(const SolT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

struct Center {
	int x, y;
	Center(int x, int y): x(x), y(y) {}
};

void testcase() {
	// we have n growing rectangles and m fixed-size rectangles. All rectangles have a fixed center
	// and start with height h (along y-axis) and width w (along x-axis). For each growing rectangle j
	// we would like to determine the growth factor a_j >= 1 such that its new height is a_j*h, its
	// new width is a_j*w and it doesn't overlap with any other rectangle. The goal is to determine 
	// the growth factors a_j that maximize the sum of perimeters of the growing rectangles P=(a_1+...a_n)*(2*(h+w))
	// Let r_i and r_j be 2 different growing rectangles and their centers c_i=(x_i, y_i) and c_j=(x_j, y_j). 
	// Let d_x = |x_i - x_j| - (a_i + a_j)*w/2 and d_y = |y_i - y_j| - (a_i + a_j)*h/2. d_x is the distance separating
	// the 2 rectangles horizontially and d_y is the same vertically.
	// If d_x < 0 we say that they overlap horizontally (along x-axis). Likewise for d_y < 0, they overlap vertically.
	// Overlapping along x-axis only or along y-axis only doesn't mean the rectangles are overlapping, e,g y_i = y_j = 0
	// and x_i = -2, x_j = +2 and h = w = 1, a_i = a_j = 1 (i,e initial state) (clearly in this example they overlap along
	// y-axis but the 2 rectangles are non-overlapping)
	// For the 2 rectangles to overlap, they must overlap along both x and y axis. Now suppose initially d_x < 0 and d_y > 0, 
	// then we keep increasing a_i and a_j until d_y = 0 (i,e rectangles hit along y-axis but do not overlap).
	// Likewise if d_x > 0 and d_y < 0 initially. 
	// The only case left is if d_x > 0 and d_y > 0 initially (i,e initally they do not overlap along x nor y axis). In this case
	// we again keep increasing a_i and a_j until one of the 2 values becomes 0. If the other one is still > 0, say d_y, then they
	// only overlap along x-axis and there is still room for improvement, so we keep increasing till d_y = 0.
	// We have a_i + a_j = (|x_i - x_j| - d_x)*2/w = (|y_i - y_j| - d_y)*2/h. Then the max value for a_i and a_j is reached when
	// (d_x <= 0 and d_y = 0) (I) or (d_x = 0 and d_y <= 0) (II).
	// In case (I), we have a_i + a_j = |y_i - y_j|*2/h = (|x_i - x_j| - d_x)*2/w => "a_i + a_j = |y_i - y_j|*2/h >= |x_i - x_j|*2/w"
	// In case (II) "a_i + a_j = |x_i - x_j|*2/w >= |y_i - y_j|*2/h"
	// The condition for non-overlapping is d_x >= 0 or d_y >= 0 => "a_i + a_j <= |x_i - x_j|*2/w or a_i + a_j <= |y_i - y_j|*2/h"
	// So in case (I): a_i + a_j is upper bounded by |y_i - y_j|*2/h and grows towards it (and isn't upper bounded by |x_i - x_j|*2/w
	// since the best value a_i + a_j >= |x_i - x_j|*2/w)
	// And in case (II): likewise a_i + a_j is upper bounded by |x_i - x_j|*2/w only and grows towards it
	// So the upper bound of a_i + a_j is the max between the value "|y_i - y_j|*2/h and |x_i - x_j|*2/w"
	// Thus our constraint for each pair of growing rectangles r_i and r_j is a_i + a_j <= max(|y_i - y_j|*2/h, |x_i - x_j|*2/w) (C1)
	// Now if we take 1 growing rectangle r_i and 1 fixed rectangle r_j then this is basically the same thing as fixing a_j = 1
	// So we get a constraint a_i <= max(|y_i - y_j|*2/h, |x_i - x_j|*2/w) - 1 for each pair (r_i, r_j) such that r_i is growing and
	// r_j is fixed. We see here that we have m constraint on a_i each one imposing some upper bound u_i,j = max(|y_i - y_j|*2/h, 
	// |x_i - x_j|*2/w) - 1, so we can aggregate these m constraint into 1 which is a_i <= min(u_i,j) for all j (C2)
	// one last constraint is a_i >= 1 (C3).
	// We have "n choose 2" constraints of type (C1) (since we only go over the pair (i, j), j!=i without taking the pairs (j, i)), 
	// n constraints of type (C2) and n constraints of type (C3), so at most 435+30+30 = 495 constraints.
	// We solve this problem using an LP, we want to maximize f(a_1, ..., a_n) = 2*(h+w)*(a_1+...a_n) so we minimize -f()
	
	int n, m, h, w; std::cin >> n >> m >> h >> w; // n=#growing_rectangles, m=#fixed_rectangles, h=height, w=width
	
	std::vector<Center> gr; // growing rectangles centers 
	std::vector<Center> fr; // fixed rectangles centers
	for(int i=0; i<n; i++) { // read positions of the free nails
		int x, y; std::cin >> x >> y;
		gr.push_back(Center(x,y));
	}
	
	for(int i=0; i<m; i++) { // read positions of the occupied nails
		int x, y; std::cin >> x >> y;
		fr.push_back(Center(x,y));
	}
	
	// Ax <= b, x = (a_1, ..., a_n), x >= 1
	Program lp (CGAL::SMALLER, true, 1, false, 0);
	
	int line = 0; // current constraint line number in A
	for(int i=0; i<n; i++) { // add constraints of type (C1)
		for(int j=i+1; j<n; j++) {
			ET dy = std::abs(gr[i].y - gr[j].y); // use type ET for precise division later
			ET dx = std::abs(gr[i].x - gr[j].x); // use type ET for precise division later
			lp.set_a(i, line, 1); lp.set_a(j, line, 1);
			lp.set_b(line, 2*std::max(dy/h, dx/w));
			line++; // next constraint
		}
	}
	
	for(int i=0; i<n; i++) {
		ET upper_bound = DBL_MAX;
		for(int j=0; j<m; j++) {
			ET dy = std::abs(gr[i].y - fr[j].y); // use type ET for precise division later
			ET dx = std::abs(gr[i].x - fr[j].x); // use type ET for precise division later
			upper_bound = std::min(upper_bound, std::max(dy/h, dx/w));
		}
		lp.set_a(i, line, 1);
		lp.set_b(line, 2*upper_bound-1);
		lp.set_c(i, -2*(h+w)); // set the function to be maximized
		line++; // next constraint
	}
	
	Solution s = CGAL::solve_linear_program(lp, ET()); // solve the LP
	std::cout << ceil_to_long(-s.objective_value()) << std::endl; // output max val
  
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
