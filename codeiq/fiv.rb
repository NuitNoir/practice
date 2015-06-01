# f_n+10 = 89*f_n + 55*f_n-1
# f_n+9  = 55*f_n + 34*f_n-1
# A = [89, 55,
#      55, 34]
class Fiv
    NUM = 50
  def calc
    a1 = 1
    a2 = 2
    count = 0

    p mat = make_fiv(NUM)
    count += (NUM-1)*1

    1.step(1000, NUM) do |i|
      a2, a1 = fiv_step(a2, a1, mat)
      count += 6
    end
    # 35877
    p '下5桁：' + (a2%100000).to_s
    p '計算回数：' + count.to_s
  end

  # 足し算 1
  def fiv(a2, a1)
    a3 = a2 + a1
    return [a3, a2]
  end
  # 計算回数 6 <-  掛け算 4, 足し算 2
  def fiv_step(a2, a1, mat)
    a3 = mat[0][0]*a2 + mat[0][1]*a1
    a2 = mat[1][0]*a2 + mat[1][1]*a1
    return [a3, a2]
  end
  # 足し算(n-1)
  def make_fiv(num)
    #mat = [arr1, arr2] -> [[1,1], [1,0]]
    arr1 = [1, 1]
    for i in 1..(num-2)
      # 足し算 1*(n-2)
      arr1 = fiv(arr1[0], arr1[1])     
    end
    arr2 = arr1
    # 足し算 1
    arr1 = fiv(arr1[0], arr1[1])
    mat = [arr1, arr2]
  end
end

fiv = Fiv.new
fiv.calc

