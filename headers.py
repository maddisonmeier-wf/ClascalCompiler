import sys

with open(sys.argv[1], "r") as file:
	data = file.readlines()

b = []

for a, i in enumerate(data):
	if i == "{\n":
		b.append(data[a-1])

c = []

for a in b:
	c.append(a.replace("\n", ";"))

with open("headers.txt", 'wb') as f:
    for item in c:
        if item != 'extern "C";':
            f.write("%s\n" % item)

