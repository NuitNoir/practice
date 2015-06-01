
class Divisor 
		MAXNUM = 10000
	def main
		for dividend in 1..MAXNUM #割られる数
			for divisor in 1..dividend #割る数
				num_hash = {}
				divide(dividend, divisor, num_hash) 
				if num_hash.size == 10
					p "devidend = " + dividend.to_s
					p "divisor = " + divisor.to_s
					break
				end
			end
		end
	end

	def divide(dividend, divisor, hash)
		# 商    quotient
		# 余り  rest 
		quotient  = dividend / divisor 
		rest      = dividend % divisor
		hash = store_hash(quotient, hash)
		hash = store_hash(rest, hash)
		hash
	end

	def store_hash(num, hash)
		num.to_s.split("").each do |ch|
			if hash[ch]
				hash[ch] += 1
			else
				hash[ch] = 1
			end
		end
		hash
	end
end



divisor = Divisor.new
divisor.main


