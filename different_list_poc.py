def memory_efficient_different_birthday_attack(n, sufficiently_large_number=19, hashspace=None):
	if hashspace is None:
		hashspace = n**2
	from random import randint
	# generate hashes
	hashes1 = [randint(0, hashspace-1) for _ in range(n)]
	hashes2 = [randint(0, hashspace-1) for _ in range(n)]
	for h in hashes1:
		if h in hashes2:
			secret_hash = h
			i1 = hashes1.index(h)
			i2 = hashes2.index(h)
			print(h)
			break
	else:
		print("yikes")
		return
	used1 = [True for _ in range(n)]
	used2 = [True for _ in range(n)]
	collide1 = [False for _ in range(n)]
	collide2 = [False for _ in range(n)]
	used_size1 = n
	used_size2 = n
	for k in range(sufficiently_large_number):
		s1 = 0
		s2 = 0
		m = max(used_size1, used_size2)
		for i in range(n): # init collide arrays with values
			if used1[i]:
				h1 = (hashes1[i] ^ k) % m
				collide1[h1] = True
			if used2[i]:
				h2 = (hashes2[i] ^ k) % m
				collide2[h2] = True
		print(collide1[(secret_hash ^ k) % used_size1], collide2[(secret_hash ^ k) % used_size2])
		for i in range(n): # filter out non-colliding values (should reduce to around 70% of original size)
			if used1[i]:
				h1 = (hashes1[i] ^ k) % m
				if not collide2[h1]:
					used1[i] = False
				else:
					s1 += 1
			if used2[i]:
				h2 = (hashes2[i] ^ k) % m
				if not collide1[h2]:
					used2[i] = False
				else:
					s2 += 1
		for i in range(n): # clear the collide array
			h1 = (hashes1[i] ^ k) % m
			h2 = (hashes2[i] ^ k) % m
			collide1[h1] = False
			collide2[h2] = False
		used_size1 = s1
		used_size2 = s2
		print(used_size1, used_size2)
	for i in range(n): # print possible hash collisions
		if used1[i]:
			print("From list 1: ", hashes1[i])
		if used2[i]:
			print("From list 2: ", hashes2[i])
