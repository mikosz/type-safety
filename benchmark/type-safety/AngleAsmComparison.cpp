#include "type-safety/Angle.hpp"

using namespace type_safety;
using namespace type_safety::angle_literals;

struct RotationAngles {
	// NOTE: on VisualStudio changing this to 0.0_rad does not yield the same code as in the floats version,
	// as some ops are actually performed on initialisation. Why?
	// Clang optimises everything away just fine.
	Angle pitch = Angle{radiansTag, 0.0f};
	Angle yaw = Angle{radiansTag, 0.0f};
	Angle roll = Angle{radiansTag, 0.0f};
};

struct ActorAngles {

	RotationAngles rotation;

	void rotate(Angle pitch, Angle yaw, Angle roll) {
		rotation.pitch += pitch;
		rotation.yaw += yaw;
		rotation.roll += roll;
	}

};

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

ActorAngles rotationExampleAngles() {
	auto actor = ActorAngles{};
	actor.rotate(0.1_rad, 0.2_rad, 0.3_rad);
	actor.rotate(0.3_rad, 0.2_rad, 0.1_rad);
	return actor;
}

ActorFloats rotationExampleFloats() {
	auto actor = ActorFloats{};
	actor.rotate(0.1f, 0.2f, 0.3f);
	actor.rotate(0.3f, 0.2f, 0.1f);
	return actor;
}

void externalAngleSink(float pitch, float yaw, float roll);

void callAngles() {
	const auto actorAngles = rotationExampleAngles();
	externalAngleSink(
		actorAngles.rotation.pitch.radians(),
		actorAngles.rotation.yaw.radians(),
		actorAngles.rotation.roll.radians()
	);
}

void callFloats() {
	const auto actorFloats = rotationExampleFloats();
	externalAngleSink(
		actorFloats.rotation.pitch,
		actorFloats.rotation.yaw,
		actorFloats.rotation.roll
	);
}
