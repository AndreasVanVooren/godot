#ifndef TEXT_LERP_IMPL
#define TEXT_LERP_IMPL

// NOTE: This follows Godot Engine formatting standards, as I'm using this for a project in said engine.

#include <algorithm>
#include <iterator>
#include <array>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <initializer_list>
#include <iterator>
#include <map>
#include <string>
#include <vector>

// UTF characters can only be 4 chars long, undef this in case we read stuff from 1993
#define USE_RFC3629_RESTRICTIONS

// NOTE: Try to make this as constexpr as possible, as we need performance here.
// NOTE: In C++20 this may be easier with u8strings.

// Somehow make a suffix tree?
namespace SuffixTree {
// Implemenation based on https://rosettacode.org/wiki/Suffix_tree#C.2B.2B,
// but modified to support any type, and also to reduce the amount of while-loopage.
// There's an alternate solution for longest common substring on the same site which uses sets,
// but this seems very hungry, and therefore less usable.
template <typename TIter, typename TEndType>
struct Character {
	union {
		TIter c;
		TEndType e;
	};
	bool bIsEndCharacter;

	Character(TIter asC) :
			c{ asC }, bIsEndCharacter{ false } {}

	Character(TEndType asE) :
			e{ asE }, bIsEndCharacter{ true } {}
};

template <typename TIter, typename TEndType>
inline bool operator==(const Character<TIter, TEndType> &lhs, const Character<TIter, TEndType> &rhs) {
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
template <typename TIter, typename TEndType>
inline bool operator!=(const Character<TIter, TEndType> &lhs, const Character<TIter, TEndType> &rhs) {
	return !(lhs == rhs);
}

// Character logger. Used in debug visualize function.
template <typename TIter, typename TEndType>
inline std::ostream &operator<<(std::ostream &stream, const Character<TIter, TEndType> &rhs) {
	if (rhs.bIsEndCharacter) {
		return stream << "#|";
	} else {
		return stream << (*rhs.c) << "|";
	}
}

// Specialization for std::string vectors.
// TODO: Support any proper character type.
template<>
inline std::ostream &operator<<(std::ostream &stream, const Character<typename std::string::const_iterator, typename std::vector<std::string>::const_iterator> &rhs) {
	if (rhs.bIsEndCharacter) {
		return stream << "#";
	} else {
		return stream << (*rhs.c);
	}
}

template <typename TIter>
struct Node {
	using TNodeType = Node<TIter>;
	using TNodeVec = std::vector<TNodeType>;
	using TNodeDiff = typename TNodeVec::difference_type;
	// Range of "string" that matches the substring. [start, end)
	std::pair<TIter, TIter> subString{};
	// Children (by index) of this node. The method of generation prevents us from using iterators here.
	std::vector<TNodeDiff> children;

	Node() {}
	Node(const TIter &start, const TIter &end, const std::initializer_list<TNodeDiff> &ch) :
			subString{ start, end }, children{ ch } {}
};

// Node logger. Used in debug visualize function.
template <typename TIter>
inline std::ostream &operator<<(std::ostream &stream, const Node<TIter> &rhs) {
	for (auto iter = rhs.subString.first; iter != rhs.subString.second; ++iter)
	{
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
	void VisualizeRecursive(TNodeConstIter child, const std::string& indentString = {}) const {
		if (child->children.size() == 0) {
			std::cout << "- " << *child << '\n';
			return;
		}

		std::cout << indentString << "+ " << *child << '\n';

		for (auto it = child->children.cbegin(); it != child->children.cend(); ++it)
		{
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
				TCharType chara{ subIt };
				characters.push_back(chara);
			}

			TCharType end{ it };
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
				// Early return;
				return;
			}

			// Find the prefix of the remaining suffix in common with the given child.
			size_t offset = 0;
			for (TCharIter subStringIt = nodes[nodeIdx2].subString.first; subStringIt != nodes[nodeIdx2].subString.second; ++subStringIt, ++offset) {
				TCharIter offsetSuffixIt = std::next(suffixIt, offset);
				
				if (*offsetSuffixIt != *subStringIt) {
					// Saw the node in half. That's a lotta damage!
					auto nodeIdx3 = nodeIdx2;
					// New node for the common part.
					nodeIdx2 = std::distance(nodes.begin(), nodes.insert(nodes.end(), TNodeType{ nodes[nodeIdx3].subString.first, subStringIt, { nodeIdx3 } }));
					// old node loses the part in common.
					nodes[nodeIdx3].subString.first = subStringIt;
					(*childIt) = nodeIdx2;
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

namespace _TextLerpInternals {
static constexpr size_t bad_byte_size = static_cast<size_t>(-1);
using codepoint_t = uint32_t;
static constexpr codepoint_t bad_code_point = static_cast<codepoint_t>(-1);
// Returns the expected byte size from the initial character.
// This is done by counting the amount of continuous leading bits.
// Exception are standard ASCII characters and continuation characters.
// ASCII characters return 1 as size, despite having no leading bits.
// Continuation characters, despite having one leading bit, return -1 (0xffffffff)
// as to indicate an invalid result (we expect the initial character, not the continuation).
inline size_t UTF8ByteSizeFromFirstChar(char s) {
	char mainChar = s;
	size_t bitsFound = 0;

	while ((mainChar & 0x80) > 0) {
		++bitsFound;
		mainChar <<= 1;
	}

	if (bitsFound == 0) {
		// Standard ASCII result.
		bitsFound = 1;
	} else if (bitsFound == 1) {
		// Illegal result, indicates that the operation was done on a continuation byte.
		return bad_byte_size;
	}

	return bitsFound;
}

inline size_t UTF8ByteSizeFromCodePoint(const codepoint_t cp) {
	if (cp < 0x80) {
		return 1;
	}

	size_t byteSize = 0;
	for (; (cp >> (6 * byteSize)) > 0; ++byteSize) {
	}
	return byteSize;
}

inline size_t UTF8StrLen(const std::string &str) {
	size_t size = 0;
	for (size_t index = 0; index < str.size();) {
		const size_t byteSize = UTF8ByteSizeFromFirstChar(str[index]);
		if (byteSize == bad_byte_size) {
			// Illegal result, trying to iter a bad string.
			return std::string::npos;
		}
		index += byteSize;
		++size;
	}
	return size;
}
inline codepoint_t UTF8ToCodePoint(std::string::const_iterator iter) {
	const size_t byteSize = UTF8ByteSizeFromFirstChar(*iter);
	if (byteSize == bad_byte_size) {
		return bad_code_point;
	}
#ifdef USE_RFC3629_RESTRICTIONS
	// Bad code point, beyond scope of Unicode
	else if (static_cast<unsigned char>(*iter) >= 0xF5) {
		return bad_code_point;
	}
#endif
	else if (byteSize == 1) {
		return static_cast<codepoint_t>(*iter);
	}

	// Assume due to the peculiar way that UTF-8 is stored that
	// the byte order is consistent no matter whether the system uses
	// big or little endian

	// First character is handled differently, as the preceding bits are variable.
	unsigned char charDynMaskInv = 0;
	unsigned char charDynVal = 0;
	// NOTE: Add one, otherwise the mask would not be in format 00000'111, but 0000'1111.
	for (size_t i = 0; i < byteSize + 1; ++i) {
		charDynMaskInv |= 0b1000'0000 >> static_cast<unsigned char>(i);
		if (i < byteSize) {
			charDynVal |= 0b1000'0000 >> static_cast<unsigned char>(i);
		}
	}
	if ((static_cast<unsigned char>(*iter) & charDynMaskInv) != charDynVal) {
		// Invalid char prefix
		return bad_code_point;
	}

	const char charDynMask = static_cast<char>(~charDynMaskInv);
	const char maskedFirstChar = ((*iter) & charDynMask);
	const size_t firstShift = (byteSize - 1) * 6;
	codepoint_t result = maskedFirstChar << firstShift;
	++iter;

	// Rest can be done in a loop, as the preceding bits will always be 10.
	for (size_t i = 1; i < byteSize; ++i, ++iter) {
		const unsigned char charMaskInv = 0b1100'0000;
		if ((static_cast<unsigned char>(*iter) & charMaskInv) != 0b1000'0000) {
			// Invalid char prefix
			return bad_code_point;
		}
		const char charMask = static_cast<char>(~charMaskInv);
		const char maskedChar = ((*iter) & charMask);
		const size_t shift = (byteSize - 1 - i) * 6;
		result |= maskedChar << shift;
	}

	return result;
}

inline std::array<char, 5> CodePointToUTF8(const codepoint_t cp) {
	std::array<char, 5> result{};
	const size_t byteSize = UTF8ByteSizeFromCodePoint(cp);
	if (byteSize == 1) {
		result[0] = static_cast<char>(cp);
		return result;
	} else if (byteSize == bad_byte_size) {
		return result;
	}

	static constexpr unsigned char BITS_PER_CONTINUE_BYTE = 6;

	// Special case for first bits
	{
		unsigned char charDynPrefix = 0;
		unsigned char charDynMaskInv = 0;
		for (size_t i = 0; i < byteSize + 1; ++i) {
			charDynMaskInv |= 0b1000'0000 >> static_cast<unsigned char>(i);
			if (i < byteSize) {
				charDynPrefix |= 0b1000'0000 >> static_cast<unsigned char>(i);
			}
		}
		const unsigned char charDynMask = ~charDynMaskInv;

		const size_t distFromEnd = byteSize - 1;
		const size_t rShift = BITS_PER_CONTINUE_BYTE * distFromEnd;
		const codepoint_t shifted = cp >> rShift;
		const unsigned char masked = static_cast<unsigned char>(shifted & charDynMask);
		const char output = static_cast<char>(charDynPrefix | masked);
		result[0] = output;
	}

	static constexpr unsigned char CONTINUE_BYTE_PRE = 0x80;
	static constexpr unsigned char CONTINUE_BYTE_MASK = 0x3f;

	for (size_t i = 1; i < byteSize; ++i) {
		const size_t distFromEnd = byteSize - 1 - i;
		const size_t rShift = BITS_PER_CONTINUE_BYTE * distFromEnd;
		const codepoint_t shifted = cp >> rShift;
		const unsigned char masked = static_cast<unsigned char>(shifted & CONTINUE_BYTE_MASK);
		const char output = static_cast<char>(CONTINUE_BYTE_PRE | masked);
		result[i] = output;
	}

	return result;
}

// https://www.geeksforgeeks.org/ukkonens-suffix-tree-construction-part-1/
template <typename TReal = double>
inline auto split_to_substring_list(const std::map<std::string, TReal> &stringMap) {
	// Subdivide the string list into sequences of matching/different sequences.
	// E.g. "[The] [ UMBRELLA CORPORATION ] [released the] [ T-virus] [.]"
	//      "[De]  [ UMBRELLA CORPORATION ]   [heeft het]  [ T-virus] [ vrijgelaten.]"
	// Reason being that matching sequences will seemingly lerp into place, instead of noising.
	// Note that we disregard white-space, but this isn't really something we care about
	using TCodepointVec = std::vector<codepoint_t>;
	using TCodepointRange = std::vector<TCodepointVec>;
	TCodepointRange transformed{};
	std::transform(
			stringMap.cbegin(), stringMap.cend(), std::back_inserter(transformed),
			[](const auto &pair) -> typename TCodepointRange::value_type {
				std::vector<codepoint_t> points;
				for (auto iter = pair.first.cbegin(); iter != pair.first.cend();) {
					const codepoint_t point = UTF8ToCodePoint(iter);
					// Found bad code point, return empty string.
					if (point == bad_code_point) {
						return { bad_code_point };
					}
					points.push_back(point);
					const size_t byteSize = UTF8ByteSizeFromCodePoint(point);
					for (size_t i = 0; i < byteSize; ++i, ++iter) {
					}
				}
				return { points };
			});

	// TODO: Split format specifiers (as optional implementation)
	// as format specifiers can be in any order, but still need to react consistently.
	// E.g. "I love {0} and {1}" and "J'aime {1} et {0}" should
	// return either "ASDF {0} ASDF {1}" or "ASDF {1} ASDF {0}".
	// Since the localization will presumably be cached, we need to figure out this step,
	// Otherwise we have weird noise.


	auto tree = SuffixTree::Tree<TCodepointRange>{ transformed };
	tree.Visualize();
	// TODO: Wrap in loop so that we can group all common substrings.
	auto longestSubstr = tree.LongestCommonSubstring();
	if(longestSubstr.size() == 0)
	{
		// End loop.
	}

	using weight_map_iter = typename std::map<std::string, TReal>::const_iterator;
	using string_iter = typename std::string::const_iterator;
	using string_range = typename std::pair<string_iter, string_iter>;
	using sequence_list = typename std::vector<string_range>;

	std::map<weight_map_iter, sequence_list> sequence_list_per_weighted_entry{};
	weight_map_iter first_elem = stringMap.cbegin();
	const std::string &str = first_elem->first;
	weight_map_iter it = first_elem;
	++it;
	for (; it != stringMap.cend(); ++it) {
		string_iter lhsIter = first_elem->first.cbegin();
		string_iter rhsIter = it->first.cbegin();
		string_iter lhsMatchStart = lhsIter;
		string_iter rhsMatchStart = rhsIter;
		string_iter lhsDiffStart = lhsIter;
		string_iter rhsDiffStart = rhsIter;

		bool bInMatchingSequence = false;
		for (; lhsIter != first_elem->first.cend(); ++lhsIter) {
			if (!bInMatchingSequence) {
				// Match is uncertain, for each character, see if we're matching somehow.
				for (; rhsIter != it->first.cend(); ++rhsIter) {
					// Match code points, not bytes. A byte may be identical for different code points.
					if (UTF8ToCodePoint(lhsIter) == UTF8ToCodePoint(rhsIter)) {
						bInMatchingSequence = true;
						lhsMatchStart = lhsIter;
						rhsMatchStart = rhsIter;
						break;
					}
				}
			} else {
			}
		}
	}

	return sequence_list_per_weighted_entry;
}

} //namespace _TextLerpInternals

// Takes a map of strings (assuming UTF-8) and weights, and interpolates them.
// Tries to account for grapheme clustering.
// Assumes all strings are UTF-8, use of UTF-16 or UTF-32 is not valid.
// Assumes the sum of all weights equals 1.0.
template <typename TReal = double>
inline std::string interpolate(const std::map<std::string, TReal> &weightedStringMap) {
	static_assert(std::is_floating_point<TReal>::value, "Can't be used with ints for weights (yet)");
	// If the map is empty, just return nothing.
	if (weightedStringMap.size() <= 0) {
		return {};
	}

	using namespace _TextLerpInternals;

	const auto sequences_per_entry = split_to_substring_list(weightedStringMap);

	for (const auto &pair : weightedStringMap) {
	}

	std::string output;
	return output;
}

// Just to see if my includes are working
static_assert(std::is_floating_point<float>::value, "Includes broke, probably needs type_traits");

#endif // TEXT_LERP_IMPL