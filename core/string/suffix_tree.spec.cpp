#include "suffix_tree.h"

#include <iostream>
#include <utility>
#include <vector>
#include <string>

using string_vec = std::vector<std::string>;
bool test(const string_vec &range, const std::string &expected) {
	string_vec rangePermutation = range;
	std::sort(rangePermutation.begin(),rangePermutation.end());
	bool bSuccess = true;
	// Use permutations to test the string array.
	// We keep the array unsorted, but a map MIGHT sort its entries by key.
	// (as in, I observed it was the case, but am not sure if it's for every compiler)
	// Although this ultimately shouldn't affect the result of the LCS determination,
	// it does affect the internal generation and representation of the suffix tree.
	do {
		const SuffixTree::Tree<string_vec> tree{rangePermutation};
		//tree.Visualize();
  		const std::string common = tree.LongestCommonSubstring();
  		if (common != expected) {
			std::cout << "==== In permutation: ";
			for (const std::string& str: rangePermutation) {
				std::cout << "'" << str << "', ";
			}
			std::cout << ":\n";
			std::cout << "Common string '" << common
  		            << "' does not meet expected string '" << expected << "'\n";
  		  bSuccess = false;
  		}
	}while (std::next_permutation(rangePermutation.begin(),rangePermutation.end()));
	return bSuccess;
}

//int main() {
//
//	using tree_test_map = std::vector<std::pair<string_vec, std::string>>;
//  const tree_test_map entries{
//      {{"banana"}, "ana"},
//      {{"xabcda", "y7abc1"}, "abc"},
//      {{"continue", "continuer", "continuare", "continuar"}, "continu"},
//      {{"Press {0} to continue", "Appuyez sur {0} pour continuer",
//        "Premi {0} per continuare", "Presiona {0} para continuar"},
//       " continu"},
//      {{"Press {0} to", "Appuyez sur {0} pour",
//        "Premi {0} per", "Presiona {0} para"},
//       " {0} "},
//      {{"e", "er",
//        "are", "ar"},
//       ""},
//      {{"Hello, my name is N00bFlesh",
//        "こんにちは、僕の名前はヌーブフレッシです"},
//       ""}};
//  int returnVal = 0;
//  for (const auto &entry : entries) {
//    if (!test(entry.first, entry.second)) {
//      returnVal = -1;
//    }
//  }
//  return returnVal;
//}
