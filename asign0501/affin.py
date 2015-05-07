import cv2
import numpy as np
import math

def main():
  img = cv2.imread("masamunya.jpg", 0)
  rows, cols = img.shape

  M = np.float32([[1, 0, 100], [0, 1, 50]])
  R = cv2.getRotationMatrix2D((cols / 6, rows / 6), 90, 1)
  # dst = cv2.warpAffine(img, R, (cols,rows))
#   dst = TransformL(img, M)
#   cv2.imshow("result_lagrange", dst)
#   cv2.waitKey(0)
#   cv2.destroyAllWindows()
  dst = TransformE(img, M, 1)
  cv2.imshow("result_euler_bi", dst)
  cv2.waitKey(0)
  cv2.destroyAllWindows()
# 
#   dst = TransformE(img, M, 0)
#   cv2.imshow("result_euler_nn", dst)
#   cv2.waitKey(0)
#   cv2.destroyAllWindows()

def TransformL(src, M):
    rows, cols = src.shape
    dst = np.zeros((rows, cols), np.float32)
    p = np.ones((2, 1), np.float32)
    p1 = np.ones((3, 1), np.float32)
    for y in range(rows): 
      for x in range(cols):
        # Lagrange
        p1 = np.array([[x], [y], [1]])
        p = M.dot(p1)
        val = NN(src, p1[0], p1[1])
        if ((p[1] < rows and p[0] < cols) and (y >= 0 and x >= 0)):
          dst[round(p[1])][round(p[0])] = val
    return dst

def TransformE(src, M, flag):
    rows, cols = src.shape
    dst = np.zeros((rows, cols), np.float32)
    p = np.ones((2, 1), np.float32)
    p1 = np.ones((3, 1), np.float32)
    for y in range(rows): 
      for x in range(cols):
        # Euler
        p1 = np.array([[x], [y], [1]])
        p = M.dot(p1)
        if (flag == 0):
            val = NN(src, p[0], p[1])
        elif (flag == 1):
            val = Bilinear(src, p[0], p[1]);
        else:
            print "invalid flag = " + flag.to_s
            print "flag must be 0:Nearest Neighbour, 1:Bilinear Interpolation"
        if ((p[1] < rows and p[0] < cols) and (y >= 0 and x >= 0)):
            # p[0] = x, p[1] = y
            #print p1
            dst[y][x] = val
            #dst[ p1[1] ][ p1[0] ] = val
    return dst

def NN(src, x, y):
    rows, cols = src.shape
    ix = round(x)
    iy = round(y)
    if ((iy < rows and ix < cols) and (iy >= 0 and ix >= 0)):
        return src[iy][ix]
    else:
        return 0

def Bilinear(src, x, y):
    rows, cols = src.shape
    ix = math.floor(x)
    iy = math.floor(y)

    diff_x = x - ix
    diff_y = y - iy

    if ((iy+1 < rows and ix+1 < cols) and (iy >= 0 and ix >= 0)):
        val = (1-diff_y)*((1-diff_x)*src[ix][iy] + diff_x*src[ix+1][iy])
        + diff_y*((1-diff_x)*src[ix][iy+1] + diff_x*src[ix+1][iy+1]);
        return val
    else:
        return 0
    

main()

