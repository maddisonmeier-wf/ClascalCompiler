import os

fname = "test.c"
code = list()
with open(fname) as f:
    code = f.readlines()

file_path = ""
with open(fname) as f:
    file_path = os.path.dirname(os.path.realpath("test.c")) + "/test.c"
    

bb_stats = dict()
for line in code:
    line = line.strip()
    if len(line) > 0:
        if line[0] is 'b':
            line = line.replace(":", "")
            bb_stats[line] = line

os.remove(file_path)
output = open("test.c", 'w')

for line in code:
    if "if" in line and "bb" in line:
        orig_line = line
        line = line.split(" ")[-1]
        line = line.replace(";", "")
        line = line.replace("\n", "")
        if line not in bb_stats:
            pass
        else:
            output.write(orig_line)
    else:
        output.write(line)