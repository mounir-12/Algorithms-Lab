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
	// by combining any amount (0 or more) of each drink type 0...j
	// nb_types is the corresponding number of drink types combined  
	VVPII table(k+1, VPII(n));
	VVPII table2(k+1, VPII(n)); // to record solution
	
	int nb_bottles = 0; // nb_botlles of drink of type 0
	PII used_cell;
	for(int i=0; i<=k; i++) {
		// i is the minimum volume in liters we want using nb_bottles of drinks[0]
		if(nb_bottles * drinks[0].volume >= i) {
			int total_cost = nb_bottles*drinks[0].cost;
			table[i][0] = std::make_pair(total_cost, 1);
			table2[i][0] = used_cell; // same as beforecell used, no change in nb_bottles used
		} else { // not enough bottles to reach i liters at least, we add 1 bottle 
		 // this works because between last and current iteration, i grown by 1 liter
		 // so adding 1 bottle is sufficient since each bottle is at least 1 liter in volume
		 nb_bottles++;
		 int total_cost = nb_bottles*drinks[0].cost;
		 table[i][0] = std::make_pair(total_cost, 1);
		 if(i == 1) { // any drink type is sufficient to get at least i = 1 liter, so for for i=0 and all j, we only use 1 bottle of drink j, so each cell uses itself to get to the minimum of i=1 liters 
			 table2[i][0] = std::make_pair(i, 0);
		 } else {			
			table2[i][0] = used_cell; // same cell
		 }
		 used_cell = std::make_pair(i, 0); // current cell is to be used
		}
	}
	
	for(int i=1; i<=k; i++) { // ignore the line with volume i=0, must be all pairs of zeros
		for(int j=1; j<n; j++) { // loop over the other drink types 1 ... n-1
			int cost = table[i][j-1].first; // get the min cost acheived by a combination of drink types 0...j-1 with the current volume i
			int nb_types = table[i][j-1].second; // likewise for the types of drinks, note: types <= j-1 since only combined drinks of types 0...j-1 
			used_cell = std::make_pair(i,j-1);
			// these are the default values for this cell, we try to improve the cost (i,e lower)
			// and update the nb_types if improvement of cost possible
			 
			int curr_cost = drinks[j].cost; // cost of 1 bottle of the current drink type
			int curr_volume = drinks[j].volume; // volume of 1 bottle of  the current drink type
			if(curr_cost < cost && curr_volume >= i) { // it is sufficient to take 1 bottle of the current drink type
				cost = curr_cost; nb_types = 1;
				used_cell = std::make_pair(i,j); // the cell itself gives us is the 1st chosen drink type
			} else if(curr_volume < i) { // 1 bottle of current drink type not sufficient
				// we still need (i-curr_volume) to reach the threshold volume i, we check the previous
				// best solutions found for min_volume=(i-curr_volume) at the same column j i,e by combining  drink types 0...j
				// but also the best solution that combines drinks of types 0...j-1 (since we are at the drink j, so we can either
				// have j already combined in the solution for min_volume=(i-curr_volume) and add 1 bottle of it
				// or not have drink j (i,e have 0...j-1) and add 1 bottle of it
				int missing_volume_cost = table[i-curr_volume][j].first; // best cost with j combined
				int missing_volume_nb_types = table[i-curr_volume][j].second; // corresponding nb_types
				PII missing_volume_cell = std::make_pair(i-curr_volume, j);
				// at min_volume=(i-curr_volume), combining j with 0...j-1 would either keep the same cost of decrease it
				// thus we consider the one that has smallest cost (can only be at j)
				// and if they have the same cost, then we take the one that maximizes its nb_types of drinks used
				if(missing_volume_cost == table[i-curr_volume][j-1].first) { // same cost
					if(missing_volume_nb_types < table[i-curr_volume][j-1].second +1 /* add the type j*/) {
						missing_volume_nb_types = table[i-curr_volume][j-1].second+1;
						missing_volume_cell = std::make_pair(i-curr_volume, j-1);
					}
				}
				if(curr_cost + missing_volume_cost < cost) { // the bottle's cost + missing_volume_cost beat the min_cost
					cost = curr_cost + missing_volume_cost; // put them as min_cost
					nb_types = missing_volume_nb_types; // put the correspoding nb_types
					used_cell = missing_volume_cell;
				} else if(curr_cost + missing_volume_cost == cost) { // if they have the same cost
					if(nb_types < missing_volume_nb_types) { // pick the one that maximizes the nb_types
						nb_types = missing_volume_nb_types;
						used_cell = missing_volume_cell;
					}
				}
			} // else i,e curr_volume >= i but curr_cost >= cost
			  // no need to use the current drink type, since if we decide to add it, then 2 cases:
			  // curr_cost = cost, so we need to remove all previous bottles and add only the current, which doesn't decrease the cost but decrease the nb_types to 1 (not wanted)
			  // or curr_cost > cost, so in this case we don't want to add it because it only worsens the cost
			  table[i][j] = std::make_pair(cost, nb_types);
			  table2[i][j] = used_cell;
				
			}
			
		}
		
		// table[k][n-1] should contains the min_cost and corresponding nb_types to get at least "k" liters
		std::cout << table[k][n-1].first << " " << table[k][n-1].second << std::endl;
		
		// uncomment below block of code to get drink types used, nb of bottles used of each drink type
		
		/*std::cout << "We are given " << n << " types of drinks. The target volume is " << k << " liters." << std::endl;
		std::cout << "Solution found has min_cost: " << table[k][n-1].first << " using " << table[k][n-1].second << " type(s) of drinks"<< std::endl;
		VI amounts(n);
		PII curr_cell = std::make_pair(k, n-1);
		do {
			used_cell = table2[curr_cell.first][curr_cell.second]; // cell used by curr_cell
			if(used_cell == curr_cell) { // used only current cell
				amounts[curr_cell.second]++; // i,e we took a bottle of drink type "curr_cell.second"
				break; // found cell that used itself, break
			} else if(used_cell.first < curr_cell.first) { // we used a cell from a previous row
				amounts[curr_cell.second]++; // i,e we took a bottle of drink type "curr_cell.second"
			}
			curr_cell = used_cell; // move to the used_cell
			
			
		}while(true);
		
		std::cout << "Drinks used:\n";
		for(int j=0; j<n; j++) {
			int amount = amounts[j];
			if(amount == 0) continue;
			std::cout << "	drink of type " << j << ": " << amount << " bottle(s), costs: " << drinks[j].cost << " per bottle of " << drinks[j].volume << " liters" << "\n";
		}
		std::cout << "\n" << std::endl;
		*/
}


int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
