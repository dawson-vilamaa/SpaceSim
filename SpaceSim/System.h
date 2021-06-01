#pragma once
#ifndef SYSTEM_H
#define SYSTEM_H
#include <array>
#include "parameters.h"
#include "Sector.h"
#include "Planet.h"
#include "random.h"
#include "Camera.h"

class Sector;

class System {
private:
	int sectorX, sectorY, sectorZ;
	int galaxyX, galaxyY, galaxyZ;
    float offsetX, offsetY, offsetZ;
	char starType;
	Sector* sector;
    int numPlanets;
    float diameter;
    std::array<Planet*, MAX_PLANETS> planets;

public:
	System(int x, int y, int z, int sx, int sy, int sz) {
        //star
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

        //planets
        numPlanets = randomInt(1, MAX_PLANETS);
        float currentRadius = 2.0;
        for (int i = 0; i < MAX_PLANETS; i++) {
            if (i < numPlanets) {
                currentRadius += randomFloat(2.0, 8.0);
                planets[i] = new Planet(i, currentRadius, this);
            }
            else planets[i] = nullptr;
        }
        diameter = currentRadius * 2.0;
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

    float getDiameter() {
        return diameter;
    }

    int getNumPlanets() {
        return numPlanets;
    }

    std::array<Planet*, MAX_PLANETS> getPlanets() {
        return planets;
    }

    void setDrawColor() {
        switch (starType) {
        case 'O':
        case 'B':
            glColor3f(0.1, 0.6, 1.0);
            break;
        case 'A':
        case 'W':
            glColor3f(1.0, 1.0, 1.0);
            break;
        case 'F':
        case 'G':
        case 'K':
            glColor3f(1.0, 1.0, 0.0);
            break;
        case 'H':
        case 'S':
        case 'M':
            glColor3f(1.0, 0.3, 0.3);
            break;
        }
    }

    float getStarSize() {
        switch (starType) {
            case 'H':
            case 'S':
                return 5.0;
            break;
            case 'O':
            case 'B':
                return 3.0;
            break;
            case 'A':
            case 'F':
                return 2.5;
            break;
            case 'G':
                return 2.0;
            break;
            case 'K':
                return 1.5;
            break;
            case 'M':
                return 1.0;
            break;
            case 'W':
                return 0.5;
            break;
        }
    }

    void display(Camera* camera) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //rotate camera
        gluLookAt(camera->getPosition().x, camera->getPosition().y / 2.0, camera->getPosition().z, //eye position
            0.0, 0.0, 0.0, //look at position
            0.0, 1.0, 0.0); //positive y axis = up
        glRotatef(camera->getRotation().x, 1.0, 0.0, 0.0);
        glRotatef(camera->getRotation().y, 0.0, 1.0, 0.0);

        //star
        this->setDrawColor();
        glutSolidSphere(this->getStarSize(), 20, 20);

        //planets
        glPointSize(1);
        for (int i = 0; i < this->getNumPlanets(); i++) {
            Planet* planet = this->getPlanets()[i];
            if (planet != nullptr) {
                float planetRadius = planet->getRadius();
                float planetAngle = planet->getAngle();
                float planetTilt = planet->getTilt();
                float planetTiltPhaseShift = planet->getTiltPhaseShift();

                //planet
                glTranslatef(planetRadius * cos(planetAngle), planetTilt * sin(planetAngle - planetTiltPhaseShift), planetRadius * sin(planetAngle));
                planet->setDrawColor();
                glutSolidSphere(planet->getDiameter(), 20, 20);

                //orbit
                glTranslatef(-(planetRadius * cos(planetAngle)), -(planetTilt * sin(planetAngle - planetTiltPhaseShift)), -(planetRadius * sin(planetAngle)));
                glColor3f(0.5, 0.5, 0.5);
                glBegin(GL_POINTS);
                for (float i = 0.0; i < 2.0; i += 1.0 / (4.0 * PI * planetRadius))
                    glVertex3f(planetRadius * cos(i * PI), planetTilt * sin((i * PI) - planetTiltPhaseShift), planetRadius * sin(i * PI));
                glEnd();

                planet->updateOrbit();
            }
        }

        glutSwapBuffers();
    }
};

#endif