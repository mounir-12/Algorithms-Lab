#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

void testcase() {
	// We have friends with different strengths and boxes with different weights. Each friend can only carry down boxes with
	// weight less than or equal to its strength. All friends are up stairs at first. We use a greedy strategy where we iterate
	// over the friends from the strongest to the weakest, and for each one of them, we assign the largest not yet chosen box
	// that he/she can carry. Once we have a friend that is too weak to carry any of the remaining boxes (or no more boxes to carry), 
	// then we send all chosen friends downstairs. If any box is still to be brought down, we bring all friends upstairs and continue
	int n, m; std::cin >> n >> m;
	
	std::vector<int> strenghts;
	for(int i=0; i<n; i++) {
		int s; std::cin >> s;
		strenghts.push_back(s);
	}
	std::sort(strenghts.begin(), strenghts.end(), [](int a, int b) {return a > b;}); // sort decreasing order
	
	std::multiset<int, std::greater<int>> weights;
	for(int i=0; i<m; i++) {
		int w; std::cin >> w;
		weights.insert(w);
	}
	
	if(*weights.begin() > *strenghts.begin()) { // compare max_strength with max_weight
		std::cout << "impossible" << std::endl;
		return;
	}
	
	int time = -1; // we suppose initially all friends are downstairs at time -1, and all come up at time 0
	while(!weights.empty()) { // there are still boxes to bring down
		time++; // make all friends that are downstairs come up in 1 min
		for(int i=0; i<n; i++) { // go through all friends from strongest to weakest, assign to each one the largest box he/she can carry
			auto w_it = weights.lower_bound(strenghts[i]); // find the largest box with weight <= strenght[i] of friend i
			if(w_it != weights.end()) { // found a box, assign it to current friend i
				weights.erase(w_it); // the box was assigned so remove it (no need to reassign it)
				if(i+1 == n or weights.empty()) { // this friend is the last/weakest friend or no more boxes to bring down so the next 
												  // step is to send all chosen friends 0...i downstairs with their boxes
					time += 2; // time to send them all downstairs with their boxes
					break;
				}
			} else { // no box is light enough for friend i, then no box is light enough for next/weaker friends, so send chosen friends
				time += 2; // time to send friends 0...(i-1) downstairs
				break;
			}
		}
	}
	std::cout << time << std::endl;
	
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
