#include <benchmark/benchmark.h>

#include <ctime>

#include "type-safety/Angle.hpp"

namespace /* anonymous */ {

using namespace type_safety;
using namespace type_safety::angle_literals;

void angleDefaultConstruction(benchmark::State& state) {
	std::srand(0);
	const auto input = static_cast<float>(std::rand());

	for (auto _ : state) {
		auto a = Angle{};
		benchmark::DoNotOptimize(a.radians() == input);
	}
}

void floatDefaultConstruction(benchmark::State& state) {
	std::srand(0);
	const auto input = static_cast<float>(std::rand());

	for (auto _ : state) {
		auto a = float();
		benchmark::DoNotOptimize(a == input);
	}
}

void angleValueConstruction(benchmark::State& state) {
	std::srand(0);
	const auto input = static_cast<float>(std::rand());

	for (auto _ : state) {
		auto a = Angle{radiansTag, 42.0f};
		benchmark::DoNotOptimize(a.radians() == input);
	}
}

void floatValueConstruction(benchmark::State& state) {
	std::srand(0);
	const auto input = static_cast<float>(std::rand());

	for (auto _ : state) {
		auto a = 42.0f;
		benchmark::DoNotOptimize(a == input);
	}
}

void angleLiteralConstruction(benchmark::State& state) {
	std::srand(0);
	const auto input = static_cast<float>(std::rand());

	for (auto _ : state) {
		auto a = 42.0_rad;
		benchmark::DoNotOptimize(a.radians() == input);
	}
}

void angleArithmetics(benchmark::State& state) {
	std::srand(0);
	const auto input = Angle{radiansTag, static_cast<float>(std::rand())};

	auto a = 42_rad;
	for (auto _ : state) {
		benchmark::DoNotOptimize(a += input);
	}
}

void floatArithmetics(benchmark::State& state) {
	std::srand(0);
	const auto input = static_cast<float>(std::rand());

	auto a = 42.0f;
	for (auto _ : state) {
		benchmark::DoNotOptimize(a += input);
	}
}

//BENCHMARK(angleDefaultConstruction);
//BENCHMARK(floatDefaultConstruction);
//BENCHMARK(angleValueConstruction);
//BENCHMARK(floatValueConstruction);
//BENCHMARK(angleLiteralConstruction);
//BENCHMARK(angleArithmetics);
//BENCHMARK(floatArithmetics);

} // anonymous namespace
