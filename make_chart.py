#!/usr/bin/env python3

############################################################################
#  make_chart.py                                                           #
############################################################################
#                           This file is part of:                          #
#                   MisterPuma80's Blazium Benchmarker                     #
#            https://github.com/MisterPuma80/blazium_benchmarker           #
############################################################################
# Copyright (c) 2024-2026 MisterPuma80 <MisterPuma80@gmail.com>            #
#                                                                          #
# Permission is hereby granted, free of charge, to any person obtaining    #
# a copy of this software and associated documentation files (the          #
# "Software"), to deal in the Software without restriction, including      #
# without limitation the rights to use, copy, modify, merge, publish,      #
# distribute, sublicense, and/or sell copies of the Software, and to       #
# permit persons to whom the Software is furnished to do so, subject to    #
# the following conditions:                                                #
#                                                                          #
# The above copyright notice and this permission notice shall be           #
# included in all copies or substantial portions of the Software.          #
#                                                                          #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          #
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       #
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   #
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     #
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     #
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        #
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   #
############################################################################

import json
import subprocess
import argparse
import sys

# Get iterations count
parser = argparse.ArgumentParser(description="Make benchmark chart")
parser.add_argument("--iterations", help="The number of iterations in the tests.")
args = parser.parse_args()
iterations = int(args.iterations)
iterations = f"{iterations:,}"

PURPLE = "\033[48;5;55m\033[38;5;255m"
GREEN = "\033[48;5;28m\033[38;5;255m"
BLUE = "\033[48;5;32m\033[38;5;255m"
RED = "\033[48;5;196m\033[38;5;255m"
NORMAL = "\u001b[0m\u001b[37m"

def csv_to_table_output(csv_data):
	command = "csvlook"
	try:
		process = subprocess.Popen(
			command,
			stdin=subprocess.PIPE,
			stdout=subprocess.PIPE,
			stderr=subprocess.PIPE,
			text=True,
			encoding='utf-8'
		)
		stdout, stderr = process.communicate(input=csv_data)

		if stderr:
			print(f"Error occurred: {stderr}", file=sys.stderr)
		return stdout
	except FileNotFoundError:
		return "Failed to run csvlook"

def file_to_json(file_name):
	with open(file_name) as f:
		data = json.load(f)
	return data

if __name__ == "__main__":
	profiles = [
		{
			"name" : "Blazium 4.5 Modified",
			"color" : GREEN,
			"column" : 1,
			"data" : file_to_json('benchmark_blazium_4_5_modified.json'),
		},
		{
			"name" : "Blazium 4.5",
			"color" : PURPLE,
			"column" : 2,
			"data" : file_to_json('benchmark_blazium_4_5.json'),
		},
	]

	compare_a = 0
	compare_b = 1

	# Add header
	output = "Test|{0}|{1}|Ratio\n".format(profiles[0]["name"], profiles[1]["name"])

	# Get the unique test names
	test_names = list(profiles[0]["data"].keys() | profiles[1]["data"].keys())
	test_names.sort()

	# Use this data for missing tests
	missing = {
		"average": -1,
		"total": -1
	}

	for name in test_names:
		a = profiles[compare_a]["data"].get(name, missing)
		b = profiles[compare_b]["data"].get(name, missing)

		# Get the ratio between selected versions
		ratio = 0
		if a['average'] == -1 or b['average'] == -1:
			ratio = -1
		elif a['average'] > 0:
			ratio = (float(b['average']) / float(a['average']))
			#print([b['average'], a['average'], ratio])
			ratio = "{:.3f}".format(ratio)

		# Put data in cells
		output += "{0}|{1}|{2}|{3}\n".format(
			name,
			profiles[0]["data"].get(name, missing)["average"],
			profiles[1]["data"].get(name, missing)["average"],
			ratio)

	#print(output)
	#exit(1)

	# Format data in cells
	output = csv_to_table_output(output)
	#print(output)
	#exit(1)

	line_len = len(output.split("\n")[-2])

	# Set output text and background to default colors
	print(NORMAL)

	# Add color to cells
	colored = ""
	lines = output.split("\n")
	for i in range(len(lines)):
		line = lines[i]
		# Add header
		if i < 2:
			colored += line + "\n"
			continue

		# Add column
		new_line = "|"
		columns = [n for n in line.split("|") if n]
		for j in range(len(columns)):
			column = columns[j]
			#print("[" + column.strip().replace(',', '') + "]")
			if j > 0 and j < len(profiles) + 1:
				new_line += profiles[j - 1]["color"] + column + NORMAL + "|"
			elif j == len(profiles) + 1:
				if column.strip() in ["-1.000", "1.000"]:
					new_line += column + NORMAL + "|"
				elif float(column.strip().replace(',', '')) > 1.0:
					new_line += profiles[compare_a]["color"] + column + NORMAL + "|"
				else:
					new_line += profiles[compare_b]["color"] + column + NORMAL + "|"
			else:
				new_line += column + NORMAL + "|"

		colored += new_line + "\n"

	# Replace -1 with NA
	colored = colored.replace("-1.000", "    NA").replace("-1", "NA")
	colored = colored[0 : -2]

	# Add footer
	colored += "| " + ("-" * (line_len - 4)) + " |\n"
	colored += "| " + "Average time in nanoseconds. {0} iterations per test.".format(iterations).ljust(line_len - 4) + " |\n"
	colored += "| " + ("-" * (line_len - 4)) + " |\n"

	print(colored)
