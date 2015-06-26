__author__ = 'kyota'

import numpy as np
import cv2

N = 6
def main():
  file1 = (open("../room1.txt"))
  file2 = (open("../room2.txt"))
  x_1 = read_positions(file1)
  x_2 = read_positions(file2)
  print "x_1 = " + str(x_1)
  print "x_2 = " + str(x_2)

  A = np.ndarray((0,9))
  for i in range(0, len(x_1)):
    x1 = x_1[i][0]; y1 = x_1[i][1]; w1 = x_1[i][2]
    x2 = x_2[i][0]; y2 = x_2[i][1]; z2 = x_2[i][2]

    ##### Ai : 2x9
    Ai = np.array([
      [0,0,0, -w1*x2, -w1*y2, -w1*z2,   y1*x2, y1*y2, y1*z2],
      [w1*x2, w1*y2, w1*z2, 0, 0, 0,  -x1*x2, -x1*y2, -x1*z2]
    ], np.float64)
    A = np.concatenate((A,Ai), axis=0)

  print "A = " + str(A)

  ##### SVD
  U, s, V = np.linalg.svd(A, full_matrices=True)
  print "U = " + str(U)
  print "s = " + str(s)
  print "V = " + str(V)

  ##### H
  H = np.array([
    [V[8][0],V[8][1],V[8][2]],
    [V[8][3],V[8][4],V[8][5]]#,
   # [V[8][6],V[8][7],V[8][8]]
  ])
  print "H = " + str(H)

  ### Affine
  src = cv2.imread("../img1.JPG", 0)
  rows, cols = src.shape
  dst = cv2.warpAffine(src, H, (cols, rows))
  # dst = src
  cv2.imwrite('out.jpg', dst)

def read_positions(file):
  x_1 = np.ndarray((0,3))
  for i in range(0, N):
    line = file.readline()
    line = line.replace("\n", "")
    arr = line.split(" ")
    arr.append(1)
    arr = map(int, arr)
    x_1 = np.concatenate((x_1, [arr]), axis=0)
  return x_1

main()

