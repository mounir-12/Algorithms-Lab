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
	// We have products p_1, ..., p_m with unit costs c = (c_1, ..., c_m)
	// each product p_j provides v_i,j of nutrient i, i = 1...n
	// a diet d is a combination of the m products, i,e 
	// d = x_1 * p_1, ..., x_m * p_m with x = (x_1, ..., x_m) fractional
	// amounts of each product. The diet d provides a total amount of nutrient i 
	// V_i = x_1 * v_i,1 + ... + x_m * v_i,m
	// we want to find the amounts x >= 0 such that for each i we have min_i <= V_i <= max_i 
	// and we minimize the total cost f(x) = x_1 * c_1 + ... + x_m * c_m, f(x) >= 0
	// We have Ax = (V_1 .... V_n -V_1 .... -V_n)^T
	// and b = (max_1 ... max_n -min_1 ... -min_n)
	// with Ax <= b
	
	int n, m; std::cin >> n >> m; // n=#nutrient, m=#products
	if(n == 0 and m == 0) return 0; // no more testcase
	
	Program lp (CGAL::SMALLER, true, 0, false, 0);
	
	for(int i=0; i<n; i++) {
		int min, max; std::cin >> min >> max;
		lp.set_b(i, max); lp.set_b(i+n, -min);
	}
	for(int j=0; j<m; j++) {
		int c; std::cin >> c; // unit cost of product j 
		lp.set_c(j, c);
		for(int i=0; i<n; i++) {
			int v; std::cin >> v; // amount of nutrient i provided by product j
			lp.set_a(j, i, v); lp.set_a(j, i+n, -v);
		}	
	}
	
	// solve the program
	Solution s = CGAL::solve_nonnegative_linear_program(lp, ET()); // cost should by >=0
	
	if(s.status() == CGAL::QP_INFEASIBLE) { // no solution found
		std::cout << "No such diet." << std::endl;
	} else if(s.status() == CGAL::QP_OPTIMAL) { // found optimal solution
		int min_cost = floor_to_int(s.objective_value());
		std::cout << min_cost << std::endl;
	}
	
	return 1; // next testcase
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	while(testcase());
	return 0;
}
