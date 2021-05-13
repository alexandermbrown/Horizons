#include "lipch.h"
#include "Lithium/Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include "glad/glad.h"

#include "glm/gtc/type_ptr.hpp"

#include "OpenGLCore.h"

namespace Li 
{
	OpenGLShader::OpenGLShader(const std::string& name, const GLSLInput& input)
		: m_Name(name), m_RendererID(0)
	{
		std::unordered_map<GLenum, const char*> sources;
		if (input.VertexSrc)
			sources[GL_VERTEX_SHADER] = input.VertexSrc;
		if (input.TessControlSrc)
			sources[GL_TESS_CONTROL_SHADER] = input.TessControlSrc;
		if (input.TessEvalSrc)
			sources[GL_TESS_EVALUATION_SHADER] = input.TessEvalSrc;
		if (input.GeometrySrc)
			sources[GL_GEOMETRY_SHADER] = input.GeometrySrc;
		if (input.FragmentSrc)
			sources[GL_FRAGMENT_SHADER] = input.FragmentSrc;
		if (input.ComputeSrc)
			sources[GL_COMPUTE_SHADER] = input.ComputeSrc;

		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		GLCall( glDeleteProgram(m_RendererID) );
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, const char*>& shader_sources)
	{
		GLuint program = glCreateProgram();
		std::vector<GLenum> shader_ids;
		shader_ids.reserve(shader_sources.size());

		for (auto& kv : shader_sources)
		{
			GLuint shader_id = glCreateShader(kv.first);
			glShaderSource(shader_id, 1, &kv.second, 0);
			glCompileShader(shader_id);

			GLint is_compiled = 0;
			glGetShaderiv(shader_id, GL_COMPILE_STATUS, &is_compiled);
			if (is_compiled == GL_FALSE)
			{
				GLint max_length = 0;
				glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &max_length);

				std::vector<GLchar> info_log(max_length);
				glGetShaderInfoLog(shader_id, max_length, &max_length, &info_log[0]);

				glDeleteShader(shader_id);

				Log::CoreError("{}: {}", m_Name, info_log.data());
				LI_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader_id);
			shader_ids.push_back(shader_id);
		}
		
		glLinkProgram(program);

		GLint is_linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
		if (is_linked == GL_FALSE)
		{
			GLint max_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

			std::vector<GLchar> info_log(max_length);
			glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);

			glDeleteProgram(program);
			for (GLenum id : shader_ids)
				glDeleteShader(id);

			Log::CoreError("{}", info_log.data());
			LI_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (GLenum id : shader_ids)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
		m_RendererID = program;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}
