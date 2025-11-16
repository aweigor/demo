#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <vector>
#include <cmath>

struct Particle {
    float x, y, z;
    float vx, vy, vz;
    float r, g, b;
    float life;
};

std::vector<Particle> particles;

// Cylinder properties - positioned to intercept particle paths
float cylinderRadius = 0.4f;
float cylinderHeight = 2.0f;
float cylinderX = 1.5f;  // Moved to the side to be clearly visible
float cylinderY = 0.0f;
float cylinderZ = 0.0f;

void createParticle() {
    Particle p;
    p.x = p.y = p.z = 0;
    
    // Random direction - biased toward the cylinder's general direction
    float angle1 = (rand() % 180 - 90) * 3.14159f / 180.0f;  // -90 to +90 degrees
    float angle2 = (rand() % 120 - 60) * 3.14159f / 180.0f;  // -60 to +60 degrees
    
    p.vx = sin(angle1) * cos(angle2) * 0.1f;
    p.vy = sin(angle1) * sin(angle2) * 0.1f;
    p.vz = cos(angle1) * 0.1f;
    
    p.r = 0.2f; p.g = 0.4f; p.b = 1.0f;
    p.life = 100 + rand() % 100;
    
    particles.push_back(p);
}

void drawCylinder() {
    glColor3f(0.8f, 0.2f, 0.2f); // Red color for the cylinder
    
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    
    glPushMatrix();
    glTranslatef(cylinderX, cylinderY - cylinderHeight/2, cylinderZ);
    
    // Draw cylinder body
    gluCylinder(quadric, cylinderRadius, cylinderRadius, cylinderHeight, 32, 32);
    
    // Draw bottom cap
    glPushMatrix();
    glRotatef(180, 1, 0, 0);
    gluDisk(quadric, 0, cylinderRadius, 32, 32);
    glPopMatrix();
    
    // Draw top cap
    glTranslatef(0, 0, cylinderHeight);
    gluDisk(quadric, 0, cylinderRadius, 32, 32);
    
    glPopMatrix();
    
    gluDeleteQuadric(quadric);
}

void updateParticles() {
    for (auto& p : particles) {
        // Anti-attractor force from origin
        //float dist = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
        //if (dist > 0.1f) {
        //    float force = 0.1f / (dist * dist);
        //    p.vx += (p.x / dist) * force;
        //    p.vy += (p.y / dist) * force;
        //    p.vz += (p.z / dist) * force;
        //}
        
        // Anti-attractor force from cylinder - smooth and distance-based
        float dx = p.x - cylinderX;
        float dy = p.y - cylinderY;
        float dz = p.z - cylinderZ;
        
        // Check if particle is within cylinder's influence range
        float verticalDist = fabs(dy);
        float horizontalDist = sqrt(dx*dx + dz*dz);
        
        // Define influence range around cylinder
        float influenceRange = cylinderRadius * 3.0f;
        float verticalInfluence = cylinderHeight * 1.5f;
        
        if (verticalDist <= verticalInfluence && horizontalDist <= influenceRange) {
            // Calculate distance to cylinder surface (considering it's infinite cylinder for force calculation)
            float distToSurface = horizontalDist - cylinderRadius;
            
            if (distToSurface < influenceRange) {
                // Smooth force that increases as particle gets closer to cylinder surface
                float normalizedDist = distToSurface / influenceRange;
                float cylinderForce = 0.0f;
                
                if (normalizedDist <= 0.1f) {
                    // Very close - strong repulsion to prevent intersection
                    cylinderForce = 0.5f;
                } else {
                    // Smooth increasing force as particle approaches
                    cylinderForce = 0.15f * (1.0f - normalizedDist) * (1.0f - normalizedDist);
                }
                
                // Apply force away from cylinder axis
                if (horizontalDist > 0.001f) {
                    //p.vx += (dx / horizontalDist) * cylinderForce;
                    //p.vz += (dz / horizontalDist) * cylinderForce;
                }
                
                // Add slight vertical repulsion to keep particles away from cylinder ends
                if (verticalDist > cylinderHeight * 0.4f) {
                    float verticalForce = 0.02f * (verticalDist - cylinderHeight * 0.4f);
                    //p.vy += (p.y > cylinderY ? verticalForce : -verticalForce);
                }
            }
        }
        
        // Update position with increasing speed
        float speed = 1.0f + (100 - p.life) * 0.1f;
        p.x += p.vx * speed;
        p.y += p.vy * speed;
        p.z += p.vz * speed;
        
        // Update color based on life and proximity to cylinder
        float age = 1.0f - (p.life / 200.0f);
        
        // Make particles change color when near cylinder
        float dx2 = p.x - cylinderX;
        float dz2 = p.z - cylinderZ;
        float cylinderDist = sqrt(dx2*dx2 + dz2*dz2);
        float cylinderProximity = 1.0f - (cylinderDist / (cylinderRadius * 4.0f));
        cylinderProximity = fmax(0.0f, fmin(1.0f, cylinderProximity));
        
        p.r = 0.2f + age * 0.8f + cylinderProximity * 0.3f;
        p.g = 0.4f - age * 0.4f - cylinderProximity * 0.2f;
        p.b = 1.0f - age * 0.8f - cylinderProximity * 0.3f;
        
        p.life--;
    }
    
    // Remove dead particles
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return p.life <= 0; }),
        particles.end());
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(5, 3, 5, 0, 0, 0, 0, 1, 0);  // Adjusted camera to see cylinder better
    
    // Draw cylinder
    drawCylinder();
    
    // Draw particles
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
    // Create new particles
    if (particles.size() < 300) {
        createParticle();
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
