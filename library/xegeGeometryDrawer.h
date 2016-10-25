/*
* xegeGeometry.h
*
*  Created on: 2016-10-27
*      Author: wysaid
*        Mail: admin@wysaid.org
*/

#ifndef _XEGE_GEOMETRYDRAWER_H_
#define _XEGE_GEOMETRYDRAWER_H_

#include <vector>
#include "algorithm/cgeMat.h"
#include "common/cgeShaderFunctions.h"

#define GET_R(color) (((color >> 16) & 0xff) / 255.0f)
#define GET_G(color) (((color >> 8) & 0xff) / 255.0f)
#define GET_B(color) ((color & 0xff) / 255.0f)
#define GET_A(color) ((color >> 24) / 255.0f)
#define SET_COLOR(program, color) \
program.sendUniformf(s_fshColorName, GET_R(color), GET_G(color), GET_B(color), GET_A(color));

//GET_DRAWER 永远不应该失败， 所以不予处理错误信息.
//如果真的有未知错误, 当 GET_DRAWER 获取失败时, 将直接跳出当前函数, 而不是给一个返回值.
#define GET_DRAWER(ClsName, varName) \
{\
auto& g = XEGEGeometryDrawer::vecGeometry(); \
if (g.size() <= ClsName::TYPE || g[ClsName::TYPE] == nullptr) \
{ \
if (g.size() != XEGEGeometryDrawer::GeometryType_Max) \
{ \
g.resize(XEGEGeometryDrawer::GeometryType_Max); \
memset(g.data(), 0, g.size() * sizeof(g[0])); \
} \
ClsName* cls = ClsName::create(); \
if (cls == nullptr) return; \
g[ClsName::TYPE] = cls; \
} \
}\
ClsName* varName = (ClsName*)(XEGEGeometryDrawer::vecGeometry()[ClsName::TYPE])

namespace xege
{
	//提供几何图形绘制的大类
	class XEGEGeometryDrawer
	{
	protected:
		static std::vector<XEGEGeometryDrawer*> s_vecGeometry;
		static CGE::Mat4 s_mvp;
		static unsigned int s_defaultColor;

		//////////////////////////////////////////////////////////////////////////

		XEGEGeometryDrawer();
		CGE::ProgramObject m_program;
	public:

		static inline std::vector<XEGEGeometryDrawer*>& vecGeometry() { return s_vecGeometry; }

		virtual ~XEGEGeometryDrawer();

		enum GeometryType
		{
			GeometryType_Point,
			GeometryType_Line,
			GeometryType_Circle,

			GeometryType_Max //计数
		};

		//////////////////////////////////////////////////////////////////////////

		static void cleanup();
		static void setMVP(const CGE::Mat4& mvp);
		static void setColor(unsigned int color);
	};

	//////////////////////////////////////////////////////////////////////////

	class XEGEGeometryDrawerPoint : public XEGEGeometryDrawer
	{
	public:
		enum { TYPE = GeometryType_Point };
		bool init();
		CGE_COMMON_CREATE_FUNC(XEGEGeometryDrawerPoint, init);
		void draw(const float* pnts, int count);
		void setColor(int color);
		void setPointSize(float pointSize);

	protected:
		GLint m_colorLoc;
		GLint m_pointSizeLoc;
	};

}


#endif // !_XEGE_GEOMETRYDRAWER_H_
