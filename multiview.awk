#
# output html comparison of several libmicro output data files
# usage: multiview file1 file2 file3 file4 ...
#
#	relative ranking is calculated using first as reference
#	color interpolation is done to indicate relative performance;
#	the redder the color, the slower the result, the greener the
#       faster

BEGIN { 
	benchmark_count = 0;
	benchmark_name = "";
	header_count = 0;
	result_header = "";
}

/^##/ {
	# Ignore comments from the driver script, bench.sh
	next;
}

/^#/ {
	if (benchmark_name != "") {
        line = ++benchmark_results_linecnt[benchmark_name,FILENAME];
		benchmark_results[benchmark_name,FILENAME,line] = $0;
    }
	next;
}

/errors/ {
	result_header = $0;
	next;
}

/^\!nh:/ {
    # Ignore numactl --hardware summary until we can process it properly
	next;
}

/^\!/ {
	split($0, A_header, ":");
	name = substr(A_header[1],2);
	gsub(/^[ \t]+/, "", name);
	gsub(/[ \t]+$/, "", name);
	headers[name]=name;
	val = A_header[2];
	gsub(/^[ \t]+/, "", val);
	gsub(/[ \t]+$/, "", val);
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
		   	benchmark_data[$1,FILENAME] = $4;
		else 
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
	printf("      body { font-family: sans-serif; }\n");
	printf("      table { border-collapse: collapse; }\n");
	printf("      td { padding: 0.1em; border: 1px solid #ccc; text-align: right; }\n");
	printf("      td.header { text-align: left; }\n");
	printf("      pre { margin-top: 0em; margin-bottom: 0em; }\n");
	printf("    </style>\n");
	printf("  </head>\n");
	printf("  <body bgcolor=\"#ffffff\" link=\"#0000ee\" vlink=\"#cc0000\" alink=\"#0000ee\">\n");
	printf("    <table border=\"1\" cellspacing=\"1\">\n");
	printf("      <tbody>\n");
	for(i = 1; i <= header_count; i++) {
		hname = headers[i];
		printf("        <tr>\n");
		printf("          <td class=\"header\">%s</td>\n", hname);
		for (j = 1; j < ARGC; j++) {
			sub("^[\t ]+", "", header_data[hname, ARGV[j]]);
			printf("          <td class=\"header\">%s</td>\n", header_data[hname, ARGV[j]]);
		}
		printf("        </tr>\n");
	}

	printf("        <tr>\n");
	printf("          <th>BENCHMARK</th>\n");
	printf("          <th align=\"right\">USECS</th>\n");
	for (i = 2; i < ARGC; i++);
		printf("          <th align=\"right\">USECS [percentage]</th>\n");
	printf("        </tr>\n");

	# Bubble sort the names of the benchmarks
	for (i = 1; i < benchmark_count; i++) {
		for (j = 1; j < benchmark_count; j++) {
			if (benchmarks[j] > benchmarks[j + 1]) {
				tmp = benchmarks[j]; 
                benchmarks[j] =  benchmarks[j+1];
                benchmarks[j+1] = tmp;
            }
        }
	}

	for (i = 1; i <= benchmark_count; i++) {
		name = benchmarks[i];
		a = benchmark_data[name, ARGV[1]];

		printf("        <tr>\n");
		printf("          <td>%s</td>\n", name);

        printf("          <td id=\"%s_1\" onclick=\"showHide('%s_1'); return false;\"", name, name);
		if (a > 0)
			printf("><pre>%f</pre></td>\n", a);
		else {
			if (a < 0)
				printf(" bgcolor=\"#ff0000\">%s</td>\n", "ERRORS");
			else 
				printf(" bgcolor=\"#ffff00\">%s</td>\n", "missing");
				
			for (j = 2; j < ARGC; j++) 
				printf("          <td id=\"%s_%d\" onclick=\"showHide('%s_%d'); return false;\">%s</td>\n", name, j, name, j, "not computed");
			continue;
		}

		for (j = 2; j < ARGC; j++) {
            printf("          <td id=\"%s_%d\" onclick=\"showHide('%s_%d'); return false;\"", name, j, name, j);
			b = benchmark_data[name, ARGV[j]];
			if (b > 0) {
				factor = b/a;
				bgcolor = colormap(factor);
				if (factor > 1)
				  percentage = -(factor * 100 - 100);
				if (factor <= 1)
				  percentage = 100/factor - 100;
				
				printf(" bgcolor=\"%s\"><pre>%11.5f[%#+7.1f%%]</pre></td>\n", 
					bgcolor, b, percentage);
			}
			
			else if (b < 0) 
				printf(" bgcolor=\"#ff0000\">%s</td>\n", "ERRORS");
			else
				printf(" bgcolor=\"#ffff00\">%25s</td>\n", "missing");
			
		}
		printf("        </tr>\n");

	}
	printf("      </tbody>\n");
    printf("    </table>\n");

	printf("    <table border=\"1\" cellspacing=\"1\">\n");
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
    printf("      	var shID_el = document.getElementById(shID);\n");
    printf("      	var shID_res_el = document.getElementById(shID+'_res');\n");
    printf("      	if (shID_el && shID_res_el) {\n");
    printf("      	  if (shID_res_el.style.display != 'none') {\n");
    printf("      	    shID_res_el.style.display = 'none';\n");
    printf("      	  }\n");
    printf("      	  else {\n");
    printf("            var pos = GetAbsPosition(shID_el);\n");
    printf("      	    shID_res_el.style.display = 'block';\n");
    printf("      	    shID_res_el.style.top = pos.y + 'px';\n");
    printf("      	    shID_res_el.style.left = pos.x + 'px';\n");
    printf("      	  }\n");
    printf("      	}\n");
    printf("      }\n");
	printf("    </script>\n");

    printf("  </body>\n");
    printf("</html>\n");
} 

function colormap(value, bgcolor, r, g, b) 
{	
	if (value <= .2)
		value = .2;
	if (value > 5)
		value = 5;

	if (value < .9) {
		r = colorcalc(.2, value, .9, 0, 255);
		g = colorcalc(.2, value, .9, 153, 255);
		b = colorcalc(.2, value, .9, 0, 255);
		bgcolor=sprintf("#%2.2x%2.2x%2.2x",  r, g, b);
	}
	else if (value < 1.1)
		bgcolor="#ffffff";
	else {
		r = 255;
		g = colorcalc(1.1, value, 5, 255, 0);
		b = colorcalc(1.1, value, 5, 255, 0);
		bgcolor=sprintf("#%2.2x%2.2x%2.2x",  r, g, b);
	}

	return (bgcolor);
}

function colorcalc(min, value, max, mincolor, maxcolor)
{
	return((value - min)/(max-min) * (maxcolor-mincolor) + mincolor);
}
