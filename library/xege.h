/*
* xege.h
*
*  Created on: 2016-10-26
*      Author: wysaid
*        Mail: admin@wysaid.org
*/

#ifndef _XEGE_H_
#define _XEGE_H_

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew/glew32s.lib")
#pragma comment(lib, "glfw/glfw3.lib")

#include <functional>

#define XEGE_FOREVER_BEGIN() runLoop(60.0, [](){
#define XEGE_FOREVER_BEGIN_WITH_FPS(fps) runLoop(fps, [](){
#define XEGE_FOREVER_END() })

namespace xege
{
	//绘图环境相关函数, 开发中, flag暂时无用
	bool initgraph(int width, int height, int flag = 0xffffffff);
	void closegraph();

	void setClearColor(int color); //设定清屏颜色
#define setclearcolor setClearColor
	void clearDevice(); //清屏函数 (默认全黑
#define cleardevice clearDevice //兼容EGE

	//将绘图buffer绑定回系统默认设定, 外部若更改了framebuffer绑定
	//则必须调用以绘制最终结果 (如果未自己使用OpenGL函数, 则一般不需要调用)
	void bindDefaultBuffer();

	//用户主动点击窗口关闭按钮之后将返回false (如windows窗口右上角的叉)
	bool isRunning();

	void setCaption(const char* title);
#define setcaption setCaption //兼容EGE

	//func 将被循环调用, fps为调用帧率, 默认 60
	void runLoop(double fps, const std::function<void()>& func);

	double getFrameDuringTime(); //返回这一帧跟上一帧之间的时间间隔


	//画点相关
	void setPointColor(int color); 
#define setpointcolor setPointColor
	void putpixel(float x, float y);
	void putpixels(float* points, int count); // count 大小为points数组长度除以2
	void setPointSize(float sz); //设置点大小.
#define setpointsize setPointSize

}

#include "xegeCompatible.h"

#endif