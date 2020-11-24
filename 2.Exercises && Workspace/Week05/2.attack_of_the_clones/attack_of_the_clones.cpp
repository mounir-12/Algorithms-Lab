#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
typedef std::pair<int, int> PII;
typedef std::pair<int, bool> PIB;
typedef std::vector<PII> VPII;
typedef std::vector<PIB> VPIB;

// greedily compute the max number of jedis which protect only segments in [left+1, right-1] and do not overlap with each other
int get_max_jedis(VPII& non_crossing_jedis, int left, int right) { 
	int nb = 0; // the value returned
	for(PII jedi : non_crossing_jedis) {
		if(jedi.first > left and jedi.second < right) { // if within bounds
			nb++;
			left = jedi.second; // continue the search in the range [left = jedi.second, right]
		}
	}
	return nb;
}

void testcase() {
	// we have a circle that we split into m equal segments where the 1st segment we index by 0, e,g with m=6:
	//   _0_
	// 5/   \1
	// 4\___/2
	//    3
	// We say that we unroll the circle at segment i if we cut the circle at the left endpoint of segment i and
	// flatten the circle (so we have 1 line), e,g above circle unrolled at segment 0: _0_ _1_ _2_ _3_ _4_ _5_
	// above circle unrolled at segment 2: _2_ _3_ _4_ _5_ _0_ _1_
	// We have n jedis each one protecting some segments. Segments protected by jedi i are specified by 2 indices 
	// a_i and b_i such that 0 <= a_i, b_i < m and if(a_i < b_i) then jedi i protects the segments "a_i, ..., b_i"
	// and if(a_i > b_i) then jedi i protects segments a_i, ..., m-1 and 0, ..., b_i (i,e jedi i protects 0)
	// or equivalently jedi i protects all segments except segments "b_i + 1, ..., a_i - 1"
	// Let C_0 be the set of jedis that protect 0 and NC_0 the complement of it (i,e the set of jedis that do not
	// protect 0). 
	// The optimal solution will either: 
	//	  1- contains exactly 1 jedi in C_0 (because choosing some jedi in C_0 automatically forbids choosing any other 
	//	     jedi in C_0 since they also protect 0) and 0 or more jedis in NC_0
	// or 2- contain no jedi in C_0 and only jedis in NC_0
	// Any jedi in NC_0 has 0 <= a_i <= b_i < m. Thus if all n jedis are in NC_0 (i,e C_0 empty), we can unroll the 
	// circle at segment 0 and the problem is just a scheduling problem that can be solved greedily by choosing amongst 
	// the unchosen jedis the one that has the earliest finish (i,e smallest b_i) and doesn't overlap with the previously 
	// chosen jedis.
	// But since we might have some jedis in C_0, then we also take into consideration the case 1 above. So for each
	// jedi i in C_0, we pick it then we pick as many jedis in NC_0. The picked ones should not ovelap with the jedi i
	// chosen from C_0, i,e they should protect only segments in [b_i+1, ..., a_i-1]. We pick those greedily as above
	// So we pick a jedi i in C_0 then we greedily pick jedis j in NC_0 such that b_i < a_j and b_j < a_i. This takes
	// O(|NC_0|) = O(n) after having sorted the jedis in NC_0 (sorting is done once)
	// The problem here is that we need to do that for every jedi in C_0 which may be large e,g C_0 contains n/2 jedis
	// and NC_0 contains n/2 so the run time would be O(n^2)
	// Note here that we do not necessarily need to use C_0 and NC_0, we may as split the jedis to C_k = "the set of jedis
	// that protect segment k" and NC_k="the set of jedis that do not protect segment k" for any k and unroll the circle 
	// at segment k
	// After picking the right value k, we can recenter the circle by pivoting it ccw untill segment k becomes segment 0
	// then we can run our algorithm as before for C_0 and NC_0.
	// The right value k to choose is the the segment k that is protected by the fewest number of jedis and we
	// know that there exists a segment that is protected by at most 10 jedis so k <= 10 then we run the greedy algorithm k+1 times.
	// To find k, the idea is to unroll the circle at segment 0 to be a line then traverse the line from 0 to m-1 and keep 1 counter
	// (1 counter not 1 counter per jedi) -- suppose for now that C_0 empty.
	// Whenever we reach an a_i, we increment the counter for each jedi that starts at a_i. Whenever we reach a b_j, we decrement 
	// the counter for each jedi that ends there.
	// So if an a_i is followed by a_j, after finishing the increments for a_i, the counter holds the number of jedis
	// that protect all segments [a_i, ..., a_j-1] since a_i is directly followed by a_j so there is no a_l or b_l between them.
	// So we get a count of the number of jedis covering these segments, so any of these segments can represent all of them since
	// they have the same count, we take a_i.
	// Likewise if a_i is followed by a b_j, then after the increments of a_i, we get a count of the jedis protecting the segments
	// [a_i, ..., b_j], we take a_i to represent those consecutive segments that have same count.
	// The same goes for b_i when followed by a_j: the count after finishing the decrements is that of segs [b_i+1, ..., a_j-1]
	// and their representant would be b_i+1
	// when followed by a b_j: the count after finishing the decrements is that of segments [b_i+1, ..., b_j] with b_i+1 as representant
	// To deal with the case where some jedi i protects [a_i, ..., m-1] and [0, ..., b_i], we see it as if they were 2 jedis
	// with protection areas [a_i, ..., m-1] and [0, ..., b_i]
	// Then, given all representents and their counts c_i, we take the one that has the smallest c_i to be k.
	
	int n, m; std::cin >> n >> m; // n=#jedis, m=#segments
	
	VPIB segs; // stores pairs (segment index, is_last) i,e (a_i, false) or (b_i, true)
			  // this allows us to sort lexicographically because false = 0 < true = 1
	VPII jedis; // each jedi i is a pair (a_i, b_i)
	for(int i=0; i<n; i++) {
		int a, b; std::cin >> a >> b;
		a--; b--; // 0-based index
		jedis.push_back(PII(a, b));
		segs.push_back(PIB(a, false));
		segs.push_back(PIB(b, true));
		if(a > b) {
			segs.push_back(PIB(0, false));
			segs.push_back(PIB(m-1, true));
		}
	}
	std::sort(segs.begin(), segs.end()); // sort pairs, if we have p1=(a_i, false) p2=(b_i, true) with a_i=b_i then p1 < p2
	
	int counter = 0;
	int min_count = INT_MAX;
	int k = 0; // the segment with smallest count
	for(uint i=0; i<segs.size(); ) {
		PIB seg = segs[i]; // the current segment a_j or b_j for some jedi j
		while(segs[i].first == seg.first and not segs[i].second) { // keep incrementing counter for each jedi that starts here
			counter++; i++; // increment counter and move to next
		}
		if(counter < min_count) { // the current segment has less jedis protecting it
			k = seg.first;
			min_count = counter;
		}
		while(segs[i].first == seg.first and segs[i].second) { // keep decrementing counter for each jedi that ends here
			counter--; i++; // decrement counter and move to next
		}
	}
	
	VPII crossing_jedis; // the jedis that cross/protect k
	VPII non_crossing_jedis; // the jedis that don't cross/protect k
	
	for(PII jedi: jedis) {
		// recenter the jedis
		int a = jedi.first, b = jedi.second;
		int new_a = a-k + (a < k ? m: 0);
		int new_b = b-k + (b < k ? m: 0);
		if(new_a == 0 or new_b < new_a) { // check if the jedi protects the new origin k, i,e jedi starts at the origin 
										  // or starts after the origin an crosses it
			crossing_jedis.push_back(PII(new_a, new_b)); // add it to the set of jedis protecting the origin
		} else { // otherwise, add the jedi to the set of jedis that do not protect/cross the new origin
			non_crossing_jedis.push_back(PII(new_a, new_b));
		}
	}
	// sort the non crossing jedis according to their last protected segment
	std::sort(non_crossing_jedis.begin(), non_crossing_jedis.end(), [](PII j1, PII j2) { return j1.second < j2.second; });
	
	int max_jedis = get_max_jedis(non_crossing_jedis, -1, m); // we do not choose any crossing jedi, so left=-1 and right=m 
															 // (i,e choose any non-crossing jedi i since a_i>left and b_i<right)
	for(PII jedi: crossing_jedis) {
		int a = jedi.first, b = jedi.second;
		if(a == 0) a = m; // jedi starts at the origin, then we need to add m to a so that a > b
		int max_jedis_with_curr = get_max_jedis(non_crossing_jedis, b, a) + 1; // get the max nb of non-crossing jedis only protecting
															// segments [b+1, ..., a-1]. We add +1 for the current crossing jedi we chose
		max_jedis = std::max(max_jedis_with_curr, max_jedis); // keep the max
	}
	std::cout << max_jedis << std::endl; // output result
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
