#include <benchmark/benchmark.h>

#include "type-safety/Unit.hpp"

namespace /* anonymous */ {

using namespace type_safety;
using namespace type_safety::unit_literals;

void unitDefaultConstruction(benchmark::State& state) {
	for (auto _ : state) {
		auto m = Mass{};
		benchmark::DoNotOptimize(m.value<Kilograms>() == 0.0f);
	}
}

void floatDefaultConstruction(benchmark::State& state) {
	for (auto _ : state) {
		auto m = float();
		benchmark::DoNotOptimize(m == 0.0f);
	}
}

void unitValueConstruction(benchmark::State& state) {
	for (auto _ : state) {
		auto m = Mass{ Kilograms{}, 42.0f };
		benchmark::DoNotOptimize(m.value<Kilograms>() != 0.0f);
	}
}

void floatValueConstruction(benchmark::State& state) {
	for (auto _ : state) {
		auto m = 42.0f;
		benchmark::DoNotOptimize(m != 0.0f);
	}
}

void unitLiteralConstruction(benchmark::State& state) {
	for (auto _ : state) {
		auto m = 42.0_kg;
		benchmark::DoNotOptimize(m.value<Kilograms>() != 0.0f);
	}
}

void unitArithmetics(benchmark::State& state) {
	auto m = 42_kg;
	auto i = 0.1_kg;
	for (auto _ : state) {
		benchmark::DoNotOptimize(m += i);
		benchmark::ClobberMemory();
	}
}

void floatArithmetics(benchmark::State& state) {
	auto m = 42.0f;
	auto i = 0.1f;
	for (auto _ : state) {
		benchmark::DoNotOptimize(m += i);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(unitDefaultConstruction);
BENCHMARK(floatDefaultConstruction);
BENCHMARK(unitValueConstruction);
BENCHMARK(floatValueConstruction);
BENCHMARK(unitLiteralConstruction);
BENCHMARK(unitArithmetics);
BENCHMARK(floatArithmetics);

} // anonymous namespace
