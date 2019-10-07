#!/usr/bin/python2.7

# Tests for the geom_test.py module

import sys 
src_path = '../'
sys.path.insert(0, src_path)
py_src_path = '../../../python_tools/'
sys.path.insert(0, py_src_path)

from geom_test import *

geom = np.ones((20,50))
xc = 10
yc = 30
dx = 4
dy = 6
geom[xc-dx/2:xc+dx/2+1, yc-dy/2:yc+dy/2+1] = 0.0
geom = geom.transpose()
## Rectangle
# Test 1
R1 = rectangle(dx, dy, xc, yc)
msg('Test 1 - correct:', RED)
print R1.correct(geom)
# Test 2 - Now add an error
geom[2,5] = 0.0
msg('Test 2 - error:', RED)
print R1.correct(geom)

## Square
# Test 3
S1 = square(dy, xc, yc)
msg('Test 3 - error:', RED)
print S1.correct(geom)
# Test 4
geom = np.ones((20,50))
xc = 10
yc = 30
ds = 4
geom[xc-ds/2:xc+ds/2+1, yc-ds/2:yc+ds/2+1] = 0.0
geom = geom.transpose()
msg('Test 4 - correct:', RED)
print S1.correct(geom)

### Ellipse
## Test 5 - new geom
geom = np.ones((20,50))
dx = 8
dy = 5
E1 = ellipse(dx,dy,xc,yc)
for ix, iy in np.ndindex(geom.shape):
	ir = (ix - xc)*(ix - xc)*dy*dy/4.0 + \
		  (iy - yc)*(iy - yc)*dx*dx/4.0
	if ir <= dx*dx*dy*dy/16.0:
		geom[ix,iy] = 0.0
	else:
		geom[ix,iy] = 1.0
geom = geom.transpose()
msg('Test 5 - correct:', RED)
print E1.correct(geom)
## Test 6 - add an error
msg('Test 6 - error:', RED)
geom[0,1] = 0
print E1.correct(geom)

## Circle
# Test 7
C1 = circle(dx, xc, yc)
msg('Test 7 - error:', RED)
print C1.correct(geom)
# Test 8
geom = np.ones((20,50))
xc = 10
yc = 30
Dc = 18
C1 = circle(Dc, xc, yc)
for ix, iy in np.ndindex(geom.shape):
	ir = (ix - xc)*(ix - xc)*Dc*Dc/4.0 + \
		  (iy - yc)*(iy - yc)*Dc*Dc/4.0
	if ir <= Dc*Dc*Dc*Dc/16.0:
		geom[ix,iy] = 0.0
	else:
		geom[ix,iy] = 1.0
geom = geom.transpose()
msg('Test 8 - correct:', RED)
print C1.correct(geom)

## Empty geom
# Test 6
msg('Test 6 - error:', RED)
print empty(geom)
# Test 7
geom = np.ones((20,50))
msg('Test 7 - correct:', RED)
print empty(geom)

## Walls
# Test 8
dh = 0
W1 = walls(dh, 'x')
msg('Test 8 - correct:', RED)
print W1.correct(geom)
# Test 9
W2 = walls(2, 'x')
msg('Test 9 - error:', RED)
print W2.correct(geom)
# Test 10
geom[0:2,:] = 0
geom[-2,:] = 0
geom[-1,:] = 0
msg('Test 10 - correct:', RED)
print W2.correct(geom)
# Test 11
W3 = walls(3, 'y')
geom = np.ones((20,50))
geom[:,0:3] = 0
geom[:,47:50] = 0
msg('Test 11 - correct:', RED)
print W3.correct(geom)
# Test 12
geom[2,47] = 1
msg('Test 12 - error:', RED)
print W3.correct(geom)

### Arrays of objects
## Test 13
#path = './'
#geom = np.loadtxt(path + 'not_stag_array.txt', dtype=np.int32)
#A1 = obj_array(4, 4, 4, 58, 10, 90, 5, 5)
#msg('Test 13 - correct:', RED)
#print A1.correct(geom)
## Test 14
#geom[10,4] = 1
#geom[11,5] = 0
#msg('Test 14 - error:', RED)
#print A1.correct(geom)
## Test 15
#geom[10,4] = 0
#msg('Test 15 - error:', RED)
#print A1.correct(geom)
## Staggered
## Test 16
#geom = np.loadtxt(path + 'stag_array.txt', dtype=np.int32)
#A2 = stag_array(2, 4, 4, 58, 15, 50, 10, 5, 60)
#msg('Test 16 - correct:', RED)
#print A2.correct(geom)
## Test 17
#geom[25,34] = 1
#geom[11,5] = 0
#msg('Test 17 - error:', RED)
#print A2.correct(geom)
## Test 18
#geom[25, 34] = 0
#msg('Test 18 - error:', RED)
#print A2.correct(geom)



