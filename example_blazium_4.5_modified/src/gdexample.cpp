/**************************************************************************/
/*  gdexample.cpp                                                         */
/**************************************************************************/
/*                         This file is part of:                          */
/*                   MisterPuma80's Blazium Benchmarker                   */
/*           https://github.com/MisterPuma80/blazium_benchmarker          */
/**************************************************************************/
/* Copyright (c) 2024-2026 MisterPuma80 <MisterPuma80@gmail.com>          */
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

#include "gdexample.h"
#include "boilerplate.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;


#include <chrono>
#include <cstdint>
#include <iostream>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/scene_tree.hpp>


Dictionary _test_string_to_lower() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "No Place Like Home";
	String b = "no place like home";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		String ret = a.to_lower();
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, b);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("String::to_lower")] = values;
	return entry;
}

Dictionary _test_string_to_upper() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "No Place Like Home";
	String b = "NO PLACE LIKE HOME";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		String ret = a.to_upper();
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, b);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("String::to_upper")] = values;
	return entry;
}







Dictionary _test_string_findn() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "No Place Like Like Home";
	String to_find = "LIKE";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		int ret = a.findn(to_find);
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, 9);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("String::findn")] = values;
	return entry;
}

Dictionary _test_string_containsn() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "No Place Like Like Home";
	String to_find = "LIKE";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		bool ret = a.containsn(to_find);
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, true);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("String::containsn")] = values;
	return entry;
}

Dictionary _test_string_rfindn() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "No Place Like Like Home";
	String to_find = "LIKE";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		int ret = a.rfindn(to_find);
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, 14);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("String::rfindn")] = values;
	return entry;
}

Dictionary _test_string_matchn() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "no place like home";
	String b = "NO ***** LIKE HOME";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		bool ret = a.matchn(b);
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, true);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("String::matchn")] = values;
	return entry;
}

Dictionary _test_string_is_subsequence_ofn() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "abc";
	String b = "AxByC";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		bool ret = a.is_subsequence_ofn(b);
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, true);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("String::is_subsequence_ofn")] = values;
	return entry;
}




Dictionary _test_string_nocasecmp_to() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "no place like like home";
	String b = "no place like like the circus";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		int ret = a.nocasecmp_to(b);
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, -1);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("String::nocasecmp_to")] = values;
	return entry;
}

Dictionary _test_string_naturalnocasecmp_to() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "no place like like home";
	String b = "no place like like the circus";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		int ret = a.naturalnocasecmp_to(b);
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, -1);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("String::naturalnocasecmp_to")] = values;
	return entry;
}

Dictionary _test_string_countn() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "no place like like home";
	String b = "LIKE";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		int ret = a.countn(b);
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, 2);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("String::countn")] = values;
	return entry;
}

Dictionary _test_string_replacen() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "no place like like home";
	String b = "LIKE";
	String c = "LiKe";
	String d = "no place LiKe LiKe home";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		String ret = a.replacen(b, c);
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, d);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("String::replacen")] = values;
	return entry;
}




Dictionary _test_string_to_camel_case() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "no_place_like_like_home";
	String b = "noPlaceLikeLikeHome";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		String ret = a.to_camel_case();
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, b);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("String::to_camel_case")] = values;
	return entry;
}

Dictionary _test_string_to_pascal_case() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "no_place_like_like_home";
	String b = "NoPlaceLikeLikeHome";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		String ret = a.to_pascal_case();
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, b);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("String::to_pascal_case")] = values;
	return entry;
}

Dictionary _test_string_capitalize() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "no_place_like_like_home";
	String b = "No Place Like Like Home";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		String ret = a.capitalize();
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, b);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("String::capitalize")] = values;
	return entry;
}





Dictionary _test_os_find_keycode_from_string() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	String a = "Escape";
	Key b = Key::KEY_ESCAPE;//"4194305";
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		Key ret = OS::get_singleton()->find_keycode_from_string(a);
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(ret, b);
	}
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("OS::find_keycode_from_string")] = values;
	return entry;
}

void GDExample::_ready() {
	Dictionary result;

	result.merge(_test_string_to_lower());
	result.merge(_test_string_to_upper());

	result.merge(_test_string_findn());
	result.merge(_test_string_containsn());
	result.merge(_test_string_rfindn());
	result.merge(_test_string_matchn());
	result.merge(_test_string_is_subsequence_ofn());

	result.merge(_test_string_nocasecmp_to());
	result.merge(_test_string_naturalnocasecmp_to());
	result.merge(_test_string_countn());
	result.merge(_test_string_replacen());

	result.merge(_test_string_to_camel_case());
	result.merge(_test_string_to_pascal_case());
	result.merge(_test_string_capitalize());

	result.merge(_test_os_find_keycode_from_string());

	benchmark_footer(this, result);
}
