def memory_efficient_birthday_attack(n, hashspace=None):
	if hashspace is None:
		hashspace = n**2
	from random import randint
	# generate hashes
	hashes = [randint(0, hashspace-1) for _ in range(n)]

	# a collision will occur if these values are different, if not run the function again
	print(len(set(hashes)), len(hashes))
	used = [[True for _ in range(n)], [True for _ in range(n)]]
	collide = [False for _ in range(n)]
	used_size = n
	for k in range(19): #replace 19 with a sufficiently large number until used_size keeps on being the same
		s = 0
		for i in range(n):
			if used[0][i]:
				h = (hashes[i] ^ k) % used_size
				if not collide[h]:
					used[1][i] = False
					collide[h] = True
		for i in range(n-1,-1,-1): # we missed the "first colliding indices" so we're re-including those with a backwards sweep
			if used[0][i]:
				h = (hashes[i] ^ k) % used_size
				if collide[h]:
					collide[h] = False
				else:
					used[1][i] = True
				used[0][i] = used[1][i]
				s += used[0][i]
		used_size = s
		print(used_size)

	# these values aren't always the same, so just sift through them normally
	for i in range(n):
		if used[0][i]:
			print(hashes[i])
