#define GLEW_STATIC // Criar executável estático com GLEW para windows
#define GLFW_INCLUDE_NONE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>


int framecount = 0; // variável global
GLfloat xc = 0.0;
GLfloat yc = 0.0;

struct vertex
{
    GLfloat x, y, r, g, b;
};

using namespace std;

string readFile(string filename)
{
    ifstream t(filename);
    stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

void timer()
{
    while (true)
    {
        cout << framecount << endl;
        framecount = 0;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_LEFT) xc = xc - 0.01;
    if (key == GLFW_KEY_RIGHT) xc = xc + 0.01;
    if (key == GLFW_KEY_UP) yc = yc + 0.01;
    if (key == GLFW_KEY_DOWN) yc = yc - 0.01;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glewInit();

    /** VBO **/
    vertex v[] =
    {
        { 0.0f, 0.0f, 1.0f, 1.0f, 1.0f} ,
// Top
        { -0.2f, 0.8f, 0.0f, 1.0f, 0.0f} ,
        { 0.2f, 0.8f, 0.0f, 0.0f, 1.0f} ,
        { 0.0f, 0.8f, 0.0f, 1.0f, 1.0f} ,
        { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f} ,
// Bottom
        { -0.2f, -0.8f, 0.0f, 0.0f, 1.0f} ,
        { 0.2f, -0.8f, 0.0f, 1.0f, 0.0f} ,
        { 0.0f, -0.8f, 0.0f, 1.0f, 1.0f} ,
        { 0.0f, -1.0f, 1.0f, 0.0f, 0.0f} ,
// Left
        { -0.8f, -0.2f, 0.0f, 1.0f, 0.0f} ,
        { -0.8f, 0.2f, 0.0f, 0.0f, 1.0f} ,
        { -0.8f, 0.0f, 0.0f, 1.0f, 1.0f} ,
        { -1.0f, 0.0f, 1.0f, 0.0f, 0.0f} ,
// Right
        { 0.8f, -0.2f, 0.0f, 0.0f, 1.0f} ,
        { 0.8f, 0.2f, 0.0f, 1.0f, 0.0f} ,
        { 0.8f, 0.0f, 0.0f, 1.0f, 1.0f} ,
        { 1.0f, 0.0f, 1.0f, 0.0f, 0.0f}
    };
    GLuint VBO1;
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
    /********************/

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)8);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glVertexPointer(2, GL_FLOAT, 0, nullptr);
    glEnableClientState(GL_VERTEX_ARRAY);


    /****** Index Buffer Object ************/

    unsigned short ind[] =
    {
// Top
        0, 1, 3,
        0, 3, 2,
        3, 1, 4,
        3, 4, 2,
// Bottom
        0, 5, 7,
        0, 7, 6,
        7, 5, 8,
        7, 8, 6,
// Left
        0, 9, 11,
        0, 11, 10,
        11, 9, 12,
        11, 12, 10,
// Right
        0, 13, 15,
        0, 15, 14,
        15, 13, 16,
        15, 16, 14
    };

    GLuint myIBO;
    glGenBuffers(1, &myIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

    /*************************************/

    /* Shaders */
    string vss = readFile("vertex.vert");
    string fss = readFile("fragment.vert");
    const char *vertexShaderText = vss.c_str();
    const char *fragmentShaderText = fss.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderText, nullptr);
    glCompileShader(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderText, nullptr);
    glCompileShader(fragmentShader);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    thread t1(timer);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        /* Código de renderização OpenGL vai aqui */
        glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_SHORT, (GLvoid*)0);
        /* Troca o buffer de fundo com o buffer de exibição */
        glfwSwapBuffers(window);

        framecount++;
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    t1.join();
    return 0;
}
