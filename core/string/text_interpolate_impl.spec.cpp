#include "text_interpolate_impl.h"

#include <algorithm>
#include <unordered_map>
#include <string>
#include <iostream>

using string_vec = std::vector<std::string>;
using weight_map = std::vector<std::pair<std::string, float>>;
bool test_tree(const string_vec &range, const std::string &expected) {
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


bool test_weight(const weight_map& range, const std::string &expected){
	const std::string output = interpolate(range);
	std::cout << "Output for " << range.cbegin()->first << " = " << output << "\n";
	if(output != expected){
		std::cout << output << " does not meet expected string " << expected << "\n";
		return false;
	}
	return true;
}
//int main(){
//	using tree_test_map = std::vector<std::pair<string_vec, std::string>>;
//	const tree_test_map entries{
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
//
//	int result = 0;
//	for (const auto& pair : entries) {
//		if(!test_tree(pair.first, pair.second)){
//			result |= 1;
//		}
//	}
//
//	using weight_test_map = std::vector<std::pair<weight_map, std::string>>;
//
//	const weight_test_map weight_tests{
//		{
//			{
//				{"xabcda", 0.5f},
//				{"y7abc1", 0.5f},
//			},
//			"abc"
//		},
//
//		{
//			{
//				{"Press {0} to continue", 0.25f},
//				{"Appuyez sur {0} pour continuer", 0.25f},
//				{"Premi {0} per continuare", 0.25f},
//				{"Presiona {0} para continuar", 0.25f},
//			},
//			"continu"
//		},
//
//		{
//			{
//				{"Hello, my name is N00bFlesh", 0.5f},
//				{"こんにちは、僕の名前はヌーブフレッシです", 0.5f},
//			},
//			""
//		},
//	};
//	for (const auto& pair : weight_tests) {
//		if(!test_weight(pair.first, pair.second)){
//			result |= 2;
//		}
//	}
//	return result;
//}
