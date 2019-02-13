#include <benchmark/benchmark.h>

#include "type-safety/Angle.hpp"

namespace /* anonymous */ {

using namespace type_safety;

void angleDefaultConstruction(benchmark::State& state) {
	for (auto _ : state) {
		[[maybe_unused]] auto a = Angle{};
		benchmark::DoNotOptimize(a.radians() == 0.0f);
	}
}

void floatDefaultConstruction(benchmark::State& state) {
	for (auto _ : state) {
		[[maybe_unused]] auto a = float();
		benchmark::DoNotOptimize(a == 0.0f);
	}
}

BENCHMARK(angleDefaultConstruction);
BENCHMARK(floatDefaultConstruction);

} // anonymous namespace
