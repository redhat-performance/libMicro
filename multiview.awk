#!/bin/sh
#
# CDDL HEADER START
#
# The contents of this file are subject to the terms
# of the Common Development and Distribution License
# (the "License").  You may not use this file except
# in compliance with the License.
#
# You can obtain a copy of the license at
# src/OPENSOLARIS.LICENSE
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing
# permissions and limitations under the License.
#
# When distributing Covered Code, include this CDDL
# HEADER in each file and include the License file at
# usr/src/OPENSOLARIS.LICENSE.  If applicable,
# add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your
# own identifying information: Portions Copyright [yyyy]
# [name of copyright owner]
#
# CDDL HEADER END
#

#
# Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#

#
# Modifications by Red Hat, Inc.
#

#
# output html comparison of several libmicro output data files
#   usage: awk -f multiview.awk file1 file2 file3 file4 ...
#
#	Relative ranking is calculated using first as reference; color
#	interpolation is done to indicate relative performance: the redder the
#	color, the slower the result, the greener the faster.
#
#   Green and red colors selected via colorbrewer2.org (selecting single hue
#   green and red color pallettes, 9 data classes, sequential data, colorblind
#   safe schemes, hex, and then throwing out the first light color)
#
BEGIN {
	benchmark_count = 0;
	benchmark_name = "";
	header_count = 0;
	result_header = "";
	base_values = "";
	curr_values = "";
}

/^##/ {
	# Ignore comments from the driver script, bench.sh
	next;
}

/^#/ {
	if (benchmark_name != "") {
		line = ++benchmark_results_linecnt[benchmark_name,FILENAME];
		benchmark_results[benchmark_name,FILENAME,line] = $0;
		if ($2 == "WARNINGS") {
			benchmark_warnings[benchmark_name,FILENAME] = 1;
		}
	}
	next;
}

/errors/ {
	result_header = $0;
	if (base_values == "") {
		base_values = $3;
	}
	curr_values = $3;
	benchmark_values[FILENAME] = $3;
	next;
}

/^\!nh:available:/ {
	split($0, avail, ":");
	numactl_available[FILENAME] = avail[3];
	next;
}

/^\!nh:node:/ {
	split($0, node, ":");
	numactl_header_cnt[FILENAME] = length(node);
	for (i = 2; i <= length(node); i++) {
		numactl_header[FILENAME, i-1] = node[i];
	}
	next;
}

/^\!nh:/ {
	split($0, row, ":");
	idx = numactl_node_idx[FILENAME]++;
	for (i = 2; i <= length(row); i++) {
		numactl_nodes[FILENAME,idx,i-1] = row[i];
	}
	next;
}

/^\!/ {
	val_idx = index($0, ":")
	name = substr($0, 2, (val_idx - 1 - 1));
	headers[name]=name;
	val = substr($0, val_idx+1)
	sub(/^[ \t]+/, "", val);
	sub(/[ \t]+$/, "", val);
	header_data[name,FILENAME] = val;
	if (header_names[name] == 0) {
		header_names[name] = ++header_count;
		headers[header_count] = name;
	}
	next;
}

{
	if (NF >= 7) {
		if (benchmark_names[$1] == 0) {
			benchmark_names[$1] = ++benchmark_count;
			benchmarks[benchmark_count] = $1;
		}
		if ($6 == 0)
			# No errors, result is good
			if (base_values == curr_values)
				benchmark_data[$1,FILENAME] = $4;
			else if (base_values == "nsecs/call") {
				# Must be microseconds
				benchmark_data[$1,FILENAME] = $4 * 1000;
			}
			else {
				benchmark_data[$1,FILENAME] = $4 / 1000;
			}
		else
			# Bad result because of errors
			benchmark_data[$1,FILENAME] = -1;
		benchmark_name = $1;

		if (result_header != "") {
			line = ++benchmark_results_linecnt[benchmark_name,FILENAME];
			benchmark_results[benchmark_name,FILENAME,line] = sprintf("# %s", result_header);
			result_header = "";
		}
		line = ++benchmark_results_linecnt[benchmark_name,FILENAME];
		benchmark_results[benchmark_name,FILENAME,line] = sprintf("# %s", $0);
		line = ++benchmark_results_linecnt[benchmark_name,FILENAME];
		benchmark_results[benchmark_name,FILENAME,line] = "#";
	}
}

