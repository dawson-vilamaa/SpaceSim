#pragma once
#ifndef SYSTEM_H
#define SYSTEM_H
#include <array>
#include "parameters.h"
#include "Sector.h"
#include "Planet.h"
#include "random.h"

class Sector;

class System {
private:
	int sectorX, sectorY, sectorZ;
	int galaxyX, galaxyY, galaxyZ;
    float offsetX, offsetY, offsetZ;
    float mouseColor[3];
	char starType;
	Sector* sector;
    std::array<Planet*, MAX_PLANETS> planets;

public:
	System(int x, int y, int z, int sx, int sy, int sz) {
		sectorX = x;
		sectorY = y;
		sectorZ = z;
		galaxyX = sx * SECTOR_WIDTH + x;
		galaxyY = sy * SECTOR_WIDTH + y;
		galaxyZ = sz * SECTOR_WIDTH + z;
        offsetX = randomFloat(-0.9, 0.9);
        offsetY = randomFloat(-0.9, 0.9);
        offsetZ = randomFloat(-0.9, 0.9);
		float fStarType = randomFloat();
        if (fStarType <= O_STAR_CHANCE) {
            if (randomBoolean())
                starType = 'O';
            else starType = 'S'; //supergiant
        }
        else if (fStarType <= B_STAR_CHANCE)
            starType = 'B';
        else if (fStarType <= GIANT_STAR_CHANCE)
            starType = 'H'; //giant (huge)
        else if (fStarType <= A_STAR_CHANCE)
            starType = 'A';
        else if (fStarType <= F_STAR_CHANCE)
            starType = 'F';
        else if (fStarType <= G_STAR_CHANCE)
            starType = 'G';
        else if (fStarType <= WHITE_DWARF_STAR_CHANCE)
            starType = 'W';
        else if (fStarType <= K_STAR_CHANCE)
            starType = 'K';
        else starType = 'M';
        this->sector = sector;
        for (int i = 0; i < randomInt(0, MAX_PLANETS); i++)
            planets[i] = new Planet(i);
        mouseColor[0] = 0.000001 * galaxyX;
        mouseColor[1] = 0.000001 * galaxyY;
        mouseColor[2] = 0.000001 * galaxyZ;
	}

    int getSectorX() {
        return sectorX;
    }

    int getSectorY() {
        return sectorY;
    }

    int getSectorZ() {
        return sectorZ;
    }

    int getGalaxyX() {
        return galaxyX;
    }

    int getGalaxyY() {
        return galaxyY;
    }

    int getGalaxyZ() {
        return galaxyZ;
    }

    float getOffsetX() {
        return offsetX;
    }

    float getOffsetY() {
        return offsetY;
    }

    float getOffsetZ() {
        return offsetZ;
    }

    char getStarType() {
        return starType;
    }

    Sector* getSector() {
        return sector;
    }

    std::array<Planet*, MAX_PLANETS> getPlanets() {
        return planets;
    }

    float* getMouseColor() {
        return mouseColor;
    }
};

#endif