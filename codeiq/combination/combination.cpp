#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <string.h>

#define MAXNUM 10000

class Combination {
public:
	int total_length = 0;
	int num = 0;
	std::vector<int> bars;
	int lengthes[MAXNUM] = {};
	// std::vector<int> is_bar[5000];
	int combination_num = 0;

	void get_values_stdin() {
		std::cin >> this->total_length;
		std::cin >> this->num;
		int bar_length;	
		///// read bars length.
		for (int i=0; i<num; i++) {
			std::cin >> bar_length;
			if (bar_length < MAXNUM) {
				this->bars.push_back(bar_length);
				lengthes[bar_length] = 1;
			}
			// this->is_bar[bar_length] = 1;
		}
	}
	int combination() {
		std::sort(bars.begin(), bars.end());
		int bars_size = (int)this->bars.size();
		int length;
		for (int i=0; i < bars_size; i++) {
			int used_bar[MAXNUM] = {}; ///// 1本選んだ後はペアを選ぶことになるので、同じ棒は使われない。
			for (int j=i+1; j < bars_size; j++) {
				int lengthes[MAXNUM] = {};
				memcpy(&lengthes, &this->lengthes, sizeof(lengthes));
				length = bars[i] + bars[j];
				int diff_length = total_length - length;
				if (diff_length <= bars[j]) continue;
				if (lengthes[diff_length] == 1 && used_bar[diff_length] != 1) {
					used_bar[diff_length] 	= 1;
					used_bar[bars[j]]		= 1;
					this->combination_num++;
					if (this->combination_num%100000 == 0) std::cout << this->bars[i] << " " <<  this->bars[j] << " " <<  diff_length << std::endl;
				}
				lengthes[j] = 0;
				// if (length > this->total_length) break;
				// for (int k=j+1; k < bars_size; k++) {
				// 	length = this->bars[i] + this->bars[j] + this->bars[k];
				// 	// std::cout << length << std::endl;
				// 	if (length > this->total_length) break;
				// 	if (length == this->total_length) {
				// 		// std::cout << this->bars[i] << " " <<  this->bars[j] << " " <<  this->bars[k] << std::endl;
				// 		this->combination_num++;
				// 		if (this->combination_num%10000 == 0) {
				// 			std::cout << this->combination_num << ' ' <<  this->bars[i] << " " <<  this->bars[j] << " " <<  this->bars[k] << std::endl;	
				// 		}
				// 	}
				// }
			}
			this->lengthes[i] = 0;
		}
		return this->combination_num;
	}
};

int main() {
	Combination comb = Combination();
	comb.get_values_stdin();
	int combination_num;
	combination_num = comb.combination();
	std::cout << combination_num << std::endl;
	// 1571200
}

// 35
// 10
// 13
// 12
// 17
// 10
// 4
// 18
// 3
// 11
// 5
// 7


