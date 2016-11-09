/*
* xege.cpp
*
*  Created on: 2016-10-26
*      Author: wysaid
*        Mail: admin@wysaid.org
*/

#include "common/cgeShaderFunctions.h"
#include "common/cgeTextureUtils.h"
#include "algorithm/cgeMat.h"

#include "xege.h"
#include "XEGEGeometryDrawer.h"

#include "glew/glew.h"
#include "glfw/glfw3.h"
#include "glfw/glfw3native.h"

using namespace CGE;

#ifndef XEGE_SHOW_FPS
#define XEGE_SHOW_FPS 1
#endif

namespace xege
{
	//单例
	class XEGEWrapper
	{
		XEGEWrapper(int w, int h)
		{
			char title[128];
			sprintf(title, "xege %s", cgeGetVersion());

			if (!glfwInit())
				return;

			m_window = glfwCreateWindow(w, h, title, nullptr, nullptr);

			if (m_window != nullptr)
			{
				glfwMakeContextCurrent(m_window);

				if (glewInit() != GLEW_OK ||
					(m_drawer = TextureDrawer::create()) == nullptr)
				{
					glfwDestroyWindow(m_window);
					m_window = nullptr;
					return;
				}

				//上下翻转, 使用屏幕坐标
				m_drawer->setFlipScale(1.0f, -1.0f);
				m_width = w;
				m_height = h;

				glfwSetFramebufferSizeCallback(m_window, resizeCallback);
				glfwSetCursorPosCallback(m_window, cursorPosCallback);
				glfwSetMouseButtonCallback(m_window, mouseButtonCallback);

				m_cacheTexture = cgeGenTextureWithBuffer(nullptr, w, h, GL_RGBA, GL_UNSIGNED_BYTE);
				m_framebuffer = new FrameBuffer;
				m_framebuffer->bindTexture2D(m_cacheTexture);

				m_mvp = Mat4::makeOrtho(0.0f, (float)w, 0.0f, (float)h, -1.0f, 1.0f);
				XEGEGeometryDrawer::setMVP(m_mvp);

#ifdef _MSC_VER
				m_hwnd = glfwGetWin32Window(m_window);
#endif // _MSC_VER
			}
		}

	public:

		~XEGEWrapper()
		{
			if (m_window != nullptr)
			{
				glDeleteTextures(1, &m_cacheTexture);
				delete m_framebuffer;
				cgeCleanup();
				glfwDestroyWindow(m_window);
				glfwTerminate();
			}
		}

		void resize(int w, int h)
		{
			m_width = w;
			m_height = h;
			cgeSetGlobalViewSize(w, h);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_cacheTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			m_mvp = Mat4::makeOrtho(0.0f, (float)w, 0.0f, (float)h, -1.0f, 1.0f);
			XEGEGeometryDrawer::setMVP(m_mvp);
		}

		//////////////////////////////////////////////////////////////////////////

		static void resizeCallback(GLFWwindow* window, int w, int h)
		{
			if (s_wrapper != nullptr)
			{
				s_wrapper->resize(w, h);
			}
		}

		static void keyCharCallback(GLFWwindow* window, unsigned int character)
		{

		}

		static void cursorPosCallback(GLFWwindow* window, double x, double y)
		{

		}

		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mode)
		{
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			switch (action)
			{
			case GLFW_PRESS:
				//g_scene.mousePress(x, y);
				break;
			case GLFW_RELEASE:
				//g_scene.mouseRelease(x, y);
				break;
			default:
				break;
			}
		}

		static inline bool init(int w, int h, int flag)
		{
			if (s_wrapper == nullptr)
			{
				s_wrapper = new XEGEWrapper(w, h);
				if (s_wrapper->m_window != nullptr)
				{
					cgeInitialize(w, h, flag);
					cgePrintGLInfo();
				}
				else
				{
					delete s_wrapper;
					s_wrapper = nullptr;
				}
			}
			else
			{
				s_wrapper->resize(w, h);
			}

			return s_wrapper != nullptr;
		}

		static inline void release()
		{
			XEGEGeometryDrawer::cleanup();
			delete s_wrapper;
			s_wrapper = nullptr;
		}

		static inline XEGEWrapper* wrapper()
		{
			if (s_wrapper == nullptr)
			{
				CGE_LOG_ERROR("You should call initgraph(int, int) first!");
				init(640, 480, 0); //出现错误默认初始化640x480的窗口
			}

			return s_wrapper;
		}

