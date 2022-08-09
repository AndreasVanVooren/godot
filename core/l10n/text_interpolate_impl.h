#ifndef TEXT_LERP_IMPL
#define TEXT_LERP_IMPL

// NOTE: This follows Godot Engine formatting standards, as I'm using this for a project in said engine.

#include <algorithm>
#include <map>
#include <string>
#include <vector>

// UTF characters can only be 4 chars long, undef this in case we read stuff from 1993
#define USE_RFC3629_RESTRICTIONS

// NOTE: Try to make this as constexpr as possible, as we need performance here.
// NOTE: In C++20 this may be easier with u8strings.

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
	else if (*iter >= static_cast<char>(0xF5)) {
		return bad_code_point;
	}
#endif
	else if (byteSize == 1) {
		return static_cast<codepoint_t>(*iter);
	}

	// Assume due to the peculiar way that UTF-8 is stored that
	// the byte order is consistent no matter whether the system uses
	// big or little endian
	// https://www.geeksforgeeks.org/ukkonens-suffix-tree-construction-part-1/

	// First character is handled differently, as the preceding bits are variable.
	unsigned char charDynMaskInv = 0;
	unsigned char charDynVal = 0;
	for (size_t i = 0; i < byteSize; ++i) {
		charDynMaskInv |= 0b1000'0000 >> static_cast<unsigned char>(i);
		if (i < byteSize - 1) {
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


template<typename TReal = double>
inline auto split_to_substring_list(const std::map<std::string, TReal> &stringMap)
{
	// Subdivide the string list into sequences of matching/different sequences.
	// E.g. "[The] [ UMBRELLA CORPORATION ] [released the] [ T-virus] [.]"
	//      "[De]  [ UMBRELLA CORPORATION ]   [heeft het]  [ T-virus] [ vrijgelaten.]"
	// Reason being that matching sequences will seemingly lerp into place, instead of noising.
	// Note that we disregard white-space, but this isn't really something we care about
	using weight_map_iter = typename std::map<std::string, TReal>::const_iterator;
	using string_iter = typename std::string::const_iterator;
	using string_range = typename std::pair<string_iter, string_iter>;
	using sequence_list = typename std::vector<string_range>;
	struct suffix_tree_node
	{
		union 
		{
			string_range r;
			weight_map_iter w;
		};
		bool bIsSuffix;
	};
	
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
			}
			else {
				
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