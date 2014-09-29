#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "math.h"

#define NUM_PARTICLES 1000
#define SLOW_DOWN_START 1000

#define WAVE 1
#define RANDOM 2
#define SPHERE 3
#define PULSE 4
#define GRAPH 5

// Question 1: In a GLUT program, how is control passed
// back to the programmer?  How is this set up during
// initialization?

// Control is passed back to the draw event
// loop via callbacks. The callback here is "display",
// which dictates what gets drawn to the GLUT display
// screen.

// -------------------------------------
// Globals

typedef struct {
    bool friendly;
    float pos[3];
    bool isDestSet;
    float dest[3];
} Particle;

Particle Field[NUM_PARTICLES];

int mode            = RANDOM;
float speed         = SLOW_DOWN_START;
float step_size     = 1 / SLOW_DOWN_START;
int num_rotations   = 0;
int translate_step  = 10;

int win_width       = 512;
int win_height      = 512;

int mouseX          = 0;
int mouseY          = 0;
bool mouseUp        = false;

int draw_mode       = 6;
int _offset         = 4;
float dot_diam      = 4.f;

int cutoffDistance  = 0.2;

float edge_prob       = 0.001;

GLfloat RED[3]      = {196.f/256.f, 30.f/256.f, 58.f/256.f};
GLfloat BLACK[3]    = {51.f/256.f, 51.f/256.f, 51.f/256.f};
GLfloat GREEN[3]    = {64.f/256.f, 130.f/256.f, 109.f/256.f};
GLfloat YELLOW[3]   = {255.f/256.f, 215.f/256.f, 0.f/256.f};
GLfloat GRAY[3]     = {100.f/256.f, 100.f/256.f, 100.f/256.f};

// -------------------------------------
// Utilities

float sq(float x) {
    return x * x;
}

float offset(float x) {
    return x + _offset;
}

float genRand() {
    return (float)rand() / RAND_MAX;
}

