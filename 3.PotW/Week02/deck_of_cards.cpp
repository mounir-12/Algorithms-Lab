#include <iostream>
#include <vector>

void testcase() {
	// solution using sliding window
	int n, k; std::cin >> n >> k; // n=#cards, k=target_value
	
	std::vector<int> card_values(n);
	for(int i=0; i<n; i++) { // read cards values
		std::cin >> card_values[i];
	}
	
	int left = 0, right = 0;
	int sum = card_values[0]; // the sum: card_values[left] + ... + card_values[right]
	int value = std::abs(k - sum);
	std::pair<int, std::pair<int, int>> min_triple(value, std::make_pair(left, right));
	while(right < n) {
		value = std::abs(k - sum);
		std::pair<int, std::pair<int, int>> triple(value, std::make_pair(left, right));
		if(triple < min_triple) { // take the smallest triple, i,e 1st compare the values then the 1st index then the 2nd index
			min_triple = triple;
		}
		if(value == 0) break; // i,e (sum == k) => found smallest possible value and lexicographically smallest pair of indices
		if (sum < k) {
			right++;
			if(right < n) {
				sum += card_values[right];
			}
		} else { // sum > k
			sum -= card_values[left];
			left++;
		}
	}
	std::pair<int, int> final_indices = min_triple.second;
	std::cout << final_indices.first << " " << final_indices.second << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}

