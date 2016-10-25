
#include "graphics.h"

int main()
{
	initgraph(800, 600);
	setcaption("Demo Point.");

	setpointsize(100.0f);
	setpointcolor(0xff00ff00); //设置点的颜色为绿色
	setclearcolor (0xff770000); //清屏颜色分量为 0xAARRGGBB

	//XEGE_FOREVER_BEGIN 和 XEGE_FOREVER_END 之间的代码将会自动循环执行
	//参数60.0 表示循环的帧率, 将尝试以接近该帧率稳定循环执行此间代码.
	XEGE_FOREVER_BEGIN_WITH_FPS(60.0);

	cleardevice();
	putpixel(100, 100);

	XEGE_FOREVER_END();

	closegraph();
	return 0;
}