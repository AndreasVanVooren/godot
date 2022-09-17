
#ifndef TEXT_INTERPOLATE_IMPL_H
#define TEXT_INTERPOLATE_IMPL_H

// NOTE: This follows Godot Engine formatting standards, as I'm using this for a project in said engine.

#include <algorithm>
#include <array>
#include <bits/utility.h>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include "suffix_tree.h"

// For unicode block range detection
#include "unicode_blocks.h"

// UTF characters can only be 4 chars long, undef this in case we read stuff from 1993
#define USE_RFC3629_RESTRICTIONS
#define WITH_DEBUGGING

namespace _TextLerpInternals {
static constexpr size_t bad_byte_size = static_cast<size_t>(-1);
using TCodepointVec = std::vector<codepoint_t>;
using TCodepointRange = std::vector<TCodepointVec>;

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

template <typename CharT, typename Traits = std::char_traits<CharT>>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& stream, const TCodepointVec& rhs){
	std::for_each(
		  rhs.cbegin()
		, rhs.cend()
		, [&stream](const codepoint_t& a) {
			const auto arr =  CodePointToUTF8(a);
			stream << arr.data();
		}
	);
	return stream;
}

template<typename InputIt>
inline InputIt find(InputIt src_first, InputIt src_last, InputIt val_first, InputIt val_last){
	for(InputIt iter = src_first; iter != src_last; ++iter) {
		if(*iter == *val_first) {
			InputIt srcIter = std::next(iter);
			InputIt valIter = std::next(val_first);
			bool bIsEqual = true;
			for (; srcIter != src_last && valIter != val_last; ++srcIter, ++valIter) {
				if(*srcIter != *valIter){
					bIsEqual = false;
					break;
				}
			}
			if(valIter != val_last) {
				// Exited loop before we reached the end of our thing, keep going.
				bIsEqual = false;
			}
			if(bIsEqual){
				return iter;
			}
		}
	}
	return src_last;
}

inline std::vector<TCodepointRange> split_recursion(const TCodepointRange &range) {
#ifdef WITH_DEBUGGING
	std::cout << "Evaluating array: ";
	std::for_each(
		range.cbegin()
		, range.cend()
		, [](const TCodepointVec& a) {
			std::cout << "'" << a << "', ";
		}
	);
	std::cout << "\n";
#endif

	const auto tree = SuffixTree::Tree<TCodepointRange>{ range };
	const TCodepointVec longestSubstr = tree.LongestCommonSubstring();

#ifdef WITH_DEBUGGING
	std::cout << "Longest common substring: '" << longestSubstr << "'\n";
#endif

	std::vector<TCodepointRange> splitVectors;
	// Don't split substrings shorter than this range.
	// This is to avoid situations where we split on one letter, which is pointless.
	static constexpr size_t MIN_SIZE_FOR_SPLIT = 2;
	if (longestSubstr.size() >= MIN_SIZE_FOR_SPLIT ) {
		TCodepointRange leftSide{};
		TCodepointRange rightSide{};
		TCodepointRange haWoMukidashite{};
		std::for_each(range.cbegin(), range.cend(), [&](const TCodepointVec& pointVec){
			const auto distance = longestSubstr.size();
			const auto pointSec = find(pointVec.cbegin(), pointVec.cend(), longestSubstr.cbegin(), longestSubstr.cend());
			TCodepointVec leftVec{pointVec.cbegin(), pointSec};
			TCodepointVec midVec{pointSec, std::next(pointSec, distance)};
			TCodepointVec rightVec{std::next(pointSec, distance), pointVec.cend()};
			leftSide.push_back(leftVec);
			haWoMukidashite.push_back(midVec);
			rightSide.push_back(rightVec);
		});

		const auto leftSplit = split_recursion(leftSide);
		splitVectors.insert(splitVectors.cend(), leftSplit.cbegin(), leftSplit.cend());

		splitVectors.push_back(haWoMukidashite);

		const auto rightSplit = split_recursion(rightSide);
		splitVectors.insert(splitVectors.cend(), rightSplit.cbegin(), rightSplit.cend());
	}
	else {
		splitVectors.push_back(range);
	}

	return splitVectors;
}

inline TCodepointVec to_codepoints(const std::string& str) {
	TCodepointVec points;
	for (std::string::const_iterator iter = str.cbegin(); iter != str.cend();) {
		const codepoint_t point = UTF8ToCodePoint(iter);
		// Found bad code point, return empty string.
		if (point == bad_code_point) {
			return { bad_code_point };
		}
		points.push_back(point);
		const size_t byteSize = UTF8ByteSizeFromCodePoint(point);
		std::advance(iter, byteSize);
	}
	return points;
}

inline std::string to_string(const TCodepointVec& vec) {
	std::stringstream stream{};
	stream << vec;
	return stream.str();
}

