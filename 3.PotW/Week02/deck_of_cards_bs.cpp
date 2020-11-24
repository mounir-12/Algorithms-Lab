#include <iostream>
#include <vector>

void testcase() {
	// solution using binary search
	int n, k; std::cin >> n >> k; // n=#cards, k=target_value
	
	std::vector<int> card_values(n);
	for(int i=0; i<n; i++) { // read cards values
		std::cin >> card_values[i];
	}
	
	std::vector<int> partial_sums(n); // store at index i the sum: card_values[0] + ... + card_values[i]
	int sum = 0;
	for(int i=0; i<n; i++) {
		sum += card_values[i];
		partial_sums[i] = sum;
	}
	// partial_sums is sorted in increasing order (since each card_values[i] >=0), and we have:
	// sum[i+1,j] = partial_sums[j] - partial_sums[i] = card_values[i+1] + ... card_values[j]
	// for each index j=0...n-1 we would like to find an index 0<=i<j such that value = k-sum[i+1, j] is the smallest
	// this gives us a triple (value, i+1, j). Then amongst all triples found, we take the one that is lexicographically smallest
	// i,e we first compare the values, we keep the one with smallest values, then if multiple triples have the same smallest value, we
	// compare the 1st index and choose the one with smallest 1st index, then if multiple triples have the same smallest value and 
	// the same smallest 1st index, we take the one that has the smallest 2nd index.
	
	std::pair<int, int> indices(0, 0); // the solution pair i<=j, at first i=j=0
	int value = std::abs(k - partial_sums[0]); // the min value amongst the found solution pairs
	std::pair<int, std::pair<int, int>> min_triple(value, indices);
	for(int j=1; j<n; j++) { // start at j=1 since j = 0 done above
		int val = partial_sums[j];
		int searched_val = val - k; // partial_sums[j] - searched_val = k, we try to find searched_value in partial_sums[0...(j-1)]
		// binary search in indices [0, j-1] for the smallest index i such that partial_sums[i] >= searched_val
		auto iter = std::lower_bound(partial_sums.begin(), partial_sums.begin()+j, searched_val);
		int i = iter - partial_sums.begin(); // found index
		int value;
		std::pair<int, int> indices;
		if(i == j) { // all values in partial_sums[0...(j-1)] are less than searched_value, we take the largest one of them
			value = std::abs(k - (partial_sums[j] - partial_sums[i-1])); // the largest is at index i-1 and minimizes this value
			indices = std::make_pair(i, j); // partial_sums[j] - partial_sums[i-1] => indices are i,j
		} else { // found a value greater or equal to searched value, we have 0 <= i < j
			// if we sum card_values from i+1 to j (included) we get a sum1 >= k so k - sum1 >= 0 and value1 = abs(k - sum1)
			// if we sum card_values from i to j (included) we get a sum2 < k so k- sum2 < 0 and value2 = abs(k - sum1)
			// check which one is closest to 0 (i,e which of sum1 and sum2 is closest to k)
			int value1 = std::abs(k - (partial_sums[j] - partial_sums[i]));
			int value2;
			if(i > 0) {
				value2 = std::abs(k - (partial_sums[j] - partial_sums[i-1])); // compute sum of card_values from i to j (included)
			} else { // i == 0
				value2 = std::abs(k - (partial_sums[j])); // compute sum of card_values from i to j (included)
			}
			if(value1 < value2) {
				value = value1;
				indices = std::make_pair(i+1, j); // for sum1 (i,e sum of card_values from i+1 to j (included))
			} else { // value2 < value1 or value2 == value1 (we prefer value2 over value2 because 1st index is smaller)
				value = value2;
				indices = indices = std::make_pair(i, j); // for sum2 (i,e sum of card_values from i to j (included))
			}
		}
		std::pair<int, std::pair<int, int>> triple(value, indices); // build the triple 
		if(triple < min_triple) { // take the smallest triple
			min_triple = triple;
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
