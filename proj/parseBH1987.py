import csv

name = []
genus = []
degree = []
group = []

src = []
tgt = []
val = []

rows = []
with open('bh1987.csv') as csvfile :
    my_csv = csv.reader(csvfile)
    for row in my_csv :
        rows.append(row)

col_range = range(3,15)
row_range = range(3, 105)

for i in row_range :
    name.append(rows[i][0][0] + " " + rows[i][1])
    genus.append(rows[i][0])
    degree.append(1)

    for j in col_range :
        if int(rows[i][j]) > 0 :
            degree[-1] = degree[-1] + 1

for i in col_range :
    name.append(rows[0][i][0] + " " + rows[1][i])
    genus.append(rows[0][i])
    degree.append(1)

    for j in row_range :
        if int(rows[j][i]) > 0 :
            degree[-1] = degree[-1] + 1

for i in row_range :
    for j in col_range :
        if int(rows[i][j]) > 0 :
            src.append(i - 3)          # subtract row offset
            tgt.append(j - 3 + 102)    # subtract column offset                           
            val.append(int(rows[i][j]))
            print name[i-3] + " -- " + name[j - 3 + 102]

curr = genus[0]                                                                           
group_num = 0
for i in range(0, len(genus)) :
    if genus[i] != curr :
        curr = genus[i]
        group_num = group_num + 1

    group.append(group_num)

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
