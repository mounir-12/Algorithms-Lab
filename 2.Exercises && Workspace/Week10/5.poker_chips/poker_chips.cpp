#include <iostream>
#include <vector>
#define FOR(i, n) for(int i=0; i<(n); i++)

// we have n pointers on the n stacks, each one pointing at the top element. So for each pointer i
// the top element of stack i is stacks[pointers[i]].
// When pointers[i] == 0 then only the bottom element is left in stack i
// When pointers[i] == -1 then stack i is empty
std::vector<int> DP;
// DP is a flattened n-dimentional matrix where n = nb_stacks
// case n=5: DP before flattening is 5-dimentional array DP[][][][][]
// such that given pointers p1, p2, p3, p4, p5 of stacks 1, 2, 3, 4, 5
// then DP[p1+1][p2+1][p3+1][p4+1][p5+1] stores the max nb of points
// we can get when the top element of stack i is p_i
// we add 1 to each pointer p_i since p_i = -1 => stack i empty
// so p_i+1 = 0 => stack i empty
// So given the stack sizes m1, m2, m3, m4 and m5, DP size is:
// (m1+1)(m2+1)(m3+1)(m4+1)(m5+1) <= 2^16 = 65536 (again we add 1 to each m_i
// for the case of having an empty stack)

int flatten_pointers( std::vector<int>& pointers, std::vector<int>& stack_sizes) {
	// Given an n-dimentional matrix, an element e is indexed using (i_1, ..., i_n)
	// where i_j is along the jth dimension, each dimension j having size s_j.
	// The flattened index would be: i_n + s_n * (i_(n-1) + s_(n-1) * (... + s_2 * (i_1))...)
	// Recursively defined: let index(n) = i_n + s_n * index(n-1) and index(1) = i_1
	// then flat_index = index(n)
	// E,g with 2 dimensions: (i_1, i_2) i_1 is the row, i_2 is the column, flat_index = index(2) = i_2 + s_2 * i_1  
	// here: stack 1 is the 1st dimension, stack 2 is the 2nd dimension, ...
	int n = stack_sizes.size();
	int flat_index = 0;
	FOR(i, n) {
		if(i>0) {
			flat_index *= (stack_sizes[i]+1); // add 1 since (pointers[i]+1) goes from 0 to stack_sizes[i]
		}
		flat_index += (pointers[i]+1); // add 1 to account for empty stack
	}
	return flat_index;
}

int optimal_solution(std::vector<std::vector<int>>& stacks, std::vector<int>& pointers, std::vector<int>& stack_sizes) {
	int n = stacks.size();
	int index = flatten_pointers(pointers, stack_sizes);
	if(DP[index] != -1) return DP[index]; // value already computed, return it
	// otherwise, compute value
	int max_points = 0; // max nb of points we can get given the current stacks
	for(int s=0; s < (1<<n); s++) { // try all 2^n <= 2^5 = 32 combinations of subsets
		bool all_matching = true; // when true, then the colors of the top chips of the chosen stacks all match
		int color = -1; // chips color
		std::vector<int> chosen_non_empty; // indices of chosen non-empty stacks
		FOR(i, n) { // loop over chosen stacks
			// check ith stack is chosen and is non-empty. bit i of s is 1 => stack i chosen, else not chosen
			if((s & (1<<i)) and pointers[i] != -1) {
				chosen_non_empty.push_back(i);
				if(color == -1) { // chip color of the first non-empty stack found
					color = stacks[i][pointers[i]]; // take the top element color as initial color
				} else if(color != stacks[i][pointers[i]]) { // found stack with different top chip color
					all_matching = false;
				}
				pointers[i]--; // pop chip from stack
			}
		}
		int k = chosen_non_empty.size(); // number of chosen non-empty stacks
		if(k == 0) {
			continue; // must choose at least 1 non-empty stack, continue
		}
		
		int points = 0; // max number of points one can get by the current choice of stacks "s"
		if(all_matching and k >= 2) { // all_matching, so we win 2^(k-2) points, if we chosen k>=2 stacks
			points += (1<< (k-2)); // add 2^(k-2) points
		}
		points += optimal_solution(stacks, pointers, stack_sizes); // add the max number of points we can get using the current stacks
		max_points = std::max(max_points, points); // max nb of points
		for(int i: chosen_non_empty) { // push the poped chips
			pointers[i]++;
		}
	}
	
	DP[index] = max_points;
	return DP[index];
}

void testcase() {
	
	int n; std::cin >> n;
	std::vector<std::vector<int>> stacks(n);
	std::vector<int> stack_sizes(n); // store at index i the size of stack i
	int DP_size = 1;
	FOR(i, n) { // read the length of each stack
		int m; std::cin >> m;
		stack_sizes[i] = m;
		DP_size *= (m+1);
	}
	
	DP = std::vector<int>(DP_size, -1); // init DP with all -1
	DP[0] = 0; // base case, all stacks empty
	
	FOR(i, n) { // read the stack, stack[i] = stack_bottom, ..., stack_top
		FOR(j, stack_sizes[i]) {
			int color; std::cin >> color; // read the color of the j'th chip of stack i
			stacks[i].push_back(color);
		}
	}
	
	std::vector<int> pointers(n); // store at index i the index of the current top element of the stack
	FOR(i, n) { // init pointers
		pointers[i] = stack_sizes[i]-1;
	}
	int max_points = optimal_solution(stacks, pointers, stack_sizes);

	std::cout << max_points << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
