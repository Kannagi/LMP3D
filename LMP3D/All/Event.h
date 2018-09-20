#ifndef __LMP3D_Event__
#define __LMP3D_Event__



#if defined(PC) || defined(WINDOWS95)

#define LMP3D_EVENT_MAX 410
#define LMP3D_JOYPAD1 330
#define LMP3D_JOYPAD2 350
#define LMP3D_JOYPAD3 370
#define LMP3D_JOYPAD4 390

#else

#define LMP3D_EVENT_MAX 20*4
#define LMP3D_JOYPAD1 00
#define LMP3D_JOYPAD2 20
#define LMP3D_JOYPAD3 40
#define LMP3D_JOYPAD4 60

#endif


typedef struct
{
    char clikright;
    char clikleft;
    char exit;
    int mousex,mousey;

    char key[LMP3D_EVENT_MAX];

}LMP3D_Event;


//For ALL
enum {
			Button_A,
			Button_B,
			Button_X,
			Button_Y,
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
			Button_Axis1X,
			Button_Axis1Y,
			Button_Axis2X,
			Button_Axis2Y,
			Button_Number,
};

//For PS1,PS2,PSP
enum {
			Button_Cross,
			Button_Circle,
			Button_Square,
			Button_Triangle,
};

//FOR GC
enum {
			Button_Z = Button_R2,

};

//FOR Wii
enum {
			Button_C = Button_R1,
			Button_1 = Button_X,
			Button_2 = Button_Y,
			Button_Plus = Button_Start,
			Button_Less = Button_Select,

};

void LMP3D_Event_Init(LMP3D_Event *event);
void LMP3D_Event_Update(LMP3D_Event *event);

#define LMP3D_KEY_DOWN   1
#define LMP3D_KEY_DOWNW  2
#define LMP3D_KEY_UP     3

#endif

