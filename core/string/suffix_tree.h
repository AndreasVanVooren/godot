/**************************************************************************/
/*  suffix_tree.h                                                         */
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

#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <initializer_list>
#include <iterator>
#include <numeric>
#include <string>
#include <unordered_set>
#include <vector>

//#define WITH_DEBUGGING
#define WITH_VISUALIZE

#if defined(WITH_DEBUGGING) || defined(WITH_VISUALIZE)
#include <iostream>
#endif

// NOTE: Try to make this as constexpr as possible, as we need performance here.
// NOTE: In C++20 this may be easier with u8strings.

// Somehow make a suffix tree?
// TODO: Expose more variables?
namespace SuffixTree {
// Implemenation based on https://rosettacode.org/wiki/Suffix_tree#C.2B.2B,
// but modified to support any type, and also to reduce the amount of while-loopage.
// There's an alternate solution for longest common substring on the same site which uses sets,
// but this seems very hungry, and therefore less usable.
template <typename TIter, typename TEnd>
// Character struct. Also contains meta data about which string it comes from.
struct Character {
	using TRangeType = TEnd;
	using TRangeDiff = typename TEnd::difference_type;
	using TCharType = TIter;
	union {
		TCharType c;
		TRangeType e;
	};
	TRangeDiff partOfString;
	bool bIsEndCharacter;

	Character(TCharType asC, TRangeDiff strIdx) :
			c{ asC }, partOfString{ strIdx }, bIsEndCharacter{ false } {}

	Character(TRangeType asE, TRangeDiff strIdx) :
			e{ asE }, partOfString{ strIdx }, bIsEndCharacter{ true } {}
};

template <typename TIter, typename TEnd>
inline bool operator==(const Character<TIter, TEnd> &lhs, const Character<TIter, TEnd> &rhs) {
	if (lhs.bIsEndCharacter != rhs.bIsEndCharacter) {
		return false;
	} else if (lhs.bIsEndCharacter) {
		// Technically this case should never be valid, but either way,
		// we compare the iterators, assuming the iterators are part of the same container.
		return lhs.e == rhs.e;
	} else {
		// Check non-end characters by value, as that is what a character is.
		// If we use the raw iterator here, it'll never equal.
		return (*lhs.c) == (*rhs.c);
	}
}
template <typename TIter, typename TEnd>
inline bool operator!=(const Character<TIter, TEnd> &lhs, const Character<TIter, TEnd> &rhs) {
	return !(lhs == rhs);
}

#if defined(WITH_DEBUGGING) || defined(WITH_VISUALIZE)
// Character logger. Used in debug visualize function.
template <typename TIter, typename TEnd>
inline std::ostream &operator<<(std::ostream &stream, const Character<TIter, TEnd> &rhs) {
	if (rhs.bIsEndCharacter) {
		return stream << "(" << rhs.partOfString << ")|";
	} else {
		return stream << (*rhs.c) << "|";
	}
}

// Specialization for std::string vectors.
// TODO: Support any proper character type.
template <>
inline std::ostream &operator<<(std::ostream &stream, const Character<typename std::string::const_iterator, typename std::vector<std::string>::const_iterator> &rhs) {
	if (rhs.bIsEndCharacter) {
		return stream << "(" << rhs.partOfString << ")";
	} else {
		return stream << (*rhs.c);
	}
}
#endif // WITH_DEBUGGING

template <typename TIter>
struct Node {
	using TCharType = typename TIter::value_type;
	using TRangeDiff = typename TCharType::TRangeDiff;
	using TNodeType = Node<TIter>;
	using TNodeVec = std::vector<TNodeType>;
	using TNodeDiff = typename TNodeVec::difference_type;
	// Range of "string" that matches the substring. [start, end)
	std::pair<TIter, TIter> subString{};
	// Children (by index) of this node. The method of generation prevents us from using iterators here.
	std::vector<TNodeDiff> children{};
	std::unordered_set<TRangeDiff> partOfStrings{};

	Node() {}
	Node(const TIter &start, const TIter &end, const std::initializer_list<TNodeDiff> &ch) :
			subString{ start, end }, children{ ch }, partOfStrings{} {}
};

#if defined(WITH_DEBUGGING) || defined(WITH_VISUALIZE)
// Node logger. Used in debug visualize function.
template <typename TIter>
inline std::ostream &operator<<(std::ostream &stream, const Node<TIter> &rhs) {
	for (auto iter = rhs.subString.first; iter != rhs.subString.second; ++iter) {
		stream << *iter;
	}
	stream << "[";
	for (const typename Node<TIter>::TRangeDiff &part : rhs.partOfStrings) {
		stream << part << ", ";
	}
	stream << "]";

	return stream;
}
#endif // WITH_DEBUGGING

template <typename TRange>
class Tree {
public:
	// NOTE: Characters use a constant iterator, as they don't mod the character list.
	using TRangeIter = typename TRange::const_iterator;
	using TSubRange = typename TRange::value_type;
	using TSubRangeIter = typename TSubRange::const_iterator;
	using TCharType = Character<TSubRangeIter, TRangeIter>;
	using TCharVec = std::vector<TCharType>;
	using TCharIter = typename TCharVec::iterator;
	using TNodeType = Node<TCharIter>;
	using TNodeVec = std::vector<TNodeType>;
	using TNodeIter = typename TNodeVec::iterator;
	using TNodeDiff = typename TNodeVec::difference_type;
	using TNodeConstIter = typename TNodeVec::const_iterator;

