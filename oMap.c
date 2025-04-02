#include "oMap.h"

u64 CARDINAL_LOWER = 0;
u64 CARDINAL_UPPER = 4;
u64 DIAGONAL_LOWER = 4;
u64 DIAGONAL_UPPER = 8;
u64 ALL_LOWER = 0;
u64 ALL_UPPER_MOVES = 8;
u64 ALL_UPPER = 9;


enum Direction reverse(enum Direction dir) {
	if (dir = NoMove) return NoMove;
	else return reverse_move(dir);
}

inline enum Direction reverse_move(enum Direction dir) {
	return dir ^ 1;
}

oMap map_from_ostr(oString* data, oString row_delimiter, HANDLE heap) {

	//First split the oString into lines
	u64 row_count;
	oString* as_lines = ostr_split(*data, NEWLINE, heap, &row_count);

	oMap result = { as_lines, as_lines[0].len, row_count};
	return result;
}

u64 move_col(oMap* map, u64 curr_col, u64 dist, enum Direction dir, bool* wrap) {

	i64 delta;

	switch (dir) {
	case NoMove: case Up: case Down:
		delta = 0;
		break;
	case Left: case UpLeft: case DownLeft:
		delta = -(i64)dist;
		break;
	case Right: case UpRight: case DownRight:
		delta = (i64)dist;
		break;
	default:
		delta = 0;
	}

	u64 new_col = curr_col + delta;
	bool wrap_local = 0;

	//Check for underflow first
	if (new_col >= map->width + dist) {
		new_col += map->width;
		wrap_local = 1;
	}
	else if (new_col >= map->width) {
		new_col -= map->width;
		wrap_local = 1;
	}

	if (wrap) *wrap = wrap_local;

	return new_col;

}

u64 move_row(oMap* map, u64 curr_row, u64 dist, enum Direction dir, bool* wrap) {

	i64 delta;

	switch (dir) {
	case NoMove: case Left: case Right:
		delta = 0;
		break;
	case Up: case UpLeft: case UpRight:
		delta = -(i64)dist;
		break;
	case Down: case DownLeft: case DownRight:
		delta = (i64)dist;
		break;
	default:
		delta = 0;
	}

	u64 new_row = curr_row + delta;
	bool wrap_local = 0;

	//Check for underflow first
	if (new_row >= map->height + dist) {
		new_row += map->height;
		wrap_local = 1;
	}
	else if (new_row >= map->height) {
		new_row -= map->height;
		wrap_local = 1;
	}

	if (wrap) *wrap = wrap_local;

	return new_row;

}

char* get_map_loc(oMap* map, u64 row, u64 col, bool* wrap) {
	
	u64 row_new = row % map->height;
	u64 col_new = col % map->width;
	if (wrap) *wrap = (row != row_new || col != col_new);

	return get_map_loc_unsafe(map, row_new, col_new);
}
inline char* get_map_loc_unsafe(oMap* map, u64 row, u64 col) {
	return &(map->map_data[row].str[col]);
}

oPoint point_new(oMap* map, u64 row, u64 col, bool* wrap) {
	
	u64 row_new = row % map->height;
	u64 col_new = col % map->width;
	if (wrap) *wrap = (row != row_new || col != col_new);

	return point_new_unsafe(map, row_new, col_new);
}
inline oPoint point_new_unsafe(oMap* map, u64 row, u64 col) {
	oPoint result = { map, row, col, get_map_loc_unsafe(map, row,col) };
	return result;
}

void get_ray(oPoint point, enum Direction dir, u64 len, char* data_out, bool* wrap) {

	if (wrap) *wrap = 0;
	bool wrap_local;

	for (int i = 0; i < len; i++) {
		point_move_mut(&point, 1, dir, &wrap_local);
		if (wrap) *wrap |= wrap_local;
		data_out[i] = *point.pos;
	}

	return;
}

void point_move_mut(oPoint* point, u64 dist, enum Direction dir, bool* wrap) {

	bool row_wrap = 0;
	bool col_wrap = 0;

	u64 row_moved = move_row(point->map, point->row, dist, dir, &row_wrap);
	u64 col_moved = move_col(point->map, point->col, dist, dir, &col_wrap);

	if (wrap) *wrap = (row_wrap || col_wrap);

	point->row = row_moved;
	point->col = col_moved;
	point->pos = get_map_loc_unsafe(point->map, row_moved, col_moved);

	return;

}

oPoint point_copy(oPoint* point, u64 dist, enum Direction dir, bool* wrap) {

	bool row_wrap = 0;
	bool col_wrap = 0;

	u64 row_moved = move_row(point->map, point->row, dist, dir, &row_wrap);
	u64 col_moved = move_col(point->map, point->col, dist, dir, &col_wrap);

	if (wrap) *wrap = (row_wrap || col_wrap);

	oPoint result = { point->map, row_moved, col_moved, get_map_loc_unsafe(point->map, row_moved, col_moved) };

	return result;

}

char map_peek_value(oPoint* point, u64 dist, enum Direction dir, bool* wrap) {

	//0 dir = NoMove
	if (!dir) return *point->pos;

	bool row_wrap = 0;
	bool col_wrap = 0;

	u64 row_moved = move_row(point->map, point->row, dist, dir, &row_wrap);
	u64 col_moved = move_col(point->map, point->col, dist, dir, &col_wrap);

	if (wrap) *wrap = (row_wrap || col_wrap);

	return *get_map_loc_unsafe(point->map, row_moved, col_moved);

}