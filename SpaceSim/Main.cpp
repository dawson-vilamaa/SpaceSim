#include <windows.h>
#include <GL/glut.h>
#include <array>
#include <random>
#include <time.h>
#include <math.h>
#include "parameters.h"
#include "Galaxy.h"
#include "random.h"
#include "Camera.h"

//global variables
Galaxy* galaxy;
Camera* camera;
Camera* galaxyCamera;
Camera* systemCamera;
enum View { GALAXY, SYSTEM };
View view;
System* selectedSystem;
bool rotatingView;
vec2 mousePos;

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f); // Set background depth to farthest
    glEnable(GL_DEPTH_TEST); // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL); // Set the type of depth-test
    glShadeModel(GL_SMOOTH); // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Nice perspective corrections
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    //glEnable(GL_POINT_SMOOTH);
}

void display() {
    switch (view) {
        case GALAXY:
            camera = galaxyCamera;
            galaxy->display(galaxyCamera);
        break;
        case SYSTEM:
            camera = systemCamera;
            selectedSystem->display(systemCamera);
        break;
        default:
            exit(-1);
    }
}

void input(unsigned char key, int xx, int yy) {
    vec3 cameraPos = camera->getPosition();
    vec2 cameraRot = camera->getRotation();
    float cameraRadius = camera->getRadius();

    switch (key) {
        case 'a':
            cameraRot.y += 1.0;
            if (cameraRot.y >= 360)
                cameraRot.y -= 360;
            camera->setRotation(cameraRot);
        break;
        case 'd':
            cameraRot.y -= 1.0;
            if (cameraRot.y <= 0)
                cameraRot.y += 360;
            camera->setRotation(cameraRot);
        break;

        case 'w':
            cameraRadius -= 0.9;
            if (cameraRadius < 10.0)
                cameraRadius = 10.0;
            camera->setPositionAndRadius(cameraPos.x, cameraPos.y, cameraRadius);
        break;
        case 's':
            cameraRadius += 0.9;
            camera->setPositionAndRadius(cameraPos.x, cameraPos.y, cameraRadius);
        break;

        case 'e':
            cameraPos.y += 0.75;
            camera->setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
        break;
        case 'q':
            cameraPos.y -= 0.75;
            camera->setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
        break;

        case '0':
            cameraPos.y = 0.0;
            cameraRot.x = 0.0;
            camera->setPosition(cameraPos.x, cameraPos.y, cameraPos.z);
            camera->setRotation(cameraRot);
        break;

        case 27:
            view = GALAXY;
        break;
    }
}

void specialInput(int key, int x, int y) {
    
}

