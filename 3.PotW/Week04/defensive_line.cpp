#include <iostream>
#include <vector>
#include <algorithm>

void testcase() {
	// we have n defense values v[i] and we have an attack value k, we would like to find m intervals [a_j, b_j]
	// 0 <= a_j <= b_j < n such that b_j < a_(j+1) j=0...(m-2) i,e the m intervals are pairwise non intersecting
	// and v[a_j] + ... + v[b_j] = k and the sum of their lengths l_0 + ... l_(m-1) with "l_j = b_j - a_j + 1" 
	// is maximized
	// We first find the set S of intervals [i, j] such that the sum v[i] + ... + v[j] = k. Let S contain the intervals ordered 
	// lexicographically and let L be the size of S and the intervals are indexed 0...(L-1). We must have L >= m, otherwise "fail". 
	// The problem now is to find a subset S_m,L of S such that S_m,L contains m intervals chosen amongst the 1st L intervals
	// such that all intervals in S_m,L are non intersecting and the sum of their lengths, call it Length(S_m,L) is maximized. 
	// We use a table DP where DP[i, j] = the length of S_i,j = length the a subset S_i,j of S such that S_i,j "contains i
	// non intersecting intervals chosen amongst the 1st j intervals" of S and Length(S_i,j) is maximized.
	// The best choice S_i,j is either S_i,(j-1) i,e we choose the best i intervals amongst the 1st j-1 intervals (so we skip
	// interval j) or we pick interval j and and the best (i-1) intervals amongst the 1st j' intervals where j' is 
	// the index of last interval not intersecting with j (i,e all intervals 0...j' do not intersect with interval j but j'+1 does)
	// So DP[i, j] = max(DP[i, j-1] , l_j + DP[i, j'])
	int n, m, k; std::cin >> n >> m >> k;
	
	std::vector<int> defense_values(n);
	for(int i=0; i<n; i++) {
		std::cin >> defense_values[i]; // every defence_value is >= 1
	}
	
	// find all intervals [left, right] such that "the sum: defense_values[left] + ... + defense_values[right] = k"
	int left = 0, right = 0; // perform sliding window, both pointers left and right initially at 0
	int sum = defense_values[0];
	std::vector<int> intervals_lefts; // left end point of intervals
	std::vector<int> intervals_rights; // right end point of intervals
	while(right < n) { 
		if(sum == k) {
			intervals_lefts.push_back(left);
			intervals_rights.push_back(right);
			sum -= defense_values[left];
			left++;
		} else if (sum < k) {
			right++;
			if(right < n) {
				sum += defense_values[right];
			}
		} else {
			sum -= defense_values[left];
			left++;
		}
	}
	int L = intervals_lefts.size(); // L=#intervals_found 
	if(L < m) {
		std::cout << "fail" << std::endl;
		return;
	}
	
	// for each interval j, store the index i < j such that interval i doesn't intersect with interval j but interval (i+1) does
	// i,e we search for the largest index i such that intervals_rights[i] < intervals_lefts[j]
	std::vector<int> last_not_intersect;
	for(int j=0; j<L; j++) {
		int left_j = intervals_lefts[j];
		// find the index i<j such that right_i >= left_j, we know i will be at most j because right_j is the smallest value >= left_j
		int i = std::lower_bound(intervals_rights.begin(), intervals_rights.end(), left_j) - intervals_rights.begin();
		i--; // i-1 would then be the largest index such that right_i < left_j
		last_not_intersect.push_back(i); // if i == -1 then all intervals i < j intersect with interval j
	}
	
	std::vector<std::vector<int>> DP(m+1, std::vector<int>(L, 0)); // DP table
	// base case, we choose 1 interval
	for(int j=0; j<L; j++) { // we only choose 1 interval, choose the largest amongst the intervals 0...j
		int l_j = intervals_rights[j] - intervals_lefts[j] + 1; // length of the interval j
		DP[1][j] = (j == 0 ? l_j: std::max(l_j, DP[1][j-1]));
	}
	for(int i=2; i<=m; i++) { // i=#intervals chosen, start with i=2
		for(int j=0; j<L; j++) { // we choose i intervals amongst the intervals 0...j
			int l_j = intervals_rights[j] - intervals_lefts[j] + 1; // length of the interval j
			if(j>0) { // try to choose i intervals from intervals 0...(j-1)
				DP[i][j] = DP[i][j-1];
			}
			int last_j = last_not_intersect[j]; // index j' where interval j and j' do not intersect but interval j and (j'+1) do
			if(last_j != -1 and DP[i-1][last_j] > 0) { // DP[i-1][j'] == 0 => cannot choose (i-1) intervals using intervals 0...j'
				DP[i][j] = std::max(DP[i][j], l_j + DP[i-1][last_j]);
			}
		}
	}
	if(DP[m][L-1] == 0) { // DP[m][L] == 0 => cannot choose m intervals using intervals 0...(L-1)
		std::cout << "fail" << std::endl;
	} else { // output the best value
		std::cout << DP[m][L-1] << std::endl;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
