
#include <stdlib.h>
#include <stdio.h>

#ifdef PLAYSTATION2

#include <kernel.h>
#include <libpad.h>

#include "LMP3D/LMP3D.h"


static int _port,_slot;
int PS2_initializePad(int port, int slot);

void PS2_Init_Pad()
{
	int ret;
	static char paddata[256] __attribute__((aligned(64)));
	padInit(0);

	_port = 0; // 0 -> Connector 1, 1 -> Connector 2
	_slot = 0; // Always zero if not using multitap

	printf("PortMax: %d\n", padGetPortMax());
	printf("SlotMax: %d\n", padGetSlotMax(_port));

	if((ret = padPortOpen(_port, _slot, paddata)) == 0) {
		printf("padOpenPort failed: %d\n", ret);
		SleepThread();
	}

	if(!PS2_initializePad(_port, _slot)) {
		printf("pad initalization failed!\n");
		SleepThread();
	}
}

void LMP3D_Event_Update(LMP3D_Event *event)
{
	static struct padButtonStatus buttons;
	static u32 padbuf;
	static u32 old_pad = 0;
	static u32 new_pad;
	static u32 end_pad;

	int i,ret;

	for(i = 0;i < Button_Number;i++)
	{
		event->key[i] = 0;
	}

	ret=padGetState(_port, _slot);
	while((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1)) {
		if(ret==PAD_STATE_DISCONN) {
			printf("Pad(%d, %d) is disconnected\n", _port, _slot);
		}
		ret=padGetState(_port, _slot);
	}

	ret = padRead(_port, _slot, &buttons); // port, slot, buttons

	if (ret == 0) return;

	padbuf = 0xffff ^ buttons.btns;

	new_pad = padbuf & ~old_pad;
	end_pad = (padbuf^old_pad)^new_pad;
	old_pad = padbuf;

	//DOWNW
	if(padbuf & PAD_SQUARE)
		event->key[Button_Square] = 2;

	if(padbuf & PAD_TRIANGLE)
		event->key[Button_Triangle] = 2;

	if(padbuf & PAD_CIRCLE)
		event->key[Button_Circle] = 2;

	if(padbuf & PAD_CROSS)
		event->key[Button_Cross] = 2;

	if(padbuf & PAD_LEFT)
		event->key[Button_Left] = 2;

	if(padbuf & PAD_RIGHT)
		event->key[Button_Right] = 2;

	if(padbuf & PAD_UP)
		event->key[Button_Up] = 2;

	if(padbuf & PAD_DOWN)
		event->key[Button_Down] = 2;

	if(padbuf & PAD_R1)
		event->key[Button_R1] = 2;

	if(padbuf & PAD_R2)
		event->key[Button_R2] = 2;

	if(padbuf & PAD_R3)
		event->key[Button_R3] = 2;

	if(padbuf & PAD_L1)
		event->key[Button_L1] = 2;

	if(padbuf & PAD_L2)
		event->key[Button_L2] = 2;

	if(padbuf & PAD_L3)
		event->key[Button_L3] = 2;

	if(padbuf & PAD_SELECT)
		event->key[Button_Select] = 2;

	if(padbuf & PAD_START)
		event->key[Button_Start] = 2;

	//DOWN
	if(new_pad & PAD_SQUARE)
		event->key[Button_Square] = 1;

	if(new_pad & PAD_TRIANGLE)
		event->key[Button_Triangle] = 1;

	if(new_pad & PAD_CIRCLE)
		event->key[Button_Circle] = 1;

	if(new_pad & PAD_CROSS)
		event->key[Button_Cross] = 1;

	if(new_pad & PAD_LEFT)
		event->key[Button_Left] = 1;

	if(new_pad & PAD_RIGHT)
		event->key[Button_Right] = 1;

	if(new_pad & PAD_UP)
		event->key[Button_Up] = 1;

	if(new_pad & PAD_DOWN)
		event->key[Button_Down] = 1;

	if(new_pad & PAD_R1)
		event->key[Button_R1] = 1;

	if(new_pad & PAD_R2)
		event->key[Button_R2] = 1;

	if(new_pad & PAD_R3)
		event->key[Button_R3] = 1;

	if(new_pad & PAD_L1)
		event->key[Button_L1] = 1;

	if(new_pad & PAD_L2)
		event->key[Button_L2] = 1;

	if(new_pad & PAD_L3)
		event->key[Button_L3] = 1;

	if(new_pad & PAD_SELECT)
		event->key[Button_Select] = 1;

	if(new_pad & PAD_START)
		event->key[Button_Start] = 1;

	//UP
	if(end_pad & PAD_SQUARE)
		event->key[Button_Square] = 3;

	if(end_pad & PAD_TRIANGLE)
		event->key[Button_Triangle] = 3;

	if(end_pad & PAD_CIRCLE)
		event->key[Button_Circle] = 3;

	if(end_pad & PAD_CROSS)
		event->key[Button_Cross] = 3;

	if(end_pad & PAD_LEFT)
		event->key[Button_Left] = 3;

	if(end_pad & PAD_RIGHT)
		event->key[Button_Right] = 3;

	if(end_pad & PAD_UP)
		event->key[Button_Up] = 3;

	if(end_pad & PAD_DOWN)
		event->key[Button_Down] = 3;

	if(end_pad & PAD_R1)
		event->key[Button_R1] = 3;

	if(end_pad & PAD_R2)
		event->key[Button_R2] = 3;

	if(end_pad & PAD_R3)
		event->key[Button_R3] = 3;

	if(end_pad & PAD_L1)
		event->key[Button_L1] = 3;

	if(end_pad & PAD_L2)
		event->key[Button_L2] = 3;

	if(end_pad & PAD_L3)
		event->key[Button_L3] = 3;

	if(end_pad & PAD_SELECT)
		event->key[Button_Select] = 3;

	if(end_pad & PAD_START)
		event->key[Button_Start] = 3;
}


