#include "unicode_blocks.h"
#include <cstddef>

using UB = UnicodeBlockRange;

static constexpr bool IsMinMax(const UB &a) {
	return std::get<0>(a) < std::get<1>(a);
}

static constexpr bool DoesNotOverlap(const UB &a, const UB &b) {
	return (std::get<0>(a) > std::get<1>(b)) || (std::get<1>(a) < std::get<0>(b));
}

static constexpr const size_t NUM_BLOCKS = sizeof(Blocks) / sizeof(UB);
static constexpr const size_t ITER_NUM = NUM_BLOCKS / 8;
template <size_t ITER_INDEX>
static constexpr bool AreBlocksValid() {
	const size_t ITER_POSSIBLE_START = ITER_INDEX * ITER_NUM;
	const size_t ITER_POSSIBLE_END = (ITER_INDEX + 1) * ITER_NUM;
	const size_t ITER_END = ITER_POSSIBLE_END > NUM_BLOCKS ? NUM_BLOCKS : ITER_POSSIBLE_END;
	const size_t ITER_START = ITER_POSSIBLE_START > NUM_BLOCKS ? NUM_BLOCKS : ITER_POSSIBLE_START;
	for (size_t i = ITER_START; i < ITER_END; ++i) {
		const bool bIsMinMaxSafe = IsMinMax(Blocks[i]);
		if (!bIsMinMaxSafe) {
			return false;
		}

		for (size_t j = i + 1; j < NUM_BLOCKS; ++j) {
			const bool bNoOverlap = DoesNotOverlap(Blocks[i], Blocks[j]);
			if (!bNoOverlap) {
				return false;
			}
		}
	}

	return true;
}

// Safety iteration over all blocks and then some.
static_assert(AreBlocksValid< 0>(), "Blocks are not valid!");
static_assert(AreBlocksValid< 1>(), "Blocks are not valid!");
static_assert(AreBlocksValid< 2>(), "Blocks are not valid!");
static_assert(AreBlocksValid< 3>(), "Blocks are not valid!");
static_assert(AreBlocksValid< 4>(), "Blocks are not valid!");
static_assert(AreBlocksValid< 5>(), "Blocks are not valid!");
static_assert(AreBlocksValid< 6>(), "Blocks are not valid!");
static_assert(AreBlocksValid< 7>(), "Blocks are not valid!");
static_assert(AreBlocksValid< 8>(), "Blocks are not valid!");
static_assert(AreBlocksValid< 9>(), "Blocks are not valid!");
static_assert(AreBlocksValid<10>(), "Blocks are not valid!");
static_assert(AreBlocksValid<11>(), "Blocks are not valid!");
static_assert(AreBlocksValid<12>(), "Blocks are not valid!");
static_assert(AreBlocksValid<13>(), "Blocks are not valid!");
static_assert(AreBlocksValid<14>(), "Blocks are not valid!");
static_assert(AreBlocksValid<15>(), "Blocks are not valid!");