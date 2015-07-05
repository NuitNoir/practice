import fileinput
class Combination:

	total_length = 0
	num = 0
	bars = []
	combination_num = 0
	def __init__(self):
		print self.total_length
	
	def get_values():
		total_length = fileinput.input()
	    # print "total length=" + self.total_length
	    self.num = STDIN.gets
	    # print "num=" + self.num
	    self.num = self.num.to_i
	    self.bars = []
	    for i in 0..(self.num-1):
	    	str = STDIN.gets
	    	self.bars[i] = str.to_i
	    	# print "length["+i.to_s+"]=" + str
	def combination():
		self.bars.sort!
		# p self.bars
		self.bars.each_with_index do |length1, i|
			length = length1
			tmp_bars = @bars[i+1..-1]
			#break if length > total_length
	  		### combine 2nd bar
	  		tmp_bars.each_with_index do |length2, j|
	  			length = length1 + length2
	  			# break if length >= total_length 
	  			tmp_bars2 = tmp_bars[j+1..-1]

	  			#### combine 3rd bar
	  			tmp_bars2.each_with_index do |length3, k|
	  				length = length1 + length2 + length3
	  				# diff_length = total_length - length
	  				break if (length > total_length)
	  				if (length == total_length)
	  					# puts length1.to_s+' '+length2.to_s+' '+length3.to_s
	  					self.combination_num += 1
	  				end
	  			end
	  		end
  		end
	  	print @combination_num
  	end
end

comb = Combination.new()
comb.get_values
comb.combination
# p "total length=" + comb.total_length.to_s
# p "num = " + comb.num.to_s
# p comb.length[0]


