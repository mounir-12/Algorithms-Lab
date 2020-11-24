#include <iostream>
#include <vector>
#include <algorithm>

void testcase() {
	int n; std::cin >> n;
	std::vector<int> x(n);
	for(int i=0; i<n; i++) {
		std::cin >> x[i];
	}
	int sum = 0;
	int E = 0;
	int O = 0;
	
	for(int i=0; i<n; i++) {
		sum += x[i];
		if(sum % 2 == 0) {
			E++;
		} else {
			O++;
		}
	}
	
	int nb = (E*(E-1))/2 + (O*(O-1))/2 + E;
	std::cout << nb << std::endl;		
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}

