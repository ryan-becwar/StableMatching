"""
This is a simple python script to parse the KB2009 data from its
original csv into two files for the networkD3 to use in R: a link file and node file
"""

import csv

# node file columns
name = []
degree = []
group = []

# link file columns
src = []
tgt = []
val = []

# Open the CSV and collect all rows
rows = []
with open('kb2009.csv') as csvfile :
    my_csv = csv.reader(csvfile)
    for row in my_csv :
        rows.append(row)

row_range = range(4, 103)
col_range = range(4, 67)

# Get name, origin, and degree of row species
for i in row_range :
    name.append(rows[i][0]) 
    degree.append(0)

    for j in col_range :
        if float(rows[i][j]) > 0.0 :
            degree[-1] = degree[-1] + 1

    if degree[-1] > 3 :
        group.append("high-connected animal")
    else :
        group.append("animal")

# Get name, origin, and degree of column species
for i in col_range :
    name.append(rows[0][i])
    degree.append(0)            # give each node a default degree of 1

    for j in row_range :
        if float(rows[j][i]) > 0.0 :
            # increase degree count with each identified neighbor
            # this will make the node bigger in the final graph
            degree[-1] = degree[-1] + 1     

    if degree[-1] > 3 :
        group.append("highly-connected plant")
    else :
        group.append("plant")

# Find all interactions
for i in row_range :
    for j in col_range :
        if float(rows[i][j]) > 0.0 :
            # record the interaction value -- this will be the weight in
            # the final graph
            value = float(rows[i][j])

            if value > 0.0 :
                src.append(i - 4)          # add species number, subtracting row offset
                tgt.append(j - 4 + 99)    # add species number subtracting column offset 
                val.append(value)
                # print out interaction by name as a sanity check
                print name[i-4] + " -- " + name[j - 4 + 99]

# Write everything out!
link_headers = ["source", "target", "value"]
node_headers = ["group", "name", "degree"] 

link_rows = []
node_rows = []

link_rows.append(link_headers)
node_rows.append(node_headers)

for i in range(0, len(name)) :
    node_row = []
    node_row.append(group[i])
    node_row.append(name[i])
    node_row.append(degree[i])

    node_rows.append(node_row)

for j in range(0, len(src)) :
    link_row = []
    link_row.append(src[j])
    link_row.append(tgt[j])
    link_row.append(val[j])

    link_rows.append(link_row)

with open("kb2009_links.csv", 'wb') as linkfile :
    writer = csv.writer(linkfile)
    writer.writerows(link_rows)

with open("kb2009_nodes.csv", 'wb') as nodefile :
    writer = csv.writer(nodefile)
    writer.writerows(node_rows)
