#include "type-safety/Angle.hpp"

using namespace type_safety;
using namespace type_safety::angle_literals;

float externalAngleSink(Angle pitch, Angle yaw, Angle roll);
float externalAngleFloatSink(float pitch, float yaw, float roll);

namespace angle_test {

struct RotationAngles {
	Angle pitch = 0.0_rad;
	Angle yaw = 0.0_rad;
	Angle roll = 0.0_rad;
};

struct ActorAngles {

	RotationAngles rotation;

	void rotate(Angle pitch, Angle yaw, Angle roll) {
		rotation.pitch += pitch;
		rotation.yaw += yaw;
		rotation.roll += roll;
	}

};

ActorAngles rotationExampleAngles() {
	auto actor = ActorAngles{};
	actor.rotate(0.42_rad, 0.0_rad, 0.0_rad);
	actor.rotate(0.0_rad, 3.14_rad, 0.0_rad);
	actor.rotate(0.0_rad, 0.0_rad, 2.71_rad);
	return actor;
}

float callAngles(Angle a) {
	const auto actorAngles = rotationExampleAngles();
	return externalAngleSink(
		actorAngles.rotation.pitch + a,
		actorAngles.rotation.yaw,
		actorAngles.rotation.roll
	);
}

struct RotationFloats {
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};

struct ActorFloats {

	RotationFloats rotation;

	void rotate(float pitch, float yaw, float roll) {
		rotation.pitch += pitch;
		rotation.yaw += yaw;
		rotation.roll += roll;
	}

};

ActorFloats rotationExampleFloats() {
	auto actor = ActorFloats{};
	actor.rotate(0.42f, 0.0f, 0.0f);
	actor.rotate(0.0f, 3.14f, 0.0f);
	actor.rotate(0.0f, 0.0f, 2.71f);
	return actor;
}

float callFloats(float f) {
	const auto actorFloats = rotationExampleFloats();
	return externalAngleFloatSink(
		actorFloats.rotation.pitch + f,
		actorFloats.rotation.yaw,
		actorFloats.rotation.roll
	);
}

} // namespace angle_test
