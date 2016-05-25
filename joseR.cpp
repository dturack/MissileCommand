// Author:   Jose Reyes
// Created:  April 28, 2016
// Modified: May 9, 2016
// Program:  joseR.cpp
// Purpose:  
//			Draw and create a functioning menu that will allow
//			the user to play, change settings, and exit the game.
// Progress:
//		*May 2nd, 2016*
//			Menu shapes, text, and shape color change on mouse hover.
//			The exit button will close on click.
//		*May 9th, 2016*
//			The play button will toggle menu code off and game code on.
//			The settings button will cout a temp message (filler code).
//				This is determined by the function gameState(...)
//			Added a way of treating the menu as a pseudo pause menu.
//				This is determined by "int inGame" in "struct Game"
//		*May 15th, 2016*
//			OpenAL (sound) functionality has been added
//			Others may play sounds by simply calling "playSound(game, int);"
//			Must include joseR.h
//		*May 20th, 2016*
//			Moved OpenAL code into a class
//			Changed how others may call the function to play a sound buffer.
//			Requires &game->sounds->playAudio(int);
//			0 = missile_miss, 10 = missile_hit, 20 = missile_fire
//			30/32 = mouse clicks
//			
#include <GL/glx.h>
#include "missileCommand.h"
#include "joseR.h"
#include <stdio.h>
extern "C" {
	#include "fonts.h"
}

using namespace std;

extern void init_opengl();

Audio::Audio()
{
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return;
	}
	alGetError();
	device = alcOpenDevice(NULL);
	if (!device) {
		printf("ERROR: device\n");
		return;
	}
	alGetError();
	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		printf("ERROR: context\n");
		return;
	}
	//Setup the listener.
	float vec[6] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
}

Audio::~Audio()
{
	alDeleteSources(1, &alSource);
	alDeleteBuffers(1, &alBuffer);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	alutExit();
}

void Audio::loadAudio()
{
	//Buffer holds the sound information.
	const string FILE[] = {
		"./sounds/missile_explosion.wav", "./sounds/missile_collision.wav",
		"./sounds/missile_launch2.wav", "./sounds/mouse_click.wav",
		"./sounds/mouse_release.wav"};
	int val = 0;
	//Load and assign sounds
	for (int i = 0; i < TOTAL_SOUNDS; i++) {
		//Load file into buffer
		const char *f = FILE[i].c_str();
		buffer[i] = alutCreateBufferFromFile(f);
		//Check if anything was loaded into the buffer
		if (!buffer[i]) {
			printf("ERROR: Audio File Not Found!\n");
			printf("[%s] - Was not loaded.\n", f);
			break;
		}
		//Sets explosions and missile sounds to 10 sources each
		if (val < 30) {
			for (int n = 0; n < 10; n++) {
				//Generate a source
				alGenSources(1, &alSource);
				//Store source in a buffer
				alSourcei(alSource, AL_BUFFER, buffer[i]);
				//Store value of that source to call later				
				//printf("File: %s stored in buffer[%d].\n", f, val);
				source[val++] = alSource;
			}
		//Sets menu click sounds
		} else if (val >= 30 && val < 34) {
			for (int n = 0; n < 2; n++) {
				alGenSources(1, &alSource);
				alSourcei(alSource, AL_BUFFER, buffer[i]);
				//printf("File: %s stored in buffer[%d].\n", f, val);
				source[val++] = alSource;
			}
		} else {
			printf("Something may have gone wrong...\n");
		}
	}
}

// ODD BUG - AUDIO GETS QUIET OR LOUD WHEN MULTIPLE GO OFF
// ODD BUG - AUDIO GETS QUIET OR LOUD WHEN MULTIPLE GO OFF
// ODD BUG - AUDIO GETS QUIET OR LOUD WHEN MULTIPLE GO OFF
// ODD BUG - AUDIO GETS QUIET OR LOUD WHEN MULTIPLE GO OFF
// ODD BUG - AUDIO GETS QUIET OR LOUD WHEN MULTIPLE GO OFF
// ODD BUG - AUDIO GETS QUIET OR LOUD WHEN MULTIPLE GO OFF

void Audio::playAudio(int num)
{
	int idx = num, max;
	if (idx < 30) {
		max = idx + 9;
	} else {
		max = idx + 1;
	}
	alSource = source[idx];
	alGetSourcei(alSource, AL_SOURCE_STATE, &source_state);
	while (source_state == AL_PLAYING) {		
		alSource = source[idx++];
		if (idx > max) {
			printf("Max sources for this sound was reached!\n");
			break;
		}
		alGetSourcei(alSource, AL_SOURCE_STATE, &source_state);
		//printf("Sound already playing!\nPlaying source[%d]\n", idx);
	}
	alSourcePlay(alSource);
}

