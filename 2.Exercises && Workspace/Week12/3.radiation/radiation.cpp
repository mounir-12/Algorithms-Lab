#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int MAX_D = 30; // max target degree of the polynomial

struct Cell {
	ET x, y, z;
	std::vector<std::vector<ET>> pows; // matrix 3 by (MAX_D+1), 1st line is for powers of x, 2nd is for y, 3rd is for z
	Cell(ET x, ET y, ET z): x(x), y(y), z(z) { 
		// precompute the power of x, y and z
		pows = std::vector<std::vector<ET>>(3, std::vector<ET>(MAX_D+1));
		for(int j=0; j<=MAX_D; j++) {
			if(j == 0) {
				pows[0][j] = 1; pows[1][j] = 1; pows[2][j] = 1;
			} else {
				pows[0][j] = pows[0][j-1] * x;
				pows[1][j] = pows[1][j-1] * y;
				pows[2][j] = pows[2][j-1] * z;
			}
		}
	}
};

// returns true if we can find a polynomial of degree d that separates healthy cells from tumor cells
bool found_sol_polynomial(int d, std::vector<Cell> cells, int h, int t) {
	// by default, we have an LP with Ax <= b where x is the vector of coefficients of the polynomial p 
	// of degree d.
	// We want p(cell.x, cell.y, cell.z) > 0 for healthy cells and < 0 for tumor cells, so we would
	// like to find some small value eps > 0 such that p(cell.x, cell.y, cell.z) >= eps for healthy cells
	// and p(cell.x, cell.y, cell.z) <= -eps for tumor cells.
	// So p(cell.x, cell.y, cell.z) - eps >= 0 (I) for healthy cells and p(cell.x, cell.y, cell.z) + eps <= 0 (II)
	// for tumor cells. 
	// We lower bound eps by 0 then we try to maximize it. The goal here is not to maximize
	// eps but to find any eps != 0. Thus the last value in the vector x is eps is also upper bounded to avoid having eps grow unbounded
	// To see why eps could grow unbounded, we show a case where this is possible.
	// Suppose we have 2 cells: a healthy cell (0, 0, 1) and a tumor cell (0, 0, 0) and we want a polynomial of degree 1 that separates
	// them i,e p(x, y, z) = a*x + b*y + c*z + d > 0 for healthy cells and < 0 for tumor cells.
	// One such polynomial would be p(x, y, z) = z - 0.5 which would separate the 2 tumor cells since p(healthy_cell) > 0 and
	// p(tumor_cell) < 0
	// But if we don't upper bound eps and try to maximize it, then conditions (I) and (II) become (I) a*x + b*y + c*z + d - eps >= 0 
	// for healthy cells (II) a*x + b*y + c*z + d + eps <= 0 for tumor cells. Plug in the coordinates of the healthy and tumor cells 
	// to get c + d - eps >= 0 and d + eps <= 0 and maximize eps
	// A solution c and d that would allow to give any arbitrarily large value to eps and still be respect the 2 constraints is
	// a = 0 and b = 0, d = -eps and c = 2*eps for any eps
	// Thus to avoid such a case, we put an upper bound on eps
	
	Program lp (CGAL::SMALLER, false, 0, false, 0);
	
	int col = 0; // index of the current column in matrix A
	for(int c=0; c<(h+t); c++) { // loop over cells, add constraint for each cell
		Cell cell = cells[c];
		col = 0;
		for(int i=0; i<=d; i++) {
			for(int j=0; j<=d; j++) {
				for(int k=0; k<=d; k++) {
					if(i+j+k <= d) { // take only monomials x^i * y^j * z^k of degree <= d
						ET monomial = cell.pows[0][i] * cell.pows[1][j] * cell.pows[2][k];
						lp.set_a(col, c, monomial);
						col++;
					} else break; // move to next j, no need to increase k anymore
				}
			}
		}
		lp.set_a(col, c, (c < h? -1: 1) ); // the value eps we try to maximize
		lp.set_r(c, (c < h ? CGAL::LARGER : CGAL::SMALLER) ); // for healthy cells, we require p(cell.x, cell.x, cell.z) > 0
															// for tumor cells, we require p(cell.x, cell.x, cell.z) < 0
	}
	
	lp.set_c(col, -1); // col has the index of "eps", so we use it to set the vector "c" of the LP at that position to -1 (since we want to maximize)
	lp.set_l(col, true, 0); // we want eps >= 0
	lp.set_u(col, true, 1); // put an upper bound on eps so that it doesn't grow unbounded
	
	CGAL::Quadratic_program_options options;
	options.set_pricing_strategy(CGAL::QP_BLAND);
	Solution s = CGAL::solve_linear_program(lp, ET(), options);
	return (s.status() == CGAL::QP_OPTIMAL and -s.objective_value() > 0); // return true of LP solvable and the solution eps > 0
}

void testcase() {
	// we try to find a polynomial p(x, y, z) of degree d such that we have p(x, y, z) > 0 for healthy cells
	// and p(x, y, z) < 0 for tumor cells. Finding the polynomial is equivalent to finding its coeffcients
	// which are linear in the monomials x^i*y^j*z^k for all 0<= i, j, k such that i+j+k <= d
	// So we have h+t cells, each cell giving us a constraint on the polynomial, so we have at most h+t<=50
	// constraints, thus we can use LP to find the coefficients of the polynomial for some degree d.
	// We also perform a binary search to find the smallest degree d.
	
	int h, t; std::cin >> h >> t; // h=#healthy_cells, t=#turmor_cells
	
	std::vector<Cell> cells; // cells 0...(h-1) are healthy cells, cells h...(h+t-1) are tumor cells
	
	for(int i=0; i<(h+t); i++) { // read healthy and tumor cells positions
		int x, y, z; std::cin >> x >> y >> z;
		cells.push_back(Cell(x, y, z));
	}
	
	if(found_sol_polynomial(0, cells, h, t)) { // first try to find a polynomial of degree 0
		std::cout << "0" << std::endl;
		return;
	}
	
	// degree 0 didn't work, then try higher degrees
	int d = 1; // we try first to quickly find a value d for which a solution polynomial exists (by succesively doubling d)
	while(d <= MAX_D) {
		if(!found_sol_polynomial(d, cells, h, t)) {
			d *= 2; // double the degree
		} else { // found an upper bound on the degree
			break;
		}
	}
	
	int d_min = std::max(d/2, 1); // the last value d' tested which didn't have a solution polynomial of degree d' (or the min value 1)
	int d_max = std::min(d, MAX_D)+1; // if d > MAX_D (i,e d == 32 for MAX_D == 30) then we search for a sol degree d_sol in d/2...MAX_D
	// we could have directly started the binary search with d_min = 1 and d_max = MAX_D+1 (also works on judge with full points)
	// but it is slower since the range of the search is larger.
	while(d_min != d_max) { // binary search in the range [d_min, d_max) d_max excluded
		int d_mid = (d_min+d_max)/2;
		if(found_sol_polynomial(d_mid, cells, h, t)) {
			d_max = d_mid;
		} else {
			d_min = d_mid + 1;
		}
	}
	
	if(d_min > MAX_D) { // didn't find a solution polynomial with degree <= MAX_D
		std::cout << "Impossible!" << std::endl;
	} else {
		std::cout << d_min << std::endl;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
