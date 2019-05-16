#include "type-safety/Angle.hpp"

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

void externalAngleSink(float pitch, float yaw, float roll);

void callFloats() {
	const auto actorFloats = rotationExampleFloats();
	externalAngleSink(
		actorFloats.rotation.pitch,
		actorFloats.rotation.yaw,
		actorFloats.rotation.roll
	);
}
