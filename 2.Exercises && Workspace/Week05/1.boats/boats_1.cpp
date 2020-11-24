#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
#include <climits>

struct Boat {
	int length;
	int ring_pos;
	int right_corner;
	Boat(int l, int p, int  r): length(l), ring_pos(p), right_corner(r) {}
};

bool operator<(Boat b1, Boat b2) { // operator< to determine which one is less prioritary
	// so b1 < b2 (i,e less prioritary) if it has a larger right corner
	// if they have the same right_corner, this happens only if one or more boats were moved
	// we prioritize the one that was moved first since all boats that have the same right corner use the same space and
	// if only 1 boat was moved, then we know that doesn't overlap anymore with the currently chosen boats 
	// whereas the unmoved one (that has the same right_corner which is also its ring_pos) might overlap (we don't know)
	// if 2 or more boats were moved such that their right_corner fall on each other
	// this means there was no boat chosen between the 2 or more moves and all boats are non-overlapping with the currently chosen
	// ones so all the moved boats are valid (thus we prioritize the 1st moved anyways)
	// Note: the 1st one moved is necessarily the one with the smallest ring_pos
	
	if(b1.right_corner == b2.right_corner) { // same right corner
		return b1.ring_pos > b2.ring_pos; // prioritize the one with smallest ring pos
	}
	return b1.right_corner > b2.right_corner;
}

void testcase() {
	// same solution as boats.cpp but a bit slower since using "set" instead of "priority queue"
	int n; std::cin >> n; // number of boats >=1
	
	std::set<Boat> boats;
	
	// we start by pushing all boats such that their right corner is at the ring pos
	// i,e we try to keep as much space as possible to the right of the boat
	// some boats will be overlapping with others
	// we then start choosing boats, choosing first the one that minimizes the overall used space
	// where the overall used space is from the "left most corner" to the "right most corner" amongst
	// the already chosen boats.
	
	// We establish a priority system between boats such that the most prioritary is the one with the smallest
	// right_corner.
	// we keep track of "limit_corner" which is the right corner of the used space
	 
	for(int i=0; i<n; i++) {
		int l,p; std::cin >> l >> p;
		boats.insert(Boat(l,p,p)); // all boats pushed to the left
	}
	
	int limit_corner = INT_MIN;

	std::vector<Boat> chosen_boats;
	
	while(!boats.empty()) {
		auto it = --boats.end(); // pointer on the most prioritary boat
		Boat b = *(it); boats.erase(it); // pick the most prioritary boat and remove it from the set
		int left_corner = b.right_corner - b.length;
		
		if(b.ring_pos < limit_corner) { // boat can't be chosen because even if we move it to the right most
			// then its left corner will be at ring_pos which overlaps with the limit corner
			continue; // do nothing (since boat was removed from queue)
		}
		// boat can be chosen
		if(left_corner >= limit_corner) { // boat doesn't overlap with previously chosen boats
			chosen_boats.push_back(b); // choose the this boat
			limit_corner = b.right_corner; // move he limit_corner
		} else { // boat overlaps, but is still a candidate to be chosen
			b.right_corner = limit_corner + b.length; // move it to the right
			boats.insert(b); // push it back to the queue to reorder it with the others depending on priority
		} 
	}
	
	std::cout << chosen_boats.size() << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
