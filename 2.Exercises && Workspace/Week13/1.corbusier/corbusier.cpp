#include <iostream>
#include <vector>

void testcase() {
	// we have an array H of n heights h_1, ..., h_n. We would like to know if we can get a subset H' of H such that
	// sum(H') = sum of heights in H' = i mod k for given i and k
	// So this means that the remainder of the division of sum(H') by k is i. When is this possible?
	// Let us reformulate the problem as follows: for each possible remainder r (i,e r=0...(k-1)), how many subsets
	// H' of H has a remainder r (H' has remainder r meaning the remainder of division of sum(H') by k is r).
	// We can build P(H) = "Power set of H" = "all possible subsets of H" by starting by an empty set {} that we add to P(H) 
	// then for each h_i "i=1...n", we take every S in the current P(H), create S' = S U {h_i}. Finally we add all created S' to P(H) 
	// and continue likewise for the next h_i
	// Once finished, then for each set H' in P(H), we compute the remainder r of H' and increment a bin_count of r.
	// But we don't need to explicitely build P(H) then compute the remainder for each H' in P(H), we can compute the remainders
	// as we add the h_i because imagine as previously that we are building P(H), we take an S in P(H) that has a remainder r_S
	// then the remainder of "S'=S U {h_i}" is r_S' = (r_S + h_i)%k, then we can increment the bin count of r_S' since we found a new
	// subset with that remainder. Thus we just need to store the found r_S values in a set R (no duplicates) then for each new h_i, 
	// we go through all values r_S in R, compute r_S' = (r_S + h_i)%k, increase the bin count of r_S' by the bin count of
	// r because h_i can be added to each previously found subset S with the same remainder r_S and will give "r_S different
	// subsets S' of same remainder r_S' ". Then we add r_S' to R to signal that this remainder was found (if not already found)
	// For the purposes of this exercise, we do not actually need to find for each possible remainder r the number of subsets with
	// that remainder r (also this number could be very large, imagine the case where i=0 and k=1, then basically any subset of H works
	// thus the number to find would be the size of P(H) which is 2^n and 2^n doesn't fit on a long or int for very large n ~ 10^3
	// even the type double wouldn't work since 2^n doesn't fit in the mantissa so we would need CGAL's exact types).
	// So we could still use bins but instead of increasing the bin count of r_S' by r_S we increase it by 1 (to signal that we found
	// a new set S' with remainder r_S').
	// Initially, R contains the remainder 0 since this is the remainder that we get with the empty set {} thus the bin_count[0]=1
	// Instead of keeping a set R of found remainders, we can directly infer from the bin counts of each remainder if found or not
	// bin_count[r] > 0 => remainder r previously found
	// Alternatively, we can replace bin_count by bin_test where we store for each remainder true or false meaning if we found it
	// or not yet. Then once we find a non-empty subset that has remainder i, we can directly stop the search (=> solution below).
	int n, i, k; std::cin >> n >> i >> k; // n=#heights, i=target_remainder, k=modulo
	
	std::vector<bool> bins(k, 0); // store at position j if we found remainder j
	bins[0] = true; // we count the empty set as having remainder 0
	bool found_non_empty = false; // to signal if we found a non empty subset with remainder "i"
	for(int j=1; j<=n; j++) {
		int h; std::cin >> h; // we read the height h as an int, no need for long
		if(found_non_empty) continue; // if already found, just continue the reads
		std::vector<bool> new_bins = bins; // copy the previously found bins so that we can update them as well as keeping
										  // the original bins to decide which remainders were previously found (i,e found before adding h) 
		for(int r=0; r<k; r++) {
			if(bins[r]) { // test if the remainder r was previously found (test on the previous bins)
				int new_r = (r+h)%k; // compute the new remainder when we add h to a set with remainder r, r+h fits in an int
				new_bins[new_r] = true; // signal that the remainder new_r was found
				if(new_r == i) found_non_empty = true; // found a non-empty subset with remainder i
			}
		}
		bins = new_bins; // replace the old bin with the new ones
	}
	std::cout << (found_non_empty ? "yes" : "no") << std::endl;	
}

int main () {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
