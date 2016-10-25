/*
* xegeCompatible.h
*
*  Created on: 2016-10-26
*      Author: wysaid
*        Mail: admin@wysaid.org
* Description: 提供某些兼容原版EGE的设定
*/

#ifndef _XEGE_COMPATIBLE_H_
#define _XEGE_COMPATIBLE_H_

namespace xege
{
	//兼容ege.
	static inline bool is_run() { return isRunning(); }

	enum rendermode_e {
		RENDER_AUTO,
		RENDER_MANUAL,
	};

	static inline void setrendermode(rendermode_e renderMode) {}

	static inline void setCaption(const wchar_t* title) { setCaption("You should use UTF8 instead of unicode!"); }
}

#endif