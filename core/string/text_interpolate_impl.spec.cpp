/**************************************************************************/
/*  text_interpolate_impl.spec.cpp                                        */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "text_interpolate_impl.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>

using string_vec = std::vector<std::string>;
using weight_map = std::vector<std::pair<std::string, float>>;
bool test_tree(const string_vec &range, const std::string &expected) {
	string_vec rangePermutation = range;
	std::sort(rangePermutation.begin(), rangePermutation.end());
	bool bSuccess = true;
	// Use permutations to test the string array.
	// We keep the array unsorted, but a map MIGHT sort its entries by key.
	// (as in, I observed it was the case, but am not sure if it's for every compiler)
	// Although this ultimately shouldn't affect the result of the LCS determination,
	// it does affect the internal generation and representation of the suffix tree.
	do {
		const SuffixTree::Tree<string_vec> tree{ rangePermutation };
		//tree.Visualize();
		const std::string common = tree.LongestCommonSubstring();
		if (common != expected) {
			std::cout << "==== In permutation: ";
			for (const std::string &str : rangePermutation) {
				std::cout << "'" << str << "', ";
			}
			std::cout << ":\n";
			std::cout << "Common string '" << common
					  << "' does not meet expected string '" << expected << "'\n";
			bSuccess = false;
		}
	} while (std::next_permutation(rangePermutation.begin(), rangePermutation.end()));
	return bSuccess;
}

bool test_weight(const weight_map &range, const std::string &expected) {
	const std::string output = interpolate(range);
	std::cout << "Output for " << range.cbegin()->first << " = " << output << "\n";
	if (output != expected) {
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
