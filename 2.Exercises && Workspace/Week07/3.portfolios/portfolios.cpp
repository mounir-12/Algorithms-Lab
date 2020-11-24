#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int testcase() {
	// a portfolio p consists of buying amounts x = (x_1, ..., x_n) of assets 1 ... n
	// each asset i has a cost c_i and an expected revenue r_i 
	// so the portfolio's total cost C_p = x_1 * c_1 + ... + x_n * c_n
	// the portfolio's total expected revenue R_p = x_1 * r_1 + ... + x_n * r_n
	// the portfolio's risk V_p = sum_i,j(x_i * x_j * v_i,j) for all pairs (i,j) i,j = 1...n
	// each investor has a requirement in terms of min revenue R, max cost C and max risk V
	// for each investor, we want to find if there is a portfolio p with x >= 0 such that: 
	// 0- C_p <= C
	// 1- R_p >= R
	// 2- V_p <= V
	// we solve this by finding the min_risk V_p_min subject to constraints 0 and 1
	// and if V_p_min <= V then there exits x that satisfies constraints 0, 1 and 2
	// otherwise if V_p_min > V then any other value of V_p satisfying constraints 
	// 0 and 1 would be higher than V_p_min i,e higher than V, so no solution x would
	// satidfy constraints 0, 1 and 2
	int n, m; std::cin >> n >> m; // n=#assets, m=#investors
	if(n == 0 and m == 0) return 0;
	
	Program qp (CGAL::SMALLER, true, 0, false, 0); // CGAL::SMALLER for constraint 0
	qp.set_r(1, CGAL::LARGER); // CGAL::LARGER for constraint 1
	
	for(int i=0; i<n; i++) {
		int c, r; std::cin >> c >> r;
		qp.set_a(i, 0, c); qp.set_a(i, 1, r);
	}
	for(int i=0; i<n; i++) {
		for(int j=0; j<n; j++) {
			int v; std::cin >> v;
			if(j <= i) {
				qp.set_d(i, j, 2*v);
			}
		}
	}
	for(int i=0; i<m; i++) {
		int C, R, V; std::cin >> C >> R >> V;
		qp.set_b(0, C); qp.set_b(1, R);
		// our objective function is x^T * D * x which is >=0 by default since D psd
		// so use solve_nonnegative_quadratic_program or solve_quadratic_program is the same here
		Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
		if(s.status() == CGAL::QP_OPTIMAL and s.objective_value() <= V) {
			std::cout << "Yes." << std::endl;
		} else {
			std::cout << "No." << std::endl;
		}
	}
	return 1;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	while(testcase());
	return 0;
}
