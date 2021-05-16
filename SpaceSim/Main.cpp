#include <windows.h>
#include <GL/glut.h>
#include <array>
#include <random>
#include <time.h>
#include <math.h>
#include "parameters.h"
#include "Galaxy.h"
#include "random.h"
#define PI 3.14159265

Galaxy* galaxy;
std::array<std::array<std::array<boolean, 50>, 50>, 50> stars;
float angleX = 0.0;
float angleY = 0.0;
float rad = 0.0;
float galaxyWidth = GALAXY_WIDTH * SECTOR_WIDTH;
float cameraRadius = galaxyWidth * 2.5;
float camX = 0.0;
float camY = 3.0;
float camZ = -5.0;
bool viewMode = false; //false - galaxy view, true = system view
bool rotatingView = false;
struct MousePos {
    int x;
    int y;
} mousePos;

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f); // Set background depth to farthest
    glEnable(GL_DEPTH_TEST); // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL); // Set the type of depth-test
    glShadeModel(GL_SMOOTH); // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Nice perspective corrections
    glEnable(GL_POINT_SMOOTH);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
    glPushMatrix();
    glLoadIdentity();                 // Reset the model-view matrix

    //rotate camera
    gluLookAt(0, camY, -cameraRadius, //eye position
        0.0, 0.0, 0.0, //look at position
        0.0, 1.0, 0.0); //positive y axis = up
    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);
    glTranslatef(-(galaxyWidth / 2), -(galaxyWidth / 2), -(galaxyWidth / 2));

    for (int x = 0; x < GALAXY_WIDTH; x++) {
        for (int y = 0; y < GALAXY_WIDTH; y++) {
            for (int z = 0; z < GALAXY_WIDTH; z++) {
                for (int a = 0; a < SECTOR_WIDTH; a++) {
                    for (int b = 0; b < SECTOR_WIDTH; b++) {
                        for (int c = 0; c < SECTOR_WIDTH; c++) {
                            System* system = galaxy->getSectors()[x][y][z]->getSystems()[a][b][c];
                            if (system != nullptr) {
                                float xPos = (SECTOR_WIDTH * x + a) * 1.0;
                                float yPos = (SECTOR_WIDTH * y + b) * 1.0;
                                float zPos = (SECTOR_WIDTH * z + c) * 1.0;

                                switch (system->getStarType()) {
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
                                    default:
                                        glColor3f(0.0, 1.0, 0.0);
                                }

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

    glPopMatrix();
    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

void input(unsigned char key, int xx, int yy) {
    switch (key) {
        case 'a':
            angleY += 1.0;
            if (angleY >= 360)
                angleY -= 360;
            rad = angleY * (PI / 180.0);
            camX = cameraRadius * cos(rad);
            camZ = cameraRadius * sin(rad);
        break;
        case 'd':
            angleY -= 1.0;
            if (angleY <= 0)
                angleY += 360;
            rad = angleY * (PI / 180.0);
            camX = cameraRadius * cos(rad);
            camZ = cameraRadius * sin(rad);
        break;
        case 'w':
            cameraRadius -= 0.01 * cameraRadius;
        break;
        case 's':
            cameraRadius += 0.01 * cameraRadius;
        break;
        case 'e':
            camY += 0.01 * cameraRadius;
        break;
        case 'q':
            camY -= 0.01 * cameraRadius;
        break;
    }
}

void mouseInput(int button, int state, int x, int y) {
    //button: 0 - left click, 1 - middle click, 2 - right click, 3 - scroll up, 4 - scroll down
    //state: 0 - active, 1 - inactive
    if (state == 0) {
        switch (button) {
            //rotate start
            case 0:
                if (!rotatingView) {
                    mousePos = { x, y };
                    rotatingView = true;
                }
                else rotatingView = false;
            break;

            //zoom in
            case 3:
                cameraRadius -= 0.02 * cameraRadius;
            break;

            //zoom out
            case 4:
                cameraRadius += 0.02 * cameraRadius;
            break;
        }
    }
    else {
        switch (button) {
            //rotate end
            case 0:
                //rotatingView = false;
            break;
        }
    }
}

void motionInput(int x, int y) {
    if (rotatingView) {
        if (x < mousePos.x)
            angleY -= 3;
        else if (x > mousePos.x)
            angleY += 3;

        if (y < mousePos.y)
            angleX -= 3;
        else if (y > mousePos.y)
            angleX += 3;
        if (angleX < -90)
            angleX = -90;
        else if (angleX > 90)
            angleX = 90;

        mousePos = { x, y };
    }
}

void timer(int value) {
    glutTimerFunc(16, timer, 0);
    glutPostRedisplay();
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {
    if (height == 0)
        height = 1;
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, STAR_RENDER_DISTANCE);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    srand(time(NULL));
    int numStars = 0;
    galaxy = new Galaxy();

    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow("Space Stuff"); // Create window with the given title
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutKeyboardFunc(input);
    glutMouseFunc(mouseInput);
    glutPassiveMotionFunc(motionInput);
    glutTimerFunc(0, timer, 0);
    glutReshapeFunc(reshape);       // Register callback handler for window re-size event
    initGL();                       // Our own OpenGL initialization
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}