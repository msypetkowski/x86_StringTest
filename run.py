#!/usr/bin/env python3

from subprocess import Popen, PIPE, call

EXECUTABLE = "./test"

print(r'''
---------\
Building |
---------/
''')
call(['make'])

print(r'''
---------\
Running  |
---------/
''')


class Result:
    def __init__(self, name, data):
        self._name = name
        self._data= {k:list(map(int,v)) for k,*v in data}

    def printTable(self):
        print(self._name)
        JUST = 10
        labels = ["", "min", "max", "med", "rel. stddev"]
        lines = []
        lines.append([JUST*"_"]*len(labels))
        lines.append(labels)
        for k,v in self._data.items():
            lines.append([JUST*"-"]*len(labels))
            medVal = sum(v)/len(v)
            relStddev = sum(map(lambda x: (x-medVal)**2, v))**0.5 / medVal
            lines.append([k, min(v), max(v), round(medVal,1), relStddev])
        for l in lines:
            print('|'.join(map(lambda x: str(x).rjust(JUST), l)))
        print()


    def __str__(self):
        ret = self._name + '\n\t'
        ret += '\n\t'.join(map(str,self._data.items()))
        return ret

p = Popen([EXECUTABLE], stdin=PIPE, stdout=PIPE, stderr=PIPE)
out = p.communicate()[0].decode().split('\n')
out = list(map(lambda x: [i.strip() for i in x.split() if i.strip()], out))
out = [o for o in out if o]

print('\n'.join(map(str,out)))

results = []
i = 0
while i < len(out):
    name = out[i][0]
    count = int(out[i][1])
    i += 1
    data=out[i:i+count]
    i += count
    results.append(Result(name,data))

print(r'''
------------\
Results rav |
------------/
''')

print('\n'.join(map(str,results)))

print(r'''
--------------\
Results tables|
--------------/
''')

for r in results:
    r.printTable()

# print(r'''
# ---------\
# Clearing |
# ---------/
# ''')
# call(['make','clean'])
