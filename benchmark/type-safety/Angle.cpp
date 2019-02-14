#include <benchmark/benchmark.h>

#include "type-safety/Angle.hpp"

namespace /* anonymous */ {

using namespace type_safety;
using namespace type_safety::angle_literals;

void angleDefaultConstruction(benchmark::State& state) {
	for (auto _ : state) {
		auto a = Angle{};
		benchmark::DoNotOptimize(a.radians() == 0.0f);
	}
}

void floatDefaultConstruction(benchmark::State& state) {
	for (auto _ : state) {
		auto a = float();
		benchmark::DoNotOptimize(a == 0.0f);
	}
}

void angleValueConstruction(benchmark::State& state) {
	for (auto _ : state) {
		auto a = Angle{radiansTag, 42.0f};
		benchmark::DoNotOptimize(a.radians() == 0.0f);
	}
}

void floatValueConstruction(benchmark::State& state) {
	for (auto _ : state) {
		auto a = 42.0f;
		benchmark::DoNotOptimize(a == 0.0f);
	}
}

void angleLiteralConstruction(benchmark::State& state) {
	for (auto _ : state) {
		auto a = 42_rad;
		benchmark::DoNotOptimize(a.radians() == 0.0f);
	}
}

void angleArithmetics(benchmark::State& state) {
	auto a = 42_rad;
	auto i = 0.1_rad;
	for (auto _ : state) {
		benchmark::DoNotOptimize(a += i);
		benchmark::ClobberMemory();
	}
}

void floatArithmetics(benchmark::State& state) {
	auto a = 42.0f;
	auto i = 0.1f;
	for (auto _ : state) {
		benchmark::DoNotOptimize(a += i);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(angleDefaultConstruction);
BENCHMARK(floatDefaultConstruction);
BENCHMARK(angleValueConstruction);
BENCHMARK(floatValueConstruction);
BENCHMARK(angleLiteralConstruction);
BENCHMARK(angleArithmetics);
BENCHMARK(floatArithmetics);

} // anonymous namespace
