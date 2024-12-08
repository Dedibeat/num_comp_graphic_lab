#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <ctime>

// Window size
const int WIDTH = 800;
const int HEIGHT = 600;

// Snake parameters
struct Segment {
    float x, y;
};
std::vector<Segment> snake;
int snakeLength = 20;       // initial length of snake segments
float snakeSpeed = 3.0f;    // speed of snake head movement
float segmentSpacing = 5.0f; // distance between snake segments

// Mouse position
int mouseX = WIDTH / 2;
int mouseY = HEIGHT / 2;

// Food
struct Food {
    float x, y;
    bool eaten;
};
std::vector<Food> foods;
int maxFood = 10;
float foodRadius = 5.0f;

// Utility functions
float distanceBetween(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

void placeFoods() {
    foods.clear();
    for (int i = 0; i < maxFood; i++) {
        Food f;
        f.x = rand() % WIDTH;
        f.y = rand() % HEIGHT;
        f.eaten = false;
        foods.push_back(f);
    }
}

// Initialize snake
void initSnake() {
    snake.clear();
    // Place snake in middle
    for (int i = 0; i < snakeLength; i++) {
        Segment s;
        s.x = WIDTH / 2.0f;
        s.y = HEIGHT / 2.0f - i * segmentSpacing;
        snake.push_back(s);
    }
}

// Draw a filled circle
void drawCircle(float cx, float cy, float r) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 32; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / 32.0f;
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// Update snake position
void updateSnake() {
    // Move head towards mouse
    float headX = snake[0].x;
    float headY = snake[0].y;

    float dx = mouseX - headX;
    float dy = mouseY - headY;
    float dist = std::sqrt(dx*dx + dy*dy);
    if (dist > 0.1f) {
        float vx = (dx / dist) * snakeSpeed;
        float vy = (dy / dist) * snakeSpeed;
        // Move head
        snake[0].x += vx;
        snake[0].y += vy;
    }

    // Move other segments to follow the head
    for (int i = 1; i < (int)snake.size(); i++) {
        float prevX = snake[i-1].x;
        float prevY = snake[i-1].y;
        float curX = snake[i].x;
        float curY = snake[i].y;
        float distSeg = distanceBetween(prevX, prevY, curX, curY);
        if (distSeg > segmentSpacing) {
            float angle = atan2(prevY - curY, prevX - curX);
            snake[i].x = prevX - cos(angle) * segmentSpacing;
            snake[i].y = prevY - sin(angle) * segmentSpacing;
        }
    }

    // Check for food collision
    float headRadius = 5.0f;
    for (auto &f : foods) {
        if (!f.eaten) {
            float d = distanceBetween(snake[0].x, snake[0].y, f.x, f.y);
            if (d < headRadius + foodRadius) {
                f.eaten = true;
                // Grow snake
                for (int i = 0; i < 5; i++) { // add 5 segments per food
                    Segment s = snake.back();
                    snake.push_back(s);
                }
            }
        }
    }
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw snake
    glColor3f(0.0f, 1.0f, 0.0f);
    for (auto &seg : snake) {
        drawCircle(seg.x, seg.y, 5.0f);
    }

    // Draw food
    for (auto &f : foods) {
        if (!f.eaten) {
            glColor3f(1.0f, 0.0f, 0.0f);
            drawCircle(f.x, f.y, foodRadius);
        }
    }

    glutSwapBuffers();
}

// Passive mouse motion
void passiveMotion(int x, int y) {
    mouseX = x;
    mouseY = HEIGHT - y; // flip y since OpenGL's origin is bottom-left
}

// Timer for updates
void timer(int value) {
    updateSnake();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Main
int main(int argc, char** argv) {
    srand((unsigned int)time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Single Snake (Slither.io-like) Demo");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    initSnake();
    placeFoods();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(passiveMotion);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
