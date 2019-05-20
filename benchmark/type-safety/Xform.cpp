#include <benchmark/benchmark.h>

#include <ctime>

#include "type-safety/Xform.hpp"

namespace /* anonymous */ {

using namespace type_safety;

Matrix randomMatrix() {
	std::srand(0);
	auto result = Matrix{};
	for (auto row = 0u; row < 4u; ++row) {
		for (auto col = 0u; col < 4u; ++col) {
			result.get(row, col) = static_cast<float>(std::rand());
		}
	}
	return result;
}

void matrixMultiplication(benchmark::State& state) {
	const auto lhs = randomMatrix();
	const auto rhs = randomMatrix();

	for (auto _ : state) {
		benchmark::DoNotOptimize(lhs * rhs);
	}
}

void noRuntimeCheckXformConcatenation(benchmark::State& state) {
	const auto lhs = Xform<space::World, space::Player>{randomMatrix()};
	const auto rhs = Xform<space::Player, space::Camera>{randomMatrix()};

	for (auto _ : state) {
		benchmark::DoNotOptimize(inSequence(lhs, rhs));
	}
}

void runtimeCheckXformConcatenation(benchmark::State& state) {
#ifdef DO_SPACE_RUNTIME_CHECKS
	const auto playerAtFrame = space::PlayerAtFrame{31};
#else
	const auto playerAtFrame = space::PlayerAtFrame{};
#endif /* DO_SPACE_RUNTIME_CHECKS */
	const auto lhs = makeXform(randomMatrix(), space::World{}, playerAtFrame);
	const auto rhs = makeXform(randomMatrix(), playerAtFrame, space::Camera{});

	for (auto _ : state) {
		benchmark::DoNotOptimize(inSequence(lhs, rhs));
	}
}

BENCHMARK(matrixMultiplication);
BENCHMARK(noRuntimeCheckXformConcatenation);
BENCHMARK(runtimeCheckXformConcatenation);

} // anonymous namespace
