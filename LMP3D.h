#ifndef __LMP3D__
#define __LMP3D__

#include "LMP3D/All/Type.h"

#include "LMP3D/All/Graphics/Graphics.h"
#include "LMP3D/All/Graphics/Texture.h"
#include "LMP3D/All/Graphics/Camera.h"
#include "LMP3D/All/Graphics/Model.h"
#include "LMP3D/All/Graphics/Draw.h"

#include "LMP3D/All/Load/tar.h"
#include "LMP3D/All/Event.h"
#include "LMP3D/All/Load.h"
#include "LMP3D/All/Math.h"
#include "LMP3D/All/Matrix.h"
#include "LMP3D/All/Script.h"
#include "LMP3D/All/Window.h"
#include "LMP3D/All/File.h"

#define LMP3D_LITTLE_ENDIAN 0
#define LMP3D_BIG_ENDIAN 1

#define LMP3D_ENDIANNESS LMP3D_LITTLE_ENDIAN

#define RW_REGISTER_FLOAT(REG) 	*((volatile float *)(REG))
#define RW_REGISTER_U8(REG) 	*((volatile unsigned char  *)(REG))
#define RW_REGISTER_U16(REG) 	*((volatile unsigned short *)(REG))
#define RW_REGISTER_U32(REG) 	*((volatile unsigned int   *)(REG))
#define RW_REGISTER_U64(REG) 	*((volatile unsigned long  *)(REG))

#ifdef __MINGW32__
#undef main
#endif

#endif


