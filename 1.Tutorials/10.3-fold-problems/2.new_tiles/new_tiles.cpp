#include <iostream>
#include <vector>

std::vector<std::vector<int>> DP;
std::vector<int> grid;

// see explanations below (after main())

void print_bits(int l) {
	int nb_bits = sizeof(l)*8;
	for(int i=nb_bits-1; i>=0; i--) {
		int bit = 0;
		if(l & (1 << i)) bit = 1;
		std::cout << bit;
	}
}

int get_max_from_children(int i, int t, int w) {
	int max = 0;
	for(int j=0; j<w; j++) {
		if(t & (1<<j)) { // j'th bit is set
			int child = t ^ (1<<j); // unset the bit using a xor
			//std::cout << "child: "; print_bits(child);
			max = std::max(max, DP[i][child]);
		}
	}
	return max;
}

bool is_valid(int i, int t, int w) { // check if tiling t valid for line i of the grid
	bool correct = true; // to check if tiling is correct
	for(int j=0; j<w; j++) {
		if(t & (1<<j)) { // check if jth bit in t is set
			if(!(grid[i] & (1<<j))) return false; // if jth square of line i not free, then return false (tiling t not valid)
			correct = !correct; // becomes true whenever 2 successive 1-bits are found
		} else { // found a 0-bit, check if tiling up to j'th bit still correct, if not, return false
			if(!correct) return false;
		}
	} 
	return correct; // after loop, we know tiling t compatible with line i, return if correct or not
}

int count_ones(int t, int w) {
	int count = 0;
	for(int j=0; j<w; j++) {
		if(t & (1<<j)) { // check if jth bit in t is set
			count++;
		}
	}
	return count;
}

int fill_DP(int h, int w) {
	int nb_tilings = (1 << w);
	int mask = nb_tilings - 1; // mask that is all ones in the 1st w bits
	int max = 0; // max value for the current line, serves for the next line when t = 0 and is the returned value
	for(int i=0; i<h; i++) { // loop through grid lines
		for(int t= 0; t < nb_tilings; t++) { // try all possible tilings of this line i
			if(i == 0) DP[i][t] = 0;
			else {
				if(t == 0) DP[i][t] = max;
				else {
					int max_from_children = get_max_from_children(i, t, w);
					DP[i][t] = max_from_children;
					if(is_valid(i, t, w) and is_valid(i-1, t, w)) {
						int t_complement = ((~t) & mask); // take complement of t (only first w bits)
						int val = DP[i-1][t_complement] + count_ones(t, w)/2; // add new 2x2 rectangles
						DP[i][t] = std::max(DP[i][t], val);
					}
				}
			}
			// take max of the current line (which is at least the max of previous line) because of t=0 at current line
			max = std::max(max, DP[i][t]); 
		}
	}
	return max;
}

void testcase() {
	
	int h, w; std::cin >> h >> w;
	
	if(h-2 < 2 or w-2 < 2) { // grid (skipping the borders) too small
		for(int i=0; i<h*w; i++) { // read input and ignore it
			int ignore; std::cin >> ignore; 
		}
		std::cout << "0" << std::endl; // no 2x2 tile can fit
		return; // next testcase
	}
	
	grid = std::vector<int>(h-2); // create a new grid, skip border lines
	
	for(int i=0; i<h; i++) { // read input
		int line_bits = 0; // ith line bits
		for(int j=w-1; j>=0; j--) {
			int bit; std::cin >> bit;
			if(i>=1 and i<h-1 and j>=1 and j<w-1) { // store grid without borders
				line_bits = line_bits | (bit << (j-1)); // put bit in right position
			}
		}
		if(i>=1 and i<h-1) { // skip borders
			grid[i-1] = line_bits;
		}
	}
	/*for(int i=0; i<grid.size();i++) { // print grid
		int line = grid[i];
		std::cout << "line " << i << ": "; print_bits(line); std::cout << std::endl;
	}*/
	h -= 2; w -= 2; // skip borders
	
	int nb_tilings = (1 << w); // total number of tilings possible
	DP = std::vector<std::vector<int>>(h, std::vector<int>(nb_tilings, 0));
	std::cout << fill_DP(h, w) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}

