/* menu.h - Data type for a menu with menu elements
 *
 * Author: Sean Rapp
 */


#ifndef MENU_H
#define MENU_H

#include "../../shared/include/container.h"
#include "../include/button.h"
#include "../include/textbox.h"

/* A menu returns an action if a button was pressed, or text
 * if text was typed into a textbox and enter was pressed */
typedef struct _menu_return {
		Action action; /* The action of the button clicked */
		char *text; /* Only used if action == LOAD_LEVEL_PATH in the Load Custom Level menu */
} MenuReturn;

typedef struct _image {
	Texture *texture;
	int x;
	int y;
} Image;

/* A menu - a list of buttons on a background */
typedef struct _menu {
	/* Background of the menu */
	Texture *background;
	/* List of buttons on the menu */
	Button **buttons;
	int buttonCount; /* Count of buttons */
	int buttonsSize; /* Length of buttons array */
	/* Textboxes on the menu */
	Textbox **textboxes;
	int textboxCount; /* Count of textboxes */
	int textboxesSize; /* Length of textboxes array */

	Image **images;
	int imageCount;
	int imagesSize;

} Menu;

/* Create a new menu with a background */
Menu * New_Menu(Texture *background);

/* ******* MENU SHORTCUTS ******** */

/* Shortcut to creating the Main Menu, just calls New_Menu and Menu_AddButton etc etc */
Menu * New_MainMenu(Container *container);

/* Shortcut to creating the Load Custom Level menu, " */
Menu * New_LoadLevelMenu(Container *container);

/* " Level Editor menu */
Menu * New_LevelEditorMenu(Container *container);

/* " Load a custom level to edit menu */
Menu * New_LevelEditorLoadCustomLevelMenu(Container *container);

/* " Create a new custom level to edit - get the width */
Menu * New_LevelEditorCreateCustomHorizontalSizeLevelMenu(Container *container);

/* " " - get the height */
Menu * New_LevelEditorCreateCustomVerticalSizeLevelMenu(Container *container);

/* ******** OVERHEAD FOR EVERYTHING ******** */

/* Overhead manager for menus, basically runs the whole deal */
void RunMenuManager(Container *container);


/* ****** ACTUAL MENU FUNCTIONS ******* */
/* Add a button to a menu */
void Menu_AddButton(Menu *m, Button *b);

/* Add a textbox to a menu */
void Menu_AddTextBox(Menu *m, Textbox *t);

/* Add an image to a menu */
void Menu_AddImage(Menu *m, Texture *i, int x, int y);

/* Wait for a button to be clicked and return its action */
MenuReturn Menu_Run(Menu *m, Container *c);

/* Destroy a menu, it's buttons, and its textbox (if any added) */
void Menu_Destroy(Menu *m);

#endif
