#!/usr/bin/python3
import sys
from subprocess import Popen, PIPE

INTERACTOR = 'interactor'
INPUTFILE = 'input.txt'
OUTPUTFILE = 'output.txt'

solution = sys.argv[1]
interaction_args = [INTERACTOR, INPUTFILE, OUTPUTFILE]

p0 = Popen(interaction_args, stdin=PIPE, stdout=PIPE)
p1 = Popen([solution], stdin=p0.stdout, stdout=p0.stdin)

p0.stdin.close()
p0.stdout.close()
p0.wait()
p1.wait()

print("Interactor ended with code " + str(p0.returncode))
print("Solution ended with code " + str(p1.returncode))
