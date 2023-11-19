// took 4 hours to do lmao (initial few hours is generating 3**20 hashes three different times, and the rest of the time is ~1/9 of that for further filtering)
// thank you SystemGlitch (https://github.com/System-Glitch/SHA256) for the library :D
// who said competitive programming is too useless and niche :P
#include <iostream>
#include <chrono>
#include <ctime>
#include "SHA256.h"
#include <unordered_set>
#include <bitset>
#define COLLISION_SPACE (1UL << 35)

using namespace std;

int k;
bitset<5230176601UL> segtree1;
bitset<5230176601UL> segtree2;
uint32_t real_hashes = 3486784401;
uint32_t fake_hashes = 3486784401;


string real_confession[22] = {"This is the secret confession of Richard Buckland",
"to be revealed by anonymous email if I should",
"mysteriously vanish.  I have left the last few hex",
"digits of the SHA256 hash of this message with my",

"trusted solicitor, Dennis Denuto, which will verify",
"that this is indeed my intended and unaltered",
"confession written by me Richard Buckland.",
"",
"Dennis has not seen this confession he has only seen",
"the last few digits of the hash.  I have also sent copies",
"of the last few digits to my bank manager and to my priest",
"Father Brown.",
"",
"On the 10th of February I saw Mark Zukerberg peeping",
"through my window and recording my private and personal",
"conversation with my friend.",
"",
"I confronted him and he was very embarrassed.  He",
"promised to pay me $1 million a year if I would stay",
"silent and not tell anyone I had seen him do this.  I",
"agreed but now I worry that it would be cheaper for him",
"to make me vanish than to keep paying me."};

string fake_confession[] = {"This is the secret confession of Richard Buckland",
"to be revealed by anonymous email if I should",
"mysteriously vanish.  I have left the last few hex",
"digits of the SHA256 hash of this message with my",
"trusted solicitor, Dennis Denuto, which will verify",
"that this is indeed my intended and unaltered",
"confession written by me Richard Buckland.",
"",
"Dennis has not seen this confession he has only seen",
"the last few digits of the hash.  I have also sent copies",
"of the last few digits to my bank manager and to my priest",
"Father Brown.",
"",
"On the 10th of February I saw Mark Zukerberg near my",
"house and we struck up a conversation.  He explained all",
"the things he was doing to ensure that Facebook respects",
"privacy - both of its users and of others.  It was very",
"impressive.",
"",
"I feel awful that I have been criticising Facebook publicly",
"for so long.  I apologised to him in our conversation and",
"now I want to confess to the world that actually Facebook",
"has more than enough privacy features, and that the reason",
"I spend so much time criticising Facebook is that I am",
"envious of Mark and wish I was a clever and smart and wise",
"as he is.  I feel so bad for having been so mean to him for",
"so many years that I am considering retreating to the outback.",
"I may well cut off all contact with the world and live as a",
"hermit from now on.  So do not worry if I vanish it is just",
"that I feel so guilty that I have been so unfair to Facebook."};

uint64_t get_hash_ending(SHA256 &sha) {
	array<uint8_t, 32> hash = sha.digest();
	uint64_t h = 0;
	for (int i = 24; i < 32; i++) {
		h = (h << 8) | (uint64_t) hash[i];
	}
	return h;
}

void generate_hashes(SHA256 &sha, string lines[], void (*callback)(uint64_t, uint64_t, bitset<COLLISION_SPACE> *, bitset<COLLISION_SPACE> *), bitset<COLLISION_SPACE> *collide1, bitset<COLLISION_SPACE> *collide2, bitset<5230176601UL> &segtree, uint64_t node = 0, int depth = 0) {
	if (!segtree[node]) {
		return;
	}
	if (depth == 20) {
		callback(get_hash_ending(sha), node, collide1, collide2);
		return;
	}
	sha.update(lines[depth]);
	SHA256 temp;
	temp = SHA256(sha);
	temp.update("\n");
	generate_hashes(temp, lines, callback, collide1, collide2, segtree, node*3+1, depth+1);
	temp = SHA256(sha);
	temp.update(" \n");
	generate_hashes(temp, lines, callback, collide1, collide2, segtree, node*3+2, depth+1);
	temp = SHA256(sha);
	temp.update("\t\n");
	generate_hashes(temp, lines, callback, collide1, collide2, segtree, node*3+3, depth+1);
	if (!segtree[node*3+1] && !segtree[node*3+2] && !segtree[node*3+3]) {
		segtree[node] = false;
	}
}

void pass1(uint64_t hash, uint64_t index, bitset<COLLISION_SPACE> *collide1, bitset<COLLISION_SPACE> *collide2) {
	uint64_t h = hash % ((1UL<<35)-k);
	collide1->set(h);
}

void pass2(uint64_t hash, uint64_t index, bitset<COLLISION_SPACE> *collide1, bitset<COLLISION_SPACE> *collide2) {
	uint64_t h = hash % ((1UL<<35)-k);
	collide2->set(h);
	if (!(*collide1)[h]) {
		fake_hashes -= 1;
		segtree2[index] = false;
	}
}

void pass3(uint64_t hash, uint64_t index, bitset<COLLISION_SPACE> *collide1, bitset<COLLISION_SPACE> *collide2) {
	uint64_t h = hash % ((1UL<<35)-k);
	if (!(*collide2)[h]) {
		real_hashes -= 1;
		segtree1[index] = false;
	}
}

int main(int argc, char ** argv) {
	SHA256 sha1;
	SHA256 sha2;
	bitset<COLLISION_SPACE> *collide1 = new bitset<COLLISION_SPACE>();
	bitset<COLLISION_SPACE> *collide2 = new bitset<COLLISION_SPACE>();
	segtree1.flip();
	segtree2.flip();
	collide1->reset();
	collide2->reset();
	for (int i = 0; i < 2; i++) {
		sha1.update(real_confession[i]);
		sha1.update("\n");
	}
	for (int i = 0; i < 10; i++) {
		sha2.update(fake_confession[i]);
		sha2.update("\n");
	}
	for (k = 0; k < 4; k++) { // you probably don't need to increase k even for larger hash collisions
		cout << k << "\n";
		uint64_t index1 = 0;
		SHA256 copy = SHA256(sha1);
		generate_hashes(copy, real_confession+2, pass1, collide1, collide2, segtree1); // populate collide1
		cout << "Finished pass 1\n";
		copy = SHA256(sha2);
		generate_hashes(copy, fake_confession+10, pass2, collide1, collide2, segtree2); // populate collide2 and check collide1
		cout << "Finished pass 2: " << fake_hashes << "\n";
		copy = SHA256(sha1);
		generate_hashes(copy, real_confession+2, pass3, collide1, collide2, segtree1); // check collide2
		cout << "Finished pass 3: " << real_hashes << "\n";
		collide1->reset();
		collide2->reset();
		printf("Sizes: %u %u\n", real_hashes, fake_hashes);
	}

	// ceebs writing code to convert the numbers into actual confessions but basically if you subtract
	// 1743392200 from each number, after the first two lines of the real confession and the first 10 lines of the
	// fake confession, its ternary representation will tell you what choices to make when selecting each line
	// (i.e. if 0, append "\n", if 1 append " \n" and if 2 append "\t\n")
	for (uint64_t t = 1743392200; t < 4294967296UL; t++) {
		if (segtree1[t]) {
			cout << t << "\n";
		}
	}
	cout << "now the fake confession id number thing\n";
	for (uint64_t t = 1743392200; t < 4294967296UL; t++) {
		if (segtree2[t]) {
			cout << t << "\n";
		}
	}
	return EXIT_SUCCESS;
}
