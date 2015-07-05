class Combination 
	attr_accessor :total_length, :num, :length
	def initialize
		@total_length = 0
		@num = 0
		@bars = []
		@is_bar = []
		@combination_num = 0
	end
	def get_values
		@total_length = STDIN.gets
	    # print "total length=" + @total_length
	    @total_length = @total_length.to_i
	    @num = STDIN.gets
	    # print "num=" + @num
	    @num = @num.to_i
	    @bars = []
	    for i in 0..(@num-1) do
	    	str = STDIN.gets
	    	@bars[i] = str.to_i
	    	@is_bar[str.to_i] = true
	    	# print "length["+i.to_s+"]=" + str
	    end
	end
	def combination
		@bars.sort!
		# p @bars
		@bars.each_with_index do |length1, i|
			length = length1
			tmp_bars = @bars[i+1..-1]
			#break if length > total_length
	  		### combine 2nd bar
	  		tmp_bars.each_with_index do |length2, j|
	  			length = length1 + length2
	  			# break if length >= total_length 
	  			tmp_bars2 = tmp_bars[j+1..-1]
	  			diff_length = total_length - length
	  			# if @bars.include?(diff_length)
	  			if @is_bar[diff_length] == true
	  				puts length1.to_s+' '+length2.to_s+' '+diff_length.to_s
	  				@combination_num += 1
	  			end
	  			#### combine 3rd bar
	  			# tmp_bars2.each_with_index do |length3, k|
	  			# 	length = length1 + length2 + length3
	  			# 	# diff_length = total_length - length
	  			# 	break if (length > total_length)
	  			# 	if (length == total_length)
	  			# 		# puts length1.to_s+' '+length2.to_s+' '+length3.to_s
	  			# 		@combination_num += 1
	  			# 	end
	  			# end
	  		end
  		end
	  	print @combination_num
  	end
end

comb = Combination.new
comb.get_values
comb.combination
# p "total length=" + comb.total_length.to_s
# p "num = " + comb.num.to_s
# p comb.length[0]


