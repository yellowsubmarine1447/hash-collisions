# takes like 2 hours rip :P
# to prove 13 is the maximum, change the constant to 14

from hashlib import sha256

HEX_DIGIT_COLLISION_LENGTH = 13

real_confession = b"""This is the secret confession of Richard Buckland
to be revealed by anonymous email if I should
mysteriously vanish.  I have left the last few hex
digits of the SHA256 hash of this message with my
trusted solicitor, Dennis Denuto, which will verify
that this is indeed my intended and unaltered
confession written by me Richard Buckland.

Dennis has not seen this confession he has only seen
the last few digits of the hash.  I have also sent copies
of the last few digits to my bank manager and to my priest
Father Brown.

On the 10th of February I saw Mark Zukerberg peeping
through my window and recording my private and personal
conversation with my friend.

I confronted him and he was very embarrassed.  He
promised to pay me $1 million a year if I would stay
silent and not tell anyone I had seen him do this.  I
agreed but now I worry that it would be cheaper for him
to make me vanish than to keep paying me.""".split(b"\n")

fake_confession = b"""This is the secret confession of Richard Buckland
to be revealed by anonymous email if I should
mysteriously vanish.  I have left the last few hex
digits of the SHA256 hash of this message with my
trusted solicitor, Dennis Denuto, which will verify
that this is indeed my intended and unaltered
confession written by me Richard Buckland.

Dennis has not seen this confession he has only seen
the last few digits of the hash.  I have also sent copies
of the last few digits to my bank manager and to my priest
Father Brown.

On the 10th of February I saw Mark Zukerberg near my
house and we struck up a conversation.  He explained all
the things he was doing to ensure that Facebook respects
privacy - both of its users and of others.  It was very
impressive.

I feel awful that I have been criticising Facebook publicly
for so long.  I apologised to him in our conversation and
now I want to confess to the world that actually Facebook
has more than enough privacy features, and that the reason
I spend so much time criticising Facebook is that I am
envious of Mark and wish I was a clever and smart and wise
as he is.  I feel so bad for having been so mean to him for
so many years that I am considering retreating to the outback.
I may well cut off all contact with the world and live as a
hermit from now on.  So do not worry if I vanish it is just
that I feel so guilty that I have been so unfair to Facebook.""".split(b"\n")


def get_hashes(i,h,length):
    if len(real_confession) == i:
        hashes.add(h.hexdigest()[-length:])
        return
    x = h.copy()
    x.update(real_confession[i] + b" \n")
    get_hashes(i+1,x, length)

    x = h.copy()
    x.update(real_confession[i] + b"\n")
    get_hashes(i+1,x,length)

def what_hashes(i,h,length):
    if len(fake_confession) == i:
        if h.hexdigest()[-length:] in hashes:
            return True
        else:
            return False
    x = h.copy()
    x.update(fake_confession[i] + b" \n")
    if what_hashes(i+1,x, length):
        print(fake_confession[i].decode() + " ")
        return True
    x = h.copy()
    x.update(fake_confession[i] + b"\n")
    if what_hashes(i+1,x, length):
        print(fake_confession[i].decode())
        return True
    return False

hashes = set()
get_hashes(0,sha256(b""),HEX_DIGIT_COLLISION_LENGTH)
print(what_hashes(0,sha256(b""),HEX_DIGIT_COLLISION_LENGTH))