template <typename TReal = double>
inline auto split_to_substring_list(const std::vector<std::pair<std::string, TReal>> &stringMap) {
	// Subdivide the string list into sequences of matching/different sequences.
	// E.g. "[The] [ UMBRELLA CORPORATION ] [released the] [ T-virus] [.]"
	//      "[De]  [ UMBRELLA CORPORATION ]   [heeft het]  [ T-virus] [ vrijgelaten.]"
	// Reason being that matching sequences will seemingly lerp into place, instead of noising.
	// Note that we disregard white-space, but this isn't really something we care about
	TCodepointRange transformed{};
	std::transform(
			stringMap.cbegin(), stringMap.cend(), std::back_inserter(transformed),
			[](const auto &pair) -> typename TCodepointRange::value_type {
				return to_codepoints(pair.first);
			});

	std::vector<TReal> weight_array;
	std::transform(
		stringMap.cbegin(), stringMap.cend(), std::back_inserter(weight_array),
		[](const auto &pair){return pair.second;});

	// TODO: Split format specifiers (as optional implementation)
	// as format specifiers can be in any order, but still need to react consistently.
	// E.g. "I love {0} and {1}" and "J'aime {1} et {0}" should
	// return either "ASDF {0} ASDF {1}" or "ASDF {1} ASDF {0}".
	// Since the localization will presumably be cached, we need to figure out this step,
	// Otherwise we have weird noise.
	return std::make_pair(split_recursion(transformed), weight_array);
}

// Naive interpolation that just weighs all the characters, and puts it in the middle.
// Simple is best.
template<typename TValue, typename TReal = double>
inline TValue interpolate_weighted(const std::vector<TValue>& char_list, const std::vector<TReal>& weight_array){
	TValue weighted_codepoint{};
	for (size_t i = 0; i < char_list.size(); ++i) {
		weighted_codepoint += static_cast<TValue>(weight_array[i] * char_list[i]);
	}
	return weighted_codepoint;
}

// More complicated interpolation, that takes into account the known unicode blocks,
// and attempts to fix the result within these code blocks.
template<typename TReal = double>
inline codepoint_t interpolate_single_char_blocks(const TCodepointVec& char_list, const std::vector<TReal>& weight_array){
	const auto minmax = std::minmax_element(char_list.cbegin(), char_list.cend());
	std::vector<unicode_block_range_t> used_ranges;
	size_t true_size = 0;
	for (const codepoint_t point : char_list) {
		unicode_block_range_t range = find_range(point);
		if(range != bad_code_point_range){
			if(std::find(used_ranges.cbegin(), used_ranges.cend(), range) == used_ranges.cend()){
				used_ranges.push_back(range);
			}
		}
		true_size += size_of_range(range);
	}
	std::sort(used_ranges.begin(), used_ranges.end());

	true_size -= *(minmax.first) - std::get<0>(*used_ranges.cbegin());
	true_size -= std::get<1>(*used_ranges.crbegin()) - (*(minmax.second) + 1);

	std::vector<size_t> offsets;
	std::transform(char_list.cbegin(), char_list.cend(), std::back_inserter(offsets),
		[&used_ranges, &minmax](const codepoint_t& point){
			size_t offset = 0;
			for (const unicode_block_range_t& range : used_ranges) {
				if(point >= std::get<0>(range) && point < std::get<1>(range)){
					offset += point - std::get<0>(range);
					break;
				} else {
					offset += size_of_range(range);
				}
			}
			offset -= *(minmax.first);
			return offset;
		}
	);

	size_t final_offset = interpolate_weighted(offsets, weight_array);
	// Find appropriate code point from ranges
	codepoint_t final_point = *minmax.first;
	if(final_point + final_offset >= std::get<1>(used_ranges[0])){
		final_offset -= size_of_range(used_ranges[0]) - *(minmax.first);
		for (size_t i = 1; i < used_ranges.size(); ++i) {
			if(std::get<0>(used_ranges[i]) + final_offset < std::get<1>(used_ranges[i])){
				return std::get<0>(used_ranges[i]) + final_offset;
			} else {
				final_offset -= size_of_range(used_ranges[i]);
			}
		}
	}
	else {
		return final_point + final_offset;
	}
	return bad_code_point;
}

template<typename TReal = double>
inline codepoint_t interpolate_single_char(const TCodepointVec& char_list, const std::vector<TReal>& weight_array){
	return interpolate_single_char_blocks(char_list,weight_array);
}

