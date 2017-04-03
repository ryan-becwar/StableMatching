"""
This is a simple python script to parse the BH1987 data from its
original csv into two files for the networkD3 to use in R: a link file and node file
"""

import csv

# node file columns
name = []
genus = []
degree = []
group = []

# link file columns
src = []
tgt = []
val = []

# Open the CSV and collect all rows
rows = []
with open('bh1987.csv') as csvfile :
    my_csv = csv.reader(csvfile)
    for row in my_csv :
        rows.append(row)

col_range = range(3,15)
row_range = range(3, 105)

# Get name, genus, and degree of row species
for i in row_range :
    name.append(rows[i][0][0] + " " + rows[i][1])
    genus.append(rows[i][0])
    degree.append(1)

    for j in col_range :
        if int(rows[i][j]) > 0 :
            degree[-1] = degree[-1] + 1

# Get name, genus, and degree of column species
for i in col_range :
    name.append(rows[0][i][0] + " " + rows[1][i])
    genus.append(rows[0][i])
    degree.append(1)            # give each node a default degree of 1

    for j in row_range :
        if int(rows[j][i]) > 0 :
            # increase degree count with each identified neighbor
            # this will make the node bigger in the final graph
            degree[-1] = degree[-1] + 1     

# Find all interactions
for i in row_range :
    for j in col_range :
        if int(rows[i][j]) > 0 :
            src.append(i - 3)          # add species number, subtracting row offset
            tgt.append(j - 3 + 102)    # add species number subtracting column offset 

            # record the interaction value -- this will be the weight in
            # the final graph
            val.append(int(rows[i][j])) 

            # print out interaction by name as a sanity check
            print name[i-3] + " -- " + name[j - 3 + 102]

# assign each genus an integer value for the network
curr = genus[0]                                                                           
group_num = 0
for i in range(0, len(genus)) :
    if genus[i] != curr :
        curr = genus[i]
        group_num = group_num + 1

    group.append(group_num)

# Write everything out!
link_headers = ["source", "target", "value"]
node_headers = ["genus", "name", "degree", "group"]

link_rows = []
node_rows = []

link_rows.append(link_headers)
node_rows.append(node_headers)

for i in range(0, len(name)) :
    node_row = []
    node_row.append(genus[i])
    node_row.append(name[i])
    node_row.append(degree[i])
    node_row.append(group[i])

    node_rows.append(node_row)

for j in range(0, len(src)) :
    link_row = []
    link_row.append(src[j])
    link_row.append(tgt[j])
    link_row.append(val[j])

    link_rows.append(link_row)

with open("bh1987_links.csv", 'wb') as linkfile :
    writer = csv.writer(linkfile)
    writer.writerows(link_rows)

with open("bh1987_nodes.csv", 'wb') as nodefile :
    writer = csv.writer(nodefile)
    writer.writerows(node_rows)
