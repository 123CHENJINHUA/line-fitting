#include<iostream>
#include<array>

int main()
{
	std::array <std::array <int, 4>, 5> arr = {};
	arr.fill({ 1, 2,3,4 });
	std::cout << arr.size() << std::endl;
	std::cout << arr[0].size() << std::endl;
	for (int i = 0; i < arr.size(); i++)
	{
		for (int j = 0; j < arr[0].size(); j++) {
			std::cout << arr[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cin.get();
	return 0;
}
