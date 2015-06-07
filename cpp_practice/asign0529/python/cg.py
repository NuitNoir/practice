__author__ = 'kyota'

import cv2
import numpy as np
import math
import skel
import transform

def main():
  file_name = "../cg.skel"
  cg = skel.SKEL(file_name)
  transformer = transform.Transformer3d()
  T = transformer.Translate(1, 1, 1)
  cg.transform(T)

main()




