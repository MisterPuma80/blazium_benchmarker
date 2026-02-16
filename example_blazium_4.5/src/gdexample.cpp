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

Dictionary _test_node_get_children() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	const int total_child_nodes = 100;
	Node *root_tree = _make_node_tree_flat(total_child_nodes, 42);
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		TypedArray<Node> all = root_tree->get_children();
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(all.size(), total_child_nodes);
	}
	_delete_node_tree(root_tree);
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("Node::get_children")] = values;
	return entry;
}

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

void GDExample::_ready() {
	Dictionary result;

	result.merge(_test_node_get_children());

	result.merge(_test_string_to_lower());
	result.merge(_test_string_to_upper());

	benchmark_footer(this, result);
}
