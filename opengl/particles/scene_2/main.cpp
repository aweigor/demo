#define GL_SILENCE_DEPRECATION

#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <vector>
#include <cmath>

#include <iostream>

struct Particle {
    float x, y, z;
    float vx, vy, vz;
    float r, g, b;
    float life;
};

std::vector<Particle> particles;

float cylinderRadius = 0.4f;
float cylinderHeight = 2.0f;
float cylinderX = 1.5f;
float cylinderY = 0.0f;
float cylinderZ = 0.0f;

void drawAxes() {
    glBegin(GL_LINES);
    
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    
    glEnd();
}

void emitParticle() {
    Particle p;
    
    
    float particle_pos[3] = {0, cylinderHeight / 2, -10};
    float cylinder_pos[3] = {0, cylinderHeight / 2, 0};

    float direction[3] = {
        cylinder_pos[0] - particle_pos[0],
        cylinder_pos[1] - particle_pos[1],
        cylinder_pos[2] - particle_pos[2]
    };

    float length = sqrt(direction[0]*direction[0] +
                       direction[1]*direction[1] +
                       direction[2]*direction[2]);
                       
    if(length > 0) {
        direction[0] /= length;
        direction[1] /= length;
        direction[2] /= length;
    }
    
    float spread = 0.3f;
    direction[0] += ((rand() % 100) / 100.0f - 0.5f) * spread;
    direction[1] += ((rand() % 100) / 100.0f - 0.5f) * spread;
    direction[2] += ((rand() % 100) / 100.0f - 0.5f) * spread;
    
    length = sqrt(direction[0]*direction[0] + direction[1]*direction[1] + direction[2]*direction[2]);
    if(length > 0) {
        direction[0] /= length;
        direction[1] /= length;
        direction[2] /= length;
    }
    
    p.x = particle_pos[0];
    p.y = particle_pos[1];
    p.z = particle_pos[2];
    
    p.vx = direction[0];
    p.vy = direction[1];
    p.vz = direction[2];
    
    p.r = 0.2f; p.g = 0.4f; p.b = 1.0f;
    p.life = 100;
    
    particles.push_back(p);
}

void updateParticles()
{
    for (auto& p : particles)
    {
        p.x += p.vx;
        p.y += p.vy;
        p.z += p.vz;
        
        float distToCylinderCenter = sqrt(p.x * p.x + p.z * p.z); // Distance in XZ plane
        float cylinderRadiusWithMargin = cylinderRadius + 0.2f; // Add some margin
        
        if (distToCylinderCenter < cylinderRadiusWithMargin &&
            p.y >= 0 && p.y <= cylinderHeight) {
            
            float pushForce = 1.0f;
            float pushX = p.x / distToCylinderCenter * pushForce;
            float pushZ = p.z / distToCylinderCenter * pushForce;
            
            p.x = pushX * cylinderRadiusWithMargin;
            p.z = pushZ * cylinderRadiusWithMargin;
            
            float tangentX = -p.z / distToCylinderCenter;
            float tangentZ = p.x / distToCylinderCenter;
            
            p.vx = tangentX * 0.1f + p.vx * 0.3f;
            p.vz = tangentZ * 0.1f + p.vz * 0.3f;
        }
        
        p.life--;
    }
    
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return p.life <= 0; }),
        particles.end());
}

void drawCylinder() {
    glColor3f(0.0f, 1.0f, 0.0f);
    
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    
    glPushMatrix();
    glTranslatef(0, cylinderHeight, 0);
    glRotatef(90, 1, 0, 0);
    
    gluCylinder(quadric, cylinderRadius, cylinderRadius, cylinderHeight, 32, 32);
    
    glPushMatrix();
    glRotatef(180, 1, 0, 0);
    gluDisk(quadric, 0, cylinderRadius, 32, 32);
    glPopMatrix();
    
    glTranslatef(0, 0, cylinderHeight);
    gluDisk(quadric, 0, cylinderRadius, 32, 32);
    
    glPopMatrix();
    
    gluDeleteQuadric(quadric);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 5, 10, 0, 0, 0, 0, 1, 0);
    
    drawCylinder();
    drawAxes();
    
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (const auto& p : particles) {
        glColor3f(p.r, p.g, p.b);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
    
    glutSwapBuffers();
}

void timer(int) {
    if (particles.size() < 300) {
        emitParticle();
    }
    
    updateParticles();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Particle System with Anti-Attractor Cylinder");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    
    glutMainLoop();
    return 0;
}
