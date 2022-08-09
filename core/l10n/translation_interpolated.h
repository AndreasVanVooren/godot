/*************************************************************************/
/*  translation_lerped.h                                                 */
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

#ifndef TRANSLATION_LERPED_H
#define TRANSLATION_LERPED_H
// [AVV] I prefer #pragma once personally, but w/e

// [AVV][TODO] Make this a better option, maybe with like an SCons define
#include "core/dictionary.h"
#include "core/variant.h"
#define USE_LERPED_TRANSLATION_SERVER

// Include std::string and std::map so that we can make the text lerp library agnostic.
#include <map>
#include <string>

#include "core/math/math_defs.h"
#include "core/translation.h"

// Variant of the Translation server which allows for multiple
// localizations to be lerped smoothly. Inherits from base server
// for easy implementation with the rest of the engine.
class TranslationInterpolatedServer : public TranslationServer {
	GDCLASS(TranslationInterpolatedServer, TranslationServer);

	static void _bind_methods();

protected:
	std::map<std::string, real_t> weighted_locale_map;
	String cached_best_locale;

public:
	// Gets the current locale.
	// If multiple locales are merged, gets the most predominant one.
	virtual String get_locale() const override;
	// Internally, calls set_locale_map with a single entry.
	// Kept for compatibility.
	virtual void set_locale(const String &p_locale) override;
	// Returns the internally used locale map.
	// Return-by-value because reflection crap
	Dictionary get_locale_map() const;
	// Sets the current locale map.
	// All entries are in the form of {Locale, Locale weight}
	// Weight values must never be negative.
	// Entries will internally be normalized to map the entire range from 0 to 1.
	// If the sum of all weights is zero, will distribute weight equally.
	// Special value "system" maps to the current system's locale
	void set_locale_map(const Dictionary &map);

	virtual StringName translate(const StringName &p_message) const override;

	virtual void set_tool_translation(const Ref<Translation> &p_translation) override;
	virtual StringName tool_translate(const StringName &p_message, const StringName &p_context) const override;
	virtual void set_doc_translation(const Ref<Translation> &p_translation) override;
	virtual StringName doc_translate(const StringName &p_message) const override;

	// Exposes the underlying API for text lerp for verifying string interpolation.
	// NOTE: this is a slow function
	String interpolate_strings(const Dictionary &map) const;
	// Returns a string as a list of Unicode code points. Does not include the null terminator.
	PoolIntArray get_code_points_from_string(const String& str) const;
	// Returns a list of Unicode code points as a string. Does not need to include the null terminator.
	String get_string_from_code_points(const PoolIntArray& str) const;
};

#endif // TRANSLATION_LERPED_H