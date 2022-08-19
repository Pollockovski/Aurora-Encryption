b64="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
from random import randint;

for i in range(256):
    print(b64[randint(0,63)],end="");