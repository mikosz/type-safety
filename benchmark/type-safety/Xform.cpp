#include <benchmark/benchmark.h>

#include <ctime>

#include "type-safety/Xform.hpp"
#include "type-safety/VecN.hpp"

namespace /* anonymous */ {

using namespace type_safety;

Matrix randomMatrix() {
	std::srand(0);
	auto result = Matrix{};
	for (auto row = 0u; row < 3u; ++row) {
		for (auto col = 0u; col < 4u; ++col) {
			result.get(row, col) = static_cast<float>(std::rand());
		}
	}
	result.get(3, 0) = 0.0f;
	result.get(3, 1) = 0.0f;
	result.get(3, 2) = 0.0f;
	result.get(3, 3) = 1.0f;
	return result;
}

Vec3 randomVec3() {
	std::srand(0);
	auto result = Vec3{};
	for (auto i = 0u; i < 3u; ++i) {
		result.get(i) = static_cast<float>(std::rand());
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

void matrixVectorMultiplication(benchmark::State& state) {
	const auto m = randomMatrix();
	const auto v = Vec4{randomVec3(), 1.0f};

	for (auto _ : state) {
		benchmark::DoNotOptimize(m * v);
	}
}

void noRuntimeCheckVectorTransform(benchmark::State& state) {
	const auto x = Xform<space::World, space::Player>{randomMatrix()};
	const auto v = Point<space::World>{randomVec3()};

	for (auto _ : state) {
		benchmark::DoNotOptimize(x.apply(v));
	}
}

void runtimeCheckVectorTransform(benchmark::State& state) {
	const auto x = Xform<space::PlayerAtFrame, space::World>{
		randomMatrix(), space::PlayerAtFrame{31}, space::World{}};
	const auto v = Vector<space::PlayerAtFrame>{randomVec3(), space::PlayerAtFrame{31}};

	for (auto _ : state) {
		benchmark::DoNotOptimize(x.apply(v));
	}
}

//BENCHMARK(matrixMultiplication);
//BENCHMARK(noRuntimeCheckXformConcatenation);
//BENCHMARK(runtimeCheckXformConcatenation);
//BENCHMARK(matrixVectorMultiplication);
//BENCHMARK(noRuntimeCheckVectorTransform);
//BENCHMARK(runtimeCheckVectorTransform);

} // anonymous namespace
