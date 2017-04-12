#credit: http://unix.stackexchange.com/questions/60590/is-there-a-command-line-utility-to-transpose-a-csv-file
import csv, sys
rows = list(csv.reader(sys.stdin))
writer = csv.writer(sys.stdout)
for col in xrange(0, len(rows[0])):
    writer.writerow([row[col] for row in rows])
