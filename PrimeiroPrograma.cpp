#include <windows.h> 
#include <GL/glew.h> 
#include <GLFW/glut.h> 
#include <iostream> 
#include <vector>
#include <random>

int width = 800;
int height = 600;

float playerX = width / 1.90;
float playerY = height / 2;

float machineX = width / 2.10;
float machineY = height / 2;

float playerSpeed = 2.0f;
float machineSpeed = 2.0f;

std::pair<float, float> playerDirection = { 1.0f, 0.0f };
std::pair<float, float> machineDirection = { -1.0f, 0.0f };

int machineFrameCount = 0;
int machineChangeDirectionFreq = 75;

std::vector<std::pair<float, float>> playerPath;
std::vector<std::pair<float, float>> machinePath;


void draw(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(0.0, 0.0, 1.0);
    glPointSize(5); 
    glBegin(GL_POINTS);
    for (const auto& point : playerPath) {
        glVertex2f(point.first, point.second);
    }
    glEnd();
    
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (const auto& point : machinePath) {
        glVertex2f(point.first, point.second);
    }
    glEnd();
    
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(0.5, 0.5, 1.0);
    glVertex2f(playerX, playerY);

    glColor3f(1.0, 0.5, 0.5);
    glVertex2f(machineX, machineY);
    glEnd();

    glutSwapBuffers();
}

void update(int value) {
   
    playerPath.push_back(std::make_pair(playerX, playerY));
    machinePath.push_back(std::make_pair(machineX, machineY));
    
    if (GetAsyncKeyState(VK_UP) & 0x8000) playerDirection = { 0.0f, 1.0f };
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) playerDirection = { 0.0f, -1.0f };
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) playerDirection = { -1.0f, 0.0f };
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) playerDirection = { 1.0f, 0.0f };
    playerX += playerSpeed * playerDirection.first;
    playerY += playerSpeed * playerDirection.second;
    
    if (playerX < 0 || playerX > width || playerY < 0 || playerY > height) {
        std::cout << "Voce atingiu as bordas da janela!" << std::endl;
        exit(0);
    }
    
    for (const auto& point : machinePath) {
        float dx = playerX - point.first;
        float dy = playerY - point.second;
        float distSquared = dx * dx + dy * dy;
        if (distSquared < 25.0f) { 
            std::cout << "Voce encostou no rastro da maquina!" << std::endl;
            exit(0);
        }
    }

    machineFrameCount++;
    if (machineFrameCount >= machineChangeDirectionFreq) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 3);
        int direction = dis(gen);
        switch (direction) {
        case 0: machineDirection = { 0.0f, 1.0f }; break;
        case 1: machineDirection = { 0.0f, -1.0f }; break;
        case 2: machineDirection = { -1.0f, 0.0f }; break;
        case 3: machineDirection = { 1.0f, 0.0f }; break;
        }
        machineFrameCount = 0;
    }
    machineX += machineSpeed * machineDirection.first;
    machineY += machineSpeed * machineDirection.second;

    if (machineX < 0 || machineX > width || machineY < 0 || machineY > height) {
        std::cout << "A maquina atingiu as bordas da janela!" << std::endl;
        exit(0);
    }
   
    for (const auto& point : playerPath) {
        float dx = machineX - point.first;
        float dy = machineY - point.second;
        float distSquared = dx * dx + dy * dy;
        if (distSquared < 25.0f) { 
            std::cout << "A maquina encostou no seu rastro!" << std::endl;
            exit(0);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Tron Game");
    glClearColor(0.0, 0.0, 0.0, 0.0);
    gluOrtho2D(0.0, width, 0.0, height);
    glutDisplayFunc(draw);
    update(0);
    glutMainLoop();
    return 0;
}