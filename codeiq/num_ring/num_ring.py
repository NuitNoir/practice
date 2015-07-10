import fileinput

def ring(nums):
  ring = {}
  count = 0
  # while(len(nums) > 0):
  for num in nums:
    print num
    if (ring[nums[num-1]] == None):
      ring[num] = nums[num-1]
    else:
      count += len(ring) - 1
      for key, val in ring.items():
        nums.remove(key)
  return count

def main():
  nums = []
  for num in fileinput.input():
    print num
    nums.append(int(num))
  print nums

  ring(nums)

main()


