#ifndef __LMP3D_Event__
#define __LMP3D_Event__


#define LMP3D_EVENT_MAX 410
#define LMP3D_JOYPAD1 330
#define LMP3D_JOYPAD2 350
#define LMP3D_JOYPAD3 370
#define LMP3D_JOYPAD4 390

typedef struct
{
    char clikright;
    char clikleft;
    char mousex;
    char mousey;
    char exit;

    char key[LMP3D_EVENT_MAX];

}LMP3D_Event;


enum {
			Button_A,
			Button_B,
			Button_C,
			Button_D,
			Button_L1,
			Button_L2,
			Button_L3,
			Button_R1,
			Button_R2,
			Button_R3,
			Button_Up,
			Button_Down,
			Button_Left,
			Button_Right,
			Button_Start,
			Button_Select,
};

enum {
			Button_Cross,
			Button_Circle,
			Button_Square,
			Button_Triangle,
};


void LMP3D_Event_Init(LMP3D_Event *event);
void LMP3D_Event_Update(LMP3D_Event *event);


#endif

