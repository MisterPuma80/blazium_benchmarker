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
#include <godot_cpp/core/class_db.hpp>

using namespace godot;


const int ITERATIONS = 1000000;
const int GOOD_SEED = 3647666;

#include <chrono>
#include <cstdint>
#include <iostream>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/random_number_generator.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

void GDExample::_bind_methods() {
}

GDExample::GDExample() {
}

GDExample::~GDExample() {
}

#define ASSERT_EQUAL(a, b) \
	do { \
		if (!(a == b)) { \
			std::cerr << vformat("Assert equal failed: \"%s\" != \"%s\" at %s:%s", a, b, __FILE__, __LINE__).utf8() << std::endl; \
			std::cerr << std::flush; \
			std::abort(); \
		} \
	} while (false)

_ALWAYS_INLINE_ std::chrono::time_point<std::chrono::high_resolution_clock> get_ticks_now() {
	return std::chrono::high_resolution_clock::now();
}

_ALWAYS_INLINE_ uint64_t get_ticks_diff(const std::chrono::time_point<std::chrono::high_resolution_clock> end, const std::chrono::time_point<std::chrono::high_resolution_clock> start) {
	return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count());
}

Node *_make_node_tree(int total_child_nodes) {
	Ref<RandomNumberGenerator> rng = memnew(RandomNumberGenerator);
	rng->set_seed(GOOD_SEED);
	Node *root_node = memnew(Node);
	Node *node = root_node;
	int k = 0;
	String groups[] { "bird", "cat", "dog" };
	for (int i = 0; i < total_child_nodes; i++) {
		Node *new_node = memnew(Node);
		new_node->set_name(vformat("da_node_%d", i));
		new_node->add_to_group(groups[k]);
		node->add_child(new_node);
		new_node->set_owner(root_node);
		int j = rng->randf_range(0, node->get_child_count());
		node->move_child(new_node, j);

		if (rng->randf() > 0.5) {
			node = new_node;
		}
		k = k < 2 ? k + 1 : 0;
	}

	return root_node;
}

Node *_make_node_tree_flat(int total_child_nodes, const int seed) {
	Ref<RandomNumberGenerator> rng = memnew(RandomNumberGenerator);
	rng->set_seed(seed);
	Node *root_node = memnew(Node);
	Node *node = root_node;
	int k = 0;
	String groups[] { "bird", "cat", "dog" };
	for (int i = 0; i < total_child_nodes; i++) {
		Node *new_node = memnew(Node);
		new_node->set_name(vformat("da_node_%d", i));
		new_node->add_to_group(groups[k]);
		node->add_child(new_node);
		new_node->set_owner(root_node);
		int j = rng->randf_range(0, node->get_child_count());
		node->move_child(new_node, j);
		k = k < 2 ? k + 1 : 0;
	}

	return root_node;
}

void _delete_node_tree(Node *root_tree) {
	TypedArray<Node> all = root_tree->find_children("*", "", true, true);

	for (int i = all.size()-1; i >= 0; i--) {
		//print_line(vformat("!!! memdelete i: \"%d\"", i));
		Node *n = Object::cast_to<Node>(all[i]);
		if (n != nullptr) {
			memdelete(n);
			all[i] = Variant(); // FIXME: Isn't there a better way to do Variant null?
		}
	}
	all.clear();
	memdelete(root_tree);
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
	entry[String("Node::get_children")] = values;
	return entry;
}

void GDExample::_ready() {
	Dictionary result;

	result.merge(_test_node_get_children());

/*
	for (int i = 0; i < result.size(); i++) {
		Dictionary entry = result[i];
		String name = entry["name"];
		double average = entry["average"];
		std::cout << vformat("%s, average ns: %d", name, average).utf8() << std::endl;
	}
*/
	// Get json file name
	String file_path = "";
	String cwd = "";
	PackedStringArray user_args = OS::get_singleton()->get_cmdline_user_args();
	for (int i = 0; i < user_args.size(); ++i) {
		if (user_args[i] == "name") {
			if (i + 1 < user_args.size()) {
				file_path = user_args[i + 1];
			}
		} else if (user_args[i] == "cwd") {
			if (i + 1 < user_args.size()) {
				cwd = user_args[i + 1];
			}
		}
	}
	if (file_path.is_empty()) {
		std::cerr << vformat("Failed to get command line argument \"name\": \"%s\"", file_path).utf8() << std::endl;
		return;
	}

	String full_file_path = cwd.path_join(file_path);
	//std::cout << vformat("full_file_path: \"%s\"\n", full_file_path).utf8() << std::endl;
	godot::Ref<godot::FileAccess> file = godot::FileAccess::open(full_file_path, godot::FileAccess::ModeFlags::WRITE);
	if (file.is_null()) {
		std::cerr << vformat("Failed to open file: \"%s\"", full_file_path).utf8() << std::endl;
		return;
	}

	// Store the JSON string in the file
	godot::String json_string = godot::JSON::stringify(result, "\t");
	file->store_string(json_string);
	file->close();

	// Flush output because the engine will exit before it can flush.
	std::cout << std::flush;
	std::cerr << std::flush;

	// Quit
	this->get_tree()->quit();
}