	Tree(const TRange &range) {
		stringCount = range.size();
		InitCharacters(range);
		ConstructTreeFromCharacters();
	}

#ifdef WITH_VISUALIZE
	// Debug function which allows visualization of the suffix tree,
	// to validate its functionality and investigate any problems.
	void Visualize() const {
		if (nodes.size() == 0) {
			std::cout << "<empty>\n";
			return;
		}

		VisualizeRecursive(nodes.cbegin());
	}
#endif

	// Returns the longest common substring. Returns a copy, not a reference.
	// If there's only one string passed through in the tree, returns the longest repeated substring.
	// If multiple common substrings have the same length, returns the first one.
	TSubRange LongestCommonSubstring() const {
		TSubRange result{};

		const std::vector<TNodeConstIter> bestCandidateVector = GetLongestCommonSubstringCandidate(nodes.cbegin());

		for (const TNodeConstIter &iterator : bestCandidateVector) {
			result.reserve(result.size() + std::distance(iterator->subString.first, iterator->subString.second));
			for (auto it = iterator->subString.first; it != iterator->subString.second; ++it) {
				if ((*it).bIsEndCharacter) {
#ifdef WITH_DEBUGGING
					std::cout << "ERROR: Expected substring to not contain any end characters.\n";
#endif
					return {};
				}

				result.push_back(*(it->c));
			}
		}

		return result;
	}

private:
	// TODO: Inline allocators?
	std::vector<TNodeConstIter> GetLongestCommonSubstringCandidate(TNodeConstIter curNode) const {
		// If we're not common, return an empty vector
		if (curNode->partOfStrings.size() != stringCount) {
			return {};
		}
		// If we're a leaf node, ignore.
		if (curNode->children.size() == 0) {
			return {};
		}

		// Find the best candidate for all our children.
		std::vector<TNodeConstIter> best{};
		size_t bestSize = 0;
		for (const TNodeDiff &child : curNode->children) {
			const auto iter = std::next(nodes.cbegin(), child);
			const std::vector<TNodeConstIter> tempResult = GetLongestCommonSubstringCandidate(iter);
			const size_t tempSize = std::accumulate(
					tempResult.cbegin(),
					tempResult.cend(),
					0,
					[](size_t init, const TNodeConstIter &node) { return init + std::distance(node->subString.first, node->subString.second); });
			if (tempSize > bestSize) {
				best = tempResult;
				bestSize = tempSize;
			}
		}

		std::vector<TNodeConstIter> result{};
		result.reserve(1 + best.size());
		result.push_back(curNode);
		result.insert(result.cend(), best.begin(), best.end());

		return result;
	}

#ifdef WITH_VISUALIZE
	void VisualizeRecursive(TNodeConstIter child, const std::string &indentString = {}) const {
		if (child->children.size() == 0) {
			std::cout << "╴ " << *child << '\n';
			return;
		}

		// DON'T USE THE '+' SIGN! For some reason the VS Code Debug Console absorbs (?) lines starting with a '+'.
		// I don't know why, but just don't do it. The end.
		std::cout << "┬ " << *child << "\n";

		for (auto it = child->children.cbegin(); it != child->children.cend(); ++it) {
			const bool bLastChild = (std::next(it) == child->children.cend());
			const std::string thisIndentString = bLastChild ? indentString + "└─" : indentString + "├─";
			const std::string subIndentString = bLastChild ? indentString + "  " : indentString + "│ ";
			std::cout << thisIndentString;
			VisualizeRecursive(std::next(nodes.cbegin(), *it), subIndentString);
		}
	}
#endif

