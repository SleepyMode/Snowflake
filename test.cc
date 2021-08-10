
#include <iostream>
#include <bitset>
#include <conio.h>
#include "snowflake.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[], [[maybe_unused]] char* envp[])
{
	SnowflakeId::initialize(0, 50);

	std::vector<SnowflakeId> list;
	list.reserve(10000);

	for (int i = 0; i < 10000; ++i)
	{
		SnowflakeId snowflakeId = SnowflakeId::generate();
		std::cout << "0" << std::bitset<41>(snowflakeId.timestamp) << "|" << std::bitset<10>(snowflakeId.machineId) << "|"
			<< std::bitset<12>(snowflakeId.workerId) << "(" << snowflakeId.workerId << ")" << std::endl;
		list.push_back(snowflakeId);
	}

	auto it = std::unique(list.begin(), list.end());
	if (it == list.end())
		std::cout << "UNIQUE" << std::endl;
	else
		std::cout << "NOT UNIQUE" << std::endl;

	return 0;
}
