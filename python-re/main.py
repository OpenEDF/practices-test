import re

# read the original file
oldtext = []
with open('text.txt') as files:
    content= files.readlines()
    oldtext.append(content)
files.close()
#print(oldtext)


# extract the data
newtext = []
for text in oldtext:
    text = str(text)
    match = re.findall(r"cdc=(\d+) ",text)
    newtext.append(match)
newtext = sum(newtext, [])
print(newtext)


# processor data
for i in range(len(newtext)):
    newtext[i] = 'cdc = ' + newtext[i] + '\n'

# write dat to file
wfile = open('macro.txt', 'w')
for line in newtext:
    wfile.write(line)
wfile.close()
