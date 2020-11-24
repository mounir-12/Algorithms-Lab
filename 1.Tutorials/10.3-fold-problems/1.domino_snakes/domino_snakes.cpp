#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

// BGL typedefs
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;	
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;

struct pos {
	int y;
	int x;
	pos(int y, int x): y(y-1), x(x-1) {} // store in 0 based index
};

int flattened_index(int i, int j, int w) {
	return i*w + j;
}

int parity(pos p) {
	return (p.x + p.y)%2;
}

void testcase() {
	// we have a grid of size h*w, so h*w squares of size 1 by 1
	// the dominos are of size 2 by 1 and we want to put them in way to find a path from entrance
	// to exit. Instead of adding dominos (which cover 2 squares) 1 at a time, we add half dominos, so 
	// for each domino we want to add, we add it's 1st half, then adjacent to it, we add its 2nd half (in any
	// available free adjacent position), this allows us to put a domino is any orientation available.
	// We do the same for the next domino (i,e add its 1st half adjacent to the previous domino's 2nd half)
	// This ensures to respect the constraint imposed: "the first square of every domino needs to touch
	// the second square of its preceeding domino"
	// If we consider each square a vertex, the problem is then reduced to finding if there is a path from
	// entrance square to exit square (path along which we add our half dominos). This path must contains an 
	// even number of squares/vertices, so that we don't add a half-domino alone (must add full dominos)
	// Now, let us create a new grid of same size, with value 0 in the upper-left corner, move right->write 1, move
	// right->write 0, move right->write 1 ... We keep filling the grid left to right, then 
	// go down 1 square then right to left, then go down 1 square, etc ... for each movement we change
	// the bit written from 0 to 1 and from 1 to 0. We can do the same by computing for each position (x,y)
	// the value v written at that position v=(x+y)%2 which is the parity of that position.
	// The idea here is to see that: if the 1st half of the 1st added domino is put in a position (x,y) with
	// "parity 0", then all next "1st halfs" will be at positions with "parity 0", and all next "2nd halfs" will 
	// be at positions with "parity 1". Same goes if 1st half of 1st added domino is at position with parity 1.
	// This means that the parity of the 1st halfs is always different of the parity of the 2nd halfs.
	// Since the entrance will lie on a 1st half of a domino and the exit will lie on a 2nd half of a domino, then
	// we can say there is no path from entrance to exit if the entrance parity is the same as the exit parity.
	// So we first check if the entrance and exit parities are different, if so, then if they are connected, then
	// there is a "dominos path" between them, otherwise, no path available.
	// To check they are connected, we can use BFS to find if there is path for each entrance-exit pair, or better, 
	// we run connected_components once then can answer in O(1) the queries of type "is position p1 and p2 connected"
	
	int h, w, p; std::cin >> h >> w >> p;
	Graph G(h*w); // 1 vertex per square
	
	std::vector<bool> can_be_tiled(h*w); // denote for each sqaure if it can be tiled of not
	std::vector<std::pair<pos, pos>> pairs; // entrance-exit pairs
	for(int i=0; i<p; i++) { // read entrance-exit pairs
		int q, r, s, t; std::cin >> q >> r >> s >> t;
		pairs.push_back(std::make_pair(pos(q,r), pos(s,t)));
	}
	for(int i=0; i<h; i++) { // read input
		for(int j=0; j<w; j++) {
			char c; std::cin >> c; // read character c='0' or c='1'
			can_be_tiled[flattened_index(i, j, w)] = (c == '1'); // can be tiled only if read character is 1
		}
	}
	for(int i=0; i<h; i++) { // build graph
		for(int j=0; j<w; j++) {
			int curr = flattened_index(i, j, w); // index of the current square in the flattened grid
			if(!can_be_tiled[curr]) continue; // curr pos is a snake cage, continue
			int n; // index of the neighbor square in the flattened grid
			if(j+1 < w) { // has a right neighbor
				n = flattened_index(i, j+1, w);
				if(can_be_tiled[n]) {
					boost::add_edge(curr, n, G);
				}
			}
			if(j > 0) { // has a left neighbor
				n = flattened_index(i, j-1, w);
				if(can_be_tiled[n]) { // neighbor is not a snake cage
					boost::add_edge(curr, n, G);
				}
			}
			if(i > 0) { // has an up neighbor
				n = flattened_index(i-1, j, w);
				if(can_be_tiled[n]) { // neighbor is not a snake cage
					boost::add_edge(curr, n, G);
				}
			}
			if(i+1 < h) { // has a down neighbor
				n = flattened_index(i+1, j, w);
				if(can_be_tiled[n]) { // neighbor is not a snake cage
					boost::add_edge(curr, n, G);
				}
			}
		}
	}
	std::vector<int> componentmap(h*w);
	boost::connected_components(G, boost::make_iterator_property_map(componentmap.begin(), boost::get(boost::vertex_index, G)));
	for(int i=0; i<p; i++) {
		pos entrance = pairs[i].first;
		pos exit = pairs[i].second;
		int entrance_component = componentmap[flattened_index(entrance.y, entrance.x, w)];
		int exit_component = componentmap[flattened_index(exit.y, exit.x, w)];
		if(parity(entrance) != parity(exit) and entrance_component == exit_component) {
			std::cout << "y";
		} else {
			std::cout << "n";
		}
	}
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
