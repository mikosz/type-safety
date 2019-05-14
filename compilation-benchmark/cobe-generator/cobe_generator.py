#!/usr/bin/env python

# Copyright (c) Techland. All rights reserved.
# Licensed under the MIT License. See License.txt in the project root for license information.

import logging
import argparse
import re
import sys
import os

def make_filename(template, i):
	fn = os.path.basename(template)
	m = re.match("([^\.]+).([^\.]+)\.cobe", fn)
	if not m:
		logging.error("Invalid template '%s' filename - expected format 'NAME.EXT.cobe'", fn)
		sys.exit(1)
	else:
		return m.group(1) + "." + str(i) + "." + m.group(2)

def find_arg_end(template_contents, pos):
	end_pos = template_contents.find("}", pos)
	if end_pos == -1:
		logging.error("Parse error around: ", template_contents[pos:20])
		sys.exit(1)
	else:
		internal_start_pos = template_contents.find("${", pos, end_pos)
		if internal_start_pos != -1:
			return find_arg_end(template_contents, end_pos + 1)
		else:
			return end_pos

def parse_arg(template_contents, pos):
	end_pos = find_arg_end(template_contents, pos)
	return template_contents[pos:end_pos], end_pos + 1

def parse_placeholder(template_contents, i, num, pos):
	args = []
	fun_pos = pos
	while template_contents[fun_pos] == '{':
		arg, fun_pos = parse_arg(template_contents, fun_pos + 1)
		args.append(arg)

	fun = template_contents[fun_pos]

	if fun == "#":
		if not args:
			val = i
		elif args[0] == "-1":
			val = i - 1;
		elif args[0] == "+1":
			val = i + 1;
		else:
			logging.error("Can't solve argument %s for function %s", args[0], fun)
			sys.exit(1)
		return str(val), fun_pos + 1
	elif fun == "E":
		if args[0] == "-1":
			val = (i > 0)
		elif args[0] == "+1":
			val = (i < num - 1)
		else:
			logging.error("Can't solve argument %s for function %s", args[0], fun)
			sys.exit(1)

		if val:
			return parse(args[1], i, num), fun_pos + 1
		elif len(args) > 2:
			return parse(args[2], i, num), fun_pos + 1
		else:
			return "", fun_pos + 1
	else:
		logging.error("Can't handle function %s", fun)
		sys.exit(1)

def parse(template_contents, i, num):
	text = ""
	
	from_pos = 0
	to_pos = template_contents.find("$", from_pos)
	
	while to_pos != -1:
		text += template_contents[from_pos:to_pos]
		contents, from_pos = parse_placeholder(template_contents, i, num, to_pos + 1)
		text += contents
		to_pos = template_contents.find("$", from_pos)
	
	text += template_contents[from_pos:]

	return text

def write(f, template_contents, i, num):
	f.write(parse(template_contents, i, num))

def generate(template, num, outdir):
	for i in range(num):
		fn = make_filename(template, i)
		logging.debug("Generating %s", fn)

		with open(template, "r") as f:
			template_contents = f.read()

		if not os.path.exists(outdir):
			os.makedirs(outdir)

		with open(outdir + "/" + fn, "w") as f:
			write(f, template_contents, i, num)

def main(args=None):
	logging.basicConfig(level=logging.INFO)

	parser = argparse.ArgumentParser()
	parser.add_argument("-o", "--outdir", required=True, help="Output directory")
	parser.add_argument("-n", "--num", required=True, type=int, help="Number of files to generate")
	parser.add_argument("input", nargs="+", help="Input files")

	args = parser.parse_args()

	for i in args.input:
		generate(i, args.num, args.outdir)

if __name__ == '__main__':
	main()
