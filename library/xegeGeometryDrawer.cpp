/*
* xegeGeometry.cpp
*
*  Created on: 2016-10-27
*      Author: wysaid
*        Mail: admin@wysaid.org
*/

#include "XEGEGeometryDrawer.h"
#include <memory>

using namespace CGE;

namespace xege
{
	static CGEConstString s_vshPosName = "vPosition";
	static CGEConstString s_vshMVPName = "mvp";
	static CGEConstString s_fshColorName = "color";

	//////////////////////////////////////////////////////////////////////////

	bool XEGEGeometryDrawerPoint::init()
	{
		CGEConstString vsh = CGE_SHADER_STRING
		(attribute vec2 vPosition;
		uniform float pointSize;
		uniform mat4 mvp;
		void main()
		{
			gl_PointSize = pointSize;
			gl_Position = mvp * vec4(vPosition, 0.0, 1.0);
		});

		CGEConstString fsh = CGE_SHADER_STRING_PRECISION_L
		(uniform vec4 color;
		void main()
		{
			gl_FragColor = color;
		}
		);

		m_program.bindAttribLocation(s_vshPosName, 0);
		if (m_program.initWithShaderStrings(vsh, fsh))
		{
			m_program.bind();
			SET_COLOR(m_program, s_defaultColor);
			m_pointSizeLoc = m_program.uniformLocation("pointSize");
			m_colorLoc = m_program.uniformLocation("color");
			glUniform1f(m_pointSizeLoc, 1.0f);
			m_program.sendUniformMat4(s_vshMVPName, 1, GL_FALSE, s_mvp.data[0]);
			return true;
		}
		return false;
	}

	void XEGEGeometryDrawerPoint::draw(const float* pnts, int count)
	{
		m_program.bind();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, pnts);
		glDrawArrays(GL_POINTS, 0, count);
	}

	void XEGEGeometryDrawerPoint::setColor(int color)
	{
		m_program.bind();
		SET_COLOR(m_program, color);
	}

	void XEGEGeometryDrawerPoint::setPointSize(float pointSize)
	{
		m_program.bind();
		glUniform1f(m_pointSizeLoc, pointSize);
	}

	//////////////////////////////////////////////////////////////////////////

	std::vector<XEGEGeometryDrawer*> XEGEGeometryDrawer::s_vecGeometry;
	CGE::Mat4 XEGEGeometryDrawer::s_mvp = CGE::Mat4::makeIdentity();
	unsigned int XEGEGeometryDrawer::s_defaultColor = 0xffffffff;

	XEGEGeometryDrawer::XEGEGeometryDrawer()
	{
	}

	XEGEGeometryDrawer::~XEGEGeometryDrawer()
	{
	}

	void XEGEGeometryDrawer::cleanup()
	{
		for (auto* g : s_vecGeometry)
		{
			delete g;
		}

		s_vecGeometry.clear();
	}

	//////////////////////////////////////////////////////////////////////////

	void XEGEGeometryDrawer::setMVP(const CGE::Mat4& mvp)
	{
		s_mvp = mvp;
		for (auto* g : s_vecGeometry)
		{
			if (g != nullptr)
			{
				g->m_program.bind();
				g->m_program.sendUniformMat4(s_vshMVPName, 1, GL_FALSE, s_mvp.data[0]);
			}
		}
	}

	void XEGEGeometryDrawer::setColor(unsigned int color)
	{
		s_defaultColor = color;
		for (auto* g : s_vecGeometry)
		{
			if (g != nullptr)
			{
				g->m_program.bind();
				SET_COLOR(g->m_program, color);
			}
		}
	}
}

