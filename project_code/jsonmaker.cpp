#include <iostream>
#include <fstream>


int main(int ac, char** av)
{
	int n = 100;
	if (ac < 2)
		std::cout << "Default n = 100" << std::endl;
	else
		n = atoi(av[1]);
	std::fstream file;
	file.open("json.txt", std::ios::out);
	file << "{\n";
	for(int i = 0; i < n; ++i)
		file << "\t\"line" << i << "\": \"This is line " << i << ".\",\n";
	file << "\t\"line" << n << "\": \"This is line " << n << ".\"\n";
	file << "}";
	return 0;
}
