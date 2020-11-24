#include <iostream>
#include <algorithm>
#include <vector>

std::vector<std::vector<int>> DP; // DP[i, j] = the largest amount you are guaranteed to win if we start the game with coins i...j

int optimal_sol(std::vector<int>& v, int i, int j) {
	if(DP[i][j] != -1) return DP[i][j];
	
	if(i == j) { // I only have 1 value to choose
		 DP[i][j] = v[i];
		 return DP[i][j];
	 }
	else if (i+1 == j) { // I only have two values to choose from, choose the max
		DP[i][j] = std::max(v[i], v[j]);
		return DP[i][j];
	}
	
	// if I choose left, then my opponent has only indices [i+1, j] to choose from
	// if my opponent chooses left, then I only have indices [i+2, j] to choose from
	// if my opponent chooses right, then I only have indices [i+1, j-1] to choose from
	// thus I am guaranteed to get the min optimal value that I can get using those 2 windows [i+2, j] and [i+1, j-1]
	int chose_left = v[i] + std::min(optimal_sol(v, i+2, j), optimal_sol(v, i+1, j-1));
	// same reasoning here if I choose right
	int chose_right = v[j] + std::min(optimal_sol(v, i+1, j-1), optimal_sol(v, i, j-2));
	// the largest value I'm guaranteed to get by either choosing left or right
	DP[i][j] = std::max(chose_left, chose_right);
	return DP[i][j];
}

void testcase() {
	int n; std::cin >> n; // n=#coins
	std::vector<int> v(n);
	for(int i=0; i<n; i++) {
		std::cin >> v[i];
	}
	
	DP = std::vector<std::vector<int>>(n, std::vector<int>(n, -1));
	
	int res = optimal_sol(v, 0, v.size()-1); // start the game with coins_i, i=0...(n-1)
	std::cout << res << std::endl;
}


int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
