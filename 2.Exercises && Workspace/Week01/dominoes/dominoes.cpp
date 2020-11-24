#include <iostream>
#include <vector>
#include <algorithm>

void testcase() {
	int n; std::cin >> n;
	std::vector<int> h(n); // heights
	for(int i=0; i<n; i++) {
		std::cin >> h[i];
	}
	
	int reached_distance = 0;
	int lfd = 0; // last fallen/touched domino
	int curr = 0; // curr domino to fall
	int nd = 0; // next domino to fall
	int size = h.size();
	
	do {
		curr = nd; // make the next domino fall
		reached_distance = curr + h[curr];
		lfd = std::min(reached_distance, size) -1;
		for(int j=curr; j<=lfd; j++) {
			if(h[nd] + nd < h[j]+j) {
				nd = j;
			}
		}
	} while(lfd < h.size()-1 && nd != curr);
	
	std::cout << (lfd+1) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
