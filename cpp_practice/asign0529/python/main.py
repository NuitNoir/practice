__author__ = 'kyota'

import skel
import camera
import numpy as np
import transform
import cv2

def main():
  skel1 = skel.SKEL("../world.skel")
  skel2 = skel.SKEL("../cube.skel")
  skels = []
  skels.append(skel1)
  skels.append(skel2)
  # print skels
  b = (255, 0, 0)
  g = (0, 255, 0)
  r = (0, 0, 255)
  rgbs = [b,g,r]
  blues = [b]
  greens = [g]
  reds = [r]
  colorses = [rgbs, blues, greens, reds]

  c1 = camera.Camera()
  c1.set_intrinsic(1,1,1,0,-1,-1)
  tf = transform.Transformer3d()
  L = tf.Translate(-1, 1,-10)
  c1.set_location(L)
  # T = tf.Translate(0, 0, 0.1)
  T = tf.RotationY(0.3)

  for i in range(0, 100):
    img = cv2.imread("none.png")
    c1.transform(T) # transform camera location by 4*4 matrix
    c1.draw_skes_perspective(img, skels, colorses)
    file_name = "img/camera" + str(i) + ".png"
    cv2.imwrite(file_name, img)

if __name__ == "__main__":
  main()