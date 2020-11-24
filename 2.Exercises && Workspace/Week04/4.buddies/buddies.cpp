#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		boost::no_property,
		boost::no_property> Graph;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;


void testcase() {
	int n, c, f; // n = #students, c=#characteristics_per_student, f= nb characteritics common to matched students by Dr. Fuzzman
	// f is optimal iff we cannot find a matching for all students such that the matched pairs have more than f common characteristics
	// the idea is to try to model students as nodes, then try to find for each student S, the other students that has 
	// "more than f" common characteristics and mark them as matchable to S by adding an edge from S to those students.
	// In the resulting graph, we try to find a MCM (Maximum Cardinality Matching) and if we can match all students then f is not optimal
	// otherwise (i,e if at least 1 student unmatched) f is optimal
	
	std::cin >> n >> c >> f;
	
	Graph G(n);
	std::vector<std::set<std::string>> student_charac(n);
	
	for(int i=0; i<n; i++) {
		for(int j=0; j<c; j++) {
			std::string charac; std::cin >> charac;
			student_charac[i].insert(charac);
		}
	}
	
	for(int i=0; i<n; i++) { // find matching pairs 
		for(int j=i+1; j<n; j++) {
			std::vector<std::string> inter;
			std::set<std::string>& s1(student_charac[i]); std::set<std::string>& s2(student_charac[j]); // the 2 sets to intersect
			std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::back_inserter(inter));
			
			if(inter.size() > f) {
				boost::add_edge(i, j, G); // mark as matchable
			}
		}
	}
	
	//find matchings
	std::vector<Vertex> matemap(n);
	boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G)));
	int matchingsize = matching_size(G, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G))); // nb of pairs matched
	if(2*matchingsize == n) { // found matching for every vertex
		std::cout << "not optimal" << std::endl;
	} else {
		std::cout << "optimal" << std::endl;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin>>t;
	while(t--) testcase();
	return 0;
}
