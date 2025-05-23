#include "AoC.h"
#include "oStrings.h"
#include "oMap.h"

char START = 'X';
oString SEARCH = { "MAS", 3 };

char START_2 = 'A';

Day_Result Day_4(oString* file_data, HANDLE day_heap) {

	//Prep - parse file data as a map and create somewhere to store rays
	oMap data = map_from_ostr(file_data, NEWLINE, day_heap);
	char* ray_store = HeapAllocEC(day_heap, 0, SEARCH.len * sizeof(char));
	bool wrap_flag;
	oPoint target_point;
	char* four_corners = HeapAllocEC(day_heap, 0, 4 * sizeof(char));

	//Part 1 - iterate through each possible start point

	u64 part_1_count = 0;
	u64 part_2_count = 0;
	u64 part_2_subcount = 0;

	for (int row = 0; row < data.height; row++) {
		for (int col = 0; col < data.width; col++) {

			target_point = point_new_unsafe(&data, row, col);

			//Perform part 1 test
			if (*target_point.pos == START) {

				for (u64 dir = ALL_LOWER; dir < ALL_UPPER_MOVES; dir++) {

					//Read the ray from our point in each direction. Don't check if the ray went off the edge
					get_ray(target_point, dir, SEARCH.len, ray_store, &wrap_flag);

					if (!wrap_flag) {
						part_1_count += is_string(ray_store, SEARCH);
					}

				}

			}

			//Perform part 2 test - only needed if point is not on the edge
			if (*target_point.pos == START_2 &&
				row > 0 &&
				col > 0 &&
				row < data.height - 1 &&
				col < data.width - 1) {

				//Get the chars in the 4 corners and then test if the pairs exist
				for (u64 dir = DIAGONAL_LOWER; dir < DIAGONAL_UPPER; dir++) {
					four_corners[dir - DIAGONAL_LOWER] = map_peek_value(&target_point, 1, dir, NULL);
				}

				part_2_subcount = 0;
				for (u64 dir = DIAGONAL_LOWER; dir < DIAGONAL_UPPER; dir++) {
					part_2_subcount += (four_corners[dir - DIAGONAL_LOWER] == 'M') && (four_corners[reverse_move(dir) - DIAGONAL_LOWER] == 'S');
				}

				part_2_count += part_2_subcount == 2;

			}
		}
	}

	oString part1 = ostr_from_int(part_1_count, day_heap);
	oString part2 = ostr_from_int(part_2_count, day_heap);

	Day_Result result = { part1, part2 };
	return result;

}