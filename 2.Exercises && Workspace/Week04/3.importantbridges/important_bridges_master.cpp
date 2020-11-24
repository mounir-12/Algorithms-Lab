// STL includes
#include <iostream>
#include <vector>
#include <algorithm> // sort, min, max
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>
// Define custom interior edge property // Option 2: use custom property
// enum edge_component_t { edge_component };
// namespace boost {
//	BOOST_INSTALL_PROPERTY(edge, component);
// }
// BGL typedefs
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		boost::no_property, // no vertex properties
		boost::property<boost::edge_name_t, int> // Option 1: (ab)use edge_name_t
		/* boost::property<boost::edge_component_t, int> // Opt.2: custom property */
		/* boost::property<boost::edge_index_t, std::size_t> // Opt.3: exterior property */> Graph;
typedef boost::property_map<Graph, boost::edge_name_t>::type ComponentMap; //Opt. 1
// typedef boost::property_map<Graph, edge_component_t>::type ComponentMap; // Opt.2
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

// Function for solving a single testcase
void testcases() {
	// Read Graph
	int V, E; std::cin >> V >> E;
	// islands and bridges
	Graph G(V);
	for (int i = 0; i < E; ++i) {
		int u, v; std::cin >> u >> v;
		Edge e; bool success;
		boost::tie(e, success) = boost::add_edge(u, v, G); // Options 1 & 2
		// boost::tie(e, success) = boost::add_edge(u, v, i, G); // Option 3
	}
	// Calculate biconnected components
	ComponentMap componentmap = boost::get(boost::edge_name, G); // Option 1
	// ComponentMap componentmap = get(edge_component, G); // Option 2
	// std::vector<std::size_t> edge_component(E); // Option 3
	// auto componentmap = boost::make_iterator_property_map(edge_component.begin(), get(boost::edge_index, G)); // Option 3
	int numbc = boost::biconnected_components(G, componentmap);

	// Iterate over all edges; count number of edges in each component.
	std::vector<int> componentsize(numbc);
	EdgeIt ebeg, eend;
	for (tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
		componentsize[componentmap[*ebeg]]++;
	}
	
	// Solution vector to record bridges
	std::vector<std::pair<int,int> > bridges;
	for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
		if (componentsize[componentmap[*ebeg]] == 1) { // If edge in a
			int u = source(*ebeg, G); // component of size = 1
			int v = target(*ebeg, G);
			bridges.push_back(std::make_pair(std::min(u,v), std::max(u,v)));
		}
	}
	std::sort(bridges.begin(), bridges.end());
	
	// Output
	std::cout << bridges.size() << std::endl;
	for (size_t i = 0; i < bridges.size(); ++i) {
		std::cout << bridges[i].first << " " << bridges[i].second << std::endl;
	}
}

// Main function, looping over the testcases
int main() {
	std::ios_base::sync_with_stdio(false);
	
	int T; std::cin >> T;
	for ( ; T > 0; --T) testcases();
	return 0;
}
