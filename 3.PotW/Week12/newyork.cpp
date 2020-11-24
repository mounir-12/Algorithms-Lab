#include <iostream>
#include <vector>
#include <set>

struct Node {
	int v; // vertex index
	int temp; // vertex temperature
	Node(int v, int temp): v(v), temp(temp) {}
};

typedef std::vector<Node> VN;
typedef std::set<Node> SN;
typedef std::vector<VN> VVN;

bool operator<(Node n1, Node n2) { // compare nodes using temprature at node
	if(n1.temp == n2.temp) {
		return n1.v < n2.v; 
	} else {
		return n1.temp < n2.temp;
	}
}

// 2 solutions implemented: recursive and non-recursive
// 1- The recursive solution works on judge but may give seg fault on laptop/desktop. This seg fault is because 
// 		the program hits the stack limit because it can have multiple levels of recursion with large n
// 		To increase this limit in Linux, first run: "ulimit -s <amount_in_kb>" (the default is 8MB)
// 		then run this code. To check the current limit (in kb), run: "ulimit -s"
// 		I tried just doubling the stack size: 8MB -> 16MB and it worked, so run: "ulimit -s 16384"
// 2- The non-recursive solution worked on my laptop (even when I halfed the stack size 8MB -> 4MB)

void traverse_G_non_recursive(VVN& G, Node& new_york, std::set<int>& sol, int m, int k) {
	// non-recursive dfs on a tree rooted at 0 (new_york)
	std::vector<Node> path; // the curr found path
	std::set<Node> sorted_sub_path; // the sub-path is composed of the last m nodes in the path, we sort these nodes by temperature
	
	VN lifo; // Last-In First-Out
	lifo.push_back(new_york);
	int l = 0; // path size
	while(!lifo.empty()) {
		Node curr_node = lifo.back(); lifo.pop_back(); // get the curr_node of the traversal
		// we only consider the sub-path of the m last nodes in the current path
		if(curr_node.v == -1) { // finished traversal of last node		
			if(l>=m) { // path has length >= m then put back the removed node (i,e the first node of the curr sub path)
				Node first = path[l-m]; // first node in the sub-path
				sorted_sub_path.insert(first);
			}
			// update path
			Node last = path.back();
			path.pop_back(); l--; // remove last node in the path
			sorted_sub_path.erase(last); // remove it from sorted_sub_path too
			continue;
		}
		// otherwise, add child
		path.push_back(curr_node); l++; // add curr_node as the last node of the path
		sorted_sub_path.insert(curr_node); // add it to the set of sorted nodes of the current sub_path

		Node first = path[std::max(0, l-m)]; // first node in the sub-path
		if(l >= m) { // sub_path of length m
			Node min = *sorted_sub_path.begin();
			Node max = *sorted_sub_path.rbegin();
			int risk = max.temp - min.temp;
			if(risk <= k) { // check if this subpath is a "feasible trip" from the node "first"
				sol.insert(first.v);
			}
		}
		// continue traversal
		if(l>= m) { // the subpath already contains m nodes, we remove the first node to make room for the child node
			sorted_sub_path.erase(first);
		}
		
		lifo.push_back(Node(-1, 0)); // push_back dummy node (delimiter) to split the children of curr_node from other nodes
									// allows us to know when we finished dfs traversal of curr_node to pop it from "path"
		for(Node child: G[curr_node.v]) {
			lifo.push_back(child);
		}		
	}	
}

void traverse_G_recursive(VVN& G, VN& path, SN& sorted_sub_path, std::set<int>& sol, int& m, int& k) {
	// we only consider the sub-path of the m last nodes in the current path
	int l = path.size();
	Node first = path[0]; // first node in the sub-path
	Node last = path[l-1]; // last node in the sub-path
	if(l >= m) { // path of length >= m
		first = path[l-m]; // correct first node of the sub-path
		Node min = *sorted_sub_path.begin();
		Node max = *sorted_sub_path.rbegin();
		int risk = max.temp - min.temp;
		if(risk <= k) { // check if this subpath is a "feasible trip" from the node "first"
			sol.insert(first.v);
		}
	}
	// continue traversal
	if(l>=m) { // the subpath already contains m nodes, so we remove the first node to make room for the child node
		sorted_sub_path.erase(first);
	}
	
	for(Node child: G[last.v]) { // move to the next child
		path.push_back(child);
		sorted_sub_path.insert(child);
		traverse_G_recursive(G, path, sorted_sub_path, sol, m, k);
		sorted_sub_path.erase(child); // remove added child
		path.pop_back();
	}
	
	if(l>=m) { // we removed the first node, so we put it back before returning
		sorted_sub_path.insert(first);
	}
	
}

