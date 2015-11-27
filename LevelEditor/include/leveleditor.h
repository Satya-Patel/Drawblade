#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include "../../shared/include/level.h"
#include "../../shared/include/texture.h"
#include "../../shared/include/container.h"
#include <stdbool.h>

typedef struct _LevelEditor {
	Container *container;
	Level *level;
	Texture *textureArray[4][7];
	Texture *backgroundArray[4];
	Texture *blankIcon;
	int currentItem;
} LevelEditor;


typedef enum _BrickChoice {
	NUDE, TOP, RIGHT, BOTTOM, LEFT, TOPRIGHT, RIGHTBOTTOM, BOTTOMLEFT, TOPLEFT, TOPRIGHTLEFT, TOPRIGHTBOTTOM, RIGHTBOTTOMLEFT, TOPBOTTOMLEFT, TOPBOTTOM, RIGHTLEFT, ALL
} BrickChoice;

/*
  All of the functions for editing a level are within this
  It is broken into making a new leveleditor, running it,
  and destroying it
*/
void Level_Edit(Level *level);


/* Creates a new LevelEditor struct and fills it */
LevelEditor *New_LevelEditor(Level *level);

/* Destroys a LevelEditor struct */
void LevelEditor_End(LevelEditor *editor);

/* This has functions for rendering and updating a level */
void LevelEditor_Run(LevelEditor *editor);

/* Components of Run */
void LevelEditor_Render(LevelEditor *editor);

/* Components of Render (Brick Rendering) */
void LevelEditor_RenderBricks(LevelEditor *editor, int i, int j);
void Texture_RenderBrick(Texture *t, SDL_Renderer *r, int x, int y, SDL_Rect *Camera, BrickChoice b);

void LevelEditor_Update(LevelEditor *editor);

/* Components of Update  */
void LevelEditor_AssertCameraBounds(LevelEditor *editor);
bool LevelEditor_checkEditTile(LevelEditor *editor);
void LevelEditor_getCurrentTheme(LevelEditor *editor);
void LevelEditor_getCurrentItemType(LevelEditor *editor);

#endif
