#include <iostream>
#include <set>

void testcase() {
	
	int q; std::cin >> q;
	std::set<int> s;
	for(int i=0; i<q; i++) {
		int a; std::cin >> a; int b; std::cin >> b;
		switch(a) {
			case 0:
				s.insert(b);
			break;
			case 1:
			auto it = s.find(b);
			if(it != s.end()) {
				s.erase(it);
			}
			break;
		}
	}
	if(s.empty()) {
		std::cout << "Empty";
	} else {
		for(auto it=s.begin(); it!=s.end();) {
			std::cout << *it;
			it++;
			if(it != s.end()) {
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
