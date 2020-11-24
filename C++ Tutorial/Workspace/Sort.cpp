#include <iostream>
#include <set>

void testcase() {
	int n; std::cin >> n;
	std::multiset<int> s;
	for(int i=0; i<n; i++) {
		int a; std::cin >> a;
		s.insert(a);
	}
	
	int x; std::cin >> x;
	if(x==0) {
		for(auto it=s.begin(); it!=s.end(); ) {
			std::cout << *it;
			it++;
			if(it != s.end()) {
				std::cout << " ";
			}
		}
	} else {
		
		for(auto it=s.rbegin(); it!=s.rend(); ) {
			std::cout << *it;
			it++;
			if(it != s.rend()) {
				std::cout << " ";
			}
		}
	}
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t; std::cin >> t;
	for(int i=0; i<t; i++) {
		testcase();
	}
	
	return 0;
}
