# available: 2 nodes (0-1)
# node 0 cpus: 0 2 4 6 8 10 12 14
# node 0 size: 12277 MB
# node 0 free: 10510 MB
# node 1 cpus: 1 3 5 7 9 11 13 15
# node 1 size: 12287 MB
# node 1 free: 10896 MB
# node distances:
# node   0   1 
#   0:  10  20 
#   1:  20  10 

# available: 1 nodes (0)
# node 0 cpus: 0 1 2 3
# node 0 size: 8075 MB
# node 0 free: 682 MB
# node distances:
# node   0 
#   0:  10 


BEGIN { 
    available_header = "";
    node_idx = 0;
    name_idx = 0;
}

/^available:/ {
    available_header = $0;
    next;
}

/^node [0-9]+ [a-z]+:/ {
    split($0, line, ":");
    split(line[1], items, " ");
    node = int(items[2]);
    nodes_by_node[node]++;
    if (nodes_by_node[node] == 1) {
        nodes_by_idx[node_idx] = node;
        node_idx++;
    }
    name = items[3];
    names_by_name[name]++;
    if (names_by_name[name] == 1) {
        names_by_idx[name_idx++] = name;
    }
    node_data[node,name] = line[2];
    next;
}

/^node distances:/ {
    next;
}

/^node +[0-9]/ {
    theline = $0;
    gsub("/  +/", " ", theline);
    split(theline, tmp, " ");
    nidx = 0;
    for (idx = 1; idx <= length(tmp); idx++) {
        if (tmp[idx] == "node")
            continue;
        nidx = idx - 2;
        if (nodes_by_idx[nidx] != tmp[idx]) {
            printf("FAILED: expected node, '%d', at idx, %d, found '%d'\n", tmp[idx], nidx, nodes_by_idx[nidx]);
            exit 1;
        }
    }
    if ((nidx + 1) != node_idx) {
        printf("FAILED: expected total node count of %d, found %d\n", node_idx, (nidx + 1));
        exit 1;
    }
    next;
}

/^ +[0-9]+: / {
    split($0, columns, ":");
    node = int(columns[1]);
    gsub("/  +/", " ", columns[2]);
    split(columns[2], dist, " ");
    for (i = 1; i <= length(dist); i++)
    for (idx = 0; idx < node_idx; idx++) {
        distances[node,nodes_by_idx[idx]] = int(dist[idx + 1]);
    }
    next;
}

{
    next;
}

END {
    printf("!nh:%s\n", available_header);

    printf("!nh:%4s", "node");
    for (name in names_by_name) {
        printf(":%s", name);
    }
    for (idx = 0; idx < node_idx; idx++) {
        printf(":%d", nodes_by_idx[idx]);
    }
    printf("\n");
    for (idx = 0; idx < node_idx; idx++) {
        node = nodes_by_idx[idx];
        printf("!nh:%d", node);
        for (name in names_by_name)
            printf(":%s", node_data[node,name]);
        for (jdx = 0; jdx < node_idx; jdx++) {
            printf(":%d", distances[node,nodes_by_idx[jdx]]);
        }
        printf("\n");
    }
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
