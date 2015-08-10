#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <list>


std::vector<int> get_prime_numbers(int num) {
	std::vector<int> prime_numbers;
	for (int i=2; i < num; i++) {
		unsigned int count = 0;
		for (unsigned int j=0; j<prime_numbers.size(); j++) {
			if (i%prime_numbers[i] != 0) count++ ;
			if (count == prime_numbers.size() ) {
				prime_numbers.push_back(prime_numbers[i]);
			}

		}
	}
	return prime_numbers;
}

int main() {
	double val = 1;
	std::vector<int> nums;
	// while (std::cin >> val) {
	// 	std::cout << val;
	// 	nums.push_back(val);
	// }
	nums.push_back(10000);
	for (unsigned int i=0; i < nums.size(); i++) {
		std::vector<int> prime_numbers = get_prime_numbers(nums[i]);
		int num = prime_numbers.size();
		std::cout << num << std::endl;
	}
	// for (int =0; i<(int)nums.size(); i++) {
	// 	std::cout << nums[i] << std::endl;
	// }
    return 0;
}
