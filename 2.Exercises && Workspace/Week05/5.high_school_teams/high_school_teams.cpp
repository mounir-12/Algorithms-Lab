#include <iostream>
#include <vector>
#include <map>
typedef std::pair<int, int> PII;

void get_splits(std::vector<int>& student_strengths, int curr_student, int r_strength, int b_strength, int k, std::map<PII, long>& res) {
	if(curr_student == student_strengths.size()) { // no more students to assign
		int diff = r_strength - b_strength;
		res[PII(k, diff)]++; // create a new mapping if non-existant then increment the number of pairs (r_strength, b_strength) found
		return;
	}
	for(int i=0; i<3; i++) {
		if(i==0) // assign curr_student to set R, so add it's strength to R's strength and move to next student
			get_splits(student_strengths, curr_student+1, r_strength+student_strengths[curr_student], b_strength, k, res);
		else if(i==1) // assign curr_student to set B, so add it's strength to B's strength and move to next student
			get_splits(student_strengths, curr_student+1, r_strength, b_strength+student_strengths[curr_student], k, res);
		else if(i==2) // add curr_student to the set of unassigned student, so incerement k and move to next student
			get_splits(student_strengths, curr_student+1, r_strength, b_strength, k+1, res);
	}
}

void testcase() {
	// We have a set S of n students, student i having strength s_i. We have 3 sets R, B and NP where R is the set of players
	// in the red team, B is the set of players in the blue team and NP is the set of non-players
	// We would like to assign each student to one of the 3 sets such that the R_strength = B_strength and NP.size() <= k
	// "R_strength = total strength of students in R" and "B_strength = total strength of students in B"
	// The goal is to find the number of assignements that respect those 2 constraints
	// We first partition S into S1 and S2 such that around half of the students are in each set.
	// For all students in S1, we find all possible ways to assign them to R1, B1 and NP1. For each such assignement (R1, B1, NP1)
	// we compute the value k' = NP1.size() and construct the pair (R1_strength, B1_strength)
	// We map all pairs (R1_strength, B1_strength) that share the same k' to k' i,e we group them in 1 vector and map them to k'
	// We do the same thing for S2.
	// So we get 2 maps map1 and map2 for S1 and S2. There is 3^(n/2) possible assignement of all students in S1 (since that for
	// each student, we have a choice between 3 sets we can assign it to). Same thing for S2.
	// Then for all 3^(n/2) triples (k', R1_strength, B1_strength) in map1 we shall find all triples (k'', R2_strength, B2_strength)
	// in map2 such that (I) k'+k'' <= k and (II) R1_strength + R2_strength == B1_strength + B2_strength
	// So we could go through all triples (k', R1_strength, B1_strength) in map1, then we create a loop over indices j=0...(k-k')
	// and get all pairs mapped to j in map2 (i,e map2[j]) and check all of them to decide which ones satisfy (II)
	// This is too expensive, so we write (II) differently as (R1_strength - B1_strength) ==  -(R2_strength - B2_strength)
	// and when creating the pairs for S1 (and S2) we compute diff = (R1_strength - B1_strength) (likewise for S2). Then instead of 
	// mapping each pair (R1_strength, B1_strength) to k' only, we map it to the pair (k', diff).
	// So now map1 has mappings (k', diff) -> "vector of pairs (R1_strength, B1_strength) that have k' non-players and diff". 
	// Likewise for map2
	// Now when we loop over all pairs (R1_strength, B1_strength) in map1 which are mapped to (k', diff), we only need to find
	// the pairs in map2 that are mapped to (j, -diff) for all j=0...k-k' (as before). The current pair in map1 could be combined with
	// any pair in map2[(j, -diff)], thus we add map2[(j, -diff)].size() to the result "r" that we output later.
	// One way we can improve here is to say, since we don't need the pairs themselves but only the number of pairs in both maps, then
	// instead of storing a vector of pairs, we only need to store its size. So each time we find a new pair (R1_strength, B1_strength)
	// with k', we compute diff then increment the number of pairs mapped to (k, diff) i,e we perform map1[(k, diff)]++. Same with S2
	// Then we only need to loop over all pairs (k', diff) in map1 and increment "r" by the value map1[(k', diff)] * map2[(j, -diff)]
	// for all j=0...(k-k')
	int n, k; std::cin >> n >> k; // n=#students, k=#max_number_of_non_participants
	
	std::vector<int> student_strengths1; // 1st n/2 students strengths
	std::vector<int> student_strengths2; // last n/2 students strengths
	for(int i=0; i<n; i++) {
		int s; std::cin >> s;
		if(i < n/2) student_strengths1.push_back(s);
		else student_strengths2.push_back(s);
	}
	
	std::map<PII, long> res1; // map1
	std::map<PII, long> res2; // map2
	get_splits(student_strengths1, 0, 0, 0, 0, res1);
	get_splits(student_strengths2, 0, 0, 0, 0, res2);
	long sol = 0; // solution to the problem
	for(auto it=res1.begin(); it!=res1.end(); it++) {
		int k1 = (it->first).first;
		int diff = (it->first).second;
		for(int j=0; j<=k-k1; j++) {
			sol += it->second * res2[PII(j, -diff)];
		}
	}
	std::cout << sol << std::endl;	
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
