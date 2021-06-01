#pragma once
#ifndef GALAXY_H
#define GALAXY_H
#include <stdlib.h>
#include <time.h>
#include <array>
#include "parameters.h"
#include "Sector.h"
#include "Camera.h"

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

	void display(Camera* camera) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //rotate camera
        gluLookAt(0, camera->getPosition().y, camera->getPosition().z, //eye position
            0.0, 0.0, 0.0, //look at position
            0.0, 1.0, 0.0); //positive y axis = up
        glRotatef(camera->getRotation().x, 1.0, 0.0, 0.0);
        glRotatef(camera->getRotation().y, 0.0, 1.0, 0.0);
        float center = -((GALAXY_WIDTH * SECTOR_WIDTH) / 2);
        glTranslatef(center, center, center);

        for (int x = 0; x < GALAXY_WIDTH; x++) {
            for (int y = 0; y < GALAXY_WIDTH; y++) {
                for (int z = 0; z < GALAXY_WIDTH; z++) {
                    for (int a = 0; a < SECTOR_WIDTH; a++) {
                        for (int b = 0; b < SECTOR_WIDTH; b++) {
                            for (int c = 0; c < SECTOR_WIDTH; c++) {
                                System* system = this->getSectors()[x][y][z]->getSystems()[a][b][c];
                                if (system != nullptr) {
                                    float xPos = (SECTOR_WIDTH * x + a) * 1.0;
                                    float yPos = (SECTOR_WIDTH * y + b) * 1.0;
                                    float zPos = (SECTOR_WIDTH * z + c) * 1.0;

                                    system->setDrawColor();

                                    glPushMatrix();
                                    glTranslatef(xPos + system->getOffsetX(), yPos + system->getOffsetY(), zPos + system->getOffsetZ());
                                    //glColor3f(xPos / galaxyWidth, yPos / galaxyWidth, zPos / galaxyWidth); //gradient
                                    glutSolidSphere(0.065, 10, 10);
                                    glPopMatrix();
                                }
                            }
                        }
                    }
                }
            }
        }

        glutSwapBuffers();
	}
};

#endif