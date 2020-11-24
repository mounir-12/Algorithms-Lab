#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose exact integral type
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> SolT;

// round up to next integer double
int ceil_to_int(const SolT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return (int)a;
}

int floor_to_int(const SolT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return (int)a;
}

int testcase() {
	int p; std::cin >> p;
	if(p == 0) return 0;
	
	int a, b; std::cin >> a >> b;

	Program qp (CGAL::SMALLER, true, 0, false, 0);
	
	int x = 0, y = 1, zz = 2;
	// setting up A and b
	qp.set_a(x, 0, 1); 		qp.set_a(y, 0, 1); 		qp.set_b(0, 4);
	qp.set_a(x, 1, 4); 		qp.set_a(y, 1, 2); 		qp.set_b(1, a*b);
	qp.set_a(x, 2, -1); 	qp.set_a(y, 2, 1); 		qp.set_b(2, 1);
	// setting up 2*D
	qp.set_d(0, 0, 2*a);
	qp.set_d(1, 0, 2*0); 	qp.set_d(1, 1, 2*0);
	// setting up c and c0
	qp.set_c(x, 0);
	qp.set_c(y, -b);
	qp.set_c0(0);
	if(p == 2) { // minimization problem
		// add column in A for z^2 and set b and comparison sign A*x >= b
		qp.set_a(zz, 0, 0); 	qp.set_b(0, -4);	 qp.set_r(0, CGAL::LARGER);
		qp.set_a(zz, 1, 1); 	qp.set_b(1, -(a*b));	 qp.set_r(1, CGAL::LARGER);
		qp.set_a(zz, 2, 0); 	qp.set_b(2, -1); 	 qp.set_r(2, CGAL::LARGER);
		// add 3rd line in 2*D
		qp.set_d(2, 0, 2*0); 	qp.set_d(2, 1, 2*0); qp.set_d(2, 2, 2*1);
		// set c
		qp.set_c(y, b);
		qp.set_c(zz, 0);
		// set lower and upper bounds
		qp.set_l(x, false, 0);	qp.set_u(x, true, 0);	// -inf <= x <= 0
		qp.set_l(y, false, 0);	qp.set_u(y, true, 0);	// -inf <= y <= 0
		qp.set_l(zz, true, 0);	qp.set_u(zz, false, 0);	// 0 <= z^2 <= inf
	}
	
	// we do not use here CGAL::solve_nonnegative_quadratic_program
	// since we allow negative any value for our objective function
	Solution s = CGAL::solve_quadratic_program(qp, ET());
	
	if (s.status() == CGAL::QP_INFEASIBLE) { // QP unfeasible
		std::cout << "no" << std::endl;
	} else if(s.status() == CGAL::QP_UNBOUNDED) { // unbounded solution
		std::cout << "unbounded" << std::endl;
	} else { // QP feasible and optimal
		if(p==1) { // maximization problem
			std::cout << floor_to_int(-s.objective_value()) << std::endl;
		} else { // minimization problem
			std::cout << ceil_to_int(s.objective_value()) << std::endl;
		}
	}
	return 1;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	while(testcase());
	return 0;
}