void assignPos(float *pos, float x, float y, float z) {
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

void updatePos(float *pos, float x, float y, float z) {
    pos[0] += x;
    pos[1] += y;
    pos[2] += z;
}

void clearDestinations() {
    for (int i = 0; i < NUM_PARTICLES; i++) {
        Field[i].isDestSet = false;
    }
}

void assignDestinations() {
    float x, y, z;
    int dir = genRand() > 0.5 ? -1 : 1;

    for (int i = 0; i < NUM_PARTICLES; i++) {
        Particle p = Field[i];
        p.isDestSet = true;
        
        if (mode == WAVE) {
            x = sin(360 * p.pos[0]);
            y = cos(360 * p.pos[0]);
            z = dir;
        }
        
        ///////////////////////////
        
        if (mode == PULSE) {
            x = p.pos[0];
            y = p.pos[1];
            z = p.pos[2];
            x *= 0.495 < x && x < 0.505 ? 2.0 : 0.0;
            y *= 0.495 < y && y < 0.505 ? 2.0 : 0.0;
            z *= 0.495 < z && z < 0.505 ? 2.0 : 0.0;
        }
        
        ///////////////////////////
        
        if (mode == SPHERE) {
            x = p.pos[0];
            y = p.pos[1];
            z = 0.5 + dir * sqrt(sq(0.5) - sq(x - 0.5) - sq(y - 0.5));
        }

        assignPos(p.dest, x, y, z);
    }
}

void drawDots(void) {
    glPointSize(dot_diam);
    glBegin(GL_POINTS);
    for (int i = 0; i < NUM_PARTICLES; i++) {
        if (Field[i].friendly) {
            glColor3fv(RED);
        } else {
            glColor3fv(BLACK);
        }
        glVertex3fv(Field[i].pos);
    }
    glEnd();
}


int lineMatrix[NUM_PARTICLES][NUM_PARTICLES];

void initLineMatrix() {
    for (int i = 0; i < NUM_PARTICLES; i++) {
        for (int j = 0; j < NUM_PARTICLES; j++) {
            lineMatrix[i][j] = -1;
        }
    }
}

void drawLines(void) {
    for (int i = 0; i < NUM_PARTICLES; i++) {
        Particle src = Field[i];
        
        for (int j = i + 1; j < NUM_PARTICLES; j++) {
            if (lineMatrix[i][j] == -1) {
                lineMatrix[i][j] = genRand() < edge_prob ? 1 : 0;
            }
            if (lineMatrix[i][j]) {
                Particle dst = Field[j];
                
                // connect pts
                glBegin(GL_LINES);
                if (src.friendly && dst.friendly)
                    glColor3fv(GREEN);
                // if not friends, chances are they quarrel
                else if ((src.friendly && !dst.friendly) ||
                         (!src.friendly && dst.friendly)) {
                    int r = genRand();
                    if (r > 0.5) {
                        glColor3fv(GREEN);
                    } else {
                        glColor3fv(RED);
                    }
                }
                
                else if (src.friendly && !dst.friendly)
                    glColor3fv(GRAY);
                glVertex3f(src.pos[0], src.pos[1], src.pos[2]);
                glVertex3f(dst.pos[0], dst.pos[1], dst.pos[2]);
                glEnd();
            }
        }
    }
}

void initPos(float *pos) {
    int sign = (genRand() > 0.5) ? -1 : 1;
    assignPos(pos, sign * genRand(), sign * genRand(), sign * genRand());
}

void initializeParticles() {
    initLineMatrix();
    
    int i;
    for (i = 0; i < NUM_PARTICLES; i++) {
        if (genRand() > 0.5) {
            Field[i].friendly = true;
        } else {
            Field[i].friendly = false;
        }
        initPos(Field[i].pos);
        
        if (mode != RANDOM) {
            assignDestinations();
        }
    }
}

bool isMousePositive() {
    return mouseX > win_height / 2;
}

void updateParticlePosition(float *pos, char sign) {
    if (sign == '-') {
        updatePos(pos, genRand() / speed, genRand() / speed, genRand() / speed);
    } else {
        updatePos(pos, genRand() / speed, genRand() / speed, genRand() / speed);
    }
}

void updateRandomMode() {
    int i;
    for (i = 0; i < NUM_PARTICLES; i++) {
        if (genRand() > 0.5) {
            updateParticlePosition(Field[i].pos, '-');
        } else {
            updateParticlePosition(Field[i].pos, '+');
        }
    }
}

void walk(Particle p) {
    if (p.dest[0] - p.pos[0]) p.pos[0] += step_size;
    else p.pos[0] -= step_size;
    
    if (p.dest[1] - p.pos[1]) p.pos[1] += step_size;
    else p.pos[1] -= step_size;
    
    if (p.dest[2] - p.pos[2]) p.pos[2] += step_size;
    else p.pos[2] -= step_size;
}

void updatePositions() {
    if (mode == RANDOM) {
        updateRandomMode();
    } else {
        int i;
        for (i = 0; i < NUM_PARTICLES; i++) {
            walk(Field[i]);
        }
    }
}

void drawAll() {
    drawDots();
    if (mode == GRAPH) drawLines();
}

void translateLeft() {
    printf(".");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glTranslatef(-translate_step, 0, 0);
    glOrtho(0., 1., 0., 1., -5., 5.);
}

void translateRight() {
    glMatrixMode(GL_PROJECTION);
    glTranslatef(translate_step, 0, 0);
}

void translateDown() {
    glMatrixMode(GL_PROJECTION);
    glTranslatef(0, -translate_step, 0);
}

void translateUp() {
    glMatrixMode(GL_PROJECTION);
    glTranslatef(0, translate_step, 0);
}

void translateIn() {
    glMatrixMode(GL_PROJECTION);
    glTranslatef(0, 0, translate_step);
}

void translateOut() {
    glMatrixMode(GL_PROJECTION);
    glTranslatef(0, 0, -translate_step);
}

// -------------------------------------
// Handlers

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseUp = true;
        translateIn();
    } else {
        mouseUp = false;
        translateOut();
    }
}

void follow(float x, float y) {
    glBegin(GL_POINT);
    glPointSize(8.);
    glColor3f(1.f,0.f,0.f);
    glBegin(GL_POINTS);
    float fx = offset(x) / win_width;
    float fy = 1 - offset(y) / win_height;
    glVertex3f(fx, fy, 0.f);
    glEnd();
}

