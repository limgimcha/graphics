#include "sound.h"

#include <Windows.h>
#include <mmsystem.h>
#include <stdio.h>
#pragma comment(lib,"winmm.lib")

using namespace std;

bool dontPlay = false;

void engine_sound(int hey) {
	if (dontPlay == false) {
		if (hey == 1) PlaySound(TEXT("./sound/engine.wav"), NULL, SND_ASYNC | SND_LOOP);
	}
	if(hey==2) PlaySound(NULL, NULL, SND_PURGE | SND_NOWAIT | SND_ASYNC);
	if (hey == 3) PlaySound(TEXT("./sound/boom.wav"), NULL, SND_ASYNC);
	if (hey == 4) {
		printf("Engine Mute\n");
		PlaySound(NULL, NULL, SND_PURGE | SND_NOWAIT | SND_ASYNC);
		dontPlay = true;
	}
}