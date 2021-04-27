import math
import random
import numpy as np
import cv2
import matplotlib.pyplot as plt

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
    return 6*(t**5) - 15*(t**4) + 10*(t**3)
    #return ((6 * t - 15) * t + 10) * t**3

def lerp(t, a1, a2):
    return a1 + t*(a2-a1)

# class Noise():
#     def __init__(self):
#         self.v = Vector2(1,2)

P = permutation()
random.shuffle(P)
random.shuffle(P)
def Noise2D(x, y):
	X = math.floor(x) and 255
	Y = math.floor(y) and 255

	xf = x-math.floor(x)
	yf = y-math.floor(y)

	topRight = Vector2(xf-1.0, yf-1.0)
	topLeft = Vector2(xf, yf-1.0)
	bottomRight = Vector2(xf-1.0, yf)
	bottomLeft = Vector2(xf, yf)
	
	
	valueTopRight = P[P[X+1]+Y+1]
	valueTopLeft = P[P[X]+Y+1]
	valueBottomRight = P[P[X+1]+Y]
	valueBottomLeft = P[P[X]+Y]
	
	dotTopRight = topRight.dot(getConsVec(valueTopRight))
	dotTopLeft = topLeft.dot(getConsVec(valueTopLeft))
	dotBottomRight = bottomRight.dot(getConsVec(valueBottomRight))
	dotBottomLeft = bottomLeft.dot(getConsVec(valueBottomLeft))
	
	u = fade(xf)
	v = fade(yf)
	
	return lerp(u,
		lerp(v, dotBottomLeft, dotTopLeft),
		lerp(v, dotBottomRight, dotTopRight)
	)


size = 300
px = np.zeros((size,size,3), dtype=np.uint8)

##for y in range(len(px)):
##    for x in range(len(px[0])):
##        n = Noise2D(x*0.01, y*0.01)
##
##        n += 1.0
##        n /= 2.0
##
##        c = round(255*n)
##        px[y][x] = [c,c,c]

for y in range(len(px)):
    for x in range(len(px[0])):
        n = 0.0
        a = 1.0
        f = 0.005
        for _ in range(8):
            v = a*Noise2D(x*f, y*f)
            n += v

            a *= 0.5
            f *= 2.0
        n += 1.0
        n *= 0.5
        rgb = round(255*n)
        if n < 0.5:
            #px[y][x] = [0,rgb*2,0]
            px[y][x] = [rgb*2/3,rgb*2/3,rgb*2/3]
        elif n < 0.9:
            #px[y][x] = [0,0,round(rgb*0.5)]
            px[y][x] = [round(rgb*0.5)/3,round(rgb*0.5)/3,round(rgb*0.5)/3]
        else:
            px[y][x] = [rgb,rgb,rgb]
        
            
px = px.astype(np.uint8)
plt.imshow(px)
plt.show()

#

#cv2.imshow('Color image', px)