void mouseInput(int button, int state, int x, int y) {
    //button: 0 - left click, 1 - middle click, 2 - right click, 3 - scroll up, 4 - scroll down
    //state: 0 - active, 1 - inactive

    switch (state) {
        case 0:
            switch (button) {
                float cameraRadius;
                case 0:
                    double modelMatrix[16];
                    double projectionMatrix[16];
                    int viewport[4];

                    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
                    glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
                    glGetIntegerv(GL_VIEWPORT, viewport);

                    switch (view) {
                        case GALAXY:
                        {
                            std::vector<std::pair<std::pair<vec3, vec3>, float>> closeStars(0);
                            vec3 cameraPos = camera->getPosition();

                            for (int sectorX = 0; sectorX < GALAXY_WIDTH; sectorX++) {
                                for (int sectorY = 0; sectorY < GALAXY_WIDTH; sectorY++) {
                                    for (int sectorZ = 0; sectorZ < GALAXY_WIDTH; sectorZ++) {
                                        for (int starX = 0; starX < SECTOR_WIDTH; starX++) {
                                            for (int starY = 0; starY < SECTOR_WIDTH; starY++) {
                                                for (int starZ = 0; starZ < SECTOR_WIDTH; starZ++) {
                                                    System* system = galaxy->getSectors()[sectorX][sectorY][sectorZ]->getSystems()[starX][starY][starZ];
                                                    if (system != nullptr) {
                                                        vec3 starPos = { (sectorX * SECTOR_WIDTH) + starX + system->getOffsetX(), (sectorY * SECTOR_WIDTH) + starY + system->getOffsetY(), (sectorZ * SECTOR_WIDTH) + starZ + system->getOffsetZ() };

                                                        //project star to screen coords
                                                        GLdouble posX, posY, posZ;
                                                        gluProject(starPos.x, starPos.y, starPos.z, modelMatrix, projectionMatrix, viewport, &posX, &posY, &posZ);
                                                        float dist = distance2D(x, y, posX, viewport[3] - posY);
                                                        if (dist < 10.0) {
                                                            closeStars.push_back(std::make_pair(std::make_pair(vec3{ (float)sectorX, (float)sectorY, (float)sectorZ }, vec3{ (float)starX, (float)starY, (float)starZ }), dist));
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            //find closest planet
                            if (closeStars.size() > 0) {
                                std::pair<std::pair<vec3, vec3>, float> closestStar = std::make_pair(std::make_pair(vec3 { -1, -1, -1 }, vec3 { -1, -1, -1 }), 1000000);
                                for (std::pair<std::pair<vec3, vec3>, float> s : closeStars) {
                                    if (s.second < closestStar.second)
                                        closestStar.first = s.first;
                                }
                                std::cout << "Sector (" << closestStar.first.first.x << ", " << closestStar.first.first.y << ", " << closestStar.first.first.z << "), "
                                    << "Star (" << closestStar.first.second.x << ", " << closestStar.first.second.y << ", " << closestStar.first.second.z << ")" << std::endl;
                                vec3 sectorLoc = closestStar.first.first;
                                vec3 starLoc = closestStar.first.second;
                                selectedSystem = galaxy->getSectors()[sectorLoc.x][sectorLoc.y][sectorLoc.z]->getSystems()[starLoc.x][starLoc.y][starLoc.z];
                                view = SYSTEM;
                            }
                        }
                        break;

                        case SYSTEM:
                            //Dawson
                            int numPlanets = selectedSystem->getNumPlanets();

                            std::vector<std::pair<int, float>> closePlanets(0);
                            vec3 cameraPos = camera->getPosition();

                            for (int i = 0; i < numPlanets; i++) {
                                Planet* planet = selectedSystem->getPlanets()[i];
                                vec3 planetPos = { planet->getRadius() * cosf(planet->getAngle()), planet->getTilt() * sin(planet->getAngle() - planet->getTiltPhaseShift()), planet->getRadius() * -sinf(planet->getAngle()) };

                                //project planet to screen coords
                                GLdouble planetX, planetY, planetZ;
                                gluProject(planetPos.x, planetPos.y, planetPos.z, modelMatrix, projectionMatrix, viewport, &planetX, &planetY, &planetZ);
                                float dist = distance2D(x, y, planetX, viewport[3] - planetY);
                                if (dist < 20.0)
                                    closePlanets.push_back(std::make_pair(i, dist));
                            }

                            //find closest planet
                            if (closePlanets.size() > 0) {
                                std::pair<int, float> closestPlanet = std::make_pair(-1, 1000000);
                                for (std::pair<int, float> p : closePlanets) {
                                    if (p.second < closestPlanet.second)
                                        closestPlanet.first = p.first;
                                }
                                std::cout << "clicked on planet " << closestPlanet.first << std::endl;
                            }

                            //Derek
                            /*GLdouble nearPos[3];
                            GLdouble farPos[3];
                            gluUnProject(x, y, 0, modelMatrix, projectionMatrix, viewport, nearPos, nearPos + 1, nearPos + 2);
                            gluUnProject(x, y, 1, modelMatrix, projectionMatrix, viewport, farPos, farPos + 1, farPos + 2);
                            Planet* planet;
                            float angleYRad = camera->getRotation().y * (PI / 180.0);
                            for (int i = 0; i < selectedSystem->getNumPlanets(); i++) {
                                planet = selectedSystem->getPlanets()[i];
                                double planetPos[3] = { planet->getRadius() * cosf(planet->getAngle() + angleYRad), planet->getTilt() * sin(planet->getAngle() - planet->getTiltPhaseShift()), planet->getRadius() * -sinf(planet->getAngle() + angleYRad) };
                                if (minPointToLineDistance(nearPos, farPos, planetPos) < 0.5)
                                    std::cout << "Clicked on planet " << i << std::endl;
                            }*/
                        break;
                    }
                break;

                case 2: //rotate start and end
                    if (!rotatingView) {
                        mousePos = { (float) x, (float) y };
                        rotatingView = true;
                    }
                    else rotatingView = false;
                break;

                //zoom in
                case 3:
                    cameraRadius = camera->getRadius();
                    camera->setRadius(cameraRadius - (0.02 * cameraRadius));
                break;

                    //zoom out
                case 4:
                    cameraRadius = camera->getRadius();
                    camera->setRadius(cameraRadius + (0.02 * cameraRadius));
                break;
            }
        break;
    }
}

void motionInput(int x, int y) {
    if (rotatingView) {
        vec2 cameraRot = camera->getRotation();
        if (x < mousePos.x)
            cameraRot.y -= 0.85;
        else if (x > mousePos.x)
            cameraRot.y += 0.85;

        if (y < mousePos.y)
            cameraRot.x -= 0.5;
        else if (y > mousePos.y)
            cameraRot.x += 0.5;
        if (camera->getRotation().x < -90)
            cameraRot.x = -90;
        else if (camera->getRotation().x > 90)
            cameraRot.x = 90;

        mousePos = { (float) x, (float) y };
        camera->setRotation(cameraRot);
    }
}

void timer(int value) {
    glutTimerFunc(16, timer, 0);
    glutPostRedisplay();
}

void reshape(GLsizei width, GLsizei height) {
    if (height == 0)
        height = 1;
    glViewport(0, 0, height, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 
    //enable perspective projection with fovy, aspect ratio, zNear and zFar
    gluPerspective(45.0f, width / height, 0.1f, STAR_RENDER_DISTANCE);
}

int main(int argc, char** argv) {
    srand(time(NULL));
    int numStars = 0;
    view = GALAXY;
    galaxy = new Galaxy();
    selectedSystem = galaxy->getSectors()[0][0][0]->getSystems()[0][0][0];
    galaxyCamera = new Camera(0.0, 20.0, 100.0);
    systemCamera = new Camera(0.0, 20.0, 100.0);
    camera = galaxyCamera;
    rotatingView = false;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Space Stuff");
    glutDisplayFunc(display);
    glutKeyboardFunc(input);
    glutSpecialFunc(specialInput);
    glutMouseFunc(mouseInput);
    glutPassiveMotionFunc(motionInput);
    glutTimerFunc(0, timer, 0);
    glutReshapeFunc(reshape);
    initGL();
    glutMainLoop();
    return 0;
}