#include <iostream>
#include <vector>
#include <set>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#define FOR(i, n) for(int i=0; i<(n); i++)

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2 Point_2;

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
  return a;
}

void testcase() {
	// We have n warehouses and m stadiums. Let f_i,j >=0 be the amount in liter of beer sent from warehouse w_i to stadum s_j
	// The total amount supplied by w_i to all s_j is: (f_i,1 + f_i,2 + ... + f_i,m) <= supply_i (I)
	// The total amount supplied to s_j by all w_i is: (f_1,j + f_2,j + ... + f_n,j) = demand_j  (II)
	// The total amount of pure alcohol supplied to s_j by all w_i is: (f_1,j * a_1/100 + f_2,j * a_2/100 + ... + f_n,j * a_n/100) <= u_j
	// where a_i is the percentage of pure alcohol in 1 liter of the alcohol made by w_i
	// This can be rewritten as (f_1,j * a_1 + f_2,j * a_2 + ... + f_n,j * a_n) <= 100*u_j (III)
	// These are all the constraints
	// We have n constraint of type (I) one per warehouse, m constraint of type (II) one per stadium and same for constraints of type (III)
	// So the total nb of constraints is M = n+2*m <= 220. The variables are the f_i,j so the number of variables in N= n*m <= 4000
	// So min{M,N} <= 220 so this problem can be efficiently solved using LP
	// Let p_i,j the profit made by sending 1 liter from w_i to s_j, we have p_i,j = r_i,j - t_i,j/100
	// We want to maximize "profit = Σ(Σ f_i,j * p_i,j)" where the outer sum is over i=1...n and the inner sum is over j=1...m
	// So we can equivalently minimize "-100*profit = -Σ(Σ f_i,j * 100*p_i,j)" where 100*p_i,j = 100*r_i,j - t_i,j
	// The relation between the found solution s and the profit is obvious: profit = -s/100
	// To verify if a segment [w_i, s_j] crosses a circle c, we could verify for each triple (w_i, s_j, c) if "w_i is outside the 
	// c and s_j inside" or vice versa. This is too expensive given that the number of circles could go up to 10^6
	// But are all the circles relevant? We see from the problem description that amongst all circles, at most 100 contain a w_i or
	// a s_j. So all the other circles contain no warehouse and no stadium thus they can't be crosses by any pair (w_i, s_j)
	// Thus we just need to find the set C of circles that contain some warehouse or some stadium then we can find take all triples
	// (w_i, s_j, c') where c' in C and find if the segment [w_i, s_j] crosses c'
	// To find if a circle c contains a warehouse or a stadium, we compute the Delaunay Triangulation of the stadiums+warehouses
	// and for each circle c, we find the closest Delaunay Vertex (i,e the closest warehouse or stadium) and check if it is inside c
	// If not inside c then all warehouses and stadiums are outside c
	// Also, no w_i or s_j is located directly on a circle c, so each w_i and s_j must be strictly inside or strictly outside
	
	// Ax <= b, x >= 0, x = (f_1,1; ...; f_1,m; f_2,1 ...; f_2,m; ...; f_n,1; ...; f_n,m)
	Program lp (CGAL::SMALLER, true, 0, false, 0);
	
	int n, m, c; std::cin >> n >> m >> c; // n=#warehouses, m=#stadiums, c=#countour_lines(circles)
	
	std::vector<Point_2> w(n); // warehouse positions
	std::vector<Point_2> s(m); // stadium positions
	std::vector<std::vector<int>> t(n, std::vector<int>(m, 0)); // t_i,j = number of circles crossed by the segment [w_i, s_j]
	int r[n][m]; // r_i,j = revenue for each liter sent from from w_i to s_j
	int a[n]; // a_i = percentage of pure alcohol in each liter of beer made by w_i
	
	int line = 0; // constraint line number
	FOR(i, n) { // loop for warehouses
		std::cin >> w[i]; // read warehouse w_i position
		int supply; std::cin >> supply >> a[i];
		lp.set_b(line, supply); // supply limit of w_i
		FOR(j, m) { // set constraint of type (I)
			int col = i*m+j; // index of f_i,j in x
			lp.set_a(col, line, 1);
		}
		line++; // next constraint
	}
	FOR(j, m) { // loop for stadiums
		std::cin >> s[j]; // read stadium s_j position
		int d, u; std::cin >> d >> u;
		lp.set_b(line, d); lp.set_r(line, CGAL::EQUAL); // fix the demand of s_j
		lp.set_b(line+m, 100*u); // upper limit on amount of pure alcohol supplied to s_j
		FOR(i, n) { // set constraints of type (II) and (III)
			int col = i*m+j; // index of f_i,j in x
			lp.set_a(col, line, 1);
			lp.set_a(col, line+m, a[i]);
		}
		line++; // next constraint
	}
	FOR(i, n) {
		FOR(j, m) {
			std::cin >> r[i][j]; // read r_i,j
		}
	}
	
	Triangulation tr;
	tr.insert(w.begin(), w.end()); // insert all warehouses
	tr.insert(s.begin(), s.end()); // insert all stadiums
	std::vector<Point_2> relevant_circles; // the set of circle that are not empty (i,e contain at least 1 warehouse or stadium)
	std::vector<long> relevant_radiuses_squared; // the squared radiuses of the relevant circles
	FOR(i, c) {
		Point_2 c; long r; std::cin >> c >> r; // read the circle
		Point_2 p = tr.nearest_vertex(c)->point(); // the closest warehouse or stadium
		if(CGAL::squared_distance(p, c) < r*r) { // check if the circle contains the warehouse or stadium found
			relevant_circles.push_back(c);
			relevant_radiuses_squared.push_back(r*r);
		}
	}
	
	int new_c = relevant_circles.size(); // update the number of circles to be considered
	FOR(i, n) {
		FOR(j, m) {
			FOR(k, new_c) {
				K::FT dist_to_w = CGAL::squared_distance(w[i], relevant_circles[k]);
				K::FT dist_to_s = CGAL::squared_distance(s[j], relevant_circles[k]);
				long r_squared = relevant_radiuses_squared[k];
				if((dist_to_w < r_squared and dist_to_s > r_squared) or (dist_to_w > r_squared and dist_to_s < r_squared)) {
					// the segment [w_i, s_j] crosses the relevant_circle[k]
					t[i][j]++; // increment the number of circles crossed by the segment [w_i, s_j]
				}
			}
		}
	}
	FOR(i, n) {
		FOR(j, m) {
			int l = i*m+j; // index of f_i,j in x
			lp.set_c(l, -(100*r[i][j]-t[i][j]));
		}
	}
	
	Solution sol = CGAL::solve_linear_program(lp, ET());
	if(sol.status() == CGAL::QP_INFEASIBLE) { // no solution possible
		std::cout << "RIOT!" << std::endl;
	} else {
		std::cout << floor_to_int(-sol.objective_value()/100.0) << std::endl;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
