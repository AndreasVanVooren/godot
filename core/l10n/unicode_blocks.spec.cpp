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
static constexpr bool IsBlockAtIndexValid(size_t index) {
	const bool bIsMinMaxSafe = IsMinMax(unicode_blocks[index]);
	if (!bIsMinMaxSafe) {
		return false;
	}

	for (size_t i = index + 1; i < NUM_BLOCKS; ++i) {
		const bool bNoOverlap = DoesNotOverlap(unicode_blocks[index], unicode_blocks[i]);
		if (!bNoOverlap) {
			return false;
		}
	}

	return true;
}


// Safety iteration over all blocks and then some.
static_assert(IsBlockAtIndexValid(0), "Block is not valid!");
static_assert(IsBlockAtIndexValid(1), "Block is not valid!");
static_assert(IsBlockAtIndexValid(2), "Block is not valid!");
static_assert(IsBlockAtIndexValid(3), "Block is not valid!");
static_assert(IsBlockAtIndexValid(4), "Block is not valid!");
static_assert(IsBlockAtIndexValid(5), "Block is not valid!");
static_assert(IsBlockAtIndexValid(6), "Block is not valid!");
static_assert(IsBlockAtIndexValid(7), "Block is not valid!");
static_assert(IsBlockAtIndexValid(8), "Block is not valid!");
static_assert(IsBlockAtIndexValid(9), "Block is not valid!");
static_assert(IsBlockAtIndexValid(10), "Block is not valid!");
static_assert(IsBlockAtIndexValid(11), "Block is not valid!");
static_assert(IsBlockAtIndexValid(12), "Block is not valid!");
static_assert(IsBlockAtIndexValid(13), "Block is not valid!");
static_assert(IsBlockAtIndexValid(14), "Block is not valid!");
static_assert(IsBlockAtIndexValid(15), "Block is not valid!");
static_assert(IsBlockAtIndexValid(16), "Block is not valid!");
static_assert(IsBlockAtIndexValid(17), "Block is not valid!");
static_assert(IsBlockAtIndexValid(18), "Block is not valid!");
static_assert(IsBlockAtIndexValid(19), "Block is not valid!");
static_assert(IsBlockAtIndexValid(20), "Block is not valid!");
static_assert(IsBlockAtIndexValid(21), "Block is not valid!");
static_assert(IsBlockAtIndexValid(22), "Block is not valid!");
static_assert(IsBlockAtIndexValid(23), "Block is not valid!");
static_assert(IsBlockAtIndexValid(24), "Block is not valid!");
static_assert(IsBlockAtIndexValid(25), "Block is not valid!");
static_assert(IsBlockAtIndexValid(26), "Block is not valid!");
static_assert(IsBlockAtIndexValid(27), "Block is not valid!");
static_assert(IsBlockAtIndexValid(28), "Block is not valid!");
static_assert(IsBlockAtIndexValid(29), "Block is not valid!");
static_assert(IsBlockAtIndexValid(30), "Block is not valid!");
static_assert(IsBlockAtIndexValid(31), "Block is not valid!");
static_assert(IsBlockAtIndexValid(32), "Block is not valid!");
static_assert(IsBlockAtIndexValid(33), "Block is not valid!");
static_assert(IsBlockAtIndexValid(34), "Block is not valid!");
static_assert(IsBlockAtIndexValid(35), "Block is not valid!");
static_assert(IsBlockAtIndexValid(36), "Block is not valid!");
static_assert(IsBlockAtIndexValid(37), "Block is not valid!");
static_assert(IsBlockAtIndexValid(38), "Block is not valid!");
static_assert(IsBlockAtIndexValid(39), "Block is not valid!");
static_assert(IsBlockAtIndexValid(40), "Block is not valid!");
static_assert(IsBlockAtIndexValid(41), "Block is not valid!");
static_assert(IsBlockAtIndexValid(42), "Block is not valid!");
static_assert(IsBlockAtIndexValid(43), "Block is not valid!");
static_assert(IsBlockAtIndexValid(44), "Block is not valid!");
static_assert(IsBlockAtIndexValid(45), "Block is not valid!");
static_assert(IsBlockAtIndexValid(46), "Block is not valid!");
static_assert(IsBlockAtIndexValid(47), "Block is not valid!");
static_assert(IsBlockAtIndexValid(48), "Block is not valid!");
static_assert(IsBlockAtIndexValid(49), "Block is not valid!");
static_assert(IsBlockAtIndexValid(50), "Block is not valid!");
static_assert(IsBlockAtIndexValid(51), "Block is not valid!");
static_assert(IsBlockAtIndexValid(52), "Block is not valid!");
static_assert(IsBlockAtIndexValid(53), "Block is not valid!");
static_assert(IsBlockAtIndexValid(54), "Block is not valid!");
static_assert(IsBlockAtIndexValid(55), "Block is not valid!");
static_assert(IsBlockAtIndexValid(56), "Block is not valid!");
static_assert(IsBlockAtIndexValid(57), "Block is not valid!");
static_assert(IsBlockAtIndexValid(58), "Block is not valid!");
static_assert(IsBlockAtIndexValid(59), "Block is not valid!");
static_assert(IsBlockAtIndexValid(60), "Block is not valid!");
static_assert(IsBlockAtIndexValid(61), "Block is not valid!");
static_assert(IsBlockAtIndexValid(62), "Block is not valid!");
static_assert(IsBlockAtIndexValid(63), "Block is not valid!");
static_assert(IsBlockAtIndexValid(64), "Block is not valid!");
static_assert(IsBlockAtIndexValid(65), "Block is not valid!");
static_assert(IsBlockAtIndexValid(66), "Block is not valid!");
static_assert(IsBlockAtIndexValid(67), "Block is not valid!");
static_assert(IsBlockAtIndexValid(68), "Block is not valid!");
static_assert(IsBlockAtIndexValid(69), "Block is not valid!");
static_assert(IsBlockAtIndexValid(70), "Block is not valid!");
static_assert(IsBlockAtIndexValid(71), "Block is not valid!");
static_assert(IsBlockAtIndexValid(72), "Block is not valid!");
static_assert(IsBlockAtIndexValid(73), "Block is not valid!");
static_assert(IsBlockAtIndexValid(74), "Block is not valid!");
static_assert(IsBlockAtIndexValid(75), "Block is not valid!");
static_assert(IsBlockAtIndexValid(76), "Block is not valid!");
static_assert(IsBlockAtIndexValid(77), "Block is not valid!");
static_assert(IsBlockAtIndexValid(78), "Block is not valid!");
static_assert(IsBlockAtIndexValid(79), "Block is not valid!");
static_assert(IsBlockAtIndexValid(80), "Block is not valid!");
static_assert(IsBlockAtIndexValid(81), "Block is not valid!");
static_assert(IsBlockAtIndexValid(82), "Block is not valid!");
static_assert(IsBlockAtIndexValid(83), "Block is not valid!");
static_assert(IsBlockAtIndexValid(84), "Block is not valid!");
static_assert(IsBlockAtIndexValid(85), "Block is not valid!");
static_assert(IsBlockAtIndexValid(86), "Block is not valid!");
static_assert(IsBlockAtIndexValid(87), "Block is not valid!");
static_assert(IsBlockAtIndexValid(88), "Block is not valid!");
static_assert(IsBlockAtIndexValid(89), "Block is not valid!");
static_assert(IsBlockAtIndexValid(90), "Block is not valid!");
static_assert(IsBlockAtIndexValid(91), "Block is not valid!");
static_assert(IsBlockAtIndexValid(92), "Block is not valid!");
static_assert(IsBlockAtIndexValid(93), "Block is not valid!");
static_assert(IsBlockAtIndexValid(94), "Block is not valid!");
static_assert(IsBlockAtIndexValid(95), "Block is not valid!");
static_assert(IsBlockAtIndexValid(96), "Block is not valid!");
static_assert(IsBlockAtIndexValid(97), "Block is not valid!");
static_assert(IsBlockAtIndexValid(98), "Block is not valid!");
static_assert(IsBlockAtIndexValid(99), "Block is not valid!");
static_assert(IsBlockAtIndexValid(100), "Block is not valid!");
static_assert(IsBlockAtIndexValid(101), "Block is not valid!");
static_assert(IsBlockAtIndexValid(102), "Block is not valid!");
static_assert(IsBlockAtIndexValid(103), "Block is not valid!");
static_assert(IsBlockAtIndexValid(104), "Block is not valid!");
static_assert(IsBlockAtIndexValid(105), "Block is not valid!");
static_assert(IsBlockAtIndexValid(106), "Block is not valid!");
static_assert(IsBlockAtIndexValid(107), "Block is not valid!");
static_assert(IsBlockAtIndexValid(108), "Block is not valid!");
static_assert(IsBlockAtIndexValid(109), "Block is not valid!");
static_assert(IsBlockAtIndexValid(110), "Block is not valid!");
static_assert(IsBlockAtIndexValid(111), "Block is not valid!");
static_assert(IsBlockAtIndexValid(112), "Block is not valid!");
static_assert(IsBlockAtIndexValid(113), "Block is not valid!");
static_assert(IsBlockAtIndexValid(114), "Block is not valid!");
static_assert(IsBlockAtIndexValid(115), "Block is not valid!");
static_assert(IsBlockAtIndexValid(116), "Block is not valid!");
static_assert(IsBlockAtIndexValid(117), "Block is not valid!");
static_assert(IsBlockAtIndexValid(118), "Block is not valid!");
static_assert(IsBlockAtIndexValid(119), "Block is not valid!");
static_assert(IsBlockAtIndexValid(120), "Block is not valid!");
static_assert(IsBlockAtIndexValid(121), "Block is not valid!");
static_assert(IsBlockAtIndexValid(122), "Block is not valid!");
static_assert(IsBlockAtIndexValid(123), "Block is not valid!");
static_assert(IsBlockAtIndexValid(124), "Block is not valid!");
static_assert(IsBlockAtIndexValid(125), "Block is not valid!");
static_assert(IsBlockAtIndexValid(126), "Block is not valid!");
static_assert(IsBlockAtIndexValid(127), "Block is not valid!");
static_assert(IsBlockAtIndexValid(128), "Block is not valid!");
static_assert(IsBlockAtIndexValid(129), "Block is not valid!");
static_assert(IsBlockAtIndexValid(130), "Block is not valid!");
static_assert(IsBlockAtIndexValid(131), "Block is not valid!");
static_assert(IsBlockAtIndexValid(132), "Block is not valid!");
static_assert(IsBlockAtIndexValid(133), "Block is not valid!");
static_assert(IsBlockAtIndexValid(134), "Block is not valid!");
static_assert(IsBlockAtIndexValid(135), "Block is not valid!");
static_assert(IsBlockAtIndexValid(136), "Block is not valid!");
static_assert(IsBlockAtIndexValid(137), "Block is not valid!");
static_assert(IsBlockAtIndexValid(138), "Block is not valid!");
static_assert(IsBlockAtIndexValid(139), "Block is not valid!");
static_assert(IsBlockAtIndexValid(140), "Block is not valid!");
static_assert(IsBlockAtIndexValid(141), "Block is not valid!");
static_assert(IsBlockAtIndexValid(142), "Block is not valid!");
static_assert(IsBlockAtIndexValid(143), "Block is not valid!");
static_assert(IsBlockAtIndexValid(144), "Block is not valid!");
static_assert(IsBlockAtIndexValid(145), "Block is not valid!");
static_assert(IsBlockAtIndexValid(146), "Block is not valid!");
static_assert(IsBlockAtIndexValid(147), "Block is not valid!");
static_assert(IsBlockAtIndexValid(148), "Block is not valid!");
static_assert(IsBlockAtIndexValid(149), "Block is not valid!");
static_assert(IsBlockAtIndexValid(150), "Block is not valid!");
static_assert(IsBlockAtIndexValid(151), "Block is not valid!");
static_assert(IsBlockAtIndexValid(152), "Block is not valid!");
static_assert(IsBlockAtIndexValid(153), "Block is not valid!");
static_assert(IsBlockAtIndexValid(154), "Block is not valid!");
static_assert(IsBlockAtIndexValid(155), "Block is not valid!");
static_assert(IsBlockAtIndexValid(156), "Block is not valid!");
static_assert(IsBlockAtIndexValid(157), "Block is not valid!");
static_assert(IsBlockAtIndexValid(158), "Block is not valid!");
static_assert(IsBlockAtIndexValid(159), "Block is not valid!");
static_assert(IsBlockAtIndexValid(160), "Block is not valid!");
static_assert(IsBlockAtIndexValid(161), "Block is not valid!");
static_assert(IsBlockAtIndexValid(162), "Block is not valid!");
static_assert(IsBlockAtIndexValid(163), "Block is not valid!");
static_assert(IsBlockAtIndexValid(164), "Block is not valid!");
static_assert(IsBlockAtIndexValid(165), "Block is not valid!");
static_assert(IsBlockAtIndexValid(166), "Block is not valid!");
static_assert(IsBlockAtIndexValid(167), "Block is not valid!");
static_assert(IsBlockAtIndexValid(168), "Block is not valid!");
static_assert(IsBlockAtIndexValid(169), "Block is not valid!");
static_assert(IsBlockAtIndexValid(170), "Block is not valid!");
static_assert(IsBlockAtIndexValid(171), "Block is not valid!");
static_assert(IsBlockAtIndexValid(172), "Block is not valid!");
static_assert(IsBlockAtIndexValid(173), "Block is not valid!");
static_assert(IsBlockAtIndexValid(174), "Block is not valid!");
static_assert(IsBlockAtIndexValid(175), "Block is not valid!");
static_assert(IsBlockAtIndexValid(176), "Block is not valid!");
static_assert(IsBlockAtIndexValid(177), "Block is not valid!");
static_assert(IsBlockAtIndexValid(178), "Block is not valid!");
static_assert(IsBlockAtIndexValid(179), "Block is not valid!");
static_assert(IsBlockAtIndexValid(180), "Block is not valid!");
static_assert(IsBlockAtIndexValid(181), "Block is not valid!");
static_assert(IsBlockAtIndexValid(182), "Block is not valid!");
static_assert(IsBlockAtIndexValid(183), "Block is not valid!");
static_assert(IsBlockAtIndexValid(184), "Block is not valid!");
static_assert(IsBlockAtIndexValid(185), "Block is not valid!");
static_assert(IsBlockAtIndexValid(186), "Block is not valid!");
static_assert(IsBlockAtIndexValid(187), "Block is not valid!");
static_assert(IsBlockAtIndexValid(188), "Block is not valid!");
static_assert(IsBlockAtIndexValid(189), "Block is not valid!");
static_assert(IsBlockAtIndexValid(190), "Block is not valid!");
static_assert(IsBlockAtIndexValid(191), "Block is not valid!");
static_assert(IsBlockAtIndexValid(192), "Block is not valid!");
static_assert(IsBlockAtIndexValid(193), "Block is not valid!");
static_assert(IsBlockAtIndexValid(194), "Block is not valid!");
static_assert(IsBlockAtIndexValid(195), "Block is not valid!");
static_assert(IsBlockAtIndexValid(196), "Block is not valid!");
static_assert(IsBlockAtIndexValid(197), "Block is not valid!");
static_assert(IsBlockAtIndexValid(198), "Block is not valid!");
static_assert(IsBlockAtIndexValid(199), "Block is not valid!");
static_assert(IsBlockAtIndexValid(200), "Block is not valid!");
static_assert(IsBlockAtIndexValid(201), "Block is not valid!");
static_assert(IsBlockAtIndexValid(202), "Block is not valid!");
static_assert(IsBlockAtIndexValid(203), "Block is not valid!");
static_assert(IsBlockAtIndexValid(204), "Block is not valid!");
static_assert(IsBlockAtIndexValid(205), "Block is not valid!");
static_assert(IsBlockAtIndexValid(206), "Block is not valid!");
static_assert(IsBlockAtIndexValid(207), "Block is not valid!");
static_assert(IsBlockAtIndexValid(208), "Block is not valid!");
static_assert(IsBlockAtIndexValid(209), "Block is not valid!");
static_assert(IsBlockAtIndexValid(210), "Block is not valid!");
static_assert(IsBlockAtIndexValid(211), "Block is not valid!");
static_assert(IsBlockAtIndexValid(212), "Block is not valid!");
static_assert(IsBlockAtIndexValid(213), "Block is not valid!");
static_assert(IsBlockAtIndexValid(214), "Block is not valid!");
static_assert(IsBlockAtIndexValid(215), "Block is not valid!");
static_assert(IsBlockAtIndexValid(216), "Block is not valid!");
static_assert(IsBlockAtIndexValid(217), "Block is not valid!");
static_assert(IsBlockAtIndexValid(218), "Block is not valid!");
static_assert(IsBlockAtIndexValid(219), "Block is not valid!");
static_assert(IsBlockAtIndexValid(220), "Block is not valid!");
static_assert(IsBlockAtIndexValid(221), "Block is not valid!");
static_assert(IsBlockAtIndexValid(222), "Block is not valid!");
static_assert(IsBlockAtIndexValid(223), "Block is not valid!");
static_assert(IsBlockAtIndexValid(224), "Block is not valid!");
static_assert(IsBlockAtIndexValid(225), "Block is not valid!");
static_assert(IsBlockAtIndexValid(226), "Block is not valid!");
static_assert(IsBlockAtIndexValid(227), "Block is not valid!");
static_assert(IsBlockAtIndexValid(228), "Block is not valid!");
static_assert(IsBlockAtIndexValid(229), "Block is not valid!");
static_assert(IsBlockAtIndexValid(230), "Block is not valid!");
static_assert(IsBlockAtIndexValid(231), "Block is not valid!");
static_assert(IsBlockAtIndexValid(232), "Block is not valid!");
static_assert(IsBlockAtIndexValid(233), "Block is not valid!");
static_assert(IsBlockAtIndexValid(234), "Block is not valid!");
static_assert(IsBlockAtIndexValid(235), "Block is not valid!");
static_assert(IsBlockAtIndexValid(236), "Block is not valid!");
static_assert(IsBlockAtIndexValid(237), "Block is not valid!");
static_assert(IsBlockAtIndexValid(238), "Block is not valid!");
static_assert(IsBlockAtIndexValid(239), "Block is not valid!");
static_assert(IsBlockAtIndexValid(240), "Block is not valid!");
static_assert(IsBlockAtIndexValid(241), "Block is not valid!");
static_assert(IsBlockAtIndexValid(242), "Block is not valid!");
static_assert(IsBlockAtIndexValid(243), "Block is not valid!");
static_assert(IsBlockAtIndexValid(244), "Block is not valid!");
static_assert(IsBlockAtIndexValid(245), "Block is not valid!");
static_assert(IsBlockAtIndexValid(246), "Block is not valid!");
static_assert(IsBlockAtIndexValid(247), "Block is not valid!");
static_assert(IsBlockAtIndexValid(248), "Block is not valid!");
static_assert(IsBlockAtIndexValid(249), "Block is not valid!");
static_assert(IsBlockAtIndexValid(250), "Block is not valid!");
static_assert(IsBlockAtIndexValid(251), "Block is not valid!");
static_assert(IsBlockAtIndexValid(252), "Block is not valid!");
static_assert(IsBlockAtIndexValid(253), "Block is not valid!");
static_assert(IsBlockAtIndexValid(254), "Block is not valid!");
static_assert(IsBlockAtIndexValid(255), "Block is not valid!");
static_assert(IsBlockAtIndexValid(256), "Block is not valid!");
static_assert(IsBlockAtIndexValid(257), "Block is not valid!");
static_assert(IsBlockAtIndexValid(258), "Block is not valid!");
static_assert(IsBlockAtIndexValid(259), "Block is not valid!");
static_assert(IsBlockAtIndexValid(260), "Block is not valid!");
static_assert(IsBlockAtIndexValid(261), "Block is not valid!");
static_assert(IsBlockAtIndexValid(262), "Block is not valid!");
static_assert(IsBlockAtIndexValid(263), "Block is not valid!");
static_assert(IsBlockAtIndexValid(264), "Block is not valid!");
static_assert(IsBlockAtIndexValid(265), "Block is not valid!");
static_assert(IsBlockAtIndexValid(266), "Block is not valid!");
static_assert(IsBlockAtIndexValid(267), "Block is not valid!");
static_assert(IsBlockAtIndexValid(268), "Block is not valid!");
static_assert(IsBlockAtIndexValid(269), "Block is not valid!");
static_assert(IsBlockAtIndexValid(270), "Block is not valid!");
static_assert(IsBlockAtIndexValid(271), "Block is not valid!");
static_assert(IsBlockAtIndexValid(272), "Block is not valid!");
static_assert(IsBlockAtIndexValid(273), "Block is not valid!");
static_assert(IsBlockAtIndexValid(274), "Block is not valid!");
static_assert(IsBlockAtIndexValid(275), "Block is not valid!");
static_assert(IsBlockAtIndexValid(276), "Block is not valid!");
static_assert(IsBlockAtIndexValid(277), "Block is not valid!");
static_assert(IsBlockAtIndexValid(278), "Block is not valid!");
static_assert(IsBlockAtIndexValid(279), "Block is not valid!");
static_assert(IsBlockAtIndexValid(280), "Block is not valid!");
static_assert(IsBlockAtIndexValid(281), "Block is not valid!");
static_assert(IsBlockAtIndexValid(282), "Block is not valid!");
static_assert(IsBlockAtIndexValid(283), "Block is not valid!");
static_assert(IsBlockAtIndexValid(284), "Block is not valid!");
static_assert(IsBlockAtIndexValid(285), "Block is not valid!");
static_assert(IsBlockAtIndexValid(286), "Block is not valid!");
static_assert(IsBlockAtIndexValid(287), "Block is not valid!");
static_assert(IsBlockAtIndexValid(288), "Block is not valid!");
static_assert(IsBlockAtIndexValid(289), "Block is not valid!");
static_assert(IsBlockAtIndexValid(290), "Block is not valid!");
static_assert(IsBlockAtIndexValid(291), "Block is not valid!");
static_assert(IsBlockAtIndexValid(292), "Block is not valid!");
static_assert(IsBlockAtIndexValid(293), "Block is not valid!");
static_assert(IsBlockAtIndexValid(294), "Block is not valid!");
static_assert(IsBlockAtIndexValid(295), "Block is not valid!");
static_assert(IsBlockAtIndexValid(296), "Block is not valid!");
static_assert(IsBlockAtIndexValid(297), "Block is not valid!");
static_assert(IsBlockAtIndexValid(298), "Block is not valid!");
static_assert(IsBlockAtIndexValid(299), "Block is not valid!");
static_assert(IsBlockAtIndexValid(300), "Block is not valid!");
static_assert(IsBlockAtIndexValid(301), "Block is not valid!");
static_assert(IsBlockAtIndexValid(302), "Block is not valid!");
static_assert(IsBlockAtIndexValid(303), "Block is not valid!");
static_assert(IsBlockAtIndexValid(304), "Block is not valid!");
static_assert(IsBlockAtIndexValid(305), "Block is not valid!");
static_assert(IsBlockAtIndexValid(306), "Block is not valid!");
static_assert(IsBlockAtIndexValid(307), "Block is not valid!");
static_assert(IsBlockAtIndexValid(308), "Block is not valid!");
static_assert(IsBlockAtIndexValid(309), "Block is not valid!");
static_assert(IsBlockAtIndexValid(310), "Block is not valid!");
static_assert(IsBlockAtIndexValid(311), "Block is not valid!");
static_assert(IsBlockAtIndexValid(312), "Block is not valid!");
static_assert(IsBlockAtIndexValid(313), "Block is not valid!");
static_assert(IsBlockAtIndexValid(314), "Block is not valid!");
static_assert(IsBlockAtIndexValid(315), "Block is not valid!");
static_assert(IsBlockAtIndexValid(316), "Block is not valid!");
static_assert(IsBlockAtIndexValid(317), "Block is not valid!");
static_assert(IsBlockAtIndexValid(318), "Block is not valid!");
static_assert(IsBlockAtIndexValid(319), "Block is not valid!");
static_assert(IsBlockAtIndexValid(320), "Block is not valid!");
static_assert(IsBlockAtIndexValid(321), "Block is not valid!");
static_assert(IsBlockAtIndexValid(322), "Block is not valid!");
static_assert(IsBlockAtIndexValid(323), "Block is not valid!");
static_assert(IsBlockAtIndexValid(324), "Block is not valid!");
static_assert(IsBlockAtIndexValid(325), "Block is not valid!");
static_assert(IsBlockAtIndexValid(326), "Block is not valid!");

//int main(){}