void traverse_G(VVN& G, Node& new_york, std::set<int>& sol, int m, int k, bool recursive) {	
	if(not recursive) {
		traverse_G_non_recursive(G, new_york, sol, m, k);
	} else {
		std::vector<Node> path;
		std::set<Node> sorted_sub_path; // the sub-path is composed of the last m nodes in the path, we sort these nodes by temperature
		path.push_back(new_york); // path starts from node 0
		sorted_sub_path.insert(new_york); // we only have 1 node in our current path
		traverse_G_recursive(G, path, sorted_sub_path, sol, m, k);
	}
}

void testcase() {
	// We have nodes (aka critical points) and edges (aka routes). A path between 2 nodes is called a trip
	// There exists exactly 1 trip from node 0 (new york) to each other node. So we have exactly 1 path from
	// node 0 to each other node. Thus the graph we have is a directed Spanning Tree rooted at node 0.
	// Each node i has a temperature temp_i.
	// We want to find for each node i if there exists a path starting at i of length m edges where
	// the max temp in path - min temp in path <= k.
	// We do this by performing a dfs traversal of the Tree starting from node 0. During the traversal, we save
	// the current path p found from node 0 to the current node. But we limit our view of the path
	// to the last m nodes. So we have a subwindow of the path of size m that contains the last m nodes found of path p.
	// So path p starts at node 0, we keep adding nodes while going deep in the tree. Once we reach a leaf, then we
	// remove the leaf (i,e we return to the parent) in order to try another path from there (i,e dfs). Thus nodes keep
	// being added and removed. The subpath is just a subwindow of p (the last m nodes), so as long as p has length < m 
	// then this subwindow is not full. Once p has exacly m nodes, then our subwindow is full. If this subwindow is full and 
	// we add a new node to p, then this node must be part of the new subwindow, thus we remove the head of the current
	// subwindow (i,e the first node). When we remove a node from p (which happens when we reach a leaf) then we need to update 
	// the subwindow accordingly.
	// Let "First" be the first node in the current subwindow and "Last" be the last node of this subwindow. Then if the
	// subwindow is full then we found a path from "First" that has m nodes, we only need to find the min and max temperature
	// in this subwindow to check if we found a "feasible trip"
	// We keep a copy of the nodes of the current subwindow in a sorted set. This will allow us to find the min and max 
	// temperature of node of this subwindow efficiently in O(1). This dynamic set changes as the subwindow changes i,e
	// nodes added to the subwindow are added to this set, nodes removed from the subwindow are removed from this set.
	
	int n, m, k; std::cin >> n >> m >> k; // n=#critical_points, m=#trip_length, k=#max_risk
	
	std::vector<int> temps;
	for(int i=0; i<n; i++) {
		int temp; std::cin >> temp;
		temps.push_back(temp);
	}
	
	std::vector<std::vector<Node>> G(n); // graph adjacency list
	for(int i=0; i<n-1; i++) {
		int u, v; std::cin >> u >> v;
		G[u].push_back(Node(v, temps[v]));
	}
	if(m > n-1) { // the goal path length larger than the number of edges in the tree
		std::cout << "Abort mission" << std::endl;
		return;
	}
	
	Node new_york = Node(0, temps[0]); // node 0 (i,e new york)
	std::set<int> sol; // the set of nodes that have a feasible trip from them
	bool recursive = false; // choose recursion or no recursion
	traverse_G(G, new_york, sol, m, k, recursive); // start traversal
	
	if(sol.empty()) {
		std::cout << "Abort mission" << std::endl;
	} else {
		for(int v: sol) {
			std::cout << v << " ";
		}
		std::cout << std::endl;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;
}
