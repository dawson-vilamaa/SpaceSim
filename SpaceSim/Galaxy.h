#pragma once
#ifndef GALAXY_H
#define GALAXY_H
#include <stdlib.h>
#include <time.h>
#include <array>
#include "parameters.h"
#include "Sector.h"

class Galaxy {
private:
	std::array<std::array<std::array<Sector*, GALAXY_WIDTH>, GALAXY_WIDTH>, GALAXY_WIDTH> sectors;

public:
	Galaxy() {
		srand(time(NULL));
		for (int x = 0; x < GALAXY_WIDTH; x++) {
			for (int y = 0; y < GALAXY_WIDTH; y++) {
				for (int z = 0; z < GALAXY_WIDTH; z++) {
					sectors[x][y][z] = new Sector(x, y, z);
				}
			}
		}
	}

	std::array<std::array<std::array<Sector*, GALAXY_WIDTH>, GALAXY_WIDTH>, GALAXY_WIDTH> getSectors() {
		return sectors;
	}
};

#endif