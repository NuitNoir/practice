# -*- coding: utf-8 -*-
import numpy as np

def main():
  kataguruma("small.in.txt")
  # kataguruma("large.in.txt")

def kataguruma(file_name):
  arr = read_file(file_name)
  arr.sort(axis=1, kind='quicksort')
  arr.sort(axis=0, kind='mergesort') # stable sort.
  num = len(arr)
  edges = np.zeros((num, num))
  print arr
  for i in range(0, num):
    for j in range(i, num):
      if (arr[i][0] < arr[j][0] and arr[i][1] < arr[j][1]):
        # i番目 < j番目 のとき
        edges[i][j] = 1
        edges[j][i] = -1
      elif (arr[i][0] > arr[j][0] and arr[i][1] > arr[j][1]):
        # j番目 < i番目 のとき
        edges[i][j] = -1
        edges[j][i] = 1
      else: # 順序関係がつかない時
        edges[i][j] = 0
        edges[j][i] = 0
  # print "edges"
  # print edges
  path_weights = np.zeros(num)
  ### 最長経路問題
  ### ただし今回はGreedyな解放で解がでるようなデータセットなのでGreedyに解く
  for i in range(0, num): ## i番目の人から開始して何人かたぐるまできるか
    flags = np.zeros(num)
    pathes = []
    if flags[i] == 1: continue ### すでにより長い経路でこのノードが使用されていたら評価しない

    # for j in range(i, num): ## i行目からnum行目まで進む
    j = i
    while j < num:
      print j
      for k in range(j, num): ## ソートしたので左対角成分に 1 はこないので右対角成分のみを見る
        if edges[j][k] == 1:  ## かたぐるまできる人がいたらpathesに追加し、
          pathes.append(k)
          print k
          j = k             ### 行をその人のところまで増やし、その行に飛ぶ
          flags[k] = 1      ### すでに長い経路で使用されたフラグをたてる
          # break
        j += 1
        # if k == num-1:
        #   j = num ### j行目でかたぐるまできる人が見つからなかったらi番目スタートの探索は終了

    np.append(path_weights, [len(pathes)])

  print path_weights




def read_file(file_name):
  f = open(file_name, "r")
  lines = f.readlines()
  f.close()
  arr = np.ndarray((0, 2))
  for line in lines:
    line = line.replace('\n', '')
    tall, weight = line.split(' ')
    arr = np.concatenate((arr, [[tall, weight]]), axis=0)
  return arr



main()

