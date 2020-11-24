#include <iostream>
#include <iomanip>
#include <vector>

std::vector<std::vector<double>> DP;

double optimal_sol(std::vector<double>& probas, int i, int balance_i, int m) {
	// trivial cases
	if(i == probas.size()) { // day n (the day the magician leaves, so no more bets possible)
		if(balance_i < m) return 0.0; // balance_n < m, so probability that balance >= m in day n is 0.0
		else return 1.0; // balance_n >= m, so probability that balance_n >= m at day n is 1.0
	}
	if(balance_i >= m) return 1.0; // case where balance_i >= m, return 1.0
	if(DP[i][balance_i] != -1.0) return DP[i][balance_i]; // check if we already know the max probability at day i given balance_i
	else { // try all possible bets and choose the bet that maximizes the probability that balance_n >=m, report
		   // this probability in DP[i][balance_i]
		double max = 0.0;
		for(int b_i=0; b_i<=balance_i; b_i++) {
			max = std::max(max, optimal_sol(probas, i+1, balance_i+b_i, m) * probas[i]
								+ optimal_sol(probas, i+1, balance_i-b_i, m) * (1.0 - probas[i]));
		}
		DP[i][balance_i] = max; // save the value and return it
		return DP[i][balance_i];
	}
}

int testcase() {
	int n, k, m; std::cin >> n >> k >> m; // n=#days, k=balance_of_day0, m=target_balance_at_day_n
	std::vector<double> probas;
	for(int i=0; i<n; i++) { // read probas
		double p; std::cin >> p;
		probas.push_back(p);
	}
	
	// let balance_n be the balance at day n (the day the magician leaves, i,e no more bet possible), we have balance_0 = k
	// DP[i, balance_i] = the max probability to get balance_n >= m for any bet b_i we bet given we start with balance_i in day i
	// i,e for all bets possible in day i (b_i=0...balance_i) we choose the one that maximizes the probability that balance_n >= m
	// and report this probability in DP[i, balance_i]
	// so we suppose day i is the 1st day and we are given that day balance_i. If we bet b_i>0 then we have 2 cases, case1: we
	// start the next day i+1 with balance = balance_i+b_i, this happens with probability p_i=probas[i], case2: we start the next 
	// day i+1 with balance = balance_i-b_i, this happens with probability 1-p_i.
	// If we bet b_i>0, then to maximize the probability that balance_n >= m, we take the max possible probability for the case1
	// i,e DP[i+1, balance_i+b_i], multiply it by the probability of case1, i,e p_i, then do the same for case2 and sum the 2 results
	// so the max probability that balance_n >= m with bet b_i in day i is: p_i * DP[i+1, balance_i+b_i] + (1-p_i) * DP[i+1, balance_i-b_i]
	// for the case of b_i=0, we have p_i * DP[i+1, balance_i+b_i] + (1-p_i) * DP[i+1, balance_i-b_i] = DP[i+1, balance_i] * (1-p_i+p_i)
	// = DP[i+1, balance_i] i,e the balance doesn't change from day i to i+1, so we only have 1 case namely that we start day i+1 with
	// balance = balance_i, this happens with probability 1.0
	// If at some day i<n, we have balance_i >= m then we know the max probability that balance_n >= m is 1.0 by betting b_j=0
	// for all days j=i...(n-1), this case is trivial and we directly know that DP[i, balance_i] for such cases is 1.0
	// thus we limit DP to balances <= m and if we encounter this case then we directly return 1.0
	DP = std::vector<std::vector<double>>(n, std::vector<double>(m, -1.0));
	std::cout << optimal_sol(probas, 0, k, m) << std::endl;
	return 0;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cout << std::fixed << std::setprecision(5);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