		static inline GLFWwindow* window()
		{
			return s_wrapper == nullptr ? nullptr : s_wrapper->m_window;
		}

		static inline void bindFrameBuffer()
		{
			if (s_wrapper != nullptr)
			{
				s_wrapper->m_framebuffer->bind();
				glViewport(0, 0, s_wrapper->m_width, s_wrapper->m_height);
			}
		}

		static inline void render()
		{
			if (s_wrapper != nullptr)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glViewport(0, 0, s_wrapper->m_width, s_wrapper->m_height);
				s_wrapper->m_drawer->drawTexture(s_wrapper->m_cacheTexture);
			}
		}

	private:
		static XEGEWrapper* s_wrapper;

	protected:
		GLFWwindow* m_window;
		FrameBuffer* m_framebuffer;
		TextureDrawer* m_drawer;
		GLuint m_cacheTexture;
		GLint m_width, m_height;

		Mat4 m_mvp;

#ifdef _MSC_VER
		HWND m_hwnd;
#endif // _MSC_VER

	};

	XEGEWrapper* XEGEWrapper::s_wrapper = nullptr;

	static void errorCallback(int error, const char* description)
	{
		CGE_LOG_ERROR(description);
	}

	//////////////////////////////////////////////////////////////////////////

	bool initgraph(int width, int height, int flag)
	{
		return XEGEWrapper::init(width, height, flag);;
	}

	void closegraph()
	{
		XEGEWrapper::release();
	}

	void setClearColor(int color)
	{
		glClearColor(GET_R(color), GET_G(color), GET_B(color), GET_A(color));
	}

	void clearDevice()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void bindDefaultBuffer()
	{
		XEGEWrapper::bindFrameBuffer();
	}

	bool isRunning()
	{
		return XEGEWrapper::window() == nullptr ? false : !glfwWindowShouldClose(XEGEWrapper::window());
	}

	void setCaption(const char* title)
	{
		if(XEGEWrapper::window())
			glfwSetWindowTitle(XEGEWrapper::window(), title);
	}

	static double s_duringTime = 0.0;

	void runLoop(double fps, const std::function<void()>& func)
	{
		if (XEGEWrapper::window() == nullptr)
		{
			XEGEWrapper::init(640, 480, 0xffffffff);
		}

		double expectFrameTime = 1.0 / fps;
		double lastFrameTime = glfwGetTime();
		double currentFrameTime = 0.0;

#if XEGE_SHOW_FPS
		double frames = 0.0, accTime = 0.0;
#endif

		while (isRunning())
		{
			XEGEWrapper::bindFrameBuffer();
			func();
			XEGEWrapper::render();
			glfwSwapBuffers(XEGEWrapper::window());
			glfwPollEvents();

			cgeCheckGLError("loopEnd");

			currentFrameTime = glfwGetTime();
			s_duringTime = currentFrameTime - lastFrameTime;
			lastFrameTime = currentFrameTime;

#if XEGE_SHOW_FPS
			accTime += s_duringTime;
			++frames;
			if (accTime > 1.0)
			{
				double fpsCount = frames / accTime;
				CGE_LOG_INFO("fps: %g\n", fpsCount);
				accTime = accTime - floor(accTime);
				frames = accTime * fpsCount;
			}
#endif

			if (s_duringTime < expectFrameTime)
			{
#ifdef _MSC_VER
//				CGE_LOG_INFO("SleepTime: %d\n", DWORD((expectFrameTime - duringTime) * 1000.0));
				//for vs
				Sleep(DWORD((expectFrameTime - s_duringTime) * 1000.0));
#else
				//for others, not completed.
#endif
			}
		}
	}

	double getFrameDuringTime()
	{
		return s_duringTime;
	}

	void setPointColor(int color)
	{
		GET_DRAWER(XEGEGeometryDrawerPoint, drawer);
		drawer->setColor(color);
	}
	
	void putpixel(float x, float y)
	{
		GET_DRAWER(XEGEGeometryDrawerPoint, drawer);
		float v[2] = { x, y };
		drawer->draw(v, 1);
	}

	void putpixels(float* points, int count)
	{
		GET_DRAWER(XEGEGeometryDrawerPoint, drawer);
		drawer->draw(points, count);
	}

	void setPointSize(float sz)
	{
		GET_DRAWER(XEGEGeometryDrawerPoint, drawer);
		drawer->setPointSize(100.0);
	}

}