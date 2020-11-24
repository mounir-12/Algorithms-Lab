#include <iostream>
#include <vector>

// check: https://stackoverflow.com/questions/5278580/non-recursive-depth-first-search-algorithm
void dfs(std::vector<std::vector<int>> matrix) {
	std::vector<int> path;
	
	std::vector<int> lifo; // may use a std::stack<int> as well (just need a LIFO data structure)
	lifo.push_back(0);
	while(!lifo.empty()) {
		int curr_node = lifo.back();
		lifo.pop_back();
		if(curr_node == -1) {
			path.pop_back();
			continue;
		}
		path.push_back(curr_node);
		lifo.push_back(-1); // push_back dummy node (delimiter) to split the children of curr_node from other nodes
							// allows us to know when we finished dfs traversal of curr_node to pop it from "path"
		for(int i=0; i < matrix[curr_node].size(); i++) {
			if(matrix[curr_node][i]) lifo.push_back(i); // push all children
		}
		std::cout << "curr path: ";
		for(int i=0; i<path.size(); i++) {
			std::cout << path[i] << (i == path.size()-1? "" : "->");
		}
		std::cout << std::endl;
	}
}

int main() {
	int V = 6; // nb of nodes
	std::vector<std::vector<int>> matrix(V, std::vector<int>(V, 0));
	matrix[0][1] = 1; matrix[0][2] = 1;
	matrix[1][3] = 1; matrix[2][4] = 1;
	matrix[2][5] = 1;
	dfs(matrix);
	return 0;
}
