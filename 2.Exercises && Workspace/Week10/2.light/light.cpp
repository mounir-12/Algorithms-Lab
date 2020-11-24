#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#define FOR(i, n) for(int i=0; i<(n); i++)

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void testcase() {
	// Each lamp i is a position (x_i, y_i, h), and thus lits the disk (on the plane R^2) centered at (x_i, y_i) and 
	// with radius h.
	// The lamps are lit in this order: 0....(n-1). Lamp i lit in round i. Whenever a participant is hit by a lamp, 
	// he is excluded from the stage (so can't be hit by any next lamp to be lit). Participant not hit by a lamp i
	// is a winner of round i and participates at round i+1
	// Participants of a round i must be winners of all previous rounds 0...(i-1)
	// We want to find the set of participants that are excluded last (or not excluded at all, if not hit by any lamp)
	// Let P_i be the set of participants at round i. We want to find a non empty set P_i such that P_(i+1) is empty
	// To do so, we perform a binary search to search for index i with P_i not empty and P(i+1) empty
	 
	int m, n; std::cin >> m >> n;
	 
	std::vector<K::Point_2> player_pos; // participants positions
	std::vector<long> player_r; // and their radiuses
	FOR(i, m) {
		K::Point_2 p; long r; std::cin >> p >> r;
		player_pos.push_back(p);
		player_r.push_back(r);
	}
	
	long h; std::cin >> h;
	
	std::vector<K::Point_2> lamps;
	FOR(i, n) {
		K::Point_2 p; std::cin >> p;
		lamps.push_back(p);
	}
	
	std::vector<int> curr_round_players;
	FOR(i, m) { // curr round = 0 so all players participate
		curr_round_players.push_back(i);
	}
	
	int curr = 0, end = n; // curr_round and end_round indices, we do not run end_round
	while(curr != end) {
		int mid = (curr+end)/2; // we run the game from round "curr" to round "mid" (included)
		// construct triangulation
		Triangulation tr;
		// check only lamps lits in rounds curr....mid (since previous lamps already checked)
		tr.insert(lamps.begin()+curr, lamps.begin()+(mid+1));
		std::vector<int> next_round_players; // players that win all rounds "curr...mid" and get to participate in round "mid+1"
		for(int player : curr_round_players) { // for each player participating in round curr, check if he wins all rounds curr...mid
			K::Point_2 p = player_pos[player];
			long r = player_r[player];
			// find closest lamp to player p amongst lamps curr...mid, if p not hit by the closest one then p not hit by any one
			K::Point_2 nearest_lamp = tr.nearest_vertex(p)->point();
			if(CGAL::squared_distance(p, nearest_lamp) >= (r+h)*(r+h)) { 
				next_round_players.push_back(player);
			}
		}
		if(next_round_players.empty()) { // the stage was emptied at some round i between curr and mid
			// so the round we are searching for is on the left of (i,e before) mid or is mid
			// find the round between curr and mid (excluded)
			end = mid; // round end is not run
			
		} else { // some players won all rounds curr...mid
			curr = mid+1; // continue running the game starting at round mid+1
			curr_round_players = next_round_players; // only those that won get to the next round
		}
	}
	
	for(auto it = curr_round_players.begin(); it != curr_round_players.end(); it++) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
