__author__ = 'kyota'
import cv2
import cv
import numpy as np
import math


def main():
  img = cv2.imread("../cat/cat1.pgm", 0)
  height, width = img.shape
  light = 255
  p = np.array([-5,2,-3])
  p1 = np.array([150,500,2])
  img = draw_line(img, p, light)
  img = draw_line(img, p1, light)

  cv2.imshow("line", img)
  cv2.waitKey(0)
  cv2.destroyAllWindows()


def draw_line(img, p, l):
  height, width = img.shape
	# P = (a, b, c)
	#	a*x + b*y + c = 0
	#		y = -a/b*x - c/b
	#		y = -p[0]/p[1]*x - p[2]/p[1]
	#
	#		x = -b/a*y - c/a
	#		x = -p[1]/p[0]*y - p[2]/p[0]
  slope = -p[0]/p[1]

  if abs(slope) <= 1 :
    x = 0
    slope = -p[0]/p[1]
    intercept = p[2]/p[1]
    # slope < 1 to right
    for x in range(width) :
      y = slope*x - intercept
      if (y<0 or y>height):
        continue
      img[math.floor(y+0.5), x] = l
  else:
    slope = -p[1]/p[0]
    intercept = p[2]/p[0]
    y = 0
    for y in range(height):
      x = slope*y - intercept
      if x>width or x<0 :
        continue
      img[y, math.floor(x)] = l
  return img

def mark(img, p, l):
  int r = 10;
  height, width = img.shape
  # p1 = np.
  if (x>=0 and y>=0 and x<=width and y<=height):
    cv.Line(img, p1, p2, l, thickness=5)


main()