static char actAlign[6];
static int actuators;

/*
 * waitPadReady()
 */
 int PS2_waitPadReady(int port, int slot)
{
    int state;
    int lastState;
    char stateString[16];

    state = padGetState(port, slot);
    lastState = -1;
    while((state != PAD_STATE_STABLE) && (state != PAD_STATE_FINDCTP1)) {
        if (state != lastState) {
            padStateInt2String(state, stateString);
            printf("Please wait, pad(%d,%d) is in state %s\n",
                       port, slot, stateString);
        }
        lastState = state;
        state=padGetState(port, slot);
    }
    // Were the pad ever 'out of sync'?
    if (lastState != -1) {
        printf("Pad OK!\n");
    }
    return 0;
}


/*
 * initializePad()
 */
int PS2_initializePad(int port, int slot)
{

    int ret;
    int modes;
    int i;

    PS2_waitPadReady(port, slot);

    // How many different modes can this device operate in?
    // i.e. get # entrys in the modetable
    modes = padInfoMode(port, slot, PAD_MODETABLE, -1);
    printf("The device has %d modes\n", modes);

    if (modes > 0) {
        printf("( ");
        for (i = 0; i < modes; i++) {
            printf("%d ", padInfoMode(port, slot, PAD_MODETABLE, i));
        }
        printf(")");
    }

    printf("It is currently using mode %d\n",
               padInfoMode(port, slot, PAD_MODECURID, 0));

    // If modes == 0, this is not a Dual shock controller
    // (it has no actuator engines)
    if (modes == 0) {
        printf("This is a digital controller?\n");
        return 1;
    }

    // Verify that the controller has a DUAL SHOCK mode
    i = 0;
    do {
        if (padInfoMode(port, slot, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK)
            break;
        i++;
    } while (i < modes);
    if (i >= modes) {
        printf("This is no Dual Shock controller\n");
        return 1;
    }

    // If ExId != 0x0 => This controller has actuator engines
    // This check should always pass if the Dual Shock test above passed
    ret = padInfoMode(port, slot, PAD_MODECUREXID, 0);
    if (ret == 0) {
        printf("This is no Dual Shock controller??\n");
        return 1;
    }

    printf("Enabling dual shock functions\n");

    // When using MMODE_LOCK, user cant change mode with Select button
    padSetMainMode(port, slot, PAD_MMODE_DUALSHOCK, PAD_MMODE_LOCK);

    PS2_waitPadReady(port, slot);
    printf("infoPressMode: %d\n", padInfoPressMode(port, slot));

    PS2_waitPadReady(port, slot);
    printf("enterPressMode: %d\n", padEnterPressMode(port, slot));

    PS2_waitPadReady(port, slot);
    actuators = padInfoAct(port, slot, -1, 0);
    printf("# of actuators: %d\n",actuators);

    if (actuators != 0) {
        actAlign[0] = 0;   // Enable small engine
        actAlign[1] = 1;   // Enable big engine
        actAlign[2] = 0xff;
        actAlign[3] = 0xff;
        actAlign[4] = 0xff;
        actAlign[5] = 0xff;

        PS2_waitPadReady(port, slot);
        printf("padSetActAlign: %d\n",
                   padSetActAlign(port, slot, actAlign));
    }
    else {
        printf("Did not find any actuators.\n");
    }

    PS2_waitPadReady(port, slot);

    return 1;
}


#endif
