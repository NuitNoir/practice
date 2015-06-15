__author__ = 'kyota'
import numpy as np
import math as m

class Transformer3d:
  def Translate(self, tx, ty, tz):
    T = np.array([[1, 0, 0, tx],
                  [0, 1, 0, ty],
                  [0, 0, 1, tz],
                  [0, 0, 0, 1]])
    return T

  def RotationX(self, angle):
    R = np.array([[1, 0, 0, 0],
                  [0, m.cos(angle), m.sin(angle), 0],
                  [0, m.sin(angle), m.cos(angle), 0],
                  [0, 0, 0, 1]])
    return R

  def RotationY(self, angle):
    R =  np.array([[m.cos(angle), 0, m.sin(angle), 0],
                    [0, 1, 0, 0],
                    [-m.sin(angle), 0, m.cos(angle), 0],
                    [0, 0, 0, 1]])
    return R

  def RotationZ(self, angle):
    R = np.array([[m.cos(angle), -m.sin(angle), 0, 0],
                    [m.sin(angle), m.cos(angle), 0, 0],
                    [0, 0, 1, 0],
                    [0, 0, 0, 1]])
    return R

if __name__ == "__main__":
  transformer = Transformer3d()
  print transformer.Translate(1, 1, 1)

