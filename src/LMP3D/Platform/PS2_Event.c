
#include <stdlib.h>
#include <stdio.h>

#ifdef PLAYSTATION2
#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <stdio.h>

#include "libpad.h"

#include "LMP3D/Event.h"
struct padButtonStatus buttons;
extern int _port,_slot;
u32 paddata;
    u32 old_pad = 0;
    u32 new_pad;

void LMP3D_Event_Update(LMP3D_Event *event)
{
	event->key[Button_Up] = 0;
	event->key[Button_Down] = 0;
	event->key[Button_Start] = 0;


	event->key[Button_R1] = 0;
	event->key[Button_L1] = 0;
	event->key[Button_R2] = 0;
	event->key[Button_L2] = 0;

	int i=0,ret;
	ret=padGetState(_port, _slot);
	while((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1)) {
		if(ret==PAD_STATE_DISCONN) {
			printf("Pad(%d, %d) is disconnected\n", _port, _slot);
		}
		ret=padGetState(_port, _slot);
	}
	if(i==1) {
		printf("Pad: OK!\n");
	}

	ret = padRead(_port, _slot, &buttons); // port, slot, buttons

	if (ret != 0)
	{
		paddata = 0xffff ^ buttons.btns;

		new_pad = paddata & ~old_pad;
		old_pad = paddata;

		// Directions
		if(new_pad & PAD_LEFT) {

		}

		if(new_pad & PAD_RIGHT) {

		}


		if(new_pad & PAD_CROSS) {

		}

		if(new_pad & PAD_CIRCLE ) {

		}

		if(new_pad & PAD_R1)
		{
			event->key[Button_R1] = 1;

		}

		if(new_pad & PAD_L1)
		{
			event->key[Button_L1] = 1;
		}

		if(new_pad & PAD_R2)
		{
			event->key[Button_R2] = 1;

		}

		if(new_pad & PAD_L2)
		{
			event->key[Button_L2] = 1;
		}

		if(new_pad & PAD_START)
		{
			event->key[Button_Start] = 1;
		}


		//----------------------------

		if(paddata & PAD_DOWN)
		{
			event->key[Button_Up] = 2;
		}

		if(paddata & PAD_UP)
		{
			event->key[Button_Down] = 2;
		}

	}
}

static char padBuf[256] __attribute__((aligned(64)));

static char actAlign[6];
static int actuators;

/*
 * waitPadReady()
 */
 int waitPadReady(int port, int slot)
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
 int
initializePad(int port, int slot)
{

    int ret;
    int modes;
    int i;

    waitPadReady(port, slot);

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

    waitPadReady(port, slot);
    printf("infoPressMode: %d\n", padInfoPressMode(port, slot));

    waitPadReady(port, slot);
    printf("enterPressMode: %d\n", padEnterPressMode(port, slot));

    waitPadReady(port, slot);
    actuators = padInfoAct(port, slot, -1, 0);
    printf("# of actuators: %d\n",actuators);

    if (actuators != 0) {
        actAlign[0] = 0;   // Enable small engine
        actAlign[1] = 1;   // Enable big engine
        actAlign[2] = 0xff;
        actAlign[3] = 0xff;
        actAlign[4] = 0xff;
        actAlign[5] = 0xff;

        waitPadReady(port, slot);
        printf("padSetActAlign: %d\n",
                   padSetActAlign(port, slot, actAlign));
    }
    else {
        printf("Did not find any actuators.\n");
    }

    waitPadReady(port, slot);

    return 1;
}


#endif
