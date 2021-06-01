#pragma once
#ifndef PLANET_H
#define PLANET_H
#include "System.h"
#include "random.h"

class System;

class Planet {
public:
	static enum PlanetType { ICY, ROCKY, METALLIC, VOLCANIC, JUPITER, NEPTUNE, DWARF };

private:
	int position;
	float diameter;
	int radius;
	float angle;
	enum PlanetType type;
	struct Color {
		float red;
		float green;
		float blue;
	} color;
	float tilt;
	float tiltPhaseShift;
	System* system;

public:
	Planet(int position, float radius, System* system) {
		this->position = position;
		this->radius = radius;
		angle = 0.0;
		if (randomFloat() < pow(1.05, radius - 70)) {
			tilt = randomFloat(-10.0, 10.0);
			tiltPhaseShift = randomFloat(0.0, 2 * PI);
		}
		else {
			tilt = 0.0;
			tiltPhaseShift = 0.0;
		}

		//planet type
		if (randomFloat() < pow(1.05, (float)radius - 60.0)) { //gas giant formula: 1.05^(radius - 60);
			switch (randomInt(1, 2)) {
				case 1:
					type = JUPITER;
					diameter = randomFloat(0.5, 0.8);
					color = { randomFloat(0.5, 1.0), randomFloat(0.4, 0.8), 0.0 };
					if (color.green > color.red)
						color.green = color.red;
				break;
				case 2:
					type = NEPTUNE;
						diameter = randomFloat(0.5, 0.8);
					color = { randomFloat(0.0, 0.3), randomFloat(0.2, 0.8), randomFloat(0.6, 1.0) };
				break;
			}
		}
		else {
			switch (randomInt(1, 5)) {
				case 1:
					type = ICY;
					diameter = randomFloat(0.1, 0.3);
					color = { 0.1, randomFloat(0.5, 0.7), randomFloat(0.8, 1.0) };
					break;
				case 2:
					type = ROCKY;
					diameter = randomFloat(0.1, 0.3);
					if (randomBoolean())
						color = { randomFloat(0.6, 0.9), randomFloat(0.4, 0.5), 0.0 };
					else {
						float brightness = randomFloat(0.5, 0.8);
						color = { brightness, brightness, brightness };
					}
					break;
				case 3:
					type = METALLIC;
					diameter = randomFloat(0.1, 0.3);
					color = { randomFloat(0.6, 0.9), randomFloat(0.4, 0.5), 0.0 };
					break;
				case 4:
					type = VOLCANIC;
					diameter = randomFloat(0.1, 0.3);
					color = { randomFloat(0.5, 0.9), 0.0, 0.0 };
					break;
				case 5:
					type = DWARF;
					diameter = 0.1f;
					float brightness = randomFloat(0.5, 0.8);
					color = { brightness, brightness, brightness };
				break;
			}
		}
		this->system = system;
	}

	int getPosition() {
		return position;
	}

	float getDiameter() {
		return diameter;
	}

	int getRadius() {
		return radius;
	}

	float getAngle() {
		return angle;
	}

	enum PlanetType getType() {
		return type;
	}

	float getTilt() {
		return tilt;
	}

	float getTiltPhaseShift() {
		return tiltPhaseShift;
	}

	void updateOrbit() {
		angle += 0.05 / radius;
		if (angle >= 2 * PI)
			angle = 0;
	}

	void setDrawColor() {
		glColor3f(color.red, color.green, color.blue);
	}
};

#endif