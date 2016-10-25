/*
* cgeShaderFunctions.cpp
*
*  Created on: 2013-12-5
*      Author: wysaid
*        Mail: admin@wysaid.org
*/

#include "cgeCommonDefine.h"
#include "cgeGLFunctions.h"
#include "cgeShaderFunctions.h"

CGE_UNEXPECTED_ERR_MSG
(
 static int sProgramCount = 0;
 )

namespace CGE
{
	ShaderObject::ShaderObject() : m_shaderType(GL_FALSE), m_shaderID(0) {}
	ShaderObject::~ShaderObject() { clear(); }

	bool ShaderObject::init(GLenum shaderType)
	{
		m_shaderType = shaderType;
		if(m_shaderID == 0)
			m_shaderID = glCreateShader(m_shaderType);
		return m_shaderID != 0;
	}

	void ShaderObject::clear()
	{
		if(m_shaderID == 0) return;
		glDeleteShader(m_shaderID);
		m_shaderID = 0;
		m_shaderType = GL_FALSE;
	}

	bool ShaderObject::loadShaderSourceFromString(const char* shaderString)
	{
		if(m_shaderID == 0)
		{
			m_shaderID = glCreateShader(m_shaderType);
			CGE_LOG_CODE(
			if(m_shaderID == 0) 
			{
				CGE_LOG_ERROR("glCreateShader Failed!");
				return false;
			})
		}
		glShaderSource(m_shaderID, 1, (const GLchar**)&shaderString, nullptr);
		glCompileShader(m_shaderID);
		GLint compiled = 0;
		glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &compiled);

		if(compiled == GL_TRUE) return true;

		CGE_LOG_CODE(
		GLint logLen;
		glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &logLen);
		if(logLen > 0)
		{
			char *buf = new char[logLen];
			if(buf != nullptr)
			{
				glGetShaderInfoLog(m_shaderID, logLen, &logLen, buf);
				CGE_LOG_ERROR("Shader %d compile faild: \n%s\n", m_shaderID, buf);
				delete[] buf;
			}
		})
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	ProgramObject::ProgramObject()
	{
		m_programID = glCreateProgram();
        
        CGE_UNEXPECTED_ERR_MSG
        (
         CGE_LOG_KEEP("ProgramObject create, total: %d\n", ++sProgramCount);
         )
	}

	ProgramObject::~ProgramObject()
	{
        CGE_UNEXPECTED_ERR_MSG
        (
         CGE_LOG_KEEP("ProgramObject release, remain: %d\n", --sProgramCount);
         )
        
		if(m_programID == 0)
			return;
		GLuint attachedShaders[32];
		int numAttachedShaders = 0;
		glGetAttachedShaders(m_programID, 32, &numAttachedShaders, attachedShaders);
		for(int i = 0; i < numAttachedShaders; ++i)
		{
			glDetachShader(m_programID, attachedShaders[i]);
		}
		glDeleteProgram(m_programID);
	}

	bool ProgramObject::initFragmentShaderSourceFromString(const char* fragShader)
	{
		return m_fragObj.init(GL_FRAGMENT_SHADER) && m_fragObj.loadShaderSourceFromString(fragShader);
	}

	bool ProgramObject::initVertexShaderSourceFromString(const char* vertShader)
	{
		return m_vertObj.init(GL_VERTEX_SHADER) && m_vertObj.loadShaderSourceFromString(vertShader);
	}

	bool ProgramObject::initWithShaderStrings(const char* vsh, const char* fsh)
	{
		return initVertexShaderSourceFromString(vsh) && initFragmentShaderSourceFromString(fsh) && link();
	}

	bool ProgramObject::linkWithShaderObject(ShaderObject& vertObj, ShaderObject& fragObj, bool shouldClear)
	{
		if(m_programID != 0)
		{
			GLuint attachedShaders[32];
			int numAttachedShaders;
			glGetAttachedShaders(m_programID, 32, &numAttachedShaders, attachedShaders);
			for(int i = 0; i < numAttachedShaders; ++i)
			{
				glDetachShader(m_programID, attachedShaders[i]);
			}
			cgeCheckGLError("Detach Shaders in useProgram");
		}
		else
		{
			m_programID = glCreateProgram();
		}
		GLint programStatus;
		glAttachShader(m_programID, vertObj.shaderID());
		glAttachShader(m_programID, fragObj.shaderID());
		cgeCheckGLError("Attach Shaders in useProgram");
		glLinkProgram(m_programID);
		glGetProgramiv(m_programID, GL_LINK_STATUS, &programStatus);

		if(shouldClear)
		{
			m_vertObj.clear();
			m_fragObj.clear();
		}

		if(programStatus != GL_TRUE)
		{
			GLint logLen = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &logLen);
			if(logLen != 0)
			{
				char *buf = new char[logLen];
				if(buf != nullptr)
				{
					glGetProgramInfoLog(m_programID, logLen, &logLen, buf);
					CGE_LOG_ERROR("Failed to link the program!\n%s", buf);
					delete[] buf;
				}
			}
			CGE_LOG_ERROR("LINK %d Failed\n", m_programID);
			return false;
		}
		cgeCheckGLError("Link Program");
		return true;
	}

}