/*
   we have a grid of h lines, each line has w squares, each square is either free or not
   we represent each grid line with w bits (where the jth bit is 1 if the jth square of the line is free, and 0 otherwise)
   we also know the borders of the grid (i,e line 0, line h-1, column 0 and column w-1) are all 0 (not free), so let us skip
   the borders and only consider the inside of the grid (so now h=h-2 and w=w-2 and grid = grid_skip_borders)
   we want to maximize the number of non-overlapping 2x2 tiles we can put in our grid (where we can put a 1x1 tile in a free square only)
   The first obvious case is (h < 2 or w < 2), the solution here is 0 (grid too small)
   So suppose from here h>=2 and w>=2
   The optimal solution will have for each grid line a tiling t (where t contains w bits, the jth bit is 1 if we put a tile in jth position,
   0 otherwise), to find this solution:
   we keep a table DP, where DP[i][t] = max nb of 2x2 tiles we can put in (sub-)grid from line 0 to line i, where line i is tiled using t
   where t goes from 000...000 (i,e no tile put on the grid line) to 1111...111 (a tile put on every position of the line).
   To fill this table, we do the following: for each line i=0...h-1, we check all tilings t=000...00 to 111...111 of line i: 
   if i=0: can't tile here, so DP[i][t] = 0
   else if (i>=1): the sub-grid is large enough to be tiles with 2x2 tiles, so:
   if t=000...000 (i,e we do not add any 1x1 tile to the current line), then line i doesn't have a contribution
   		so we take the "max nb of 2x2 tiles we can put in (sub-)grid from line 0 to line i-1" i,e max(DP[i-1][t]) for all t
   if t != 000...000, then we check that t is a valid tiling, i,e we check two conditions:
   		1-The tiling t must compatible with the grid line i (i,e we only put a 1x1 tile in a free square of line i, more formally
			at each position j, if line_i[j] = 0 then t[j] = 0, if line_i[j] = 1 then t[j] = 1 or 0)
  		2-The tiling t is correct (a correct tiling each tile can be matched to one and only one adjacent tile since we want to add 2x2 tiles)
   The 2nd condition implies that the number of added tiles (i,e number of 1-bits in t) is even
   If none of the conditions are met, then we need to unset some bits in t to make it a valid tiling (by unsetting some bit in t, we get a
   new tiling t' that is compatible with t "compatibility as defined in condition 1"). So let T = {t'| t' is valid and compatible with t}
   T is non-empty, it contains at least the value 000...000.
   So if none of the conditions is met, then DP[i][t] = max(DP[i][t']) for all t' in T
   We are guaranteed that DP[i][t'] was computed since for all t' in T, t' < t
   To compute T, we could compute the tree of children of t (i,e values t' compatible with t), where t is the root, 
   and each child unsets 1 set bit of the parent, e,g:
   t = 1101, the tree is:
                ----------------------------t=1101----------------------------	(root has 3 set bits, so has 3 children, each one unsets 1 position)
                |                              |                             |
        ------0101-------             --------1001-----            --------1100-----	(same at this level)
        |                |            |               |            |               |
        0001            0100         0001            1000        0100             1000
         |               |            |                |           |               |
        0000            0000         0000             0000       0000            0000	(last level consists of only 0)
   then take only "valid nodes" and these will be in the set T.
   It is too expensive to build the whole tree for every t, so since we are only interested in DP[i][t] = max(DP[i][t']) for all t' in T
   and t goes from 000...001 to 1111..111, we could go down only 1 level from t (i,e to the direct children of t)
   and take max(DP[i][t']) "for all t' direct child of t". This works because for each node n below t, we save the max over the sub-tree
   rooted in "n" at DP[i][n].
   So if some condition is unmet, then DP[i][t] = max(DP[i][t']) "for all t' direct child of t"
   This effectively just takes the best previous tiling found for line i
   if both conditions are met, then we should check if t_complement="all bits of t flipped" is a valid tiling of line i-1
   so that we can tile line i and line i-1 with tiling t. If this is possible then "DP[i][t] = DP[i][t_complement] + nb_set_bits(t) / 2"
   (i,e we add nb_set_bits(t) / 2 tiles of size 2x2 to line i and i-1)
   but since doing (i,e adding t to lines i and i-1) this could be less optimal than not doing it (i,e taking one of the previously found
   tiling of line i), then DP[i][t] = max( val1, val2) where
   val1 = "max(DP[i][t']) for all t' direct child of t" and val2 = "DP[i][t_complement] + nb_set_bits(t) / 2"
*/
