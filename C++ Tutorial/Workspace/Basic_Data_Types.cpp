#include <iostream>
#include <string>
#include <iomanip>


void testcase() {
	int i; std::cin >> i;
	long long l; std::cin >> l;
	std::string s; std::cin >> s;
	double d; std::cin >> d;
	
	std::cout << std::setprecision(2) << std::fixed << i << " " << l << " " << s << " " << d << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t; std::cin >> t; // read #(test cases)
	
	for(int i=0; i<t; i++) {
		testcase();
	}
	return 0;
}
