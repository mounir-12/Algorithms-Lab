#include <iostream>
#include <vector>

void testcase() {
	int n; std::cin >> n;
	std::vector<int> v(n);
	for(int i=0; i<n; i++) {
		std::cin >> v[i];
	}
	
	int d; std::cin >> d;
	int a; std::cin >> a; int b; std::cin >> b;
	
	v.erase(v.begin()+d); // erase element d
	v.erase(v.begin()+a, v.begin()+b+1); // erase the elements with indices in range [a,b]
	
	
	if(v.empty()) {
		std::cout << "Empty";
	} else {
		for(size_t i=0; i<v.size(); i++) {
			std::cout << v[i];
			if(i+1 != v.size()) {
				std::cout << " ";	
			}
		}
	}
	std::cout <<  std::endl;
	
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	for(int i=0; i<t; i++) {
		testcase();
	}
	return 0;
}
