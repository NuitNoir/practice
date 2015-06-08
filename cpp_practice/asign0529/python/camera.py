__author__ = 'kyota'
import numpy as np
import cv2

class Camera:
  def __init__(self):
    self.intrinsic  = np.ndarray((4,4))
    self.location   = np.ndarray((4,4))

  def set_intrinsic(self, f, ku, kv, s, u0, v0):
    self.intrinsic = np.array([[f*ku, f*s, u0],
                                [0, f*ku, v0],
                                [0, 0, 1] ])
    return self.intrinsic

  def set_location(self, M):
    self.location = M

  def transform(self, T):
    self.location = self.location * T

  def draw_skes_perspective(self, img, skels, colorses):
    I0 = np.array([[1,0,0,0], [0,1,0,0], [0,0,1,0]])
    I1 = np.array([[1,0,0], [0,1,0], [0,0,1], [0,0,0]])
    for i in range(0, len(skels)):
      R = np.dot(I1, np.dot(self.intrinsic, np.dot(I0, self.location)))
      skels[i].transform(R)
      self.draw_skel_perspective(img, skels[i], colorses[i%len(colorses)])

  def draw_skel_perspective(self, img, skel, colors):
    proj_vecs = np.ndarray((0, 3))
    # print proj_vecs
    for vertice in skel.vertices:
      # print proj_vecs
      # print proj_vec
      # print [self.perspective_projection(vertice)]
      proj_vecs = np.concatenate((proj_vecs, [self.perspective_projection(vertice)]), axis=0)

    view_depth = -20
    view_start = -1
    hide_flg  = False
    f         = 1
    bias      = 250
    # for line in skel.lines:
    for i in range(0, len(skel.lines)):
      line = skel.lines[i]
      for j in range(1, int(line[0])):
        if (line[0] >=3):
          hide_flg = self.hide_line(proj_vecs[line[1]], proj_vecs[line[2]], proj_vecs[line[3]])
          if (hide_flg == True):
            continue
          hide_flg = False
        # pt1 = np.zeros((0, 2))
        pt1 = proj_vecs[line[j]]*f + bias
        pt2 = proj_vecs[line[j+1]]*f + bias
        # if (pt1[2] > view_start and pt2[2] > view_start): continue
        # if (pt1[2] < view_depth and pt2[2] < view_depth): continue

        cv2.line(img, (int(pt1[0]), int(pt1[1])), (int(pt2[0]), int(pt2[1])), colors[i%len(colors)])

  def perspective_projection(self, vertice):
    f = 30
    P = np.array([[f, 0, 0, 0],
              [0, f, 0, 0],
              [0, 0, 1, 0]])
    return np.dot(P, vertice)

  def hide_line(self, p0, p1, p2):
    vec1 = p1 - p0
    vec2 = p2 - p0
    cross_product_z = np.cross(vec1, vec2)[2]
    # print "cross"
    # print cross_product_z
    if (cross_product_z > 0):
      return True
    else:
      return False

