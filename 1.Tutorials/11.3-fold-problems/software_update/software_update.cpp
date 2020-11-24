#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>

// kernel
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
// exact number type
typedef K::FT ET;

// program and solution types
typedef CGAL::Quadratic_program<K::FT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> SolT;

long floor_to_long(const K::FT& x)
{
	double a = std::floor(CGAL::to_double(x));
	while (a > x) a -= 1;
	while (a+1 <= x) a += 1;
	return a;
}

long ceil_to_long_sol(const SolT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase() {
	int n; std::cin >> n;
	std::vector<K::Point_2> cannons;
	for(int i=0; i<n; i++) {
		K::Point_2 p; std::cin >> p;
		cannons.push_back(p);
	}
	
	K::FT d_min; /* smallest pair distance */ bool first = true;
	// compute distance between every pair (cannon_i, cannon_j)
	std::vector<std::vector<K::FT>> distances(n, std::vector<K::FT>(n, 0.0));
	for(int i=0; i<n; i++) {
		for(int j=i+1; j<n; j++) {
			K::FT dist = CGAL::squared_distance(cannons[i], cannons[j]);
			distances[i][j] = dist;
			distances[j][i] = dist;
			if(first) {
				d_min = dist;
				first = false;
			} else {
				d_min = CGAL::min(d_min, dist);
			}
		}
	}
	
	long r = floor_to_long(CGAL::sqrt(d_min/4.0));
	
	Program lp (CGAL::SMALLER, true, 0, false, 0);
	
	// the variable x=(r1, r2, ..., r_n) where r_i is the radius of the ith cannon
	int line = 0; // constraint line in the matrix A
	for(int i=0; i<n; i++) {
		for(int j=i+1; j<n; j++) {
			// add the constraint r_i + r_j <= dist(i, j)
			lp.set_a(i, line, 1); lp.set_a(j, line, 1);
			lp.set_b(line, CGAL::sqrt(distances[i][j]));
			line++;
		}
		// we want to maximize ((r1-r)+(r2-r)+...+(r_n-r)) = (r1+r2+...+r_n) - n*r  so we minimize -(r1+r2+...+r_n) + n*r
		lp.set_c(i, -1);
	}
	lp.set_c0(n*r);
	
	// each radius r_i must be at least r
	for(int i=0; i<n; i++) {
		lp.set_a(line, i, 1); lp.set_b(line, r); lp.set_r(line, CGAL::LARGER);
		line++;
	}
	
	Solution s = CGAL::solve_linear_program(lp, ET());
	
	std::cout << ceil_to_long_sol(-s.objective_value()) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
