/*************************************************************************/
/*  translation_lerped.cpp                                               */
/*************************************************************************/
/* Copyright (c) 2022 Andreas Van Vooren.                                */
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

#include "translation_interpolated.h"

#include "core/io/resource_loader.h"
#include "core/string/text_interpolate_impl.h"
#include "core/string/locales.h"
#include "core/os/os.h"
#include "core/config/project_settings.h"
#include "core/string/translation.h"
#include "core/variant/variant.h"
#include <iterator>
#include <map>
#include <utility>

void TranslationInterpolatedServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_locale_map", "map"), &TranslationInterpolatedServer::set_locale_map);
	ClassDB::bind_method(D_METHOD("get_locale_map"), &TranslationInterpolatedServer::get_locale_map);
	ClassDB::bind_method(D_METHOD("interpolate_strings", "map"), &TranslationInterpolatedServer::interpolate_strings);
	ClassDB::bind_method(D_METHOD("get_code_points_from_string", "str"), &TranslationInterpolatedServer::get_code_points_from_string);
	ClassDB::bind_method(D_METHOD("get_string_from_code_points", "arr"), &TranslationInterpolatedServer::get_string_from_code_points);
}

String TranslationInterpolatedServer::get_locale() const {
	return cached_best_locale;
}

void TranslationInterpolatedServer::set_locale(const String &locale) {
	// Don't forget to update the old method, as a temp setting.
	TranslationServer::set_locale(locale);
	Dictionary map{};
	map[locale] = 1.0;
	set_locale_map(map);
}

Dictionary TranslationInterpolatedServer::get_locale_map() const {
	Dictionary result{};
	for (const auto &pair : weighted_locale_map) {
		result[pair.first.c_str()] = pair.second;
	}
	return result;
}

void TranslationInterpolatedServer::set_locale_map(const Dictionary &map) {
	weighted_locale_map.clear();
	const int element_count = map.size();
	// If no elements are passed through, don't do anything.
	ERR_FAIL_COND(element_count == 0);
	real_t total_weight = 0.0;
	for (const Variant *elem = map.next(); elem; elem = map.next(elem)) {
		ERR_FAIL_COND(elem->get_type() != Variant::STRING);
		const Variant &valueVar = map[*elem];
		ERR_FAIL_COND(valueVar.get_type() != Variant::FLOAT);

		// NOTE: This is a pretty complicated string copy, but I don't think there's a faster way.
		// (Internally strings are wide char, because they are.)
		const std::string key{ static_cast<String>(*elem).utf8().ptr() };
		const real_t value = valueVar;

		// Don't want to change anything if one of the weights is negative.
		ERR_FAIL_COND(value < 0.0);

		weighted_locale_map[key] = value;
		total_weight += value;
	}
	// Should never be negative weight
	ERR_FAIL_COND(total_weight < 0.0);

	if (total_weight > 0.0) {
		const real_t weight_reciprocal = 1.0 / total_weight;
		// Standard case, just fill in our cache with the new stuff.
		for (auto &pair : weighted_locale_map) {
			pair.second *= weight_reciprocal;
		}
	} else {
		const real_t weight_value = 1.0 / element_count;
		// Zero case, distribute weight value among all elements.
		for (auto &pair : weighted_locale_map) {
			pair.second = weight_value;
		}
	}

	// Erase zero elements, so they don't contribute to weight.
	// C++20 has erase_if which works better for this,
	// but it already complained about using C++17 features, so I doubt Godot will support it.
	for (auto it = weighted_locale_map.begin(); it != weighted_locale_map.end();) {
		if (it->second <= 0.0) {
			it = weighted_locale_map.erase(it);
		} else {
			++it;
		}
	}

	// Extra step: Cache the best locale candidate to speed up get_locale
	const std::string *best_locale_ptr = nullptr;
	real_t best_weight = -1.0;
	for (const auto &pair : weighted_locale_map) {
		if (pair.second > best_weight) {
			best_locale_ptr = &pair.first;
			best_weight = pair.second;
		}
	}

	ERR_FAIL_COND(best_locale_ptr == nullptr);
	cached_best_locale = String{ (*best_locale_ptr).c_str() };
}

