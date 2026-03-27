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

Dictionary _test_node_find_children() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	const int total_child_nodes = 100;
	Node * root_tree = _make_node_tree(total_child_nodes);
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		TypedArray<Node> all = root_tree->find_children("*", "", true, true);
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
//		for (int i = 0; i < all.size(); i++) {
//			Node *c = Object::cast_to<Node>(all[i]);
//			print_line(vformat("!!! i: %d, name: \"%s\"", i, c->get_name()));
//		}
		//print_line(vformat("!!! all->size: \"%d\"", all->size()));
		ASSERT_EQUAL(all.size(), total_child_nodes);
	}
//	root_tree->queue_free();
	_delete_node_tree(root_tree);
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("find_children")] = values;
	return entry;
}

Dictionary _test_node_find_children_by_name() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	const int total_child_nodes = 100;
	Node * root_tree = _make_node_tree(total_child_nodes);
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		TypedArray<Node> all = root_tree->find_children("da_node_1?", "", true, true);
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
//		for (int i = 0; i < all.size(); i++) {
//			Node *c = Object::cast_to<Node>(all[i]);
//			print_line(vformat("!!! i: %d, name: \"%s\"", i, c->get_name()));
//		}
		//print_line(vformat("!!! all->size: \"%d\"", all->size()));
		ASSERT_EQUAL(all.size(), 10);
	}
//	root_tree->queue_free();
	_delete_node_tree(root_tree);
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("find_children by name")] = values;
	return entry;
}

Dictionary _test_node_find_children_by_type() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	const int total_child_nodes = 100;
	Node * root_tree = _make_node_tree(total_child_nodes);
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		TypedArray<Node> all = root_tree->find_children("*", "Node", true, true);
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
//		for (int i = 0; i < all.size(); i++) {
//			Node *c = Object::cast_to<Node>(all[i]);
//			print_line(vformat("!!! i: %d, name: \"%s\"", i, c->get_name()));
//		}
		//print_line(vformat("!!! all->size: \"%d\"", all->size()));
		ASSERT_EQUAL(all.size(), total_child_nodes);
	}
//	root_tree->queue_free();
	_delete_node_tree(root_tree);
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("find_children by type")] = values;
	return entry;
}

Dictionary _test_node_find_children_by_group() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	const int total_child_nodes = 100;
	Node * root_tree = _make_node_tree(total_child_nodes);
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		TypedArray<Node> all;
		TypedArray<Node> to_search = root_tree->find_children("*", "", true, true);
		int count = to_search.size();
		const StringName group_name = "bird";
		for (int i = 0; i < count; i++) {
			const Node *node = Object::cast_to<Node>(to_search[i]);
			if (node->is_in_group(group_name)) {
				all.push_back(node);
			}
		}
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
//		for (int i = 0; i < all.size(); i++) {
//			Node *c = Object::cast_to<Node>(all[i]);
//			print_line(vformat("!!! i: %d, name: \"%s\"", i, c->get_name()));
//		}
		//print_line(vformat("!!! all->size: \"%d\"", all->size()));
		ASSERT_EQUAL(all.size(), 34);
	}
//	root_tree->queue_free();
	_delete_node_tree(root_tree);
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("find_children by group")] = values;
	return entry;
}

Dictionary _test_node_get_children() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	const int total_child_nodes = 100;
	Node * root_tree = _make_node_tree_flat(total_child_nodes, 42);
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		TypedArray<Node> all = root_tree->get_children();
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
//		for (int i = 0; i < all.size(); i++) {
//			Node *c = Object::cast_to<Node>(all[i]);
//			print_line(vformat("!!! i: %d, name: \"%s\"", i, c->get_name()));
//		}
		//print_line(vformat("!!! all->size: \"%d\"", all->size()));
		ASSERT_EQUAL(all.size(), total_child_nodes);
	}
//	root_tree->queue_free();
	_delete_node_tree(root_tree);
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("get_children")] = values;
	return entry;
}

