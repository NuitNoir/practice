__author__ = 'kyota'
import numpy as np
import math as m

class Transformer3d:
  def Translate(self, tx, ty, tz):
    return np.array([[1, 0, 0, tx],
             [0, 1, 0, ty],
             [0, 0, 1, tz],
             [0, 0, 0, 1]])

  def RotationX(self, angle):
    return np.array([[1, 0, 0, 0],
                    [0, m.cos(angle), m.sin(angle), 0],
                    [0, m.sin(angle), m.cos(angle), 0],
                    [0, 0, 0, 1]])

  def RotarionY(self, angle):
    return np.array([m.cos(angle), 0, m.sin(angle), 0],
                    [0, 1, 0, 0],
                    [-m.sin(angle), 0, m.cos(angle), 0],
                    [0, 0, 0, 1])
  def RotationZ(self, angle):
    return np.array([m.cos(angle), -m.sin(angle), 0, 0],
                    [m.sin(angle), m.cos(angle), 0, 0],
                    [0, 0, 1, 0],
                    [0, 0, 0, 1])

if __name__ == "__main__":
  transformer = Transformer3d()
  print transformer.Translate(1, 1, 1)

