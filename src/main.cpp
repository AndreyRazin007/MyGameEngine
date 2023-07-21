#include <glad/glad.h>
#include <GLFW/glfw3.h>

/* Подключение постоянных значений для OpenGL */
#include "constants.hpp"

#include <iostream>

/* Функция для обработки изменения размера окна */
void glfw_window_size_callback(GLFWwindow *window, int width, int height)
{
    g_window_size_x = width;
    g_window_size_y = height;

    glViewport(0, 0, g_window_size_x, g_window_size_y);
}

/* Функция для обработки закрытия окна */
void glfw_key_callback(GLFWwindow* p_window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(p_window, GL_TRUE);
    }
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "glfwInit() failed!\n";
        exit(-1);
    }

    /* Указание используемой версии OpenGL */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    /* Указании спецификации OpenGL */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *p_window = glfwCreateWindow(g_window_size_x, g_window_size_y,
        "MyGame", nullptr, nullptr);

    if (!p_window)
    {
        std::cerr << "glfwCreateWindow() failed!\n";
        glfwTerminate();
        exit(-1);
    }

    /* Изменение размера окна */
    glfwSetWindowSizeCallback(p_window, glfw_window_size_callback);

    /* Закрытие окна */
    glfwSetKeyCallback(p_window, glfw_key_callback);

    glfwMakeContextCurrent(p_window);

    if (!gladLoadGL())
    {
        std::cerr << "Can't load GLAD!" << '\n';
        exit(-1);
    }

    /* Цвет заливки окна */
    glClearColor(1, 0, 0, 1);

    /* Главный цикл обработки изображения */
    while (!glfwWindowShouldClose(p_window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(p_window);

        glfwPollEvents();
    }

    /* Очистка ресурсов и уничтожение приложения OpenGL */
    glfwTerminate();
    return 0;
}
