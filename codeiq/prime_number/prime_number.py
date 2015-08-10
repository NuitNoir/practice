__author__ = 'kyota'
import fileinput

def get_prime_numbers(num):
  prime_numbers = []
  for i in range(2, num):
    count = 0
    print prime_numbers
    for prime in prime_numbers:
      if i%prime == 0: break
      if prime == prime_numbers[-1]:
        prime_numbers.append(prime)
    # for j in range(0, len(prime_numbers)):
    #   if i % prime_numbers[j] == 0 : break
    # if (j == len(prime_numbers)-1 and i%prime_numbers[-1]!=0):
    #     prime_numbers.append(i)
  return prime_numbers

def get_prime_num(num):
  prime_numbers = get_prime_numbers(num)
  return len(prime_numbers)

def main():
  numbers = [100000]
  # for line in fileinput.input():
  #   val = int(line)
  #   numbers.append(val)
  # print numbers

  for number in numbers:
    num = get_prime_num(number)
    print num


main()
