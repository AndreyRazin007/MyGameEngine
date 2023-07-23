#include "ShaderProgram.hpp"

#include <iostream>
#include <string>

namespace Renderer
{
	ShaderProgram::ShaderProgram(const std::string& vertex_shader, const std::string& fragment_shader)
	{
		GLuint vertex_shader_id{};
		if (!create_shader(vertex_shader, GL_VERTEX_SHADER, vertex_shader_id))
		{
			std::cerr << "VERTEX SHADER compile time error\n";
			exit(-1);
		}

		GLuint fragment_shader_id{};
		if (!create_shader(fragment_shader, GL_FRAGMENT_SHADER, fragment_shader_id))
		{
			std::cerr << "FRAGMENT SHADER compile time error\n";
			glDeleteShader(vertex_shader_id);

			exit(-1);
		}

		m_id = glCreateProgram();
		glAttachShader(m_id, vertex_shader_id);
		glAttachShader(m_id, fragment_shader_id);
		glLinkProgram(m_id);

		GLint success{};
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar info_log[1024]{};
			glGetShaderInfoLog(m_id, 1024, nullptr, info_log);
			std::cerr << "ERROR::SHADER: Link time error:\n" << info_log << '\n';
		}
		else
		{
			m_is_compiled = true;
		}

		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);
	}

	bool ShaderProgram::create_shader(const std::string& source, const GLenum shader_type, GLuint& shader_id)
	{
		shader_id = glCreateShader(shader_type);
		const char* code = source.c_str();
		glShaderSource(shader_id, 1, &code, nullptr);
		glCompileShader(shader_id);

		GLint success{};
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			GLchar info_log[1024]{};
			glGetShaderInfoLog(shader_id, 1024, nullptr, info_log);
			std::cerr << "ERROR::SHADER: Compile time error:\n" << info_log << '\n';

			return false;
		}

		return true;
	}

	void ShaderProgram::use() const
	{
		glUseProgram(m_id);
	}

	ShaderProgram& ShaderProgram::operator = (ShaderProgram&& shader_program) noexcept
	{
		glDeleteProgram(m_id);
		m_id = shader_program.m_id;
		m_is_compiled = shader_program.m_is_compiled;

		shader_program.m_id = 0;
		shader_program.m_is_compiled = false;

		return *this;
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shader_program) noexcept
	{
		glDeleteProgram(m_id);
		m_id = shader_program.m_id;
		m_is_compiled = shader_program.m_is_compiled;

		shader_program.m_id = 0;
		shader_program.m_is_compiled = false;
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_id);
	}
}
