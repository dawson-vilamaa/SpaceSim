#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Vectors.h"
#include "MathStuff.h"

class Camera {
private:
	vec3 position;
	vec2 rotation;
	float radius;

public:
	Camera(float x, float y, float radius) {
		position = { x, y, sqrtf(pow(radius, 2) - pow(y, 2)) };
		rotation = { 0.0, 0.0 };
		this->radius = radius;
	}

	vec3 getPosition() {
		return position;
	}

	vec2 getRotation() {
		return rotation;
	}

	float getRadius() {
		return radius;
	}

	void setPositionAndRadius(float x, float y, float radius) {
		position = { x, y, sqrtf(pow(radius, 2) - pow(y, 2)) };
		this->radius = radius;
	}

	void setPosition(float x, float y, float z) {
		position = { x, y, z };
		radius = distance(0, 0, 0, x, y, z);
	}

	void setRotation(vec2 rotation) {
		this->rotation = rotation;
		//position = { radius * sin(rotation.y), sin(rotation.x), position.z };
	}

	void setRadius(float radius) {
		position = { position.x, position.y, sqrtf(pow(radius, 2) - pow(position.y, 2)) };
		this->radius = radius;
	}
};

#endif