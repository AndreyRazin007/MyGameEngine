#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

/* Coordinates of the points of the triangle */
GLfloat points[]{
	 0.0f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};

/* Color coordinates of the triangle in RGB */
GLfloat colors[]{
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

/* Notes for the vertex shaders */
const char* s_vertex_shader{
	"#version 460\n"
	"layout(location = 0) in vec3 vertex_position;"
	"layout(location = 1) in vec3 vertex_color;"
	"out vec3 color;"
	"void main() {"
	"   color = vertex_color;"
	"   gl_Position = vec4(vertex_position, 1.0);"
	"}"
};

/* Notes for the fragment shaders */
const char* s_fragment_shader{
	"#version 460\n"
	"in vec3 color;"
	"out vec4 frag_color;"
	"void main() {"
	"   frag_color = vec4(color, 1.0);"
	"}"
};

/* Main window sizes */
int g_window_size_x{ 900 };
int g_window_size_y{ 700 };

/* My function for handling the resizing of the main window */
void glfw_window_resize_callback(GLFWwindow* p_window, int width, int height)
{
	g_window_size_x = width;
	g_window_size_y = height;
	glViewport(0, 0, g_window_size_x, g_window_size_y);
}

/* Function for processing the closing of the main window */
void glfw_key_escape_callback(GLFWwindow* p_window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(p_window, GL_TRUE);
	}
}

int main()
{
	/* Checking for initialization */
	if (!glfwInit())
	{
		std::cerr << "glfwInit() failed!\n";
		exit(-1);
	}

	/* Specifies the OpenGL version and specification */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Creation of the main window */
	GLFWwindow* p_window = glfwCreateWindow(g_window_size_x, g_window_size_y, "MyGame", nullptr, nullptr);
	if (!p_window)
	{
		std::cerr << "glfwCreateWindow() failed!\n";
		glfwTerminate();
		exit(-1);
	}

	glfwSetWindowSizeCallback(p_window, glfw_window_resize_callback);
	glfwSetKeyCallback(p_window, glfw_key_escape_callback);

	/* Specifying the current OpenGL context */
	glfwMakeContextCurrent(p_window);

	/* Checking for download glad */
	if (!gladLoadGL())
	{
		std::cerr << "Can't load GLAD!" << std::endl;
	}

	/* Displaying the video card and OpenGL version in the console */
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	/* Background color of the main window */
	glClearColor(1, 1, 1, 1);

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &s_vertex_shader, nullptr);
	glCompileShader(vertex_shader);

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &s_fragment_shader, nullptr);
	glCompileShader(fragment_shader);

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	GLuint points_vbo{ 0 };
	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLuint colors_vbo{ 0 };
	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	GLuint vao{ 0 };
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	/* Main rendering cycle */
	while (!glfwWindowShouldClose(p_window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		/* Shaders rendering */
		glUseProgram(shader_program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(p_window);

		glfwPollEvents();
	}

	/* Cleaning up resources and closing OpenGL */
	glfwTerminate();

	exit(0);
}