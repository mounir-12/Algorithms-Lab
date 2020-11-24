#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>

#define MAX_DIST 200

void insert_pos(std::set<int>& best_pos, int x, bool insert_next) {
	best_pos.insert(x);
	if(insert_next) {
		best_pos.insert(x+1);
	}	
}

void testcase() {
	// sliding window solution
	int n; std::cin >> n; // n=#parasols
	std::vector<int> coord(n); // coordinates of parasols
	for(int i=0; i<n; i++) {
		std::cin >> coord[i];
	}
	
	std::sort(coord.begin(), coord.end());
	
	std::set<int> best_pos;
	int left = 0, right = 0; // left and right pointers of the window
	int max_nb_clients = 0;
	int min_max_dist = std::abs(*(coord.end()-1) - *(coord.begin()));
	
	while(right < n) {
		int dist = coord[right] - coord[left]; // distance between the left and right most client
		while(dist > MAX_DIST) { // clients too far for each other, keep moving the left pointer
			left++; // move left pointer
			dist = coord[right] - coord[left]; // compute the new distance between the left and right most client
		}
		int nb_clients = right - left +1; // nb of clients in this window
		
		int sum = coord[right]+coord[left];
		int x = std::floor(sum/2.0); // the position that minimizes the max distance to be walked is the exact middle, we round it down
									 // use floor to account for sum < 0, but if sum >= 0 then sum/2 == std::floor(sum/2.0)
		// if sum is odd then the above value x = std::floor(sum/2.0) is an not the precise center coordinate
		// => the precise value x_p has 0.5 as decimal part (i,e x_p = x + 0.5), so x_p is the center of [coord[left], coord[right]]
		// the distance from coord[left] to coord[right] is at most 200, so the distance from x_p to coord[right] is at most 100
		// so coord[right] <= x_p+100, but because x_p has 0.5 as decimal part and coord[right] in an integer (i,e has no decimal
		// part), then x_p+100 cannot equal coord[right] so coord[right] < x_p+100 => coord[right] < x+0.5+100 => coord[right] <= x+100 (I)
		// Likewise for coord[left], we have x_p-100 < coord[left] => x+0.5-100 < coord[left] => (x+1)-100 <= coord[left] (II)
		// so from (I) and (II) we have that both x and x+1 are candidate positions that reach the same clients
		// the distance from x_p to coord[left] and to coord[right] is the same call it d, then the distance from x=x_p-0.5 to coord[left]
		// is d-0.5 and the distance from x to coord[right] is d+0.5. We take their max. Likewise, if (x+1) is also a candidate
		// position, then dist(x+1, coord[left]) = d+0.5 and dist(x+1, coord[right]) = d-0.5. We take their max. So in both cases we
		// take the max between d+0.5 and d-0.5, so just compute max_dist_travled = max(dist(x, coord[left]), dist(x, coord[right]))
		bool insert_next = (sum % 2 != 0); // odd sum, insert x+1
		int max_dist = std::max(std::abs(x-coord[left]), std::abs(coord[right]-x));
		
		if(nb_clients > max_nb_clients) {
			max_nb_clients = nb_clients;
			best_pos.clear();
			insert_pos(best_pos, x, insert_next);
			min_max_dist = max_dist;
		} else if(nb_clients == max_nb_clients) {
			if(max_dist < min_max_dist) { // new optimal location
				best_pos.clear();
				insert_pos(best_pos, x, insert_next);
				min_max_dist = max_dist;
			} else if(max_dist == min_max_dist) {
				insert_pos(best_pos, x, insert_next);
			}
		}
		right++;
	}
	
	std::cout << max_nb_clients << " " << min_max_dist << "\n";
	for(std::set<int>::iterator it= best_pos.begin(); it!=best_pos.end();) {
		std::cout << *it;
		it++;
		if(it!=best_pos.end()) {
			std::cout << " ";
		}
	}
	
	std::cout << std::endl;
	
}


int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}