	void InitCharacters(const TRange &range) {
		// Init character list. This is the joined list of all strings.
		// TODO: Use transform or something?
		for (TRangeIter it = range.begin(); it != range.end(); ++it) {
			for (TSubRangeIter subIt = it->begin(); subIt != it->end(); ++subIt) {
				TCharType chara{ subIt, std::distance(range.cbegin(), it) };
				characters.push_back(chara);
			}

			TCharType end{ it, std::distance(range.cbegin(), it) };
			characters.push_back(end);
		}
	}
	void ConstructTreeFromCharacters() {
		nodes.push_back(TNodeType{});
		for (TCharIter it = characters.begin(); it != characters.end(); ++it) {
			AddSuffix(it, characters.end());
#if defined(WITH_DEBUGGING) && defined(WITH_VISUALIZE)
			if (it->bIsEndCharacter) {
				Visualize();
			}
#endif
		}
	}
	void AddSuffix(const TCharIter &start, const TCharIter &end) {
		// NOTE: Some optimizations/cleanup could be done in this function:
		// Originally I made this using node vector iterators as the children type.
		// ...before I realized that by adding children in this function, I'd invalidate said iterators.
		// As such, this is a bit of a mismash between by-iterator and by-index access, which isn't pretty.
		using TChildVec = std::vector<TNodeDiff>;
		using TChildIter = typename TChildVec::iterator;
		TCharIter suffixIt = start;
		TNodeDiff nodeIdx = 0;
		for (; suffixIt != end;) {
			TNodeDiff matchingNodeIdx = nodes.size();
			TChildIter childIt = nodes[nodeIdx].children.begin();
			// Find a child that's matching the thing
			for (; childIt != nodes[nodeIdx].children.end(); ++childIt) {
				// Found the start of a substring, break.
				if (*(nodes[*childIt].subString.first) == *suffixIt) {
					matchingNodeIdx = *childIt;
					break;
				}
			}
			const bool bNoMatchingChild = (matchingNodeIdx == static_cast<TNodeDiff>(nodes.size()));
			if (bNoMatchingChild) {
				// Insert returns the iterator, so use that instead of
				// push_back (returns nothing), or emplace_back (may return ref, which is unwanted)
				// This is a non-const iterator, but STD also requires iterators
				// to be implicitly convertible to const_iterator, so we good.
				const auto newNodeIt = nodes.insert(nodes.end(), TNodeType{ suffixIt, end, {} });
				const auto newNodeIdx = std::distance(nodes.begin(), newNodeIt);
				nodes[nodeIdx].children.push_back(newNodeIdx);
				// NOTE: Also indicate that the parent and the child are a part of the suffix's given string.
				nodes[nodeIdx].partOfStrings.insert(suffixIt->partOfString);
				nodes[newNodeIdx].partOfStrings.insert(suffixIt->partOfString);
#ifdef WITH_DEBUGGING
				std::cout << "New child node: [" << nodeIdx << "] -> [" << newNodeIdx << "] ";
				for (auto it = suffixIt; it != end; ++it) {
					std::cout << *it;
				}
				std::cout << "\n";

				std::cout << "Parent and child node are part of string " << suffixIt->partOfString << "\n";
#endif
				// Early return;
				return;
			}

			// Find the prefix of the remaining suffix in common with the given child.
			size_t offset = 0;
			bool bCausedNodeSplit = false;
			for (TCharIter subStringIt = nodes[matchingNodeIdx].subString.first; subStringIt != nodes[matchingNodeIdx].subString.second; ++subStringIt, ++offset) {
				TCharIter offsetSuffixIt = std::next(suffixIt, offset);
				if (*offsetSuffixIt != *subStringIt) {
					// Saw the node in half. That's a lotta damage!

					// NOTE: Split of the insert from the std::distance, as for some bizarre reason,
					// the index returned by std::distance becomes out of range?
					// This is probably some weird optimization issue that LLVM causes.

					// NOTE: There's a bit of weirdness here with nodeIdx2 and nodeIdx3 being quite ephemeral.
					// The summary is that a node index stored in Idx2 needs to be here in order for the iteration to keep going.
					auto newNodeIdx = matchingNodeIdx;
					// New node for the common part.
					const auto newNodeIt = nodes.insert(nodes.end(), TNodeType{ nodes[newNodeIdx].subString.first, subStringIt, { newNodeIdx } });
					matchingNodeIdx = std::distance(nodes.begin(), newNodeIt);
					bCausedNodeSplit = true;

					// old node loses the part in common.
					nodes[newNodeIdx].subString.first = subStringIt;
					(*childIt) = matchingNodeIdx;

					nodes[matchingNodeIdx].partOfStrings.insert(nodes[newNodeIdx].partOfStrings.cbegin(), nodes[newNodeIdx].partOfStrings.cend());
					nodes[newNodeIdx].partOfStrings.insert(subStringIt->partOfString);

#ifdef WITH_DEBUGGING
					std::cout << "Split new node: [" << matchingNodeIdx << "] ";
					for (auto it = nodes[matchingNodeIdx].subString.first; it != nodes[matchingNodeIdx].subString.second; ++it) {
						std::cout << *it;
					}
					std::cout << " - [" << newNodeIdx << "] ";
					for (auto it = nodes[newNodeIdx].subString.first; it != nodes[newNodeIdx].subString.second; ++it) {
						std::cout << *it;
					}
					std::cout << "\n";
#endif
					break;
				}
			}

			if (!bCausedNodeSplit) {
				nodes[matchingNodeIdx].partOfStrings.insert(suffixIt->partOfString);
			}

			std::advance(suffixIt, offset);

			nodeIdx = matchingNodeIdx;
		}
	}

	std::vector<TCharType> characters;
	std::vector<TNodeType> nodes;
	size_t stringCount;
};
} //namespace SuffixTree

#undef WITH_DEBUGGING
#undef WITH_VISUALIZE

#endif // SUFFIX_TREE_H
