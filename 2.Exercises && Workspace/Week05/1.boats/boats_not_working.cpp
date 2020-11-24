#include <iostream>
#include <vector>
#include <algorithm>

struct Boat {
	int length;
	int pos;
	Boat(int l, int p): length(l), pos(p) {}
};

bool operator<(Boat b1, Boat b2) {
	return b1.pos < b2.pos;
}

void testcase() {
	int n; std::cin >> n; // number of boats >=1
	
	std::vector<Boat> boats;
	for(int i=0; i<n; i++) {
		int l,p; std::cin >> l >> p;
		boats.push_back(Boat(l,p));
	}
	
	std::sort(boats.begin(), boats.end());
	
	// Not Working: counter example:
	// Suppose at some point, the current boat and the next boat are both very long boats
	// then by this algorithm, we will choose one of the 2
	// but picking either one would use a lot of space and take many other boats space
	
	// Better approach: we establish a priority system between boats such that
	// the boat with the smallest right corner (i,e uses the least space possible) is the one
	// to be picked. 
	
	// each boat can have its left corner in the range [pos-length, pos]
	// we try to push the current boat all the way to the left untill either its left corner
	// touches the right corner of the previous boat or reaches the left limit pos-length
	// note when left corner is in pos-length then right corner is in pos
	
	// the base case: the boat with the left most pos will be pushed left all the way to the left limit (i,e its right corner at pos)
	// the idea: we take the the curr boat and its next, we push the curr boat all the way to the left
	// and check where its right corner right_corner_curr lands
	// then we remove the curr boat and push the next boat all the way to the left
	// then check where its right right_corner_next corner lands
	// whichever has its right corner before the other will be chosen
	// that because we want to use as little space as possible from -inf to to chosen right corner
	// so that the next boats the space in [first_right_corner; second_right_corner] available for usage
	
	int prev_boat_right_corner = boats[0].pos;
	std::vector<Boat> chosen_boats;
	std::vector<std::pair<int, int>>  chosen_boats_ranges;
	chosen_boats.push_back(boats[0]); // add the left most boat
	chosen_boats_ranges.push_back(std::make_pair(boats[0].pos - boats[0].length, boats[0].pos));
	for(int i=1; i<n; i++) {
		Boat curr = boats[i];
		if(curr.pos < prev_boat_right_corner) { // can't add this boat
			continue;
		}
		int curr_boat_left_corner = std::max(prev_boat_right_corner, curr.pos - curr.length);
		int curr_boat_right_corner = curr_boat_left_corner + curr.length;
		// can add this boat
		if(i==n-1) { // last boat, just add it
			chosen_boats.push_back(curr);
			chosen_boats_ranges.push_back(std::make_pair(curr_boat_left_corner, curr_boat_right_corner));
			break;
		}

		Boat next = boats[i+1];
		int next_boat_left_corner = std::max(prev_boat_right_corner, next.pos - next.length);
		int next_boat_right_corner = next_boat_left_corner + next.length;
		if(curr_boat_right_corner <= next_boat_right_corner) { // we add curr boat
			chosen_boats.push_back(curr);
			chosen_boats_ranges.push_back(std::make_pair(curr_boat_left_corner, curr_boat_right_corner));
			prev_boat_right_corner = curr_boat_right_corner;
		} else { // we add the next boat
			chosen_boats.push_back(next);
			chosen_boats_ranges.push_back(std::make_pair(next_boat_left_corner, next_boat_right_corner));
			prev_boat_right_corner = next_boat_right_corner;
			i++;
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
