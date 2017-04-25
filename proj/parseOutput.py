"""
Parses dataset from output file and attaches to dataframe for network links.
"""
import csv

greedy = [8, 1, 6, 3, 0, 5, 2, 10, 7, 9, 11, 4]
pagerank = [11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0]
regret = [8, 1, 6, 0, 3, 4, 2, 5, 7, 10, 11, 9]

nodes = open('bh_nodes.csv', 'r')
node_reader = csv.reader(nodes)
node_rows = []

for r in node_reader :
    node_rows.append(r)

node_rows[0].append('greedy_priority')
node_rows[0].append('regret_priority')
node_rows[0].append('pagerank_priority')
for i in range(0, len(greedy)) :
    if i < 6 :
        group = 'high priority' 
    else :  
        group = 'medium priority'
    
    node_rows[greedy[i] + 103].append(group)
    node_rows[regret[i] + 103].append(group)
    node_rows[pagerank[i] + 103].append(group)
    
    expected_length = len(node_rows[greedy[i] + 103])

for n in node_rows :
    if len(n) < expected_length :
        n.append('low priority')
        n.append('low priority')
        n.append('low priority')

# get link_rows csv
link_rows = []
links = open('bh_links.csv', 'r')
link_reader = csv.reader(links)
for row in link_reader :
    link_rows.append(row)

for i in range(1, len(link_rows)) :
    for j in range(0, len(link_rows[i])) :
        link_rows[i][j] = int(link_rows[i][j])

scores = ['greedy', 'pagerank', 'regret']
score_rows = []
skip = 0
for score in scores :
    # get matrix into csv format
    score_file = open('bh_' + score + '.csv', 'r')
    score_reader = csv.reader(score_file)
    for srow in score_reader :
        if skip >= 2 :
            score_rows.append(srow)
        else :
            skip = skip + 1
  
    # clean up values of extra space 
    for i in range(0, len(score_rows)) :
        for j in range(0, len(score_rows[i])) :
            score_rows[i][j] = score_rows[i][j].split(' ')[-1]

    vals = []
    for i in range(0, len(score_rows)) :
        tgt = i + 103 - 1
        src = score_rows[i].index('1')
        vals.append((src, tgt))

    link_rows[0].append(score + '_pairing')
    for v in vals :
        print v
        for l in link_rows :
            if l[0] == v[0] and l[1] == v[1] :
                l.append(1)

    for l in link_rows :
        if len(l) < len(link_rows[0]) :
            l.append(0)
    skip = 0
    score_rows = []

for l in link_rows :
    print l

fptr = open("bh_nodes_temp.csv", "w")
writer = csv.writer(fptr)
writer.writerows(node_rows)

fptr = open("bh_links_temp.csv", "w")
writer = csv.writer(fptr)
writer.writerows(link_rows)                                                               
