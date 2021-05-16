#pragma once
#ifndef SECTOR_H
#define SECTOR_H
#include <array>
#include <math.h>
#include "parameters.h"
#include "System.h"

class System;

class Sector {
private:
	int x, y, z;
	std::array<std::array<std::array<System*, SECTOR_WIDTH>, SECTOR_WIDTH>, SECTOR_WIDTH> systems;

	float distance(int x1, int y1, int z1, int x2, int y2, int z2) {
		return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) + pow(z2 - z1, 2));
	}

	float getSystemChance(int x, int y, int z) {
		int center = (GALAXY_WIDTH * SECTOR_WIDTH) / 2;
		float dist = distance(center, center, center, x, y, z);
		return -(1.0 / dist) + (randomFloat(1.0 - CLUSTER_SIZE_OUTER, CLUSTER_SIZE_INNER));
	}

public:
	Sector(int x, int y, int z) {
		this->x = x;
		this->y = y;
		this->z = z;
		for (int x = 0; x < SECTOR_WIDTH; x++) {
			for (int y = 0; y < SECTOR_WIDTH; y++) {
				for (int z = 0; z < SECTOR_WIDTH; z++) {
					if (getSystemChance(SECTOR_WIDTH * this->x + x, SECTOR_WIDTH * this->y + y, SECTOR_WIDTH * this->z + z) < SYSTEM_CHANCE)
						systems[x][y][z] = new System(x, y, z, this->x, this->y, this->z);
					else systems[x][y][z] = nullptr;
				}
			}
		}
	}

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}

	int getZ() {
		return z;
	}

	std::array<std::array<std::array<System*, SECTOR_WIDTH>, SECTOR_WIDTH>, SECTOR_WIDTH> getSystems() {
		return systems;
	}
};

#endif