void idle() {
    glClear( GL_COLOR_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    updatePositions();
    drawAll();
    if (mode == GRAPH) drawLines();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    for (int i = 0; i < num_rotations; i++)
        glRotatef(0.1, 0, 1, 0);
    glOrtho(0., 1., 0., 1., -1., 1.);
    glViewport(0, 0, win_width, win_height);

    num_rotations++;
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutPostRedisplay();
}

void motion(int x, int y) {
    mouseX = x;
    mouseY = y;
}



void drawCube() {
    /** TODO for Justin
     Currently, this function attempts to draw a large cube and display it on the screen
     using just triangle strips. However, it doesn't do a good job. Try running it and see.
     
     The todo is to adjust the points on the vertex such that the cube looks more like a cube.
     **/
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // red panel
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.f,0.f,0.f);
    glVertex3f(0,0,0);
    glVertex3f(0,.1,0);
    glVertex3f(.1,0,0);
    glVertex3f(.1,.1,0);
    glEnd();

    // pink panel
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.f,0.f,1.f);
    glVertex3f(.15,.05,-.1);
    glVertex3f(.15,.15,-.1);
    glVertex3f(.05,.05,-.1);
    glVertex3f(.05,.15,-.1);
    glEnd();

    // blue panel
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.f,0.f,1.f);
    glVertex3f(.1,0,0);
    glVertex3f(.1,.1,0);
    glVertex3f(.15,.05,-.1);
    glVertex3f(.15,.15,-.1);
    glEnd();

    // green panel
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.f,1.f,0.f);
    glVertex3f(.15,.15,-.1);
    glVertex3f(.05,.15,-.1);
    glVertex3f(.1, .1, 0);
    glVertex3f(0,.1,0);
    glEnd();
    
    // white panel
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.f,1.f,1.f);
    glVertex3f(.15,.05,-.1);
    glVertex3f(.05,.05,-.1);
    glVertex3f(.1,0,0);
    glVertex3f(0,0,0);
    glEnd();

    // yellow panel
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.f,1.f,0.f);
    glVertex3f(0,.1,0);
    glVertex3f(0,0,0);
    glVertex3f(.05,.15,-.1);
    glVertex3f(.05,.05,-.1);
    glEnd();
}

void display( void ) {
    glClear( GL_COLOR_BUFFER_BIT );
    drawAll();
    drawCube();
    glutSwapBuffers();
}

void reshape( int w, int h ) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Question 3: What do the calls to glOrtho()
    // and glViewport() accomplish?
    
    // glOrtho creates an orthogonic projection
    // of the drawing space into the specified
    // dims (here, [0, 1] for x/y and [-1,1] for z)
    
    glOrtho(0., 1., 0., 1., -5., 5.);
    
    // glViewport resizes the viewport such that it
    // matches the new projection
    
    glViewport(0, 0, w, h);
    
    win_width = w;
    win_height = h;
}

void keyboard(unsigned char key, int x, int y) {
    printf("%c", key);
    bool no_key = false;
    switch(key) {
        case 'c':
            mode = SPHERE;
            break;
        case 'w':
            mode = WAVE;
            break;
        case 'p':
            mode = PULSE;
            break;
        case 'h':
            mode = GRAPH;
            break;
        case '+':
            edge_prob += 0.005;
            dot_diam -= 0.001;
            break;
        case '-':
            edge_prob -= 0.005;
            dot_diam += 0.001;
            break;
        case 27: // Escape key
            exit(0);
            break;
        case 'j':
            translateLeft();
            break;
        case 'k':
            translateDown();
            break;
        case 'i':
            translateUp();
            break;
        case 'l':
            translateRight();
            break;
        default:
            printf("Defaulting\n");
            no_key = true;
            break;
    }
    if (!no_key) initializeParticles();
}

int main (int argc, char *argv[]) {
    
    glutInit( &argc, argv );
    
    // Question 2: What does the parameter to glutInitDisplayMode()
    // specify?
    
    // The "mode" parameter turns on or off flags that control the
    // display mode. GLUT_RGBA specifies that the window be rasterized
    // in RGBA mode, and GLUT_DOUBLE specifies a double-buffered window.
    
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE );
    glutInitWindowSize(win_width, win_height );
    
    glutCreateWindow("Intro Graphics Assignment 1" );
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    
    initializeParticles();

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glClearColor(0.85, 0.85, 0.85, 1.0);
    glutMainLoop();
    
    return 0;
}
