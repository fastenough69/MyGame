#pragma once

#include <string>
#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Render{
	class ProgramShader {
		bool is_Compiled{ false };
		GLuint m_id;
		bool createShader(const std::string& src, const GLenum shaderType, GLuint& shader_id);

	public:
		ProgramShader(const std::string& ver_shader, const std::string& fr_shader);
		~ProgramShader();

		bool get_compile() const { return is_Compiled; };
		bool usage() const;
		void setInt(const std::string& name, const GLint value);
        void setMat4(const std::string &name, glm::mat4 matrix);
		GLuint get_id() const;

		ProgramShader() = delete;
		ProgramShader(ProgramShader&) = delete;
		ProgramShader& operator=(const ProgramShader&) = delete;

		ProgramShader(ProgramShader&& right) noexcept;
		ProgramShader& operator=(ProgramShader&& right) noexcept;

	};
}