Dictionary _test_node_get_children_by_name() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	const int total_child_nodes = 100;
	Node *root = _make_node_tree(total_child_nodes);
	Node *node = root->get_node<Node>("da_node_1/da_node_3/da_node_5/da_node_6/da_node_7/da_node_12/da_node_15/da_node_17/da_node_19/da_node_20/da_node_24/da_node_25/da_node_26/da_node_29/da_node_32/da_node_34/da_node_39/da_node_45/da_node_47/da_node_48/da_node_49/da_node_51/da_node_53/da_node_56/da_node_58/da_node_61/da_node_62/da_node_63/da_node_64");
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		TypedArray<Node> all = node->find_children("da_node_7?", "", false, true);
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
//		for (int i = 0; i < all.size(); i++) {
//			Node *c = Object::cast_to<Node>(all[i]);
//			print_line(vformat("!!! i: %d, name: \"%s\"", i, c->get_name()));
//		}
		//print_line(vformat("!!! all->size: \"%d\"", all->size()));
		ASSERT_EQUAL(all.size(), 10);
	}
//	root->queue_free();
	_delete_node_tree(root);
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("get_children by name")] = values;
	return entry;
}

Dictionary _test_node_get_children_by_group() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	const int total_child_nodes = 100;
	Node *root = _make_node_tree(total_child_nodes);
	Node *node = root->get_node<Node>("da_node_1/da_node_3/da_node_5/da_node_6/da_node_7/da_node_12/da_node_15/da_node_17/da_node_19/da_node_20/da_node_24/da_node_25/da_node_26/da_node_29/da_node_32/da_node_34/da_node_39/da_node_45/da_node_47/da_node_48/da_node_49/da_node_51/da_node_53/da_node_56/da_node_58/da_node_61/da_node_62/da_node_63/da_node_64");
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		TypedArray<Node> all;
		TypedArray<Node> to_search = node->get_children();
		int count = to_search.size();
		const StringName group_name = "cat";
		for (int i = 0; i < count; i++) {
			const Node *node = Object::cast_to<Node>(to_search[i]);
			if (node->is_in_group(group_name)) {
				all.push_back(node);
			}
		}
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
//		for (int i = 0; i < all.size(); i++) {
//			Node *c = Object::cast_to<Node>(all[i]);
//			print_line(vformat("!!! i: %d, name: \"%s\"", i, c->get_name()));
//		}
		//print_line(vformat("!!! all->size: \"%d\"", all->size()));
		ASSERT_EQUAL(all.size(), 11);
	}
//	root->queue_free();
	_delete_node_tree(root);
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("get_children by group")] = values;
	return entry;
}

Dictionary _test_node_get_groups() {
	uint64_t ticks = 0, total = 0, average = 0;
	auto start = get_ticks_now();
	auto end = get_ticks_now();
	Node *node = memnew(Node);
	TypedArray<StringName> groups;
	groups.append("Food");
	groups.append("Enemy");
	groups.append("Weapon");
	groups.append("Level");
	groups.append("Dynamic");
	for (int i = 0; i < groups.size(); i++) {
		node->add_to_group(groups[i]);
	}
	for (int i = 0; i < ITERATIONS; i++) {
		start = get_ticks_now();
		TypedArray<StringName> all = node->get_groups();
		end = get_ticks_now();
		ticks = get_ticks_diff(end, start);
		total += ticks;
		ASSERT_EQUAL(all, groups);
	}
	_delete_node_tree(node);
	average = total / ITERATIONS;

	Dictionary values;
	values[String("total")] = total;
	values[String("average")] = average;
	Dictionary entry;
	entry[String("get_groups")] = values;
	return entry;
}

void GDExample::_ready() {
	Dictionary result;

	result.merge(_test_node_find_children());
	result.merge(_test_node_find_children_by_name());
	result.merge(_test_node_find_children_by_type());
	result.merge(_test_node_find_children_by_group());

	result.merge(_test_node_get_children());
	result.merge(_test_node_get_children_by_name());
	result.merge(_test_node_get_children_by_group());

	result.merge(_test_node_get_groups());

	benchmark_footer(this, result);
}
