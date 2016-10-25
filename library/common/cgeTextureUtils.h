/*
* cgeTextureUtils.h
*
*  Created on: 2015-7-29
*      Author: wysaid
*/

#ifndef _CGETEXTUREUTILS_H_
#define _CGETEXTUREUTILS_H_

#include "cgeGLFunctions.h"

namespace CGE
{
	class TextureDrawer
	{
	protected:
		TextureDrawer() : m_vertBuffer(0) {}
		virtual bool init();
	public:
		
        virtual ~TextureDrawer();

		CGE_COMMON_CREATE_FUNC(TextureDrawer, init);

		//绘制 texture 到当前的framebuffer上
		//同样不对viewport进行处理
		virtual void drawTexture(GLuint src);

		//设置绘制时的旋转(弧度)
		//旋转操作先于翻转缩放
		void setRotation(float rad);

		//设置绘制时的翻转或者缩放, 翻转和缩放操作都后于旋转执行
		//eg. 当x，y大于0时为缩放
		//    当x/y 小于0时， 该边将会翻转
		void setFlipScale(float x, float y);

		//特殊外部用法, 绑定扇形逆时针方向四顶点
		void bindVertexBuffer();

        inline ProgramObject& getProgram() { return m_program; }
        
	protected:
		ProgramObject m_program;
		//不使用cgeGlobal里面的， 避免出现context冲突
		GLuint m_vertBuffer;
		GLint m_rotLoc, m_flipScaleLoc;

		void _rotate(GLint location, float rad);

	protected:
		virtual CGEConstString getFragmentShaderString();
		virtual CGEConstString getVertexShaderString();
	};
}

#endif
