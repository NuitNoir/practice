__author__ = 'kyota'
# -*- coding: utf-8 -*-

import math

def main():
  order = 5
  fiz_buz_ex(order)
  simple_sum(order)
  order = 9
  # fiz_buz_ex(order)

#### simple_sum: 10^order以下のprimesの倍数の総和を求める。
# 方針: max_numまでループして、primesの倍数なら足していく。
def simple_sum(order):
  max_num = int(math.pow(10, order))
  primes = [3,5,7,11,13,17,19,23,29,31]
  sum = 0

  for i in range(1, max_num+1):
    for val in primes:
      if i%val == 0:
        sum += i
        # print "i=" + str(i) + "  val = " + str(val)
        break
  print "simple sum           = " + str(sum)
  return sum

#### fiz_buz_ex: 10^order以下のprimesの倍数の総和を求める
## 方針: 1. primesの各max_numまでの和を求めて、それぞれを足し合わせる。
## 　　  2. 公倍数で重複したものは引く。
def fiz_buz_ex(order):
  max_num = int(math.pow(10, order))
  sub_sum = 0
  primes = [3,5,7,11,13,17,19,23,29,31]

  total_sum = get_total_sum(primes, max_num)

  # 全てのパターンを網羅するようにループ
  for i in range(0, len(primes)):
    layer = 1
    sub_sum += common_multiple(primes[i], primes[i+1:], layer, max_num)
    print sub_sum

  print "total_sum  = " + str(total_sum)
  print "sub_sum    = " + str(sub_sum)
  sum = total_sum - sub_sum
  print "##### the answer. order is " + str(order) +" #####"
  print "fast calculation sum = " + str(sum)

##### get_total_sum: arrに含まれる数字のmax_numまでの倍数の総和を求める。
# 返り値: total_sum
# arr: arr自然数の配列。
# max_num: 最大数
def get_total_sum(arr, max_num):
  total_sum = 0
  for num in arr:
    quotient = max_num/num
    sum = (quotient*(quotient+1)*num)/2 # S = (n/2)*(2a + (n-1)d). a == d
    total_sum += sum
  return total_sum

######### common_multiple: arrに含まれる公倍数(max_num以下)の総和を計算。再帰的に計算する。
# 返り値: sum
# base: 掛け算の基数
# arr: primesの部分集合
# layer: 階層。layer+1個の公倍数を計算
# sum: 後で差し引く数の合計
# max_num: 最大数
def common_multiple(base, arr, layer, max_num):
  sum = 0
  for i in range(0, len(arr)):
    val = base*arr[i]
    if val < max_num:
      print "base = " + str(base) + "\tarr["+str(i)+"] = " + str(arr[i]) + "\tval = " + str(val) +  "\tlayer = " + str(layer)
      sum += val*layer # 素数の公倍数はその「要素数−１」かけたぶんだけ後で引く
      # sum = common_multiple(val, arr[i+1:], layer+1, sum, max_num)
      sum += common_multiple(val, arr[i+1:], layer+1, max_num)
    else:
      break
  if len(arr) > 1:
    sum += common_multiple(base*arr[0], arr[1:], layer+1, max_num)
  print "sum = " + str(sum) + "\tlayer = " + str(layer)
  return sum

main()