from random import randint
from sys import argv
if len(argv) == 1:
	print(f"Usage: {argv[0]} <number of processess> <max random>")
	exit()
maxr = int(argv[2]) if len(argv) > 2 else 10 # Default randint 10
for i in range(1, int(argv[1])+1):
	print(f"P{i}:{randint(1, maxr)}:{randint(1, maxr)}:{randint(1, maxr)}")
