#include <iostream>
#include <vector>
#include <algorithm>

struct Word {
	int lab; // label
	int pos; // position
	Word(int lab, int pos): lab(lab), pos(pos) {}
};

bool operator<(Word& w1, Word& w2) { // compare words according to the position
	return w1.pos < w2.pos;
}

void print_matrix(std::vector<std::vector<int>>& m);

void testcase() {
	int n; std::cin >> n; // n=#words, n>= 2
	std::vector<int> m(n); // number of occurence of each word
	std::vector<Word> words;
	
	for(int i=0; i<n; i++) {
		std::cin >> m[i];
	}
	for(int i=0; i<n; i++) {
		for(int j=0; j<m[i]; j++) {
			int pos; std::cin >> pos;
			words.push_back(Word(i, pos));
		}
	}
	
	std::sort(words.begin(), words.end());
	
	// sliding window on "words"
	int left = 0, right = 0; // left and right pointer of the window
	std::vector<int> words_count(n, 0); // for each word, stores its nb of occurences in the range [left, right]
	int words_found = 1; // nb of distinct words found
	int min_length = words[words.size()-1].pos - words[0].pos + 1; // length of the min range found, currently [first_pos, last_pos]
	words_count[words[right].lab] = 1;
	
	// keep moving the right pointer till you have all words in the window
	// then keep moving the left pointer as long as the window [left, right]
	// contains all words, then again move the right pointer ...
	while(right < words.size()) {
		if(words_found == n) { // the range [li, ri] contains all words, we now move the left pointer
			int length = words[right].pos - words[left].pos + 1; // compute its length and keep it if smaller
			if(length < min_length) {
				min_length = length;
			}
			// we are moving the left pointer so decrease the count of the word pointed by it
			if(--words_count[words[left].lab] == 0) { // no more instance of this word after moving the pointer
				words_found--; // decrease the number of words found in the new window
			}
			left++;
			
		} else { // not all words found, move right pointer
			right++;
			if(right == words.size()) { // we are past the last word
				break;
			}
			if(words_count[words[right].lab]++ == 0) { // check if the current value is 0, then increment
				words_found++; // word was not on the range [left, right], increment nb of words found	
			}
		}
	}

	std::cout << min_length << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	int t; std::cin >> t;
	while(t--) testcase();
	return 0;	
}
