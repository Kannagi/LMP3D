#ifndef __AURAE_Event__
#define __AURAE_Event__



#if defined(PC) || defined(WINDOWS95)

#define AURAE_EVENT_MAX 410
#define AURAE_JOYPAD1 330
#define AURAE_JOYPAD2 350
#define AURAE_JOYPAD3 370
#define AURAE_JOYPAD4 390

#else

#define AURAE_EVENT_MAX 20*4
#define AURAE_JOYPAD1 00
#define AURAE_JOYPAD2 20
#define AURAE_JOYPAD3 40
#define AURAE_JOYPAD4 60

#endif


typedef struct
{
	char clikright;
	char clikleft;
	char exit;
	int mousex,mousey;

	char key[AURAE_EVENT_MAX];

}AURAE_Event;


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

void AURAE_Event_Init(AURAE_Event *event);
void AURAE_Event_Update(AURAE_Event *event);

#define AURAE_KEY_DOWN   1
#define AURAE_KEY_DOWNW  2
#define AURAE_KEY_UP	 3

#endif

