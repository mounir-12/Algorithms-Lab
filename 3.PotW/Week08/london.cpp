#include <iostream>
#include <vector>
#include <string>
#include <map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

// BGL typedefs
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;

class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

void testcase() {
	// We have a string, call it note, of size n and a 2 grids (the front and back grid) of characters of h lines and w columns
	// The cell (i, j) in front grid corresponds to cells (i, w-1-j) in back grid. We merge those 2 grid into 1 grid T
	// such that the cell (i, j) of T contains a pair of characters (c1, c2) where c1 is the character in cell (i, j) of the front
	// grid and c2 is the character in cell (i, w-1-j) in back grid.
	// We would like to know if we can choose some cells in T, and for each cell choose one of (c1, c2) such that the chosen characters
	// can be used to construct the "note".
	// We are not interested in the order of the characters in the "note", we only need to know the count of each of its characters.
	// Since we need to perform a choice for each cell of T, i,e we need to match each cell to one of its characters (c1, c2), so this
	// looks like a bipartite matching problem where we have a graph G with source and target vertex, we also have 1 vertex per cell
	// and 1 vertex per char A...Z. Then we add an edge from the source to each cell with capacity 1, from each cell to the 2 chars
	// of the cell with capacity 1 and from each char c to the target with capacity "count(c) in note". Then run a flow algorithm
	// and check if all in-edges of the target are saturated (i,e flow == n).
	// This would work but is too expensive since we have h*w cells/vertices and 2 <= h, w <= 10^3, so h*w <= 10^6.
	// The idea here is to observe that the number of pairs of chars (c1, c2) possible is limited to 26*26 (since we have 26 letters). 
	// So we might have multiple vertices of different cells but that represent the same pair (c1, c2). So we merge those vertices 
	// into 1 vertex. For the edges, we could let them as is (i,e multiple parallel edge from source to vertex (c1, c2) all with 
	// capacity 1, same thing from (c1, c2) to vertex c1 and to vertex c2) or we can merge the parallel edges into 1 edge of 
	// capacity="sum of capacities of merged edges".
	// So the capacity of the edges source -> (c1, c2) and (c1, c2) -> c1 and (c1, c2) -> c2 would be "count of (c1, c2) in T"
	// Also, instead of creating a vertex for each char A...Z, we limit ourselves to the chars that occur in the "note"
	// This way, we have at most 26*26 + 26 + 2 vertices and can thus run push_relabel algorithm
	
	int h, w; std::cin >> h >> w; // h=#lines, w=#columns
	std::string note; std::cin >> note; // read the note
	int n = note.size(); // number of chars in note
	
	std::vector<std::vector<std::pair<char, char>>> grid(h, std::vector<std::pair<char, char>>(w)); // the grid T
	
	for(int i=0; i<h; i++) { // read the front grid
		for(int j=0; j<w; j++) {
			char c; std::cin >> c;
			grid[i][j].first = c;
		}
	}
	
	for(int i=0; i<h; i++) { // read the back grid (up to bottom and left to right)
		for(int j=w-1; j>=0; j--) { // index of the current "back" letter in the front grid
			char c; std::cin >> c;
			grid[i][j].second = c;
		}
	}
	
	std::map<std::pair<int, int>, int> map_pairs; // map each character pair in grid to its nb of occurences in grid
	for(int i=0; i<h; i++) {
		for(int j=0; j<w; j++) {
			map_pairs[grid[i][j]]++; // if grid[i][j] already in map, increment its count, else create a mapping to 0 and increment it
		}
	}
	
	std::map<char, int> map_chars; // map each character in note to its nb of occurences
	for(int i=0; i<n; i++) {
		char c = note[i];
		map_chars[c]++; // if c already in map, increment its count, otherwise create a mapping to 0 and increment it
	}
	
	int V1 = map_pairs.size();
	int V2 = map_chars.size();
	Graph G(V1+V2); // vertices 0...(V1-1) for character pairs (in the same order they occur in map_pairs)
					// vertices V1...(V1+V2-1) for note characters (in the same order they occur in map_chars)
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	//ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);
	
	Vertex source = boost::add_vertex(G);
	Vertex target = boost::add_vertex(G);
	
	std::map<char, int> char_to_vertex; // map each char in map_chars to the corresponding vertex
	int v = V1;
	for(auto it = map_chars.begin(); it != map_chars.end(); it++) { // iterate over map_chars
		char c = it->first; int count = it->second; // get the char and its count
		char_to_vertex[c] = v; // map the char to the vertex v
		eaG.addEdge(v, target, count); // add edge from v to target with capacity "count of c in note"
		v++; // move to the next vertex
	}
	v=0; // vertex index of the character pair
	for(auto it = map_pairs.begin(); it != map_pairs.end(); it++) { // iterate over the map_pairs
		char c1 = (it->first).first, c2 = (it->first).second; // get the pair (c1, c2)
		int count = it->second;
		if(char_to_vertex.find(c1) != char_to_vertex.end()) { // c1 is part of the note chars
			int u = char_to_vertex[c1];
			eaG.addEdge(v, u, count); // add edge from v (i,e pair (c1, c2)) to u (i,e char c1) with capacity "count of (c1, c2)"
		}
		if(char_to_vertex.find(c2) != char_to_vertex.end()) { // c2 is part of the note chars
			int u = char_to_vertex[c2];
			eaG.addEdge(v, u, count); // add edge from v (i,e pair (c1, c2)) to u (i,e char c2) with capacity "count of (c1, c2)"
		}
		eaG.addEdge(source, v, count); // add edge from source to v with capacity "count of (c1, c2)"
		v++; // move to the next vertex
	}
	
	long flow = boost::push_relabel_max_flow(G, source, target);
	
	if(flow == n) { // all in-edges of target saturated
		std::cout << "Yes" << std::endl;
	} else {
		std::cout << "No" << std::endl;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
