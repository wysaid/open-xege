/*
* cgeShaderFunctions.h
*
*  Created on: 2013-12-6
*      Author: wysaid
*/

#ifndef _CGESAHDERFUNCTIONS_H_
#define _CGESAHDERFUNCTIONS_H_

#include "cgeCommonDefine.h"
#include "cgeGLFunctions.h"

namespace CGE
{

	class ShaderObject
	{
	private:
		explicit ShaderObject(const ShaderObject&) {}
	public:
        ShaderObject();
		~ShaderObject();

		bool init(GLenum shaderType);

		bool loadShaderSourceFromString(const char* shaderString);
		inline GLuint shaderID() { return m_shaderID; }
		inline GLenum shaderType() { return m_shaderType; }
		void clear();
	private:

		GLenum m_shaderType;
		GLuint m_shaderID;
	};

	class ProgramObject
	{
	private:
		explicit ProgramObject(const ProgramObject&) {}
	public:
		ProgramObject();
		~ProgramObject();

		bool initWithShaderStrings(const char* vsh, const char* fsh);
		
		bool initFragmentShaderSourceFromString(const char* fragShader);
		bool initVertexShaderSourceFromString(const char* vertShader);

		bool linkWithShaderObject(ShaderObject& vertObj, ShaderObject& fragObj, bool shouldClear = true);
		inline bool link() { return linkWithShaderObject(m_vertObj, m_fragObj); }
		inline void bind() { glUseProgram(m_programID); }

		// For usage convenience, do not use template here.
		inline void sendUniformf(const char* name, GLfloat x)
		{
			GLint uniform = _getUniform(m_programID, name);
			glUniform1f(uniform, x);
		}

		inline void sendUniformf(const char* name, GLfloat x, GLfloat y)
		{
			GLint uniform = _getUniform(m_programID, name);
			glUniform2f(uniform, x, y);
		}

		inline void sendUniformf(const char* name, GLfloat x, GLfloat y, GLfloat z)
		{
			GLint uniform = _getUniform(m_programID, name);
			glUniform3f(uniform, x, y, z);
		}

		inline void sendUniformf(const char* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
		{
			GLint uniform = _getUniform(m_programID, name);
			glUniform4f(uniform, x, y, z, w);
		}

		inline void sendUniformi(const char* name, GLint x)
		{
			GLint uniform = _getUniform(m_programID, name);
			glUniform1i(uniform, x);
		}

		inline void sendUniformi(const char* name, GLint x, GLint y)
		{
			GLint uniform = _getUniform(m_programID, name);
			glUniform2i(uniform, x, y);
		}

		inline void sendUniformi(const char* name, GLint x, GLint y, GLint z)
		{
			GLint uniform = _getUniform(m_programID, name);
			glUniform3i(uniform, x, y, z);
		}

		inline void sendUniformi(const char* name, GLint x, GLint y, GLint z, GLint w)
		{
			GLint uniform = _getUniform(m_programID, name);
			glUniform4i(uniform, x, y, z, w);
		}

		inline void sendUniformMat2(const char* name, int count, GLboolean transpose, const GLfloat* matrix)
		{
			GLint uniform = _getUniform(m_programID, name);
			glUniformMatrix2fv(uniform, count, transpose, matrix);
		}

		inline void sendUniformMat3(const char* name, GLsizei count, GLboolean transpose, const GLfloat* matrix)
		{
			GLint uniform = _getUniform(m_programID, name);
			glUniformMatrix3fv(uniform, count, transpose, matrix);
		}

		inline void sendUniformMat4(const char* name, GLsizei count, GLboolean transpose, const GLfloat* matrix)
		{
			GLint uniform = _getUniform(m_programID, name);
			glUniformMatrix4fv(uniform, count, transpose, matrix);
		}

		inline GLuint programID() { return m_programID; }
		inline GLint attributeLocation(const char* name) { return glGetAttribLocation(m_programID, name); }
		inline GLint uniformLocation(const char* name) { return glGetUniformLocation(m_programID, name); } 
		inline void bindAttribLocation(const char* name, GLuint index) { glBindAttribLocation(m_programID, index, name); }

	protected:
		inline GLint _getUniform(GLuint programId, const char* name)
		{
			GLint uniform = glGetUniformLocation(programId, name);
			CGE_LOG_CODE(
				if(uniform < 0)
					CGE_LOG_ERROR("uniform name %s does not exist!\n", name);
			);
			return uniform;
		}

	private:
		ShaderObject m_vertObj, m_fragObj;
		GLuint m_programID;
	};

}

#endif
