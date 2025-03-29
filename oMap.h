#pragma once

#include "oStrings.h"

//The struct contains a pointer to an oString, which will be an array of oStrings with one entry for each row
typedef struct map {
	oString* map_data;
	u64 width;
	u64 height;
} oMap;

//This struct represents a point in a given map
typedef struct point {
	oMap* map;
	u64 row;
	u64 col;
	u64 pos; //The pos is the actual address of the position char - can de-ref to peek the value at the position
} oPoint;

//Directions ordered so that 1-4 can be iterated over when only cardinal directions are relevant
enum Direction {NoMove = 0, Up = 1, Right = 2, Down = 3, Left = 4, UpLeft = 5, UpRight = 6, DownRight = 7, DownLeft = 8};
u64 CARDINAL_UPPER = 5;
u64 DIAGONAL_UPPER = 9;

//Get a pointer to a particular position in the map for read/write
//Out of bounds query returns a null pointer
char* get_map_loc(oMap* map, u64 row, u64 col, char wrap);

//Creates a new point object. Result will be wrapped into the grid.
//If potential out of bounds is a concern, then create a point certainly inside and use the move methods
//which handle the boundary
oPoint point_new(oMap* map, u64 row, u64 col);

//Mutably moves a point in a given direction. An out of bounds move results in no movement.
void point_move_mut(oPoint* point, Direction dir, char wrap);

//Copies a point with the option to move it to an adjacent space.
oPoint point_copy(oPoint* point, Direction dir, char wrap);

//Peak at a point or an adjacent position. Out of bounds query returns 0
void peek_at_point(oPoint* point, Direction dir, char wrap);

