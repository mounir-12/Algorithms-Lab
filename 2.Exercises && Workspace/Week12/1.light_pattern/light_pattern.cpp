#include <iostream>
#include <vector>

// We have n bulbs labeled 0...(n-1). We take each consecutive k bulbs (starting from the left) and
// group them as a block, that way we have b=n/k blocks of size k
// Each bulb is on or off (1 or 0) and we want to make each block match a certain pattern
// (i,e a certain sequence of zeros and ones).
// Let the blocks be labeled 1...b. We are at block b, we have 2 cases: 
// 1- 	flip all blocks 1...b then make block b match the pattern (by individually flipping bulbs of block b),
//		the first action costs 1 and the 2nd action's cost is c_b' (index b here denoting we are on block b 
// 		and the ' means the block is flipped). We should add to those 2 costs the min cost of making the flipped
//		blocks 1...(b-1) match the pattern, call this cost C_(b-1)'
// 2- 	we do not flip the blocks 1...b and we directly flip the individual bulbs of block b to make it match the pattern.
// 		This costs c_b. We add to this cost the min cost to make the (non-flipped) blocks 1...(b-1) match 
// 		the pattern, call this cost C_(b-1)
// We take the cheapest of the 2 cases.
// For the case 1, to find C_(b-1)', we solve the same problem but as if the initial blocks given are the flipped 
// blocks 1...(b-1) (i,e we try to make those blocks match the pattern as cheaply as possible)
// Likewise for case 2, to find C_(b-1) we solve the same problem but the initial blocks given are blocks 1...(b-1)
// (i,e we try to make them match the pattern as cheaply as possible)
// So given our initial b blocks, we see that we will need to find the min cost to make blocks 1...i
// match the pattern for the 2 cases where these block are flipped or not flipped.
// Now relabeling the blocks to be 0-based index, we create 2 arrays min_cost and min_cost_flipped
// where min_cost[i] = "min cost to make the initial blocks match the pattern when the intial blocks are blocks 0...i" 
// and min_cost_flipped[i] = "min cost to make the initial blocks match the pattern when the intial blocks are the flipped blocks 0...i" 
// Then the desired solution is min(min_cost[b-1], min_cost_flipped[b-1] +1 ). +1 here due to flipping  blocks 0...(b-1). 
// So min_cost[i] = C_i and min_cost_flipped[i] = C_i' (upper case C, as defined above but we 0-based index)
// And we have min_cost[i] = min(c_i' + 1 + min_cost_flipped[i-1], c_i + min_cost[i-1]) note: lower case c_i and c_i' defined below
// And min_cost_flipped[i] = min(c_i + 1 + min_cost[i-1], c_i' + min_cost_flipped[i-1])
// where c_i =  nb of mismatches between block i and the pattern (i,e cost of making block i match pattern)
// and c_i' = nb of mismatches between flipped block i and the pattern (i,e cost of making flipped block i match pattern) => c_i' = k - c_i
// base case, we only have block 0: min_cost[0] = min(c_0' + 1, c_0) and min_cost_flipped[0] = min(c_0 + 1, c_0')

void testcase() {
	int n, k, x; std::cin >> n >> k >> x; // n=#bulbs, k=#bulbs_per_block, x=#pattern
	
	int b = n/k; // number of blocks
	std::vector<std::vector<bool>> blocks(b, std::vector<bool>(k)); // vector of b blocks, each block having k bool values for the k bits
	for(int i=0; i<n; i++) {
		int bit; std::cin >> bit;
		blocks[i/k][i%k] = (bit == 1);
	}
	
	std::vector<bool> pattern(k);
	for(int i=0; i<k; i++) { // convert x to vector of bool as for the blocks
		pattern[k-i-1] = (x & (1 << i)); // [(x & (1 << i)) == 0] => false, else true
	}
	
	std::vector<int> min_cost(b);
	std::vector<int> min_cost_flipped(b);
	for(int i=0; i<b; i++) { // bottom-up computation
		std::vector<bool>& block(blocks[i]); // ith block
		int c_i = 0; // number of mismatches of block i with the pattern
		for(int j=0; j<k; j++) {
			if(block[j] != pattern[j]) c_i++;
		}
		
		int c_i_prime = k - c_i; // number of mismatches of flipped block i with the pattern
		if(i == 0) { // base case
			min_cost[i] = std::min(c_i_prime + 1, c_i);
			min_cost_flipped[i] = std::min(c_i + 1, c_i_prime);
		} else {
			min_cost[i] = std::min(c_i_prime + 1 + min_cost_flipped[i-1], c_i + min_cost[i-1]);
			min_cost_flipped[i] = std::min(c_i + 1 + min_cost[i-1], c_i_prime + min_cost_flipped[i-1]);
		}
	}
	
	std::cout << std::min(min_cost[b-1], min_cost_flipped[b-1] + 1) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