void drawMenu(Game *game)
{
	for (int j = 0; j < BUTTONS; j++) {
		game->buttonSpacer[j] = (WINDOW_HEIGHT - 200) -(j*95);
		game->mButton[j].width = 120;
		game->mButton[j].height = 25;
		game->mButton[j].center.x = WINDOW_WIDTH / BUTTON_X;
		game->mButton[j].center.y = WINDOW_HEIGHT - game->buttonSpacer[j];
	}
}

//Settings buttons draw function goes here

void drawSettings(Game *game)
{
	//
	Shape *s;
	s = &game->menu1;
	s->width = WINDOW_WIDTH - 650;
	s->height = WINDOW_HEIGHT - 450;
	s->center.x = WINDOW_WIDTH / 2;
	s->center.y = WINDOW_HEIGHT / 2;
}

void renderMenuObjects(Game *game)
{
	Shape *s;
	//Check if game is in progress so menu can act as a pause menu.
	//Otherwise will clear screen
	if (game->inGame == 0) {
		glClearColor(0.15, 0.15, 0.15, 0.15);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	float w, h;
	for (int i = 0; i < BUTTONS; i++) {
		s = &game->mButton[i];
		glColor3ub(128,128,128);
		//Button colors based on mouse position
		if (game->mouseOnButton[i] == 1) {
			//Button selected color
			glColor3ub(190,190,190);
		}
		glPushMatrix();
		glTranslatef(s->center.x, s->center.y, s->center.z);
		w = s->width;
		h = s->height;
		glBegin(GL_QUADS);
			glVertex2i(-w,-h);
			glVertex2i(-w, h);
			glVertex2i( w, h);
			glVertex2i( w,-h);
		glEnd();
		glPopMatrix();
		glFlush();
	}
}

void renderSettings(Game *game)
{
	Shape *s;
	glClearColor(0.15, 0.15, 0.15, 0.15);
	glClear(GL_COLOR_BUFFER_BIT);
	s = &game->menu1;
	float w, h;
	glColor3ub(128,128,128);
	glPushMatrix();
	glTranslatef(s->center.x, s->center.y, s->center.z);
	w = s->width;
	h = s->height;
	glBegin(GL_QUADS);
		glVertex2i(-w,-h);
		glVertex2i(-w, h);
		glVertex2i( w, h);
		glVertex2i( w,-h);
	glEnd();
	glPopMatrix();
	glFlush();
}

void renderMenuText(Game *game)
{
	Rect rt;
	int j = 0;
	rt.bot = WINDOW_HEIGHT - game->buttonSpacer[j] - 10;
	rt.left = WINDOW_WIDTH / BUTTON_X;
	//std::cout << rt.bot << " " << rt.left << std::endl;
	rt.center = 1;
	ggprint16(&rt, 16, 0x00ffffff, "Quit");
	j++;
	rt.bot = WINDOW_HEIGHT - game->buttonSpacer[j] - 10;
	ggprint16(&rt, 16, 0x00ffffff, "Settings");
	j++;
	rt.bot = WINDOW_HEIGHT - game->buttonSpacer[j] - 10;
	if (game->inGame == 0) {
		ggprint16(&rt, 16, 0x00ffffff, "Play");
	} else {
		ggprint16(&rt, 16, 0x00ffffff, "Resume");
	}
}

void mouseOver(int savex, int savey, Game *game)
{
	Shape *s;
	for (int j = 0; j < BUTTONS; j++) {			
		s = &game->mButton[j];
		if (savey >= s->center.y - (s->height) &&
			savey <= s->center.y + (s->height) &&
			savex >= s->center.x - (s->width) &&
			savex <= s->center.x + (s->width)) {
				game->mouseOnButton[j] = 1;
		} else {
			game->mouseOnButton[j] = 0;
		}
	}
}

void menuClick(Game *game)
{
	//Play Button (2)
	if (game->mouseOnButton[2] == 1) {
		game->gMenu = 0;
		game->inGame = 1;
	}
	//Settings Button (1)
	if (game->mouseOnButton[1] == 1) {
		game->gMenu = 2;
	}
	//Exit Button (0)
	if (game->mouseOnButton[0] == 1) {
		//std::cout << "Quitting..." << std::endl;
		game->menuExit = 1;
	}
}

int gameState(Game *game)
{
	int state = 0;
	int g;
	g = game->gMenu;
	if (g == 1) {
		state = 1;
	} else if (g == 2) {
		state = 2;
	} else {
		state = 0;
	}
	return state;
}