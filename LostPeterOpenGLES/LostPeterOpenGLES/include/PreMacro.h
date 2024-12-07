/****************************************************************************
* LostPeterOpenGLES - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterOpenGLES
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _PRE_MACRO_H_
#define _PRE_MACRO_H_

#include "FPreMacro.h"

#if F_PLATFORM == F_PLATFORM_WINDOW
	#if defined(OPENGLES_EXPORTS)
		#define openglesExport        	__declspec(dllexport) 
	#else
		#define openglesExport      	__declspec(dllimport)
	#endif
#elif F_PLATFORM == F_PLATFORM_ANDROID
	#define openglesExport				__attribute__ ((visibility("default")))
#else
	#define openglesExport
#endif


#endif