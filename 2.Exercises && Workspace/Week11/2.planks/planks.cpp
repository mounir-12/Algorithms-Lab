#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

typedef std::vector<long> VL;
typedef std::vector<VL> VVL;
typedef std::stack<int> SI;

void quadruples_lengths(VL& sets_lengths, SI& planks, VVL& Q, VL& lengths) {
	if(planks.empty()) { // all planks assigned, add the set lengths found to Q (Q is set of quadruples lengths)
		Q.push_back(sets_lengths);
		return;
	}
	int plank = planks.top(); // plank to assign, we try assigning it to each of the sets 
	planks.pop();
	for(int i=0; i<sets_lengths.size(); i++) { // we assign the plank to each set (we add it's length to the set's length)
		sets_lengths[i] += lengths[plank]; // assign plank to ith set (add the plank length to the ith set's length)
		quadruples_lengths(sets_lengths, planks, Q, lengths); // assign the next planks to the some set
		sets_lengths[i] -= lengths[plank]; // remove plank from ith set
	}
	planks.push(plank); // push back the plank
}

void quadruples_lengths_init(SI& planks, VVL& Q, VL& lengths) {
	VL sets_lengths(4, 0);
	quadruples_lengths(sets_lengths, planks, Q, lengths);
}

void testcase() {
	// we have n planks each plank p_i having length l_i. We need to use all planks to build a square
	// the square side must have length L = (l_1 + ... + l_n) / 4.
	// Let S1, S2, S3, S4 be the set of planks used in side 1, 2, 3 and 4 respectively of the square.
	// So the square can be represented as a quadruple (S1, S2, S3, S4) such that S_i ∩ S_j = {} (i!=j) (I)
	// and S1 ∪ S2 ∪ S3 ∪ S4 = P = {p_1, ..., p_n} (II) and Length(S_i) = sum of length of planks in S_i = L (III)
	// Permutating planks within a same S_i or permutating the sets S_i in the quadruple (S1, S2, S3, S4)
	// gives us back a square that not "really different" (as defined in the exercice). So the order of 
	// the planks whithin the same S_i doesn't matter, neither does the order of the S_i in the quadruple
	// So we only need to find the number of ways to create a quadruple (S1, S2, S3, S4) such that conditions
	// (I), (II) and (III) are met. For each plank p_i, we have 4 choices (either put it in S1 or S2 or S3 or S4)
	// so there are 4^n ways to create a quadruple (S1, S2, S3, S4). But since amongst those 4^n possibilities
	// there are quadruples (S1, S2, S3, S4) and (S1', S2', S3', S4') the latter is just a permuattion of the former
	// We know that for each quadruple there are 4! = 24 ways to permute it, so each 24 quadruples can be reduce to 1
	// quadruple. So if the number of quadruples that respect conditions (I), (II) and (III) is N, then the solution
	// would be N/24. This solution would be O(n*4^n) since we need to list all 4^n quadruples of disjoint sets S_i
	// and for each one check if it respects condition (III)
	// Now if we split the set P in half (approximately), we get 2 disjoint sets P1 and P2, so each one has size around n/2.
	// Now, instead of listing the 4^n ways to split P into disjoint sets (S1, S2, S3, S4), we do this for P1 and P2 i,e
	// we find Q1 = "the set of quadruples (S1, S2, S3, S4) such that S_i are disjoint and ∪(S_i) = P1". Likewise for Q2.
	// Then for each quadruple (S1, S2, S3, S4) in Q1, we compute (L1, L2, L3, L4) such that L_i = Length(S_i)
	// We do the same for Q2, i,e for each quadruple (S1', S2', S3', S4') in Q2, compute (L1', L2', L3', L4') such that 
	// L_i' = Length(S_i')
	// Now we only need to find for each (S1, S2, S3, S4) in Q1 the number of quadruples (S1', S2', S3', S4') in Q2
	// such that (L1, L2, L3, L4) + (L1', L2', L3', L4') = (L, L, L, L) because the quadruples (S1 U S1', S2 U S2', S3 U S3', S4 U S4')
	// for each of the found (S1', S2', S3', S4') in Q2 respects the conditions (I), (II) and (III) for the set P = P1 U P2
	// So summing those numbers found for each quadruple in Q1 gives back N (of set P), thus the result is again N/24
	// This split and list method (split P = P1 U P2 then List quadruples of P1 and P2) runs in time (n*4^(n/2)) = O(n*2^n)
	// which is acceptable
	 
	int n; std::cin >> n; // n=#planks
	
	std::vector<long> lengths;
	long p = 0; // square perimeter
	for(int i=0; i<n; i++) { // read planks lengths
		long l; std::cin >> l;
		lengths.push_back(l);
		p += l;
	}
	if(n < 4 || p%4 != 0) { // must have at least 4 planks (1 per square side) and p = 4*L, so p must be divisible by 4 
		std::cout << "0" << std::endl; // no square can be built
		return;
	}
	long L = p/4;
	SI P1; SI P2;
	int mid = n/2;
	for(int i=0; i<n; i++) { // put each plank numbers 0...((n/2)-1) in P1 and (n/2)...(n-1) in P2
		if(i < mid) {
			P1.push(i);
		} else {
			P2.push(i);
		}
	}
	
	VVL Q1; // the set of quadruples of P1
	quadruples_lengths_init(P1, Q1, lengths);
	VVL Q2; // the set of quadruples of P2
	quadruples_lengths_init(P2, Q2, lengths);
	
	std::sort(Q2.begin(), Q2.end()); // sort the quadruple lengths of Q2
	long N = 0;
	for(VL quadruple: Q1) {
		VL complement = quadruple; // create the complentary quadruple such that L = complement[i] + quadruple[i]
		for(int i=0; i< complement.size(); i++) {
			complement[i] = L - complement[i];
		}
		// compute it_first and it_last such that all quadruples in [it_first, it_last) equal to complement
		auto it_first = std::lower_bound(Q2.begin(), Q2.end(), complement);
		auto it_last = std::upper_bound(Q2.begin(), Q2.end(), complement);
		long nb = std::distance(it_first, it_last); // number of quadruples equal to complement
		N += nb;
	}
	
	std::cout << N/24 << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
