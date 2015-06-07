__author__ = 'kyota'

import skel
import camera
import numpy as np
import transform
import cv2

def main():
  skel1 = skel.SKEL("../world.skel")
  skel2 = skel.SKEL("../")
  skels = []
  skels.append(skel1)
  print skels

  c1 = camera.Camera()
  c1.set_intrinsic(1,1,1,0,1,1)
  tf = transform.Transformer3d()
  R = tf.Translate(1,1,1)
  c1.set_location(R)
  T = tf.Translate(0, 0, 0.1)

  height  = 500
  width   = 500
  for i in range(0, 100):
    buf = np.zeros((height, width, 3), np.uint8)
    # img = cv2.imdecode(buf, 0)
    img = cv2.imread("none.png")
    c1.transform(T)
    c1.draw_skes_perspective(img, skels, 255)
    file_name = "img/camera" + str(i) + ".png"
    cv2.imwrite(file_name, img)
    print img


if __name__ == "__main__":
  main()