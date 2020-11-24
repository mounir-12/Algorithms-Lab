#include <iostream>
#include <map>
#include <string>
#include <set>

void testcase() {
	int q; std::cin >> q;
	std::multimap<std::string, int> mm;
	for(int i=0; i<q; i++) {
		int a; std::cin >> a; std::string b; std::cin >> b;
		if(a == 0) {
			mm.erase(b);
		} else if(a > 0) {
			mm.insert(std::make_pair(b, a));
		}
	}
	std::string s; std::cin >> s;
	std::multiset<int> ms;
	
	auto low = mm.lower_bound(s); auto up = mm.upper_bound(s);
	while(low != up) {
		ms.insert(low->second);
		low++;
	}
	if(ms.empty()) {
		std::cout << "Empty";
	} else {
		for(auto it=ms.begin(); it!=ms.end();) {
			std::cout << *it;
			it++;
			if(it != ms.end()) {
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
