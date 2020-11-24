#include <iostream>
#include <vector>
#include <algorithm>

int binary_search(std::vector<int>& arr, int v) {
	int left =0; int right = arr.size(); // pointers on the subarray considered right index excluded
	while(left != right) { // equivalently this means left < right i,e left ≤ right-1
		// let subarray = arr[left...(right-1)], index “right-1” included
		// we know subarray.size() > 0 since left ≤ right-1
		int mid = (left+right)/2; 	// middle element of subarray, mid < right 
						// if subarray.size() = 1 then mid == left, else mid != left
		if(v <= arr[mid]) { // the value searched for is on the left half
			right = mid; // consider the left half of subarray in the next iteration
		}else { // the value searched for is on the right half
			left = mid + 1; // consider the left half of subarray in the next iteration
		}
	}
	return right;
}

int main() {
	std::vector<int> arr = {100, 50, 6, 20};
	std::sort(arr.begin(), arr.end());
	for(int v: arr) {
		std::cout << v << " ";
	}
	std::cout << std::endl;
	int v = 200;
	int index = binary_search(arr, v);
	std::cout << "array size: " << arr.size() << std::endl;
	std::cout << "found val: " << v << " at " << index << std::endl;
	return 0;
}
