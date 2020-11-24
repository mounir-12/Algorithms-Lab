#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

struct Ball {
	int j; // index of the ball
	int t_j; // time in minutes for the ball to explode
	Ball(int j, int t_j): j(j), t_j(t_j) {}
};

bool operator<(Ball b1, Ball b2) {
	return b1.t_j < b2.t_j;
}

bool deactivate_ball(int i, int& time, std::vector<Ball>& balls, std::vector<bool>& deactivated, std::unordered_map<int, int>& map, int& n) {
	Ball b = balls[i];
	int j = b.j;
	int t_j = b.t_j;
	if(deactivated[j]) return true; // already deactivated ball b, return true
	if(j >= (n-1)/2 and !deactivated[j]) { // ball on the ground and not yet deactivated
		if(time+1 <= t_j) { // if we have time to deactivate it, then do it
			deactivated[j] = true;
			time++; // it take 1 minute to deactivate
			return true;
		} else { // cannot deactivate on time, then can't save the world
			return false;
		}
	} else { // ball is on top on 2 balls (aka its parents)
		int p1 = 2*j+1, p2 = 2*j + 2; // the parents of the ball to be deactivated
		int i1 = map[p1], i2 = map[p2]; // index of p1 and p2 in balls[] vector
		if(balls[i2].t_j < balls[i1].t_j) { // we start by the parent that would explode first
			std::swap(p1, p2);
			std::swap(i1, i2);
		}
		if(!deactivated[p1] and !deactivate_ball(i1, time, balls, deactivated, map, n)) {
			return false; // p1 not deactivated and cannot deactivate it before it explodes, return false
		}
		if(!deactivated[p2] and !deactivate_ball(i2, time, balls, deactivated, map, n)) {
			return false; // p2 not deactivated and cannot deactivate it before it explodes, return false
		}
		// at this point both p1 and p2 are deactivated, so proceed with deactivation ball j
		if(time+1 <= t_j) { // we have time to deactivate it, then do it
			deactivated[j] = true;
			time++; // it take 1 minute to deactivate
			return true;
		} else { // cannot deactivate on time, then can't save the world
			return false;
		}
	}
}

void testcase() {
	// We want to deactivate balls/bombs before anyone explodes, we have n balls b_j where for j = (n-1)/2 ... (n-1)
	// b_j is on the ground and for j = 0 ... ((n-1)/2 - 1) we have that b_j is on top of b_(2*j+1) and b_(2*j+2), aka the parents of b_j
	// Each b_j has time t_j after activation before it explodes and all b_j are activated at time t=0. Let curr_t be current
	// time. Initially curr_t=0. In order to deactivate b_j then we check if it has parents, if so we need to first deactivate them 
	// then proceed with b_j, if not we directly proceed with the deactivation of b_j. Whenever we proceed with the deactivation of 
	// some b_j we check if we have enough time (i,e 1 minute) to do so, so we must have curr_time+1 <= t_j. If we can deactivate it
	// then we do so and increment curr_t by 1 (since 1 minute passed to do the deactivation), if not then we cannot save the world.
	// We proceed greedily by starting with balls that have the smallest t_j. So we deactivate the b_j from the one with smallest t_j
	// to the one with largest t_j (but if needed we first deactivate the parents (also in the same fashion, i,e starting with
	// the one that would explode first))
	
	int n; std::cin >> n; // n=#balls
	
	std::vector<Ball> balls;
	for(int j=0; j<n; j++) {
		int t_j; std::cin >> t_j;
		balls.push_back(Ball(j, t_j));
	}
	
	std::sort(balls.begin(), balls.end()); // sort balls according to the times t_j
	std::unordered_map<int, int> map; // map the index j of ball j to its new index i in the balls[] vector after sorting
	
	for(int i=0; i<n; i++) {
		int j = balls[i].j;
		map[j] = i;
	}
	
	std::vector<bool> deactivated(n, false); // to store at index j if ball j desactivated
	int time = 0; // curr_time
	for(int i=0; i<n; i++) { // proceed with deactivation of all ball
		if(!deactivate_ball(i, time, balls, deactivated, map, n)) { // cannot deactivate ball on time
			std::cout << "no" << std::endl;
			return;
		}
	}
	std::cout << "yes" << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
