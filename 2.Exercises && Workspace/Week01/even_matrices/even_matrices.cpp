#include <iostream>
#include <vector>
#include <algorithm>

int even_pairs(std::vector<int> x) {
	int sum = 0;
	int E = 0;
	int O = 0;
	int n = x.size();
	
	for(int i=0; i<n; i++) {
		sum += x[i];
		if(sum % 2 == 0) {
			E++;
		} else {
			O++;
		}
	}
	
	int nb = (E*(E-1))/2 + (O*(O-1))/2 + E;
	return nb;
}

void testcase() {
	int n; std::cin >> n;
	
	std::vector<std::vector<int>> M(n, std::vector<int>(n)); // matrix M is n by n
	for(int i=0; i<n; i++) { // read the matrix
		for(int j=0; j<n; j++) {
			std::cin >> M[i][j];
		}
	}
	
	// table sums stores at index (i, j) the sum: M[i][0] + ... M[i][j] (i,e partial sums along each line from column 0 to column j)
	std::vector<std::vector<int>> sums(n, std::vector<int>(n));
	for(int i=0; i<n; i++) {
		int sum = 0;
		for(int j=0; j<n; j++) {
			sum += M[i][j];
			sums[i][j] = sum;
		}
	}
	
	int count = 0; // nb of even sub-matrices
	for(int j1=0; j1<n; j1++) { // loop over all pairs of columns (j1, j2) where j1 <= j2
		for(int j2=j1; j2<n; j2++) {
			std::vector<int> values; // store at index i the sum "M[i][j1] + ... + M[i][j2]"
			for(int i=0; i<n; i++) { // gather all sums "M[i][j1] + ... + M[i][j2]" for all lines i
				int sum;  // the sum "M[i][j1] + ... + M[i][j2]"
				if(j1 == 0) {
					sum = sums[i][j2];
				} else { // j1 > 0
					sum = sums[i][j2] - sums[i][j1-1];
				}
				values.push_back(sum);
			}
			count += even_pairs(values); // perform even pairs on the gathered values, i,e find the number of pairs i1 <= i2 such that
										 // values[i1] + ... + values[i2] is even
		} 
	}
	
	std::cout << count << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
