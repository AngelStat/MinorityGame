#include <iostream>
#include <random>
#define PRINTLN(x) std::cout << x << std::endl

auto randGen = std::mt19937_64();

typedef unsigned int uint;

template <uint size_strategies>
struct Player {
	uint performance[size_strategies];
	uint strategy[size_strategies];
};



void test() {
	const uint N = 11;
	Player< 8 > players[N];
	const uint maxVal = 4;
	auto dist = std::uniform_int_distribution<uint>(0, maxVal);
	auto booldist = std::uniform_int_distribution<uint>(0, 1);

	//generate strategies
	for (uint i = 0; i < N; i++) {
		for (uint j = 0; j < maxVal; j++) {
			players[i].strategy[0] = 2 * dist(randGen);
		}

	}

	uint tally[2] = {0};

	uint iterations = 1000;
	uint history = 0;
	for (uint i = 0; i < iterations; i++) {
		
	}
}


int main() {
	
	return 0;
}