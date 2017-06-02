#!/usr/bin/env python3

from subprocess import Popen, PIPE, call

EXECUTABLE = "./test"

print('''
---------\\
Building |
---------/
''')
call(['make'])

print('''
---------\\
Running  |
---------/
''')


class Result:
    def __init__(self, name, data):
        self._name = name
        self._data= {k:list(map(int,v)) for k,*v in data}

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

print('''
---------\\
Results  |
---------/
''')

print('\n'.join(map(str,results)))

print('''
---------\\
Clearing |
---------/
''')
call(['make','clean'])