// Naive version of string interpolation which takes a list of strings, then interps each character individually.
template<typename TReal = double>
inline TCodepointVec interpolate_from_split_code_point_string_naive(const TCodepointRange& range, const std::vector<TReal>& weight_array){
	// If there are no elements in the range, return empty.
	if(range.size() == 0){
		return {};
	}

	// Return an empty string if we don't have a weight array.
	if(range.size() != weight_array.size()){
		return {};
	}

	size_t min_size = std::numeric_limits<size_t>::max();
	size_t max_size = std::numeric_limits<size_t>::lowest();
	TReal weighted_size_float = 0.0;
	for (size_t i = 0; i < range.size(); ++i) {
		const auto& vec = range[i];
		min_size = std::min(min_size, vec.size());
		max_size = std::max(max_size, vec.size());
		weighted_size_float += weight_array[i] * vec.size();
	}
	size_t weighted_size = static_cast<size_t>(std::round(weighted_size_float));

	// If all the strings in the range are empty, return an empty one.
	if(max_size <= 0){
		return {};
	}

	// If all elements are equal in the code point range, there's no point, so just return the first element
	if(range.cend() == std::adjacent_find(range.cbegin(), range.cend(), std::not_equal_to<TCodepointVec>{})){
		return *range.cbegin();
	}

	// Code point of the padded value.
	static constexpr const codepoint_t PADDING_CODEPOINT = '0';

	TCodepointVec result;
	result.reserve(weighted_size);

	for (size_t i = 0; i < weighted_size; ++i) {
		TCodepointVec points{};
		for (size_t range_idx = 0; range_idx < range.size(); ++range_idx) {
			const TCodepointVec& vec = range[range_idx];
			size_t vec_size = vec.size();
			if(i < vec_size){
				points.push_back(vec[i]);
			} else {
				points.push_back(PADDING_CODEPOINT);
			}
		}
		result.push_back(interpolate_single_char(points, weight_array));
	}

	return result;

}

// More visually appealing interpolation
template<typename TReal = double>
inline TCodepointVec interpolate_from_split_code_point_string_special(const TCodepointRange& range, const std::vector<TReal>& weight_array){
	// If there are no elements in the range, return empty.
	if(range.size() == 0){
		return {};
	}

	// Return an empty string if we don't have a weight array.
	if(range.size() != weight_array.size()){
		return {};
	}

	size_t min_size = std::numeric_limits<size_t>::max();
	size_t max_size = std::numeric_limits<size_t>::lowest();
	TReal weighted_size_float = 0.0;
	for (size_t i = 0; i < range.size(); ++i) {
		const auto& vec = range[i];
		min_size = std::min(min_size, vec.size());
		max_size = std::max(max_size, vec.size());
		weighted_size_float += weight_array[i] * vec.size();
	}
	size_t weighted_size = static_cast<size_t>(std::round(weighted_size_float));

	// If all the strings in the range are empty, return an empty one.
	if(max_size <= 0){
		return {};
	}

	// If all elements are equal in the code point range, there's no point, so just return the first element
	if(range.cend() == std::adjacent_find(range.cbegin(), range.cend(), std::not_equal_to<TCodepointVec>{})){
		return *range.cbegin();
	}

	// Code point of the padded value.
	static constexpr const codepoint_t PADDING_CODEPOINT = '0';

	TCodepointVec result;
	result.reserve(weighted_size);

	for (size_t i = 0; i < weighted_size; ++i) {
		TCodepointVec points{};
		for (size_t range_idx = 0; range_idx < range.size(); ++range_idx) {
			const TCodepointVec& vec = range[range_idx];
			size_t vec_size = vec.size();
			if(i < vec_size){
				points.push_back(vec[i]);
			} else {
				points.push_back(PADDING_CODEPOINT);
			}
		}
		result.push_back(interpolate_single_char(points, weight_array));
	}

	return result;

}

template<typename TReal = double>
inline TCodepointVec interpolate_from_split_code_point_string(const TCodepointRange& range, const std::vector<TReal>& weight_array){
	return interpolate_from_split_code_point_string_naive(range, weight_array);
}

} //namespace _TextLerpInternals

// Takes a map of strings (assuming UTF-8) and weights, and interpolates them.
// Tries to account for grapheme clustering.
// Assumes all strings are UTF-8, use of UTF-16 or UTF-32 is not valid.
// Assumes the sum of all weights equals 1.0.
template <typename TReal = double>
inline std::string interpolate(const std::vector<std::pair<std::string, TReal>> &weightedStringMap) {
	static_assert(std::is_floating_point<TReal>::value, "Can't be used with ints for weights (yet)");
	// If the map is empty, just return nothing.
	if (weightedStringMap.size() <= 0) {
		return {};
	}

	// If it only has one element, we don't need any interpolation.
	if(weightedStringMap.size() == 1) {
		return weightedStringMap.cbegin()->first;
	}

	using namespace _TextLerpInternals;

	const auto split_substrings_and_weight = split_to_substring_list(weightedStringMap);

	TCodepointVec finalCodePoints{};
	for (const auto &vec : split_substrings_and_weight.first) {
		const TCodepointVec points = interpolate_from_split_code_point_string(vec,split_substrings_and_weight.second);
		finalCodePoints.insert(
			finalCodePoints.cend(), points.cbegin(), points.cend()
		);
	}

	return to_string(finalCodePoints);
}

// Just to see if my includes are working
static_assert(std::is_floating_point<float>::value, "Includes broke, probably needs type_traits");

#endif // TEXT_INTERPOLATE_IMPL_H
