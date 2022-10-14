#include <iostream>
#include <random>
#include <fstream>
#include <array>
#define PRINTLN(x) std::cout << x << std::endl

auto randGen = std::mt19937_64();

typedef unsigned int uint;

template <uint size_strategies>
struct Player {
	uint performance[2];
	uint strategy[2][size_strategies];
};



template <typename T, typename T1>
void writeOutXY(std::string name, T iterable, T1 iterable1) {
	std::ofstream file;
	file.open(name);
	for (auto t : iterable) {
		file << t << " ";
	}
	file << "\n";
	for (auto t : iterable1) {
		file << t << " ";
	}
	file.close();
}

//template<typename T>
//std::ostream& operator<<(std::ostream o, std::vector<T> v) {
//	for (T x : v) o << x << ", ";
//	return o;
//}

void printArray(uint *v, size_t sz) {
	while (sz-- != 0) std::cout << *(v++) << ", ";
	std::cout << std::endl;
}



void test() {
	const uint N = 11;
	auto booldist = std::uniform_int_distribution<uint>(0, 1);

	auto players = std::vector<Player< (1 << 12)>> (N);
	
	const uint memoryIterations = 11;
	const uint outerIterations = 10;
	const uint innerIterations = 1000;

	std::vector<double> sigma = std::vector<double>(memoryIterations);
	std::vector<double> mean = std::vector<double>(memoryIterations);
	std::vector<double> N1 = std::vector<double>(innerIterations);

	uint tally[2] = { 0 };
	uint history = 2;

	for (uint memory = 2; memory < 2+memoryIterations; memory++) {
		//const uint memory = 2;
		uint maxVal = (1 << memory);
		PRINTLN(memory);
		

		for (uint j = 0; j < outerIterations; j++) {

			//generate strategies
			for (uint i = 0; i < N; i++) {
				for (uint j = 0; j < maxVal; j++) {
					players[i].strategy[0][j] = booldist(randGen);
					players[i].strategy[1][j] = booldist(randGen);
				}

			}

			//for (uint i = 0; i < N; i++) {
			//	PRINTLN("P" << i);
			//	printArray(players[i].strategy[0], 4);
			//	printArray(players[i].strategy[1], 4);
			//}



			double runMean = 0;
			double runSigma = 0;
			std::fill(N1.begin(), N1.end(), 0);


			for (uint i = 0; i < innerIterations; i++) {
				//PRINTLN("////");
				for (auto p : players) {
					uint bestStrategy = (p.performance[0] >= p.performance[1]) ? 0 : 1;
					//PRINTLN(bestStrategy << " " << p.performance[0] << " " << p.performance[1]);
					
					tally[p.strategy[bestStrategy][history]]++;

				}

				uint minority = (tally[0] >= tally[1]) ? 1U : 0U;

				N1[i] = (double)tally[1];
				runMean += N1[i]; //gotta be careful with overflow here

				//PRINTLN("tally was " << tally[0] << ", " << tally[1]);
				//PRINTLN("minority was " << minority);

				for (auto &p : players) {
					p.performance[0] += (p.strategy[0][history] == minority);
					p.performance[1] += (p.strategy[1][history] == minority);

				}

				history = (history >> 1) + (minority << (memory - 1));
				tally[0] = 0;
				tally[1] = 0;

				//PRINTLN(history << " " << minority);

			}

			
			runMean /= innerIterations;
			for (uint i = 0; i < innerIterations; i++) {
				double tmp = (runMean - N1[i]);
				runSigma += tmp * tmp;
			}
			runSigma /= innerIterations;

			mean[memory - 2] += runMean;
			sigma[memory - 2] += runSigma;

		}

		sigma[memory - 2] /= outerIterations;
		mean[memory - 2] /= outerIterations;

	}

	writeOutXY("sigma.txt", mean, sigma);

	
	



}



int main() {
	
	uint x = 2;
	test();

	return 0;
}