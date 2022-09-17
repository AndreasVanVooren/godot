/*************************************************************************/
/*  unicode_blocks.spec.cpp                                              */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "unicode_blocks.h"
#include <cstddef>

using UB = unicode_block_range_t;

static constexpr bool IsMinMax(const UB &a) {
	return std::get<0>(a) < std::get<1>(a);
}

static constexpr bool DoesNotOverlap(const UB &a, const UB &b) {
	return (std::get<0>(a) >= std::get<1>(b)) || (std::get<1>(a) <= std::get<0>(b));
}

static constexpr const size_t NUM_BLOCKS = sizeof(unicode_blocks) / sizeof(UB);
static constexpr const size_t ITER_NUM = NUM_BLOCKS / 8;
template <size_t ITER_INDEX>
static constexpr bool AreBlocksValid() {
	const size_t ITER_POSSIBLE_START = ITER_INDEX * ITER_NUM;
	const size_t ITER_POSSIBLE_END = (ITER_INDEX + 1) * ITER_NUM;
	const size_t ITER_END = ITER_POSSIBLE_END > NUM_BLOCKS ? NUM_BLOCKS : ITER_POSSIBLE_END;
	const size_t ITER_START = ITER_POSSIBLE_START > NUM_BLOCKS ? NUM_BLOCKS : ITER_POSSIBLE_START;
	for (size_t i = ITER_START; i < ITER_END; ++i) {
		const bool bIsMinMaxSafe = IsMinMax(unicode_blocks[i]);
		if (!bIsMinMaxSafe) {
			return false;
		}

		for (size_t j = i + 1; j < NUM_BLOCKS; ++j) {
			const bool bNoOverlap = DoesNotOverlap(unicode_blocks[i], unicode_blocks[j]);
			if (!bNoOverlap) {
				return false;
			}
		}
	}

	return true;
}

// Safety iteration over all blocks and then some.
static_assert(AreBlocksValid<0>(), "Blocks are not valid!");
static_assert(AreBlocksValid<1>(), "Blocks are not valid!");
static_assert(AreBlocksValid<2>(), "Blocks are not valid!");
static_assert(AreBlocksValid<3>(), "Blocks are not valid!");
static_assert(AreBlocksValid<4>(), "Blocks are not valid!");
static_assert(AreBlocksValid<5>(), "Blocks are not valid!");
static_assert(AreBlocksValid<6>(), "Blocks are not valid!");
static_assert(AreBlocksValid<7>(), "Blocks are not valid!");
static_assert(AreBlocksValid<8>(), "Blocks are not valid!");
static_assert(AreBlocksValid<9>(), "Blocks are not valid!");
static_assert(AreBlocksValid<10>(), "Blocks are not valid!");
static_assert(AreBlocksValid<11>(), "Blocks are not valid!");
static_assert(AreBlocksValid<12>(), "Blocks are not valid!");
static_assert(AreBlocksValid<13>(), "Blocks are not valid!");
static_assert(AreBlocksValid<14>(), "Blocks are not valid!");
static_assert(AreBlocksValid<15>(), "Blocks are not valid!");

//int main(){}
