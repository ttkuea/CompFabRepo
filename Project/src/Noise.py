import math
import random
import numpy as np

class Vector2():
    def __init__(self, x, y):
        self.x = x
        self.y = y
    
    def dot(self, other):
        return self.x * other.x + self.y * other.y
    
    def __str__(self):
        return '[' + str(self.x) + ', ' + str(self.y) + ']'


def permutation():
    p = [i for i in range(256)]
    random.shuffle(p)
    for i in range(256):
        p.append(p[i])
    return p

def getConsVec(v):
    h = v and 3
    if h == 0:
        return Vector2(1.0, 1.0)
    elif h == 1:
        return Vector2(-1.0, 1.0)
    elif h == 2:
        return Vector2(-1.0, -1.0)
    else:
		return Vector2(1.0, -1.0)

def fade(t):
    return ((6 * t - 15) * t + 10) * t**3

def lerp(t, a1, a2):
    return a1 + t*(a2-a1)

class Noise():
    def __init__(self):
        self.v = Vector2(1,2)



