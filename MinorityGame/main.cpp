#include <iostream>
#include <random>
#include <fstream>
#include <array>
#include <chrono>
#define PRINTLN(x) std::cout << x << std::endl


auto randGen = std::mt19937_64();

typedef unsigned int uint;

template <uint size_strategies>
struct Player {
	uint cstreak = 0;
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

template <typename T>
void writeOutX(std::string name, T iterable) {
	std::ofstream file;
	file.open(name);
	for (auto t : iterable) {
		file << t << " ";
	}
	file.close();
}

template <typename T>
void writeOutMat(std::string name, T iterable) {
	std::ofstream file;
	file.open(name);
	for (auto t : iterable) {
		for (auto elem : t) {
			file << elem << " ";
		}
		file << std::endl;
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
	uint N = 101;
	

	auto booldist = std::uniform_int_distribution<uint>(0, 1);

	auto players = std::vector<Player< (1 << 12)>> (N);
	
	const uint memoryIterations = 10;
	const uint outerIterations = 100;
	const uint innerIterations = 1000;

	std::vector<double> sigma = std::vector<double>(memoryIterations);
	std::vector<double> mean = std::vector<double>(memoryIterations);
	std::vector<double> N1 = std::vector<double>(innerIterations);
	std::vector<double> minorityTally = std::vector<double>(memoryIterations);
	std::vector<std::vector<uint>> streak = std::vector<std::vector<uint>>(memoryIterations);
	std::vector<std::vector<double>> sigmas = std::vector<std::vector<double>>(memoryIterations);

	//std::fill(streak.begin(), streak.end(), 0);

	uint tally[2] = { 0 };
	uint history = 2;

	for (uint memory = 2; memory < 2+memoryIterations; memory++) {
		//memory = 6;
		uint maxVal = (1 << memory);
		//uint maxVal = (1 << 2);

		//N *= 2;
		//N++;
		//players.resize(N);
		
		PRINTLN(memory);
		streak[memory - 2] = std::vector<uint>(innerIterations);
		std::fill(streak[memory-2].begin(), streak[memory - 2].end(), 0);

		sigmas[memory - 2] = std::vector<double>();
		
		//bool curStrat = false;

		for (uint j = 0; j < outerIterations; j++) {

			//generate strategies


			for (uint i = 0; i < N; i++) {
				for (uint j = 0; j < maxVal; j++) {
					players[i].cstreak = 0;
					players[i].strategy[0][j] = booldist(randGen);
					players[i].strategy[1][j] = booldist(randGen);
				}

			}

			//for (uint i = 0; i < N; i++) {
			//	PRINTLN("P" << i);
			//	printArray(players[i].strategy[0], 4);
			//	printArray(players[i].strategy[1], 4);
			//}
			

			double runMinorityTally = 0;
			double runMean = 0;
			double runSigma = 0;
			std::fill(N1.begin(), N1.end(), 0);

			auto start = std::chrono::high_resolution_clock::now();
			for (uint i = 0; i < innerIterations; i++) {

				//PRINTLN("////");
				for (const auto &p : players) {
					uint bestStrategy = (p.performance[0] >= p.performance[1]) ? 0 : 1;
					//PRINTLN(bestStrategy << " " << p.performance[0] << " " << p.performance[1]);
					
					tally[p.strategy[bestStrategy][history]]++;

				}

				uint minority = (tally[0] >= tally[1]) ? 1U : 0U;

				N1[i] = (double)tally[1];
				runMean += N1[i]; //gotta be careful with overflow here

				runMinorityTally += ((tally[0] >= tally[1]) ? tally[1] : tally[0]);

				//PRINTLN("tally was " << tally[0] << ", " << tally[1]);
				//PRINTLN("minority was " << minority);

				for (auto &p : players) {
					bool curStrat = (p.performance[0] > p.performance[1]);

					p.performance[0] += (p.strategy[0][history] == minority);
					p.performance[1] += (p.strategy[1][history] == minority);

					bool tmp = (p.performance[0] > p.performance[1]);
					if ( (curStrat == tmp) && (!curStrat == minority)  ) { p.cstreak++; }
					else {
						//streak[memory-2][p.cstreak]++;
						//p.cstreak = 0;
					}
				    //cstreak = tmp;
					

				}

				history = (history >> 1) + (minority << (memory - 1));
				tally[0] = 0;
				tally[1] = 0;

				//PRINTLN(history << " " << minority);

			}
			for (auto& p : players) {
				streak[memory - 2][p.cstreak]++;
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto dur = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
			//PRINTLN(dur.count());

			runMinorityTally /= innerIterations;
			runMean /= innerIterations;
			for (uint i = 0; i < innerIterations; i++) {
				double tmp = (runMean - N1[i]);
				runSigma += tmp * tmp;
			}
			runSigma /= innerIterations;

			mean[memory - 2] += runMean;
			sigma[memory - 2] += runSigma;
			sigmas[memory - 2].push_back(runSigma);
			minorityTally[memory - 2] += runMinorityTally;
			
			

		}

		sigma[memory - 2] /= outerIterations;
		mean[memory - 2] /= outerIterations;
		minorityTally[memory - 2] /= outerIterations;
		//break;

	}

	//writeOutXY("sigma.txt", mean, sigma);
	//writeOutX("sigmaN.txt", minorityTally);
	//writeOutMat("streaks.txt", streak);
	writeOutMat("sigmas.txt", sigmas);
	
	



}



int main() {
	
	test();

	return 0;
}