/**************************************************************************/
/*  translation.h                                                         */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "core/io/resource.h"
#include "core/object/gdvirtual.gen.inc"

// Include std::string and std::map so that we can make the text lerp library agnostic.
#include <map>
#include <string>

class Translation : public Resource {
	GDCLASS(Translation, Resource);
	OBJ_SAVE_TYPE(Translation);
	RES_BASE_EXTENSION("translation");

	String locale = "en";
	HashMap<StringName, StringName> translation_map;

	virtual Vector<String> _get_message_list() const;
	virtual Dictionary _get_messages() const;
	virtual void _set_messages(const Dictionary &p_messages);

	void _notify_translation_changed_if_applies();

protected:
	static void _bind_methods();

	GDVIRTUAL2RC(StringName, _get_message, StringName, StringName);
	GDVIRTUAL4RC(StringName, _get_plural_message, StringName, StringName, int, StringName);

public:
	void set_locale(const String &p_locale);
	_FORCE_INLINE_ String get_locale() const { return locale; }

	virtual void add_message(const StringName &p_src_text, const StringName &p_xlated_text, const StringName &p_context = "");
	virtual void add_plural_message(const StringName &p_src_text, const Vector<String> &p_plural_xlated_texts, const StringName &p_context = "");
	virtual StringName get_message(const StringName &p_src_text, const StringName &p_context = "") const; //overridable for other implementations
	virtual StringName get_plural_message(const StringName &p_src_text, const StringName &p_plural_text, int p_n, const StringName &p_context = "") const;
	virtual void erase_message(const StringName &p_src_text, const StringName &p_context = "");
	virtual void get_message_list(List<StringName> *r_messages) const;
	virtual int get_message_count() const;
	virtual Vector<String> get_translated_message_list() const;

	Translation() {}
};

class TranslationServer : public Object {
	GDCLASS(TranslationServer, Object);

	String locale = "en";
	String fallback;

	HashSet<Ref<Translation>> translations;
	Ref<Translation> tool_translation;
	Ref<Translation> doc_translation;
	Ref<Translation> property_translation;

	bool enabled = true;

	bool pseudolocalization_enabled = false;
	bool pseudolocalization_accents_enabled = false;
	bool pseudolocalization_double_vowels_enabled = false;
	bool pseudolocalization_fake_bidi_enabled = false;
	bool pseudolocalization_override_enabled = false;
	bool pseudolocalization_skip_placeholders_enabled = false;
	bool editor_pseudolocalization = false;
	float expansion_ratio = 0.0;
	String pseudolocalization_prefix;
	String pseudolocalization_suffix;

	StringName tool_pseudolocalize(const StringName &p_message) const;
	String get_override_string(String &p_message) const;
	String double_vowels(String &p_message) const;
	String replace_with_accented_string(String &p_message) const;
	String wrap_with_fakebidi_characters(String &p_message) const;
	String add_padding(const String &p_message, int p_length) const;
	const char32_t *get_accented_version(char32_t p_character) const;
	bool is_placeholder(String &p_message, int p_index) const;

	static TranslationServer *singleton;
	bool _load_translations(const String &p_from);
	String _standardize_locale(const String &p_locale, bool p_add_defaults) const;

	StringName _get_message_from_translations(const StringName &p_message, const StringName &p_context, const String &p_locale, bool plural, const String &p_message_plural = "", int p_n = 0) const;

	static void _bind_methods();

	struct LocaleScriptInfo {
		String name;
		String script;
		String default_country;
		HashSet<String> supported_countries;
	};
	static Vector<LocaleScriptInfo> locale_script_info;

	static HashMap<String, String> language_map;
	static HashMap<String, String> script_map;
	static HashMap<String, String> locale_rename_map;
	static HashMap<String, String> country_name_map;
	static HashMap<String, String> country_rename_map;
	static HashMap<String, String> variant_map;

	void init_locale_info();

	std::map<std::string, real_t> weighted_locale_map;

public:
	_FORCE_INLINE_ static TranslationServer *get_singleton() { return singleton; }

	void set_enabled(bool p_enabled) { enabled = p_enabled; }
	_FORCE_INLINE_ bool is_enabled() const { return enabled; }

	virtual void set_locale(const String &p_locale);
	virtual String get_locale() const;
	Ref<Translation> get_translation_object(const String &p_locale);

	// Returns the internally used locale map.
	Dictionary get_locale_map() const;
	// Sets the current locale map.
	// All entries are in the form of {Locale, Locale weight}
	// Weight values must never be negative.
	// Entries will internally be normalized to map the entire range from 0 to 1.
	// If the sum of all weights is zero, will distribute weight equally.
	// Special value "system" maps to the current system's locale
	void set_locale_map(const Dictionary &map);

	Vector<String> get_all_languages() const;
	String get_language_name(const String &p_language) const;

	Vector<String> get_all_scripts() const;
	String get_script_name(const String &p_script) const;

	Vector<String> get_all_countries() const;
	String get_country_name(const String &p_country) const;

	String get_locale_name(const String &p_locale) const;

	PackedStringArray get_loaded_locales() const;

	void add_translation(const Ref<Translation> &p_translation);
	void remove_translation(const Ref<Translation> &p_translation);

	virtual StringName translate(const StringName &p_message, const StringName &p_context = "") const;
	StringName translate_plural(const StringName &p_message, const StringName &p_message_plural, int p_n, const StringName &p_context = "") const;

	StringName pseudolocalize(const StringName &p_message) const;

	bool is_pseudolocalization_enabled() const;
	void set_pseudolocalization_enabled(bool p_enabled);
	void set_editor_pseudolocalization(bool p_enabled);
	void reload_pseudolocalization();

	String standardize_locale(const String &p_locale) const;

	int compare_locales(const String &p_locale_a, const String &p_locale_b) const;

	String get_tool_locale();
	virtual void set_tool_translation(const Ref<Translation> &p_translation);
	Ref<Translation> get_tool_translation() const;
	virtual StringName tool_translate(const StringName &p_message, const StringName &p_context = "") const;
	StringName tool_translate_plural(const StringName &p_message, const StringName &p_message_plural, int p_n, const StringName &p_context = "") const;
	virtual void set_doc_translation(const Ref<Translation> &p_translation);
	virtual StringName doc_translate(const StringName &p_message, const StringName &p_context = "") const;
	StringName doc_translate_plural(const StringName &p_message, const StringName &p_message_plural, int p_n, const StringName &p_context = "") const;
	void set_property_translation(const Ref<Translation> &p_translation);
	StringName property_translate(const StringName &p_message) const;

	// Exposes the underlying API for text lerp for verifying string interpolation.
	// NOTE: this is a slow function
	String interpolate_strings(const Dictionary &map) const;
	// Returns a string as a list of Unicode code points. Does not include the null terminator.
	PackedInt32Array get_code_points_from_string(const String &str) const;
	// Returns a list of Unicode code points as a string. Does not need to include the null terminator.
	String get_string_from_code_points(const PackedInt32Array &str) const;

	void setup();

	void clear();

	void load_translations();

	TranslationServer();
};

#endif // TRANSLATION_H
