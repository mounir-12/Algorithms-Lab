#include <iostream>
#include <vector>
#include <algorithm>

typedef std::pair<int, int> PII;
typedef std::vector<int> VI;
typedef std::vector<PII> VPII;
typedef std::vector<VPII> VVPII;

struct Drink {
	int cost;
	int volume;
	Drink(int c, int v): cost(c), volume(v) {}
};

void testcase() {
	int n, k; std::cin >> n >> k; // n=#(distinct drinks), k=#people
	std::vector<Drink> drinks; 
	
	for(int i=0; i<n; i++) {
		int c, v; std::cin >> c >> v;
		drinks.push_back(Drink(c,v));
	}
	
	// table contains in each cell i,j a pair (min_cost, nb_types)
	// where min_cost is the min_cost to get at least "i" liters in volume
	// by combining any amount (0 or more) of "each" drink type 0...j
	// nb_types is the corresponding number of drink types combined (i,e whith amount > 0)
	// note that in the same line i, the cost from j to j+1 can only decrease (or stay the same)
	// because if adding drink type j+1 would increase the cost, then just don't add it and take
	// the solution at j (i,e add 0 amount of drink type j+1)
	VVPII table(k+1, VPII(n));
	
	//base case, we only use drink type 0 (so we fill column 0)
	int nb_bottles = 0; // nb_botlles of drink of type 0
	for(int i=0; i<=k; i++) {
		if(nb_bottles * drinks[0].volume >= i) { // check that nb_bottle of drinks[0] suffice to get volume i at least
			int total_cost = nb_bottles*drinks[0].cost;
			table[i][0] = std::make_pair(total_cost, 1);
		} else { // not enough bottles to reach i liters at least, adding 1 bottle gives us at least i liters
		 // this is because between last and current iteration, i (the min volume in liters) has grown by 1 liter
		 // so adding 1 bottle is sufficient since each bottle is at least 1 liter in volume
		 nb_bottles++;
		 int total_cost = nb_bottles*drinks[0].cost;
		 table[i][0] = std::make_pair(total_cost, 1);	
		}
	}
	
	for(int i=1; i<=k; i++) { // ignore the line with volume i=0, must be all pairs of zeros
		for(int j=1; j<n; j++) { // loop over the other drink types 1 ... n-1
			int cost = table[i][j-1].first; // get the min cost acheived by a combination of drink types 0...j-1 to get at least i liters
			int nb_types = table[i][j-1].second; // likewise for the types of drinks, note: types <= j-1 since only combined drinks of types 0...j-1 
			// these are the default values for this cell, we try to improve the cost (i,e lower)
			// and update the nb_types if improvement of cost possible
			 
			int curr_cost = drinks[j].cost; // cost of 1 bottle of the current drink type
			int curr_volume = drinks[j].volume; // volume of 1 bottle of  the current drink type
			if(curr_cost < cost && curr_volume >= i) { // check if taking 1 bottle of type j gives us lower cost and volume >= i
				cost = curr_cost; nb_types = 1;
			} else if(curr_volume < i) { // 1 bottle of current drink type not sufficient to reach i liters
				// we still need (i-curr_volume) to reach the threshold volume i, we check the previous
				// best solutions found for min_volume=(i-curr_volume) at the same column j (i,e by combining  drink types 0...j)
				// but also the best solution that combines drinks of types 0...j-1 (since we are at the drink j, so we can either
				// have j already combined in the solution for min_volume=(i-curr_volume) and add 1 bottle of it
				// or not have drink j (i,e have 0...j-1) and add 1 bottle of it)
				int missing_volume_cost = table[i-curr_volume][j].first; // best cost with j combined types
				int missing_volume_nb_types = table[i-curr_volume][j].second; // corresponding nb_types
				// at line min_volume=(i-curr_volume), adding drink j to drinks 0...j-1 would either keep the same cost or decrease it
				// thus we consider the column that has smallest cost (can only be at j)
				// and if they have the same cost, then we take the one that has more drink types used
				if(missing_volume_cost == table[i-curr_volume][j-1].first) { // same cost
					missing_volume_nb_types = std::max(missing_volume_nb_types, table[i-curr_volume][j-1].second +1 /* add the type j*/); 
				}
				if(curr_cost + missing_volume_cost < cost) { // the bottle's cost + missing_volume_cost beat the min_cost
					cost = curr_cost + missing_volume_cost; // put them as min_cost
					nb_types = missing_volume_nb_types; // put the correspoding nb_types
				} else if(curr_cost + missing_volume_cost == cost) { // if they have the same cost
					nb_types = std::max(nb_types, missing_volume_nb_types); // pick the one that maximizes the nb_types
				}
			} // else i,e curr_volume >= i but curr_cost >= cost
			  // no need to use the current drink type, since if we decide to add it, then 2 cases:
			  // curr_cost = cost, so we need to remove all previous bottles and add only the current, which doesn't decrease the cost 
			  // but decreases the nb_types to 1 (this is clearly not wanted) 
			  // or curr_cost > cost, so in this case we don't want to add it because it only worsens the cost
			  table[i][j] = std::make_pair(cost, nb_types);
			}
		}
		
		// table[k][n-1] contains the min_cost and corresponding nb_types to get at least "k" liters by combining drink types 0...(n-1)
		std::cout << table[k][n-1].first << " " << table[k][n-1].second << std::endl; 
}


int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
