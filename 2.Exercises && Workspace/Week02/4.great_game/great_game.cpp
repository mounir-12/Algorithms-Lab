#include <iostream>
#include <vector>
#include <limits>

int min_arrival_steps(std::vector<std::vector<int>>& transitions, int pos);
int max_arrival_steps(std::vector<std::vector<int>>& transitions, int pos);

std::vector<int> min_steps; // at each index i, we have the min steps to go from node i to n
std::vector<int> max_steps; // at each index i, we have the max steps to go from node i to n

int max_arrival_steps(std::vector<std::vector<int>>& transitions, int pos) {
	if(max_steps[pos] == -1) {
			int steps = std::numeric_limits<int>::min();
			for(uint i=0; i<transitions[pos].size(); i++) {
				// perform transition
				// pick the transition that maximizes the arrival steps from the current position
				// we suppose the opponent will try to find the fastest way to reach the target
				// thus to minimize the remaining steps
				// we add +1 for the current transition
				steps = std::max(steps, min_arrival_steps(transitions, transitions[pos][i])+1);
			}
			max_steps[pos] = steps;
		}
		return max_steps[pos];
}

int min_arrival_steps(std::vector<std::vector<int>>& transitions, int pos) {
	if(min_steps[pos] == -1) {
			int steps = std::numeric_limits<int>::max();
			for(uint i=0; i<transitions[pos].size(); i++) {
				// perform transitions
				// pick the transition that minimizes the arrival steps from the current position
				// the next step is done by the opponent so we suppose he will try to take the 
				// transition that maximizes the arrival steps
				// we add +1 for the current transition
				steps = std::min(steps, max_arrival_steps(transitions, transitions[pos][i])+1);
			}
			min_steps[pos] = steps;
		}
		return min_steps[pos];	
}

void testcase() {
	int n,m; std::cin >> n >> m; // n=#nodes, m=#edges
	int r,b; std::cin >> r >> b; // r=starting_pos_of_red, b=starting_pos_of_black
	
	std::vector<std::vector<int>> transitions(n+1); // nodes 0 to n, we ignore node 0, start at node 1
	min_steps = std::vector<int>(n+1, -1); // all min steps unknown
	max_steps = std::vector<int>(n+1, -1); // all max steps unknown
	min_steps[n] = 0; max_steps[n] = 0; // no step needed to go from n to n
	
	for(int i=0; i<m; i++) { // read the transitions
		int u,v; std::cin >> u >> v;
		transitions[u].push_back(v);
	}
	
	int red_min_steps = min_arrival_steps(transitions, r); // find the min steps to reach the target node n from position r
	int black_min_steps = min_arrival_steps(transitions, b); // find the min steps to reach the target node n from position b
	
	// arrival turn corresponding to min_steps for each of red and black
	int red_min_turn = -1;
	int black_min_turn = -1;
	
	// the game goes like this: 1: r_s, 2: b_m, 3: b_s, 4: r_m -> 5: r_s, 6: b_m, 7: b_s, 8: r_m -> ...
	// where r_s = red moved by sherlock, r_m = red moved by moriarty
	// b_s = black moved by sherlock, b_m = black moved by moriarty
	if(red_min_steps % 2 == 0) { // even nb_steps => last step is r_m
		red_min_turn = red_min_steps * 2;
	} else { // odd nb_steps => last step is r_s
		red_min_turn = red_min_steps * 2 - 1;
	}

	if(black_min_steps % 2 == 0) { // even nb_steps => last step is b_s
		black_min_turn = black_min_steps * 2 -1;
	} else { // odd nb_steps => last step is b_m
		black_min_turn = black_min_steps * 2;
	}

	if(red_min_turn < black_min_turn) {
		std::cout << "0" << std::endl;
	} else {
		std::cout << "1" << std::endl;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin>>t;
	while(t--) testcase();
	return 0;
}
