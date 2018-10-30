import sys
import random

def station(x):
	return chr(97 + x)

def generate(link_number):
	n = 20
	print(n)
	stn = []
	for i in range(n):
		stn += [station(i)]
	print(*stn, sep=",")

	for i in range(n):
		for j in range(n):
			print(random.randint(0, 10), end=" ")
		print("")

	for i in range(n):
		if i != 0:
			print(",", end="")
		print(random.randint(1, 9) / 10, end="")
	print("")

	link_number -= 3
	x = [1, 1, 1]

	while link_number > 0:
		x[random.randint(0, 2)] += 1
		link_number -= 1;

	for i in range(3):
		random.shuffle(stn)
		print(*stn[:x[i]], sep=",")

	print(50000)
	print("70,80,70")

if __name__ == "__main__":
	generate(int(sys.argv[1]))