END {
	printf("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"\n");
	printf("\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n");
	printf("<html xmlns=\"http://www.w3.org/1999/xhtml\">\n");
	printf("  <head>\n");
	printf("    <meta http-equiv=\"content-type\" content=\"text/html; charset=ISO-8859-1\" />\n");
	printf("    <meta name=\"author\" content=\"autogen\" />\n");
	printf("    <title>multiview comparison</title>\n");
	printf("    <style type=\"text/css\">\n");
	printf("      body { font-family: sans-serif; background-color:#ffffff; }\n");
	printf("      table { border-collapse: collapse; }\n");
	printf("      table.data tr { display: none; }\n");
	printf("      table tr.heading { display:table-row; }\n");
	printf("      table.header tr.header { display:table-row; }\n");
	printf("      table.noise tr.noise { display:table-row; }\n");
	printf("      table.pos tr.pos { display:table-row; }\n");
	printf("      table.neg tr.neg { display:table-row; }\n");
	printf("      table.err tr.err { display:table-row; }\n");
	printf("      table.war tr.war { display:table-row; }\n");
	printf("      table.mis tr.mis { display:table-row; }\n");
	printf("      td { padding: 0.1em; border: 1px solid #ccc; text-align: right; }\n");
	printf("      td.errors { background-color: #ff0000; }\n");
	printf("      td.missing { background-color: #ffff00; }\n");
	printf("      td.numactl_center { background-color: #00ff00 }\n");
	printf("      td.header { text-align: left; }\n");
	printf("      pre { margin-top: 0em; margin-bottom: 0em; color: blue; }\n");
	printf("      td.fast1 pre, td.slow1 pre { color: black; }\n");
	printf("      td.fast2 pre { color: black; background-color: #e5f5e0; }\n");
	printf("      td.fast3 pre { color: black; background-color: #c7e9c0; }\n");
	printf("      td.fast4 pre { color: black; background-color: #a1d99b; }\n");
	printf("      td.fast5 pre { color: black; background-color: #74c476; }\n");
	printf("      td.fast6 pre { color: black; background-color: #41ab5d; }\n");
	printf("      td.fast7 pre { color: black; background-color: #238b45; }\n");
	printf("      td.fast8 pre { color: white; background-color: #006d2c; }\n");
	printf("      td.fast9 pre { color: white; background-color: #00441b; }\n");
	printf("      td.slow2 pre { color: black; background-color: #fee0d2; }\n");
	printf("      td.slow3 pre { color: black; background-color: #fcbba1; }\n");
	printf("      td.slow4 pre { color: black; background-color: #fc9272; }\n");
	printf("      td.slow5 pre { color: black; background-color: #fb6a4a; }\n");
	printf("      td.slow6 pre { color: black; background-color: #ef3b2c; }\n");
	printf("      td.slow7 pre { color: black; background-color: #cb181d; }\n");
	printf("      td.slow8 pre { color: white; background-color: #a50f15; }\n");
	printf("      td.slow9 pre { color: white; background-color: #67000d; }\n");
	printf("    </style>\n");
	printf("  </head>\n");
	printf("  <body link=\"#0000ee\" vlink=\"#cc0000\" alink=\"#0000ee\">\n");

	printf("    <table border=\"2\" cellspacing=\"2\">\n");
	printf("      <tbody>\n");

	printf("        <tr>\n");
	printf("          <th class=\"header\">Toggle:</th>\n");
	printf("          <th class=\"header\"><button onclick=\"toggleRows('header')\">Headers</button></th>\n");
	printf("          <th class=\"header\"><button onclick=\"toggleRows('noise')\">Noise</button></th>\n");
	printf("          <th class=\"header\"><button onclick=\"toggleRows('pos')\">Positives</button></th>\n");
	printf("          <th class=\"header\"><button onclick=\"toggleRows('neg')\">Negatives</button></th>\n");
	printf("          <th class=\"header\"><button onclick=\"toggleRows('err')\">Errors</button></th>\n");
	printf("          <th class=\"header\"><button onclick=\"toggleRows('war')\">Warnings</button></th>\n");
	printf("          <th class=\"header\"><button onclick=\"toggleRows('mis')\">Missing</button></th>\n");
	printf("        </tr>\n");

	printf("      </tbody>\n");
	printf("    </table>\n");

	printf("    <table id=\"maintable\" border=\"1\" cellspacing=\"1\" class=\"data header noise pos neg err mis\">\n");
	printf("      <tbody>\n");

	printf("        <tr class=\"heading\">\n");
	printf("          <th>PARAMETER</th>\n");
	printf("          <th>VALUE</th>\n");
	for (i = 2; i < ARGC; i++)
		printf("          <th>VALUE</th>\n");
	printf("        </tr>\n");

	for(i = 1; i <= header_count; i++) {
		hname = headers[i];
		printf("        <tr class=\"header\">\n");
		printf("          <td class=\"header\">%s</td>\n", hname);
		for (j = 1; j < ARGC; j++) {
			sub("^[\t ]+", "", header_data[hname, ARGV[j]]);
			printf("          <td class=\"header\">%s</td>\n", header_data[hname, ARGV[j]]);
		}
		printf("        </tr>\n");
	}

	printf("        <tr class=\"heading\">\n");
	printf("          <th>BENCHMARK</th>\n");
	printf("          <th align=\"right\">%s</th>\n", toupper(base_values));
	for (i = 2; i < ARGC; i++)
		printf("          <th align=\"right\">%s [percentage]</th>\n", toupper(base_values));
	printf("        </tr>\n");

	for (i = 1; i <= benchmark_count; i++) {
		name = benchmarks[i];
		a = benchmark_data[name, ARGV[1]];

		errors = 0;
		missing = 0;
		if (a < 0) {
			errors++;
		}
		else if (a == 0) {
			missing++;
		}

		warnings = 0;
		if (benchmark_warnings[name, ARGV[1]]) {
			warnings++;
		}

		positives = 0;
		negatives = 0;
		noise = 0;
		for (j = 2; j < ARGC; j++) {
			b = benchmark_data[name, ARGV[j]];
			if (b > 0) {
				if (a > 0) {
					factor = b/a;

					if (factor > 1)
						percentage = -((factor * 100) - 100);
					if (factor <= 1)
						percentage =   (100 / factor) - 100;

					class = colormap(percentage);

					if (factor > 1) {
						if (class != "slow1")
							negatives++;
						else
							noise++;
					}
					else {
						# assert (factor <= 1)
						if (class != "fast1")
							positives++;
						else
							noise++;
					}
				}
			}
			else if (b == 0) {
				missing++;
			}
			else {
				# assert (b < 0)
				errors++;
			}
			if (benchmark_warnings[name, ARGV[j]]) {
				warnings++;
			}
		}

		benchmark_summary_data[name, "pos"] = positives;
		benchmark_summary_data[name, "neg"] = negatives;
		benchmark_summary_data[name, "err"] = errors;
		benchmark_summary_data[name, "war"] = warnings;
		benchmark_summary_data[name, "mis"] = missing;
		if (noise && positives == 0 && negatives == 0)
			benchmark_summary_data[name, "noise"] = 1;
		else
			benchmark_summary_data[name, "noise"] = 0;
	}

	# Bubble sort the names of the benchmarks
	for (i = 1; i < benchmark_count; i++) {
		for (j = 1; j < benchmark_count; j++) {
			if (benchmarks[j] > benchmarks[j + 1]) {
				tmp = benchmarks[j];
				benchmarks[j] =	 benchmarks[j+1];
				benchmarks[j+1] = tmp;
			}
		}
	}

	for (i = 1; i <= benchmark_count; i++) {
		name = benchmarks[i];
		a = benchmark_data[name, ARGV[1]];

		if (benchmark_summary_data[name, "noise"] > 0)
			c_noise = "noise";
		else
			c_noise = "";

		if (benchmark_summary_data[name, "pos"] > 0)
			c_pos = " pos";
		else
			c_pos = "";

		if (benchmark_summary_data[name, "neg"] > 0)
			c_neg = " neg";
		else
			c_neg = "";

		if (benchmark_summary_data[name, "err"] > 0)
			c_err = " err";
		else
			c_err = "";

		if (benchmark_summary_data[name, "war"] > 0)
			c_err = " war";
		else
			c_err = "";

		if (benchmark_summary_data[name, "mis"] > 0)
			c_mis = " mis";
		else
			c_mis = "";

		printf("        <tr class=\"%s%s%s%s%s%s\">\n", c_noise, c_pos, c_neg, c_err, c_war, c_mis);
		printf("          <td>%s</td>\n", name);

		printf("          <td id=\"%s_1\" onclick=\"showHide('%s_1'); return false;\"", name, name);
		if (a > 0) {
			if (base_values == "nsecs/call" && base_values == benchmark_values[ARGV[1]]) {
				printf("><pre>%5d</pre></td>\n", a);
			}
			else {
				printf("><pre>%11.5f</pre></td>\n", a);
			}
		}
		else {
			if (a < 0)
				printf(" class=\"errors\">%s</td>\n", "ERRORS");
			else
				printf(" class=\"missing\">%s</td>\n", "missing");
		}

		for (j = 2; j < ARGC; j++) {
			printf("          <td id=\"%s_%d\" onclick=\"showHide('%s_%d'); return false;\"", name, j, name, j);
			b = benchmark_data[name, ARGV[j]];
			if (b > 0) {
				if (a > 0) {
					factor = b/a;
					if (factor > 1)
						percentage = -((factor * 100) - 100);
					if (factor <= 1)
						percentage =   (100 / factor) - 100;
					class = colormap(percentage);

					if (base_values == "nsecs/call" && base_values == benchmark_values[ARGV[j]]) {
						printf(" class=\"%s\"><pre>%5d [%#+7.1f%%]</pre></td>\n",
								class, b, percentage);
					}
					else {
						printf(" class=\"%s\"><pre>%11.5f [%#+7.1f%%]</pre></td>\n",
								class, b, percentage);
					}
				}
				else {
					if (base_values == "nsecs/call" && base_values == benchmark_values[ARGV[j]]) {
						printf("><pre>%5d           </pre></td>\n",	b);
					}
					else {
						printf("<pre>%11.5f           </pre></td>\n", b);
					}
				}
			}
			else if (b < 0)
				printf(" class=\"errors\">%s</td>\n", "ERRORS");
			else
				printf(" class=\"missing\">%s</td>\n", "missing");

		}
		printf("        </tr>\n");

	}
	printf("      </tbody>\n");
	printf("    </table>\n");

	printf("    <table border=\"2\" cellspacing=\"2\">\n");
	printf("      <tbody>\n");

	printf("        <tr>\n");
	printf("          <th class=\"header\">Column #</th>\n");
	printf("          <th class=\"header\">Result File</th>\n");
	printf("        </tr>\n");

	for (j = 1; j < ARGC; j++) {
		printf("        <tr>\n");
		printf("          <td class=\"header\">%d</td>\n", j);
		printf("          <td class=\"header\">%s</td>\n", ARGV[j]);
		printf("        </tr>\n");
	}

	printf("      </tbody>\n");
	printf("    </table>\n");

	printf("    <table border=\"4\" cellspacing=\"2\">\n");
	printf("      <tbody>\n");

	printf("        <tr>\n");
	for (j = 1; j < ARGC; j++) {
		file = ARGV[j];
		printf("          <th class=\"header\">%s<br>Available: %s</th>\n", file, numactl_available[file]);
	}
	printf("        </tr>\n");

	printf("        <tr>\n");

	for (j = 1; j < ARGC; j++) {
		file = ARGV[j];
		printf("          <td>\n");
		printf("            <table border=\"1\" cellspacing=\"1\">\n");
		printf("              <tbody>\n");

		printf("                <tr>\n");
		cnt = numactl_header_cnt[file];
		for (i = 2; i < cnt; i++) {
			printf("                  <th class=\"header\">%s</th>\n", numactl_header[file, i]);
		}
		printf("                </tr>\n");

		for (k = 0; k < numactl_node_idx[file]; k++) {
			printf("                <tr>\n");
			for (i = 2; i < cnt; i++) {
				val = numactl_nodes[file,k,i]
				if (cnt < 4)
					printf("                  <td>%s</td>\n", val);
				else {
					val = int(val)
					if (val == 10)
						printf("                  <td class=\"numactl_center\">%s</td>\n", val);
					else
						printf("                  <td>%s</td>\n", val);
				}
			}
			printf("                </tr>\n");
		}

		printf("              </tbody>\n");
		printf("            </table>\n");
		printf("          </td>\n");
	}

	printf("        </tr>\n");

	printf("      </tbody>\n");
	printf("    </table>\n");

	for (i = 1; i <= benchmark_count; i++) {
		name = benchmarks[i];
		for (j = 1; j < ARGC; j++) {
			cnt = benchmark_results_linecnt[name, ARGV[j]];
			printf("    <div id=\"%s_%d_res\" onclick=\"showHide('%s_%d'); return false;\" title=\"Results for %s from %s\" style=\"display:none; padding: 2px; border: 2px solid #000; position: absolute; background: #fff\">\n",
				   name, j, name, j, name, ARGV[j]);
			printf("      <pre>\n");
			for (k = 1; k <= cnt; k++) {
				printf("%s\n", benchmark_results[name,ARGV[j],k]);
			}
			printf("      </pre>\n");
			printf("    </div>\n");
		}
	}

	printf("    <script>\n");
	printf("      function GetAbsPosition(object) {\n");
	printf("        var position = new Object;\n");
	printf("        position.x = 0;\n");
	printf("        position.y = 0;\n");
	printf("        if (object) {\n");
	printf("          position.x = object.offsetLeft;\n");
	printf("          position.y = object.offsetTop;\n");
	printf("          if (object.offsetParent) {\n");
	printf("            var parentpos = GetAbsPosition(object.offsetParent);\n");
	printf("            position.x += parentpos.x;\n");
	printf("            position.y += parentpos.y;\n");
	printf("          }\n");
	printf("        }\n");
	printf("        position.cx = object.offsetWidth;\n");
	printf("        position.cy = object.offsetHeight;\n");
	printf("        return position;\n");
	printf("      }\n");
	printf("      function showHide(shID) {\n");
	printf("        var shID_el = document.getElementById(shID);\n");
	printf("        var shID_res_el = document.getElementById(shID+'_res');\n");
	printf("        if (shID_el && shID_res_el) {\n");
	printf("          if (shID_res_el.style.display != 'none') {\n");
	printf("            shID_res_el.style.display = 'none';\n");
	printf("          }\n");
	printf("          else {\n");
	printf("            var pos = GetAbsPosition(shID_el);\n");
	printf("            shID_res_el.style.display = 'block';\n");
	printf("            shID_res_el.style.top = pos.y + 'px';\n");
	printf("            shID_res_el.style.left = pos.x + 'px';\n");
	printf("          }\n");
	printf("        }\n");
	printf("      }\n");
	printf("      function toggleRows(c) {\n");
	printf("        document.getElementById('maintable').classList.toggle(c);\n");
	printf("      }\n");
	printf("    </script>\n");

	printf("  </body>\n");
	printf("</html>\n");
}

function colormap(percentage)
{
	if (percentage < 0) {
		norm_percent = percentage * -1;
		map = "slow";
	}
	else {
		norm_percent = percentage;
		map = "fast";
	}

	if (norm_percent < 2.0) {
		idx = 1;
	}
	else if (norm_percent >= 100.0) {
		idx = 9;
	}
	else if (norm_percent >= 86.0) {
		idx = 8;
	}
	else if (norm_percent >= 72.0) {
		idx = 7;
	}
	else if (norm_percent >= 58.0) {
		idx = 6;
	}
	else if (norm_percent >= 44.0) {
		idx = 5;
	}
	else if (norm_percent >= 30.0) {
		idx = 4;
	}
	else if (norm_percent >= 16.0) {
		idx = 3;
	}
	else if (norm_percent >= 2.0) {
		idx = 2;
	}
	else {
		idx = 1;
	}

	return sprintf("%s%d", map, idx);
}
