#pragma once

#include <glad/glad.h>

#include <string>

namespace Renderer
{
	class ShaderProgram
	{
	private:
	    /* Create shaders */
		bool create_shader(const std::string& source, const GLenum shader_type, GLuint& shader_id);

		bool m_is_compiled{false};
		GLuint m_id{0};

	public:
		ShaderProgram(const std::string& vertex_shader, const std::string& fragment_shader);
		~ShaderProgram();

        /* Use shaders */
		void use() const;

        /* Compiling shaders */
		bool is_compiled() const
		{
			return m_is_compiled;
		}

		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram& shader_program) = delete;
		ShaderProgram& operator = (const ShaderProgram& shader_program) = delete;

		ShaderProgram& operator = (ShaderProgram&& shader_program) noexcept;
		ShaderProgram(ShaderProgram&& shader_program) noexcept;
	};
}
