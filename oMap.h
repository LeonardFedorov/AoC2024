#pragma once

#include "oStrings.h"

//The struct contains a pointer to an oString, which will be an array of oStrings with one entry for each row
//The map's design implicitly assumes that all rows are the same width
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
	char* pos; //The pos is the actual address of the position char - can de-ref to peek the value at the position
} oPoint;

//Directions ordered so that 1-4 can be iterated over when only cardinal directions are relevant
enum Direction {Up = 0, Down = 1, Right = 2, Left = 3, UpLeft = 4, DownRight = 5, UpRight = 6, DownLeft = 7, NoMove = 8};

u64 CARDINAL_LOWER;
u64 CARDINAL_UPPER;
u64 DIAGONAL_LOWER;
u64 DIAGONAL_UPPER;
u64 ALL_LOWER;
u64 ALL_UPPER_MOVES;
u64 ALL_UPPER;

//Reverses the given direction. Use reverse_move if you know NoMove is not a valid possibility
enum Direction reverse(enum Direction dir);
inline enum Direction reverse_move(enum Direction dir);

//Builds a map from an oString where the rows are separated by a delimiter. This function does not validate that all
//rows are the same width. The first row is the only one explicitly measured.
//The map is definded referring to the oString's underlying data. No copying is performed.
oMap map_from_ostr(oString* data, oString row_delimiter, HANDLE heap);

//Returns new column/row co-ordinate after moving dist in dir. Returned value will be wrapped, but pointer can be passed to
//receive a bool indicating whether this occurred
u64 move_col(oMap* map, u64 curr_col, u64 dist, enum Direction dir, bool* wrap);
u64 move_row(oMap* map, u64 curr_row, u64 dist, enum Direction dir, bool* wrap);

//Get a ray of points in direction dir from point with distance len written to existing memory block data_out
//Ray will wrap - the wrap bool pointer can be used to receive a flag as to whether this occurred
void get_ray(oPoint point, enum Direction dir, u64 len, char* data_out, bool* wrap);

//Get a pointer to a particular position in the map for read/write
//Safe will wrap to the grid, optional bool pointer returns if this occured
//Unsafe performs no checking but is faster. Caller must check points are in bounds or bad things can happen
char* get_map_loc(oMap* map, u64 row, u64 col, bool* wrap);
inline char* get_map_loc_unsafe(oMap* map, u64 row, u64 col);

//Creates a new point object. Safe version will be wrapped into the grid.
//Bool pointer reports if this occurs
//Unsafe will perform no wrap check, caller must check points are in bounds
oPoint point_new(oMap* map, u64 row, u64 col, bool* wrap);
inline oPoint point_new_unsafe(oMap* map, u64 row, u64 col);

//Mutably moves a point in a given direction. Bool pointer reports if a wrap occurred
void point_move_mut(oPoint* point, u64 dist, enum Direction dir, bool* wrap);

//Creates a point from an existing point, with the option to move it to a nearby space.
oPoint point_copy(oPoint* point, u64 dist, enum Direction dir, bool* wrap);

//Gets the value from a point or a nearby point
char map_peek_value(oPoint* point, u64 dist, enum Direction dir, bool* wrap);