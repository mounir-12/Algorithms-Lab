#include <iostream>
#include <string>
#include <algorithm>    // std::reverse


void testcase() {
	std::string a; std::cin >> a;
	std::string b; std::cin >> b;
	
	std::cout << a.size() << " " << b.size() << "\n";
	std::cout << a + b << "\n";
	std::reverse(a.begin(), a.end()); std::reverse(b.begin(), b.end());
	char temp = a[0];
	a[0] = b[0];
	b[0] = temp;
	std::cout << a << " " << b << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	int t; std::cin >> t; // read #(test cases)
	
	for(int i=0; i<t; i++) {
		testcase();
	}
	return 0;
}

