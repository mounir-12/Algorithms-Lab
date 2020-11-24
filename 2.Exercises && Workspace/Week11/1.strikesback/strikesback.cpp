#include <iostream>
#include <vector>
// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#define FOR(i, n) for(int i=0; i<(n); i++) 

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

//LP
typedef K::FT ET;
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
	// we have "a" particles, "s" points shot, and b bounties. We also have "e" energy available
	// Each point_shot_i is the center of the circle of explosion of radius r_i.
	// We can modify the radiuses r_i independently of each other and independently of the energy "e" available
	// but we must ensure that the distance l_i between shot_point i and the closest bounty is such that r_i <= l_i
	// So we can set each r_i as the maximum possible, i,e r_i = l_i and we need to find l_i
	// To do so, we triangulate the bounties with Delaunay and find for each point_shot i the nearest_bounty_i, so
	// l_i = distance(point_shot_i, nearest_bounty_i).
	// We then compute for each particle j the energy E_j it receives, E_j = alpha_1,j * e_1 + ... + alpha_s,j * e_s where
	// e_i is the energy the sent to shot_point_i. To compute the alpha_i,j, we first compute d_i,j = distance(shot_point_i,
	// particle_j) and we set "alpha_i,j = 1.0/max(1.0, d_i,j^2) if d_i,j <= r_i and alpha_i,j = 0 otherwise"
	// We want to know if there is a way to set the e_i such that E_j >= density_j (I) and (e_1 + ... + e_s) <= s
	// This is clearly an LP problem with "a <=30" constraints of type (I) + 1 constraint (e_1 + ... + e_s) <= s
	// and we want to maximize each e_i so we minimize -(e_1 + ... + e_s)
	// A corner case to take into consideration is when b=0 (no bounty) so each r_i is infinite
	
	int a, s, b; std::cin >> a >> s >> b; // a=#particles, s=#points_shot, b=#bounties
	int e; std::cin >> e; // e=energy_available
	
	std::vector<K::Point_2> particles; // particles positions
	std::vector<int> densities; // particles densities
	FOR(i, a) { // read particles positions and densities
		K::Point_2 p; int d; std::cin >> p >> d;
		particles.push_back(p);
		densities.push_back(d);
	}
	
	std::vector<K::Point_2> shot_points; // positions of the shot points
	FOR(i, s) {
		K::Point_2 p; std::cin >> p;
		shot_points.push_back(p);
	}
	
	std::vector<K::Point_2> bounties; // positions of the bounties
	std::vector<K::FT> squared_radiuses; // squared radius of each shot
	if(b > 0) { // only perform these tasks when b > 0
		FOR(i, b) {
			K::Point_2 p; std::cin >> p;
			bounties.push_back(p);
		}
		
		// construct triangulation
		Triangulation tr;
		tr.insert(bounties.begin(), bounties.end());
		
		FOR(i, s) { // find the closest bounty to each shot point
			K::Point_2 nearest_bounty = tr.nearest_vertex(shot_points[i])->point();
			K::FT r = CGAL::squared_distance(shot_points[i], nearest_bounty);
			squared_radiuses.push_back(r);
		}
	}
	
	// LP with Ax >= b, x=(e1, ..., e_s) where e_i is the energy sent to the shot point i
	Program lp (CGAL::LARGER, true, 0, false, 0); 
	int line = 0; // constraint line in matrix A
	FOR(j, a) { // we add 1 constraint per particle
		FOR(i, s) { // set the matrix A
			K::FT dist = CGAL::squared_distance(particles[j], shot_points[i]); // squared distance from particle j to the shot point i
			if(b == 0 || dist <= squared_radiuses[i]) { // particle must be inside the circle of explosion
														// but if there is no bounty, we know radius is infinite
				lp.set_a(i, line, 1.0/CGAL::max(1.0, dist));
			}
		}
		lp.set_b(line, densities[j]);
		line++;
	}
	
	FOR(i, s) { // set the constraint e_1+...+e_s <= e and the mimized function f(x) = -(e_1+...+e_s)
		lp.set_a(i, line, 1);
		lp.set_c(i, -1);
	}
	lp.set_r(line, CGAL::SMALLER); lp.set_b(line, e);
	line++;
	
	// solve the LP
	Solution sol = CGAL::solve_linear_program(lp, ET());
	
	if(sol.status() == CGAL::QP_INFEASIBLE) { // no solution found
		std::cout << "n" << std::endl;
	} else {
		std::cout << "y" << std::endl;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
