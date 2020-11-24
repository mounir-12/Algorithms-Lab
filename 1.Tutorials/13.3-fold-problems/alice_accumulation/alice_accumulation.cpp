#include <iostream>
#include <vector>
#include <climits>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

void testcase() {
	int k; std::cin >> k;
	
	VVI tree(k);
	for(int i=0; i<k; i++) {
		for(int j=0; j<=i; j++) {
			int v; std::cin >> v;
			tree[i].push_back(v);
		}
	}
	
	int max_val = INT_MIN;
	for(int i=k-1; i>=0; i--) {
		for(int j=0; j<=i; j++) {
			if(i+1 < k) {
				tree[i][j] += (tree[i+1][j] + tree[i+1][j+1]);
				if(i+2 < k) {
					tree[i][j] -= tree[i+2][j+1];
				}
			}
			max_val = std::max(max_val, tree[i][j]);
		}
	}
	std::cout << max_val << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