StringName TranslationInterpolatedServer::translate(const StringName &p_message, const StringName &p_context) const {
	return TranslationServer::translate(p_message, p_context);
}

void TranslationInterpolatedServer::set_tool_translation(const Ref<Translation> &p_translation) {
	TranslationServer::set_tool_translation(p_translation);
}

StringName TranslationInterpolatedServer::tool_translate(const StringName &p_message, const StringName &p_context) const {
	return TranslationServer::tool_translate(p_message, p_context);
}

void TranslationInterpolatedServer::set_doc_translation(const Ref<Translation> &p_translation) {
	TranslationServer::set_doc_translation(p_translation);
}

StringName TranslationInterpolatedServer::doc_translate(const StringName &p_message, const StringName &p_context) const {
	return TranslationServer::doc_translate(p_message, p_context);
}

String TranslationInterpolatedServer::interpolate_strings(const Dictionary &map) const {
	std::vector<std::pair<std::string, real_t>> weighted_locale_map{};
	const int element_count = map.size();
	// If no elements are passed through, don't do anything.
	ERR_FAIL_COND_V(element_count == 0, {});
	real_t total_weight = 0.0;
	for (const Variant *elem = map.next(); elem; elem = map.next(elem)) {
		ERR_FAIL_COND_V(elem->get_type() != Variant::STRING, {});
		const Variant &valueVar = map[*elem];
		ERR_FAIL_COND_V(valueVar.get_type() != Variant::FLOAT, {});

		// NOTE: This is a pretty complicated string copy, but I don't think there's a faster way.
		// (Internally strings are wide char, because they are.)
		const std::string key{ static_cast<String>(*elem).utf8().ptr() };
		const real_t value = valueVar;

		// Don't want to change anything if one of the weights is negative.
		ERR_FAIL_COND_V(value < 0.0, {});

		weighted_locale_map.push_back({key, value});
		total_weight += value;
	}
	// Should never be negative weight
	ERR_FAIL_COND_V(total_weight < 0.0, {});

	if (total_weight > 0.0) {
		const real_t weight_reciprocal = 1.0 / total_weight;
		// Standard case, just fill in our cache with the new stuff.
		for (auto &pair : weighted_locale_map) {
			pair.second *= weight_reciprocal;
		}
	} else {
		const real_t weight_value = 1.0 / element_count;
		// Zero case, distribute weight value among all elements.
		for (auto &pair : weighted_locale_map) {
			pair.second = weight_value;
		}
	}

	// Erase zero elements, so they don't contribute to weight.
	// C++20 has erase_if which works better for this,
	// but it already complained about using C++17 features, so I doubt Godot will support it.
	for (auto it = weighted_locale_map.begin(); it != weighted_locale_map.end();) {
		if (it->second <= 0.0) {
			it = weighted_locale_map.erase(it);
		} else {
			++it;
		}
	}

	std::string str = interpolate(weighted_locale_map);
	return String{ str.c_str() };
}

PackedInt32Array TranslationInterpolatedServer::get_code_points_from_string(const String &str) const {
	const std::string key{ str.utf8().ptr() };
	PackedInt32Array result{};
	for (auto it = key.cbegin(); it != key.cend();) {
		auto size = _TextLerpInternals::UTF8ByteSizeFromFirstChar(*it);
		auto cp = _TextLerpInternals::UTF8ToCodePoint(it);
		result.append(cp);
		std::advance(it, size);
	}
	return result;
}
String TranslationInterpolatedServer::get_string_from_code_points(const PackedInt32Array &str) const {
	std::string intermediate;
	for (auto i = 0; i < str.size(); ++i) {
		auto cp = str[i];
		intermediate += _TextLerpInternals::CodePointToUTF8(cp).data();
	}
	String result;
	result.parse_utf8(intermediate.c_str());
	return result;
}