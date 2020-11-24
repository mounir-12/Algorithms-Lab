#include <iostream>
#include <vector>
#include <map>
#include <string>
#define FOR(i, n) for(int i=0; i<(n); i++)
#define Vector(i) std::vector<i>

struct Query {
	int query_nb; // serves to know the index where to store the query response in the answers array
	int max_age;
	Query(int query_nb, int max_age): query_nb(query_nb), max_age(max_age) {} 
};

int binary_search(Query& query, Vector(int)& path, Vector(int)& ages) { // answer query using found path and ages of the nodes in path
	int max_age = query.max_age;
	int left = 0, right = path.size(); // left and right pointers
	while(left != right) {
		int mid = (left+right)/2; // node in the middle of the path
		if(ages[path[mid]] <= max_age) { // age of the mid node <= max_age, move right pointer
			right = mid;
		} else { // age of the mid node > max_age, move left pointer
			left = mid+1;
		}
	}
	return path[right]; // the left pointer contains the index of the node with largest age <= max_age
}

// the vector path contains the path found from root to u (included)
// ages argument not used here, used in binary_search
void dfs(int u, Vector(Vector(int))& children, Vector(int)& path, Vector(int)& ages, Vector(Vector(Query))& queries, Vector(int)& answers) {
	for(Query q: queries[u]) { // found path from root to u, then answer all queries of node u
		answers[q.query_nb] = binary_search(q, path, ages);
	}
	
	for(int v: children[u]) { // continue dfs by moving to each the children of node u
		path.push_back(v); // move to v
		dfs(v, children, path, ages, queries, answers);
		path.pop_back(); // roll path back to u
	}							
}

void testcase() {
	// We have a tree of species rooted at LUCA. We know that for each specie s there exits a unique path from the root r to s
	// for each s, we have that age[s] >= age[child] where "child" is a direct child of s (so the root r is the oldest) 
	// the queries ask for the oldest ancestor of a specie s not older than a certain age b
	// So we know that for a certain specie s, the answer to its queries is some ancestor that lies in the path from r to s
	// We can thus build the path from r to s (using dfs) and once s reached answer all queries related to s
	// the path would be like: r->ancestor1->ancestor2->...->s and the ages: age[r]->age[ancestor1]->age[ancestor2]->...->age[s]
	// are sorted in decreasing order. So to we can use binary search on theses ages to find the one having the largest age <= b
	// By using DFS, we effectively find the path from root to every specie s which we use to answer queries about the specie s (if any)
	
	int n, q; std::cin >> n >> q; // n=#species, q=#queries
	
	std::map<std::string, int> specie_to_index; // map each specie name to a node index
	std::vector<int> ages(n); // species ages
	std::vector<std::string> names(n); // species names, used at the end for output
	FOR(i, n) {
		std::string name; int age; std::cin >> name >> age;
		specie_to_index[name] = i;
		ages[i] = age;
		names[i] = name;
	}
	
	std::vector<std::vector<int>> children(n); // store at index i the children of specie i
	std::vector<bool> has_parent(n, false); // used to find root, root is node that has no parent
	int root_index;
	FOR(i, n-1) { // read children of each node
		std::string parent, child; std::cin >> child >> parent;
		int parent_index = specie_to_index[parent];
		int child_index = specie_to_index[child];
		children[parent_index].push_back(child_index); // add child
		has_parent[child_index] = true;
	}
	FOR(i, n) {
		if(!has_parent[i]) { // found node that has no parent, i,e root
			root_index = i;
			break;
		}
	}
	
	std::vector<std::vector<Query>> queries(n); // store at index i the array of queries for specie i
	FOR(i, q) { // read queries
		std::string specie; int max_age; std::cin >> specie >> max_age;
		int j = specie_to_index[specie]; // specie_index
		queries[j].push_back(Query(i, max_age)); // add query to list of queries of specie j
	}
	
	std::vector<int> answers(q); // store at index i the answer of query i
	// perform dfs starting from root, answering queries related to node i as the path from root to node i gets build
	std::vector<int> path; path.push_back(root_index);
	dfs(root_index, children, path, ages, queries, answers);
	FOR(i, answers.size()) {
		std::cout << names[answers[i]] << (i < answers.size()-1 ? " " : ""); // print the name of the node that is the answer to the query
	}
	std::cout << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
