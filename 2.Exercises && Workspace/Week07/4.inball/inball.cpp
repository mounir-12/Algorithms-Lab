#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> SolT;

int floor_to_int(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return (int)a;
}

int testcase() {
	// we have a polyhedra C that is represented by n constraints
	// each constraint i is of the form a_i^T * x - b_i <= 0
	// so each constraint defines a hyperplane p_i of the polyhedra with equation
	// (p_i): a_i^T * x - b_i = 0 where a_i is the normal vector
	// and the points x that satisfy constraint i are the points that are to the left of the plane
	// i,e points in the direction of -a_i
	// we would like to find the center x_c of the hypersphere S inside C with maximum radius r
	// S inside C => the radius r <= |d_i| i = 0...(n-1) where d_i = (a_i^T * x_c - b_i) / ||a_i|| 
	// is the signed the distance from x_c to plane p_i
	// we also have that a_i^T * x_c - b_i <= 0  (due to ith constraint of C) then divide both sides of
	// this inequality by ||a_i|| which gives d_i on the left hand side
	// so d_i <= 0 i,e |d_i| = -d_i so r <= -d_i
	// So we have 2n constraints which are:
	//	-constraints 0...(n-1): r <= -d_i for all i = 0...(n-1)
	//	-constraints n...(2n-1): a_i^T * x_c - b_i <= 0 for all i = 0...(n-1)
	// but since we only want r >= 0 then adding the constraint r >=0 makes constraints 0...(n-1) imply
	// constraints n...(2n-1) since (r <= -d_i) implies (d_i <= -r <= 0) implies (a_i^T * x_c - b_i <= 0)
	// so we only have n+1 constraints:
	// 	0...(n-1): 	(r <= -d_i) => (d_i + r <= 0) => (a_i^T * x_c + ||a_i|| * r <= b_i) for all i = 0...(n-1)
	// 	n:			r >= 0
	// so we have d+1 variables: the d coordinates of x_c and the radius r
	int n; std::cin >> n; // n=#hyperplanes
	if(n == 0) return 0;
	
	int d; std::cin >> d;
	// the constraints 0...(n-1) have a <= sign
	Program lp (CGAL::SMALLER, false, 0, false, 0); // the coordinates of the center x_c can take any value from -inf to inf
	
	// we want to maximize r so we minimize -r
	lp.set_c(d, -1);
	
	// the nth constraint (the last one) is r >= 0
	lp.set_r(n, CGAL::LARGER);
	lp.set_a(d, n, 1);
	
	for(int i=0; i<n; i++) { // set up the constraints 0...(n-1)
		long a_i_norm = 0;
		for(int j=0; j<d; j++) {
			int a_ij; std::cin >> a_ij;
			lp.set_a(j, i, a_ij);
			a_i_norm += (a_ij * a_ij);
		}
		a_i_norm = sqrt(a_i_norm);
		
		lp.set_a(d, i, a_i_norm);
		
		int b_i; std::cin >> b_i;
		lp.set_b(i, b_i);
	}
	
	Solution s = CGAL::solve_linear_program(lp, ET());
	
	if(s.status() == CGAL::QP_INFEASIBLE) { // no solution found
		std::cout << "none" << std::endl;
	} else if(s.status() == CGAL::QP_OPTIMAL) { // solution found
		int r = floor_to_int(-s.objective_value()); // s.objective_value() = -r
		std::cout << r << std::endl;
	} else { // unbounded solution
		std::cout << "inf" << std::endl;
	}
	return 1;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	while(testcase());
	return 0;	
}
