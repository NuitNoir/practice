__author__ = 'kyota'

import numpy as np
import math

class SKEL:
  def __init__(self, file_name):
    print 'construct skel'
    self.vertices = np.ndarray((0,4))
    self.lines = np.ndarray((0,4))
    self.matrix = np.ndarray((4,4))

    file = open(file_name, "r")
    file.close
    lines = file.readlines()
    arr = []
    row = 0
    v_num = 0
    p_num = 0
    # for i in range(0, len(lines)):
    for line in lines:
      line = line.replace("\n", "")
      _arr = line.split(" ")
      if _arr.__contains__("#"):
        print 'skip'
        continue
      row += 1
      if row == 1:
        type = _arr[0]
      if row == 2:
        v_num = int(_arr[0])
        p_num = int(_arr[1])
      if row > 2 and row <= 2 + v_num:
        _arr = map(int, _arr)
        self.vertices = np.concatenate((self.vertices, [_arr]), axis=0)
      if row > 2+v_num and row <= 2+v_num+p_num:
        _arr = map(int, _arr)
        self.lines = np.concatenate((self.lines, [_arr]), axis=0)

  def set_matrix(self, matrix):
    self.matrix = matrix

  def transform(self, T):
    # print len(self.vertices)
    for i in range(0, len(self.vertices)):
      self.vertices[i] = np.dot(T, self.vertices[i])

if __name__ == "__main__":
  skel = SKEL("../cg.skel")
