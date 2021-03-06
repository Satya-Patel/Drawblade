/* objecttype.c - implementation for type ObjectType
 *
 * See objecttype.h for details.
 *
 * Author: Sean Rapp
 */

#include <SDL2/SDL.h>
#include "../include/object.h"
#include "../../shared/include/container.h"

/* Create a new Object, with a spritesheet already created */
ObjectType * New_ObjectType(Spritesheet *spritesheet, int w, int h) {

	ObjectType *ret = malloc(sizeof(ObjectType));
	if (!ret) {
		fprintf(stderr, "Error creating object\n");
		return NULL;
	}

	ret->spritesheet = spritesheet;
	ret->size.w = w;
	ret->size.h = h;

	/* Create the animations for the object */
	/* Make the animations list */
	ret->animations = malloc(spritesheet->animation_count * sizeof(SDL_Rect *));
	for (int i = 0; i < spritesheet->animation_count; i++) {
		/* Make the list of sprites in the animation */
		ret->animations[i] = malloc(spritesheet->frames_in_animation[i] * sizeof(SDL_Rect));
		for (int k = 0; k < spritesheet->frames_in_animation[i]; k++) {
			ret->animations[i][k].x = w * k;
			ret->animations[i][k].y = h * i;
			ret->animations[i][k].w = w;
			ret->animations[i][k].h = h;
		}
	}

	/* Create room for 2 instances - this can be increased when objects are added */
	ret->instances = malloc(2 * sizeof(Object));
	ret->instances_size = 2;
	ret->instance_count = 0;

	return ret;
}

/* Create a new object of an ObjectType ot */
void ObjectType_AddObject(ObjectType *ot, int x, int y) {
	/* Make sure ot->instances is large enough to have another object */
	if (ot->instance_count >= ot->instances_size) {
		/* Double the size of ot->instances */
		ot->instances = realloc(ot->instances, ot->instances_size * 2 * sizeof(Object));
		ot->instances_size *= 2;
	}

	SDL_Rect *dstrect = &ot->instances[ot->instance_count].dstrect;
	/* Copy the passed initial position */
	dstrect->x = x;
	dstrect->y = y;
	dstrect->w = ot->size.w;
	dstrect->h = ot->size.h;
	
	//Shortcut to hitboxes used in update function
	SDL_Rect *hitboxes = ot->instances[ot->instance_count].hitboxes;
	
	//In the following code, the new pointer is a shortcut to each hitbox
	//It then sets the hitboxes to that space

	SDL_Rect *temprect = &hitboxes[TOP_HITBOX];
	temprect->x = x;
	temprect->y = y;
	temprect->w = ot->size.w;
	temprect->h = 8;

	temprect = &hitboxes[LEFT_HITBOX];
	temprect->x = x;
	temprect->y = y + 8;
	temprect->w = 8;
	temprect->h = ot->size.h - 16;

	temprect = &hitboxes[RIGHT_HITBOX];
	temprect->x = x + ot->size.w - 8;
	temprect->y = y + 8;
	temprect->w = 8;
	temprect->h = ot->size.h - 16;

	temprect = &hitboxes[BOTTOM_HITBOX];
	temprect->x = x;
	temprect->y = y + ot->size.h - 8;
	temprect->w = ot->size.w;
	temprect->h = 8;

	/* Copy the passed initial animation and initial sprite */
	ot->instances[ot->instance_count].animation = 0;
	ot->instances[ot->instance_count].lastAnimation = 0;

	ot->instances[ot->instance_count].sprite_index = calloc(ot->spritesheet->animation_count, sizeof(int));
	debug_msg("Added object to ObjectType at %p, count: %d\n", ot, ot->instance_count + 1);
	ot->instance_count++;
}

/* Set the animation for an object */
void ObjectType_SetObjectAnimation(ObjectType *ot, int instance_index, int animation) {
	ot->instances[instance_index].animation = animation;
}

/* Change the object's sprite to the following sprite in the animation */
void ObjectType_ObjectNextSprite(ObjectType *ot, int instance_index) {
	bool odd = ot->instances[instance_index].animation % 2 == 1;
	/* Increment the instance's sprite index */
	ot->instances[instance_index].sprite_index[ot->instances[instance_index].animation]++;

	if (odd) {
		ot->instances[instance_index].sprite_index[ot->instances[instance_index].animation - 1]++;
	}
	else {
		ot->instances[instance_index].sprite_index[ot->instances[instance_index].animation + 1]++;
	}
}

/* Render an instance of ObjectType */
void ObjectType_RenderObject(ObjectType *ot, int instance_index, unsigned int dt, Container *container) {
	SDL_Rect *objectrefrect = &ot->instances[instance_index].dstrect;
	
	/*uses the camera to create a new location for rendering */
	SDL_Rect dstrect = {
		objectrefrect->x - container->camera->x,
		objectrefrect->y - container->camera->y,
		objectrefrect->w,
		objectrefrect->h
	};
	
	if (dstrect.x + dstrect.w < 0 || dstrect.y + dstrect.h< 0 ||
		dstrect.x > 1280 || dstrect.y > 720) {
		return;
	}

//	printf("s %d\n", ot->instances[instance_index].sprite_index[ot->instances[instance_index].animation]);
//	printf("a %d\n", ot->instances[instance_index].animation);
	//adds the object to the renderer
	SDL_RenderCopy(
		container->renderer,
		ot->spritesheet->texture->texture,
		/* NOICE */
		&ot->animations[ot->instances[instance_index].animation][ot->instances[instance_index].sprite_index[ot->instances[instance_index].animation]],
		&dstrect
	);
}

/* Destroy an ObjectType and all of its sub parts */
void Destroy_ObjectType(ObjectType *ot) {
	for (int i = 0; i < ot->spritesheet->animation_count; i++) {
		free(ot->animations[i]);
	}
	for (int i = 0; i < ot->instance_count; i++) {
		free(ot->instances[i].sprite_index);
	}
	free(ot->animations);
	free(ot->instances);
	Destroy_Spritesheet(ot->spritesheet);
	free(ot);
	ot = NULL;
}
/* Resets all of the indexes of all of the animations except for the one of the current animation and its reflection */
void ObjectType_ResetSpriteIndexes(ObjectType *ot, int ii, int animation) {
	bool odd = (animation % 2 == 1);
	for (int i = 0; i < ot->spritesheet->animation_count; i++) {
		if (i != animation && ((odd && i != animation - 1) || (!odd && i != animation + 1))) {
			ot->instances[ii].sprite_index[i] = 0;
		}
	}
}
