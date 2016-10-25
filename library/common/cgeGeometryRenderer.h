/*
 * cgeGeometryRenderer.h
 *
 *  Created on: 2015-1-16
 *      Author: wysaid
 */

#ifndef _CGEGEOMETRY_H_
#define _CGEGEOMETRY_H_

#include "cgeGLFunctions.h"

#define CGE_GEOMETRY_DRAWER_STATIC_FUNC(cls) \
static inline cls* create() \
{\
cls* drawer = new cls(); \
if(!drawer->init()) \
{ \
delete drawer; \
drawer = nullptr; \
CGE_LOG_ERROR("create %s failed!", #cls); \
} \
return drawer; \
}\

namespace CGE
{
    class GeometryDrawer
    {
        GeometryDrawer() : m_vertBuffer(0), m_dataType(GL_FLOAT), m_dataElemSize(2), m_dataElemStride(0), m_geometryCount(0) {}
        virtual bool init();
    public:
        virtual ~GeometryDrawer();
        
        CGE_GEOMETRY_DRAWER_STATIC_FUNC(GeometryDrawer);
        
        virtual void drawGeometry(GLenum mode = GL_POINTS);        
        virtual void drawGeometry(GLenum mode, int first, int count);
        
        void setupBufferData(const void* data, int dataSize, int dataElemSize, int geomCount, GLenum dataType = GL_FLOAT, int dataElemStride = 0, GLenum usage = GL_STATIC_DRAW);
        
        void bindVertexBuffer();
        
        void setFragColor(float r, float g, float b, float a);
        
        void setDataNormalized(bool norm) { m_dataNormalized = norm; }
        
    protected:
        ProgramObject m_program;
        GLuint m_vertBuffer;
        GLenum m_dataType;
        
        int m_dataElemSize, m_dataElemStride;
        int m_geometryCount;
        
        bool m_dataNormalized;
        
    protected:
        virtual CGEConstString getFragmentShaderString();
        virtual CGEConstString getVertexShaderString();
        static CGEConstString paramFragColorName;
    };
}



#endif
