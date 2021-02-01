#ifndef __AURAE__
#define __AURAE__

#include "AURAE/All/Type.h"

#include "AURAE/All/Graphics/Graphics.h"
#include "AURAE/All/Graphics/Texture.h"
#include "AURAE/All/Graphics/Camera.h"
#include "AURAE/All/Graphics/Model.h"
#include "AURAE/All/Graphics/Draw.h"

#include "AURAE/All/Load/tar.h"
#include "AURAE/All/Event.h"
#include "AURAE/All/Load.h"
#include "AURAE/All/Math.h"
#include "AURAE/All/Matrix.h"
#include "AURAE/All/Script.h"
#include "AURAE/All/Window.h"
#include "AURAE/All/File.h"

#define AURAE_LITTLE_ENDIAN 0
#define AURAE_BIG_ENDIAN 1

#define AURAE_ENDIANNESS AURAE_LITTLE_ENDIAN

#define RW_REGISTER_FLOAT(REG) 	*((volatile float *)(REG))
#define RW_REGISTER_U8(REG) 	*((volatile unsigned char  *)(REG))
#define RW_REGISTER_U16(REG) 	*((volatile unsigned short *)(REG))
#define RW_REGISTER_U32(REG) 	*((volatile unsigned int   *)(REG))
#define RW_REGISTER_U64(REG) 	*((volatile unsigned long  *)(REG))

#ifdef __MINGW32__
#undef main
#endif

#endif


