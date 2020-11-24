#include <iostream>

void testcase() {
	int n; std::cin >> n;
	long sum = 0;
	for(int i=0; i<n; i++) {
		long a; std::cin >> a;
		sum += a;
	}
	
	std::cout << sum << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
