#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <set>
#include <string>
#include <vector>

#define WITH_INTERNAL_DEBUG_LOGS

#ifdef WITH_INTERNAL_DEBUG_LOGS
#include <iostream>
#endif

// NOTE: Try to make this as constexpr as possible, as we need performance here.
// NOTE: In C++20 this may be easier with u8strings.

// Somehow make a suffix tree?
namespace SuffixTree {
// Implemenation based on https://rosettacode.org/wiki/Suffix_tree#C.2B.2B,
// but modified to support any type, and also to reduce the amount of while-loopage.
// There's an alternate solution for longest common substring on the same site which uses sets,
// but this seems very hungry, and therefore less usable.
template <typename TIter, typename TEnd>
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
	std::vector<TNodeDiff> children;
	std::set<TRangeDiff> partOfStrings;

	Node() {}
	Node(const TIter &start, const TIter &end, const std::initializer_list<TNodeDiff> &ch) :
			subString{ start, end }, children{ ch }, partOfStrings{} {}
};

// Node logger. Used in debug visualize function.
template <typename TIter>
inline std::ostream &operator<<(std::ostream &stream, const Node<TIter> &rhs) {
	for (auto iter = rhs.subString.first; iter != rhs.subString.second; ++iter) {
		stream << *iter;
	}
	return stream;
}

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
		InitCharacters(range);
		ConstructTreeFromCharacters();
	}

	// Debug function which allows visualization of the suffix tree,
	// to validate its functionality and investigate any problems.
	void Visualize() const {
		if (nodes.size() == 0) {
			std::cout << "<empty>\n";
			return;
		}

		VisualizeRecursive(nodes.cbegin());
	}

	// Returns the longest common substring. Returns a copy, not a reference.
	TSubRange LongestCommonSubstring() const {
		// Problem: "banana", "<some chinese characters>"
		// would report "ana" (or "an" or "na") as its longest common substring,
		// but we actually don't want a result in this case.
		// "Press {0} to continue" and "Appuyez {0} pour continuer" could return "continue"
		// "Press {0} to continue" and "{0} <chinese>" could return {0}

		TSubRange result{};

		return result;
	}

private:
	void VisualizeRecursive(TNodeConstIter child, const std::string &indentString = {}) const {
		if (child->children.size() == 0) {
			std::cout << "- " << *child << '\n';
			return;
		}

		std::cout << indentString << "+ " << *child << "\n";

		for (auto it = child->children.cbegin(); it != child->children.cend(); ++it) {
			const std::string subIndentString = (std::next(it) == child->children.cend()) ? indentString + "  " : indentString + "| ";
			std::cout << indentString << "+-";
			VisualizeRecursive(std::next(nodes.cbegin(), *it), subIndentString);
		}
	}

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
			TNodeDiff nodeIdx2 = nodes.size();
			TChildIter childIt = nodes[nodeIdx].children.begin();
			bool bNoMatchingChild = true;
			// Find a child that's matching the thing
			for (; childIt != nodes[nodeIdx].children.end(); ++childIt) {
				nodeIdx2 = *childIt;
				// Found the start of a substring, break.
				if (*(nodes[nodeIdx2].subString.first) == *suffixIt) {
					bNoMatchingChild = false;
					break;
				}
			}
			if (bNoMatchingChild) {
				// Insert returns the iterator, so use that instead of
				// push_back (returns nothing), or emplace_back (may return ref, which is unwanted)
				// This is a non-const iterator, but STD also requires iterators
				// to be implicitly convertible to const_iterator, so we good.
				nodeIdx2 = std::distance(nodes.begin(), nodes.insert(nodes.end(), TNodeType{ suffixIt, end, {} }));
				nodes[nodeIdx].children.push_back(nodeIdx2);
				#ifdef WITH_INTERNAL_DEBUG_LOGS
				std::cout << "New child node: [" << nodeIdx << "] -> [" << nodeIdx2 << "] ";
				for (auto it = suffixIt; it != end; ++it) {
					std::cout << *it;
				}
				std::cout << "\n";
				#endif
				// Early return;
				return;
			}

			// Find the prefix of the remaining suffix in common with the given child.
			size_t offset = 0;
			for (TCharIter subStringIt = nodes[nodeIdx2].subString.first; subStringIt != nodes[nodeIdx2].subString.second; ++subStringIt, ++offset) {
				TCharIter offsetSuffixIt = std::next(suffixIt, offset);
				if (*offsetSuffixIt == *subStringIt) {
					nodes[nodeIdx].partOfStrings.insert(offsetSuffixIt->partOfString);
				} else {
					// Saw the node in half. That's a lotta damage!
					auto nodeIdx3 = nodeIdx2;
					// New node for the common part.
					nodeIdx2 = std::distance(nodes.begin(), nodes.insert(nodes.end(), TNodeType{ nodes[nodeIdx3].subString.first, subStringIt, { nodeIdx3 } }));

					// old node loses the part in common.
					nodes[nodeIdx3].subString.first = subStringIt;
					(*childIt) = nodeIdx2;
					#ifdef WITH_INTERNAL_DEBUG_LOGS
					std::cout << "Split new node: [" << nodeIdx2 << "] ";
					for (auto it = nodes[nodeIdx2].subString.first; it != nodes[nodeIdx2].subString.second; ++it) {
						std::cout << *it;
					}
					std::cout << " - [" << nodeIdx3 << "] ";
					for (auto it = nodes[nodeIdx3].subString.first; it != nodes[nodeIdx3].subString.second; ++it) {
						std::cout << *it;
					}
					std::cout << "\n";
					#endif
					break;
				}
			}

			std::advance(suffixIt, offset);

			nodeIdx = nodeIdx2;
		}
	}

	std::vector<TCharType> characters;
	std::vector<TNodeType> nodes;
};
} //namespace SuffixTree

#undef WITH_INTERNAL_DEBUG_LOGS

#endif // SUFFIX_TREE_H