
#ifndef _JOSER_H_
#define _JOSER_H_

//Class

//Function Prototypes
extern void drawMenu(Game *game);
extern void drawSettings(Game *game);
extern void mouseOver(int x, int y, Game *game);
extern void gameOver(Game *game);
extern void renderGameOverExpl(Game *game, double n);
extern void renderMenuObjects(Game *game);
extern void renderMenuText(Game *game);
extern void renderSettings(Game *game);
extern void renderSettingsText(Game *game);
extern int lvlState(Game *game);
extern void resetLevelEnd(Game *game);
extern void levelEnd(Game *game);
extern void renderBonusA(Game *game, int rCount, int cCount, bool type);
extern void renderNewLevelMsg(Game *game);
extern void menuClick(Game *game);
extern int gameState(Game *game);
extern void classicMode(Game *game);

//Textures
extern GLuint mCounter;
extern GLuint cCounter;
extern GLuint gameoverTexture;
extern GLuint mainmenuTexture;

#endif