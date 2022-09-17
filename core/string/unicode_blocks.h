#ifndef UNICODE_BLOCKS_IMPL
#define UNICODE_BLOCKS_IMPL

#include <cstdint>
#include <tuple>

using codepoint_t = uint32_t;
using unicode_block_range_t = std::tuple<codepoint_t, codepoint_t>;
static constexpr codepoint_t bad_code_point = static_cast<codepoint_t>(-1);
static constexpr unicode_block_range_t bad_code_point_range {bad_code_point, bad_code_point};

// Blocks in unicode file are formatted in the form startCode..endCode; name
// If this needs to update, use regex ([0-9A-F]*)\.\.([0-9A-F]*); (.*)
// and replace with {0x$1, 0x$2}, // $3
static constexpr const unicode_block_range_t unicode_blocks[]{
	{0x0000, 0x007F+1}, // Basic Latin
	{0x0080, 0x00FF+1}, // Latin-1 Supplement
	{0x0100, 0x017F+1}, // Latin Extended-A
	{0x0180, 0x024F+1}, // Latin Extended-B
	{0x0250, 0x02AF+1}, // IPA Extensions
	{0x02B0, 0x02FF+1}, // Spacing Modifier Letters
	{0x0300, 0x036F+1}, // Combining Diacritical Marks
	{0x0370, 0x03FF+1}, // Greek and Coptic
	{0x0400, 0x04FF+1}, // Cyrillic
	{0x0500, 0x052F+1}, // Cyrillic Supplement
	{0x0530, 0x058F+1}, // Armenian
	{0x0590, 0x05FF+1}, // Hebrew
	{0x0600, 0x06FF+1}, // Arabic
	{0x0700, 0x074F+1}, // Syriac
	{0x0750, 0x077F+1}, // Arabic Supplement
	{0x0780, 0x07BF+1}, // Thaana
	{0x07C0, 0x07FF+1}, // NKo
	{0x0800, 0x083F+1}, // Samaritan
	{0x0840, 0x085F+1}, // Mandaic
	{0x0860, 0x086F+1}, // Syriac Supplement
	{0x0870, 0x089F+1}, // Arabic Extended-B
	{0x08A0, 0x08FF+1}, // Arabic Extended-A
	{0x0900, 0x097F+1}, // Devanagari
	{0x0980, 0x09FF+1}, // Bengali
	{0x0A00, 0x0A7F+1}, // Gurmukhi
	{0x0A80, 0x0AFF+1}, // Gujarati
	{0x0B00, 0x0B7F+1}, // Oriya
	{0x0B80, 0x0BFF+1}, // Tamil
	{0x0C00, 0x0C7F+1}, // Telugu
	{0x0C80, 0x0CFF+1}, // Kannada
	{0x0D00, 0x0D7F+1}, // Malayalam
	{0x0D80, 0x0DFF+1}, // Sinhala
	{0x0E00, 0x0E7F+1}, // Thai
	{0x0E80, 0x0EFF+1}, // Lao
	{0x0F00, 0x0FFF+1}, // Tibetan
	{0x1000, 0x109F+1}, // Myanmar
	{0x10A0, 0x10FF+1}, // Georgian
	{0x1100, 0x11FF+1}, // Hangul Jamo
	{0x1200, 0x137F+1}, // Ethiopic
	{0x1380, 0x139F+1}, // Ethiopic Supplement
	{0x13A0, 0x13FF+1}, // Cherokee
	{0x1400, 0x167F+1}, // Unified Canadian Aboriginal Syllabics
	{0x1680, 0x169F+1}, // Ogham
	{0x16A0, 0x16FF+1}, // Runic
	{0x1700, 0x171F+1}, // Tagalog
	{0x1720, 0x173F+1}, // Hanunoo
	{0x1740, 0x175F+1}, // Buhid
	{0x1760, 0x177F+1}, // Tagbanwa
	{0x1780, 0x17FF+1}, // Khmer
	{0x1800, 0x18AF+1}, // Mongolian
	{0x18B0, 0x18FF+1}, // Unified Canadian Aboriginal Syllabics Extended
	{0x1900, 0x194F+1}, // Limbu
	{0x1950, 0x197F+1}, // Tai Le
	{0x1980, 0x19DF+1}, // New Tai Lue
	{0x19E0, 0x19FF+1}, // Khmer Symbols
	{0x1A00, 0x1A1F+1}, // Buginese
	{0x1A20, 0x1AAF+1}, // Tai Tham
	{0x1AB0, 0x1AFF+1}, // Combining Diacritical Marks Extended
	{0x1B00, 0x1B7F+1}, // Balinese
	{0x1B80, 0x1BBF+1}, // Sundanese
	{0x1BC0, 0x1BFF+1}, // Batak
	{0x1C00, 0x1C4F+1}, // Lepcha
	{0x1C50, 0x1C7F+1}, // Ol Chiki
	{0x1C80, 0x1C8F+1}, // Cyrillic Extended-C
	{0x1C90, 0x1CBF+1}, // Georgian Extended
	{0x1CC0, 0x1CCF+1}, // Sundanese Supplement
	{0x1CD0, 0x1CFF+1}, // Vedic Extensions
	{0x1D00, 0x1D7F+1}, // Phonetic Extensions
	{0x1D80, 0x1DBF+1}, // Phonetic Extensions Supplement
	{0x1DC0, 0x1DFF+1}, // Combining Diacritical Marks Supplement
	{0x1E00, 0x1EFF+1}, // Latin Extended Additional
	{0x1F00, 0x1FFF+1}, // Greek Extended
	{0x2000, 0x206F+1}, // General Punctuation
	{0x2070, 0x209F+1}, // Superscripts and Subscripts
	{0x20A0, 0x20CF+1}, // Currency Symbols
	{0x20D0, 0x20FF+1}, // Combining Diacritical Marks for Symbols
	{0x2100, 0x214F+1}, // Letterlike Symbols
	{0x2150, 0x218F+1}, // Number Forms
	{0x2190, 0x21FF+1}, // Arrows
	{0x2200, 0x22FF+1}, // Mathematical Operators
	{0x2300, 0x23FF+1}, // Miscellaneous Technical
	{0x2400, 0x243F+1}, // Control Pictures
	{0x2440, 0x245F+1}, // Optical Character Recognition
	{0x2460, 0x24FF+1}, // Enclosed Alphanumerics
	{0x2500, 0x257F+1}, // Box Drawing
	{0x2580, 0x259F+1}, // Block Elements
	{0x25A0, 0x25FF+1}, // Geometric Shapes
	{0x2600, 0x26FF+1}, // Miscellaneous Symbols
	{0x2700, 0x27BF+1}, // Dingbats
	{0x27C0, 0x27EF+1}, // Miscellaneous Mathematical Symbols-A
	{0x27F0, 0x27FF+1}, // Supplemental Arrows-A
	{0x2800, 0x28FF+1}, // Braille Patterns
	{0x2900, 0x297F+1}, // Supplemental Arrows-B
	{0x2980, 0x29FF+1}, // Miscellaneous Mathematical Symbols-B
	{0x2A00, 0x2AFF+1}, // Supplemental Mathematical Operators
	{0x2B00, 0x2BFF+1}, // Miscellaneous Symbols and Arrows
	{0x2C00, 0x2C5F+1}, // Glagolitic
	{0x2C60, 0x2C7F+1}, // Latin Extended-C
	{0x2C80, 0x2CFF+1}, // Coptic
	{0x2D00, 0x2D2F+1}, // Georgian Supplement
	{0x2D30, 0x2D7F+1}, // Tifinagh
	{0x2D80, 0x2DDF+1}, // Ethiopic Extended
	{0x2DE0, 0x2DFF+1}, // Cyrillic Extended-A
	{0x2E00, 0x2E7F+1}, // Supplemental Punctuation
	{0x2E80, 0x2EFF+1}, // CJK Radicals Supplement
	{0x2F00, 0x2FDF+1}, // Kangxi Radicals
	{0x2FF0, 0x2FFF+1}, // Ideographic Description Characters
	{0x3000, 0x303F+1}, // CJK Symbols and Punctuation
	{0x3040, 0x309F+1}, // Hiragana
	{0x30A0, 0x30FF+1}, // Katakana
	{0x3100, 0x312F+1}, // Bopomofo
	{0x3130, 0x318F+1}, // Hangul Compatibility Jamo
	{0x3190, 0x319F+1}, // Kanbun
	{0x31A0, 0x31BF+1}, // Bopomofo Extended
	{0x31C0, 0x31EF+1}, // CJK Strokes
	{0x31F0, 0x31FF+1}, // Katakana Phonetic Extensions
	{0x3200, 0x32FF+1}, // Enclosed CJK Letters and Months
	{0x3300, 0x33FF+1}, // CJK Compatibility
	{0x3400, 0x4DBF+1}, // CJK Unified Ideographs Extension A
	{0x4DC0, 0x4DFF+1}, // Yijing Hexagram Symbols
	{0x4E00, 0x9FFF+1}, // CJK Unified Ideographs
	{0xA000, 0xA48F+1}, // Yi Syllables
	{0xA490, 0xA4CF+1}, // Yi Radicals
	{0xA4D0, 0xA4FF+1}, // Lisu
	{0xA500, 0xA63F+1}, // Vai
	{0xA640, 0xA69F+1}, // Cyrillic Extended-B
	{0xA6A0, 0xA6FF+1}, // Bamum
	{0xA700, 0xA71F+1}, // Modifier Tone Letters
	{0xA720, 0xA7FF+1}, // Latin Extended-D
	{0xA800, 0xA82F+1}, // Syloti Nagri
	{0xA830, 0xA83F+1}, // Common Indic Number Forms
	{0xA840, 0xA87F+1}, // Phags-pa
	{0xA880, 0xA8DF+1}, // Saurashtra
	{0xA8E0, 0xA8FF+1}, // Devanagari Extended
	{0xA900, 0xA92F+1}, // Kayah Li
	{0xA930, 0xA95F+1}, // Rejang
	{0xA960, 0xA97F+1}, // Hangul Jamo Extended-A
	{0xA980, 0xA9DF+1}, // Javanese
	{0xA9E0, 0xA9FF+1}, // Myanmar Extended-B
	{0xAA00, 0xAA5F+1}, // Cham
	{0xAA60, 0xAA7F+1}, // Myanmar Extended-A
	{0xAA80, 0xAADF+1}, // Tai Viet
	{0xAAE0, 0xAAFF+1}, // Meetei Mayek Extensions
	{0xAB00, 0xAB2F+1}, // Ethiopic Extended-A
	{0xAB30, 0xAB6F+1}, // Latin Extended-E
	{0xAB70, 0xABBF+1}, // Cherokee Supplement
	{0xABC0, 0xABFF+1}, // Meetei Mayek
	{0xAC00, 0xD7AF+1}, // Hangul Syllables
	{0xD7B0, 0xD7FF+1}, // Hangul Jamo Extended-B
	{0xD800, 0xDB7F+1}, // High Surrogates
	{0xDB80, 0xDBFF+1}, // High Private Use Surrogates
	{0xDC00, 0xDFFF+1}, // Low Surrogates
	{0xE000, 0xF8FF+1}, // Private Use Area
	{0xF900, 0xFAFF+1}, // CJK Compatibility Ideographs
	{0xFB00, 0xFB4F+1}, // Alphabetic Presentation Forms
	{0xFB50, 0xFDFF+1}, // Arabic Presentation Forms-A
	{0xFE00, 0xFE0F+1}, // Variation Selectors
	{0xFE10, 0xFE1F+1}, // Vertical Forms
	{0xFE20, 0xFE2F+1}, // Combining Half Marks
	{0xFE30, 0xFE4F+1}, // CJK Compatibility Forms
	{0xFE50, 0xFE6F+1}, // Small Form Variants
	{0xFE70, 0xFEFF+1}, // Arabic Presentation Forms-B
	{0xFF00, 0xFFEF+1}, // Halfwidth and Fullwidth Forms
	{0xFFF0, 0xFFFF+1}, // Specials
	{0x10000, 0x1007F+1}, // Linear B Syllabary
	{0x10080, 0x100FF+1}, // Linear B Ideograms
	{0x10100, 0x1013F+1}, // Aegean Numbers
	{0x10140, 0x1018F+1}, // Ancient Greek Numbers
	{0x10190, 0x101CF+1}, // Ancient Symbols
	{0x101D0, 0x101FF+1}, // Phaistos Disc
	{0x10280, 0x1029F+1}, // Lycian
	{0x102A0, 0x102DF+1}, // Carian
	{0x102E0, 0x102FF+1}, // Coptic Epact Numbers
	{0x10300, 0x1032F+1}, // Old Italic
	{0x10330, 0x1034F+1}, // Gothic
	{0x10350, 0x1037F+1}, // Old Permic
	{0x10380, 0x1039F+1}, // Ugaritic
	{0x103A0, 0x103DF+1}, // Old Persian
	{0x10400, 0x1044F+1}, // Deseret
	{0x10450, 0x1047F+1}, // Shavian
	{0x10480, 0x104AF+1}, // Osmanya
	{0x104B0, 0x104FF+1}, // Osage
	{0x10500, 0x1052F+1}, // Elbasan
	{0x10530, 0x1056F+1}, // Caucasian Albanian
	{0x10570, 0x105BF+1}, // Vithkuqi
	{0x10600, 0x1077F+1}, // Linear A
	{0x10780, 0x107BF+1}, // Latin Extended-F
	{0x10800, 0x1083F+1}, // Cypriot Syllabary
	{0x10840, 0x1085F+1}, // Imperial Aramaic
	{0x10860, 0x1087F+1}, // Palmyrene
	{0x10880, 0x108AF+1}, // Nabataean
	{0x108E0, 0x108FF+1}, // Hatran
	{0x10900, 0x1091F+1}, // Phoenician
	{0x10920, 0x1093F+1}, // Lydian
	{0x10980, 0x1099F+1}, // Meroitic Hieroglyphs
	{0x109A0, 0x109FF+1}, // Meroitic Cursive
	{0x10A00, 0x10A5F+1}, // Kharoshthi
	{0x10A60, 0x10A7F+1}, // Old South Arabian
	{0x10A80, 0x10A9F+1}, // Old North Arabian
	{0x10AC0, 0x10AFF+1}, // Manichaean
	{0x10B00, 0x10B3F+1}, // Avestan
	{0x10B40, 0x10B5F+1}, // Inscriptional Parthian
	{0x10B60, 0x10B7F+1}, // Inscriptional Pahlavi
	{0x10B80, 0x10BAF+1}, // Psalter Pahlavi
	{0x10C00, 0x10C4F+1}, // Old Turkic
	{0x10C80, 0x10CFF+1}, // Old Hungarian
	{0x10D00, 0x10D3F+1}, // Hanifi Rohingya
	{0x10E60, 0x10E7F+1}, // Rumi Numeral Symbols
	{0x10E80, 0x10EBF+1}, // Yezidi
	{0x10EC0, 0x10EFF+1}, // Arabic Extended-C
	{0x10F00, 0x10F2F+1}, // Old Sogdian
	{0x10F30, 0x10F6F+1}, // Sogdian
	{0x10F70, 0x10FAF+1}, // Old Uyghur
	{0x10FB0, 0x10FDF+1}, // Chorasmian
	{0x10FE0, 0x10FFF+1}, // Elymaic
	{0x11000, 0x1107F+1}, // Brahmi
	{0x11080, 0x110CF+1}, // Kaithi
	{0x110D0, 0x110FF+1}, // Sora Sompeng
	{0x11100, 0x1114F+1}, // Chakma
	{0x11150, 0x1117F+1}, // Mahajani
	{0x11180, 0x111DF+1}, // Sharada
	{0x111E0, 0x111FF+1}, // Sinhala Archaic Numbers
	{0x11200, 0x1124F+1}, // Khojki
	{0x11280, 0x112AF+1}, // Multani
	{0x112B0, 0x112FF+1}, // Khudawadi
	{0x11300, 0x1137F+1}, // Grantha
	{0x11400, 0x1147F+1}, // Newa
	{0x11480, 0x114DF+1}, // Tirhuta
	{0x11580, 0x115FF+1}, // Siddham
	{0x11600, 0x1165F+1}, // Modi
	{0x11660, 0x1167F+1}, // Mongolian Supplement
	{0x11680, 0x116CF+1}, // Takri
	{0x11700, 0x1174F+1}, // Ahom
	{0x11800, 0x1184F+1}, // Dogra
	{0x118A0, 0x118FF+1}, // Warang Citi
	{0x11900, 0x1195F+1}, // Dives Akuru
	{0x119A0, 0x119FF+1}, // Nandinagari
	{0x11A00, 0x11A4F+1}, // Zanabazar Square
	{0x11A50, 0x11AAF+1}, // Soyombo
	{0x11AB0, 0x11ABF+1}, // Unified Canadian Aboriginal Syllabics Extended-A
	{0x11AC0, 0x11AFF+1}, // Pau Cin Hau
	{0x11B00, 0x11B5F+1}, // Devanagari Extended-A
	{0x11C00, 0x11C6F+1}, // Bhaiksuki
	{0x11C70, 0x11CBF+1}, // Marchen
	{0x11D00, 0x11D5F+1}, // Masaram Gondi
	{0x11D60, 0x11DAF+1}, // Gunjala Gondi
	{0x11EE0, 0x11EFF+1}, // Makasar
	{0x11F00, 0x11F50+1}, // Kawi
	{0x11FB0, 0x11FBF+1}, // Lisu Supplement
	{0x11FC0, 0x11FFF+1}, // Tamil Supplement
	{0x12000, 0x123FF+1}, // Cuneiform
	{0x12400, 0x1247F+1}, // Cuneiform Numbers and Punctuation
	{0x12480, 0x1254F+1}, // Early Dynastic Cuneiform
	{0x12F90, 0x12FFF+1}, // Cypro-Minoan
	{0x13000, 0x1342F+1}, // Egyptian Hieroglyphs
	{0x13430, 0x1343F+1}, // Egyptian Hieroglyph Format Controls
	{0x14400, 0x1467F+1}, // Anatolian Hieroglyphs
	{0x16800, 0x16A3F+1}, // Bamum Supplement
	{0x16A40, 0x16A6F+1}, // Mro
	{0x16A70, 0x16ACF+1}, // Tangsa
	{0x16AD0, 0x16AFF+1}, // Bassa Vah
	{0x16B00, 0x16B8F+1}, // Pahawh Hmong
	{0x16E40, 0x16E9F+1}, // Medefaidrin
	{0x16F00, 0x16F9F+1}, // Miao
	{0x16FE0, 0x16FFF+1}, // Ideographic Symbols and Punctuation
	{0x17000, 0x187FF+1}, // Tangut
	{0x18800, 0x18AFF+1}, // Tangut Components
	{0x18B00, 0x18CFF+1}, // Khitan Small Script
	{0x18D00, 0x18D7F+1}, // Tangut Supplement
	{0x1AFF0, 0x1AFFF+1}, // Kana Extended-B
	{0x1B000, 0x1B0FF+1}, // Kana Supplement
	{0x1B100, 0x1B12F+1}, // Kana Extended-A
	{0x1B130, 0x1B16F+1}, // Small Kana Extension
	{0x1B170, 0x1B2FF+1}, // Nushu
	{0x1BC00, 0x1BC9F+1}, // Duployan
	{0x1BCA0, 0x1BCAF+1}, // Shorthand Format Controls
	{0x1CF00, 0x1CFCF+1}, // Znamenny Musical Notation
	{0x1D000, 0x1D0FF+1}, // Byzantine Musical Symbols
	{0x1D100, 0x1D1FF+1}, // Musical Symbols
	{0x1D200, 0x1D24F+1}, // Ancient Greek Musical Notation
	{0x1D2C0, 0x1D2DF+1}, // Kaktovik Numerals
	{0x1D2E0, 0x1D2FF+1}, // Mayan Numerals
	{0x1D300, 0x1D35F+1}, // Tai Xuan Jing Symbols
	{0x1D360, 0x1D37F+1}, // Counting Rod Numerals
	{0x1D400, 0x1D7FF+1}, // Mathematical Alphanumeric Symbols
	{0x1D800, 0x1DAAF+1}, // Sutton SignWriting
	{0x1DF00, 0x1DFFF+1}, // Latin Extended-G
	{0x1E000, 0x1E02F+1}, // Glagolitic Supplement
	{0x1E030, 0x1E08F+1}, // Cyrillic Extended-D
	{0x1E100, 0x1E14F+1}, // Nyiakeng Puachue Hmong
	{0x1E290, 0x1E2BF+1}, // Toto
	{0x1E2C0, 0x1E2FF+1}, // Wancho
	{0x1E4D0, 0x1E4FF+1}, // Nag Mundari
	{0x1E7E0, 0x1E7FF+1}, // Ethiopic Extended-B
	{0x1E800, 0x1E8DF+1}, // Mende Kikakui
	{0x1E900, 0x1E95F+1}, // Adlam
	{0x1EC70, 0x1ECBF+1}, // Indic Siyaq Numbers
	{0x1ED00, 0x1ED4F+1}, // Ottoman Siyaq Numbers
	{0x1EE00, 0x1EEFF+1}, // Arabic Mathematical Alphabetic Symbols
	{0x1F000, 0x1F02F+1}, // Mahjong Tiles
	{0x1F030, 0x1F09F+1}, // Domino Tiles
	{0x1F0A0, 0x1F0FF+1}, // Playing Cards
	{0x1F100, 0x1F1FF+1}, // Enclosed Alphanumeric Supplement
	{0x1F200, 0x1F2FF+1}, // Enclosed Ideographic Supplement
	{0x1F300, 0x1F5FF+1}, // Miscellaneous Symbols and Pictographs
	{0x1F600, 0x1F64F+1}, // Emoticons
	{0x1F650, 0x1F67F+1}, // Ornamental Dingbats
	{0x1F680, 0x1F6FF+1}, // Transport and Map Symbols
	{0x1F700, 0x1F77F+1}, // Alchemical Symbols
	{0x1F780, 0x1F7FF+1}, // Geometric Shapes Extended
	{0x1F800, 0x1F8FF+1}, // Supplemental Arrows-C
	{0x1F900, 0x1F9FF+1}, // Supplemental Symbols and Pictographs
	{0x1FA00, 0x1FA6F+1}, // Chess Symbols
	{0x1FA70, 0x1FAFF+1}, // Symbols and Pictographs Extended-A
	{0x1FB00, 0x1FBFF+1}, // Symbols for Legacy Computing
	{0x20000, 0x2A6DF+1}, // CJK Unified Ideographs Extension B
	{0x2A700, 0x2B73F+1}, // CJK Unified Ideographs Extension C
	{0x2B740, 0x2B81F+1}, // CJK Unified Ideographs Extension D
	{0x2B820, 0x2CEAF+1}, // CJK Unified Ideographs Extension E
	{0x2CEB0, 0x2EBEF+1}, // CJK Unified Ideographs Extension F
	{0x2F800, 0x2FA1F+1}, // CJK Compatibility Ideographs Supplement
	{0x30000, 0x3134F+1}, // CJK Unified Ideographs Extension G
	{0x31350, 0x323AF+1}, // CJK Unified Ideographs Extension H
	{0xE0000, 0xE007F+1}, // Tags
	{0xE0100, 0xE01EF+1}, // Variation Selectors Supplement
	{0xF0000, 0xFFFFF+1}, // Supplementary Private Use Area-A
	{0x100000, 0x10FFFF+1}, // Supplementary Private Use Area-B
};

static constexpr const unicode_block_range_t& find_range(codepoint_t point) {
	for	(const unicode_block_range_t& block : unicode_blocks) {
		if(std::get<0>(block) <= point && point <= std::get<1>(block)){
			return block;
		}
	}
	return bad_code_point_range;
}

static constexpr const std::size_t size_of_range(const unicode_block_range_t& range) {
	return std::get<1>(range) - std::get<0>(range);
}

#endif // UNICODE_BLOCKS_IMPL
