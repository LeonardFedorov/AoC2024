#include "AoC.h"
#include "oStrings.h"

i64 MIN_STEP = 1;
i64 MAX_STEP = 3;

unsigned char safe_step(i64 a, i64 b, signed char* overall_direction);

Day_Result Day_2(oString* file_data, HANDLE day_heap) {

	u64 line_count;

	oString* data_as_lines = ostr_split(*file_data, NEWLINE, day_heap, &line_count);

	//Counter for the result
	u64 safe_count_1 = 0;
	u64 safe_count_2 = 0;

	//Working variables needed in the loop
	signed char overall_direction;
	signed char curr_direction;
	u64 chars_read;
	u64 cursor;
	i64 prev_num;
	i64 curr_num;
	u64 bad_count;
	
	//Iterate through the lines
	for (u64 i = 0; i < line_count; i++) {
		
		//PART 1 TEST

		//Initalise by reading the first number, and set cursor to the end of that number
		prev_num = str_parse_int(&data_as_lines[i].str[0], &cursor);
		cursor++;
		overall_direction = 0;
		
		while (1) {
			curr_num = str_parse_int(&data_as_lines[i].str[cursor], &chars_read);
			cursor += chars_read + 1;

			//If no number was read, the end of the line has been reached
			if (!chars_read) break;

			//If an unsafe step has been found, then break
			if (!safe_step(prev_num, curr_num, &overall_direction)) {
				safe_count_1 -= 1;
				break;
			}
			
			prev_num = curr_num;
		}

		safe_count_1 += 1;

		//PART 2 TEST

	}

	oString part1 = ostr_from_int(safe_count_1, day_heap);
	oString part2 = ostr_from_int(0, day_heap);

	Day_Result result = { part1, part2 };
	return result;

}

//Given a pair of numbers, test if the step between them is safe
unsigned char safe_step(i64 a, i64 b, signed char* overall_direction) {

	unsigned char result = 1;

	i64 difference = a - b;

	i64 a_difference = difference > 0 ? difference : -difference;
	signed char direction = difference > 0 ? 1 : -1;

	//If the overall direction has not been set yet, set it
	if (!*overall_direction) {
		*overall_direction = direction;
	}
	else if (direction != *overall_direction) { //If overall direction is set, fail if difference
		result = 0;
	}

	if (a_difference < MIN_STEP || a_difference > MAX_STEP) {
		result = 0;
	}

	return result;

}

//Extend an array to at least new_len. If new_len < curr_len, the array will be doubled
//The original pointer supplied will be nulled by this function - always switch to the returned pointer#
//Newly added array elemenets will not be initialised
i64* extend_array(i64* array, u64 curr_len, u64 new_len, HANDLE heap) {

	//Process optionality of new_len arg
	new_len = curr_len > new_len ? new_len << 1 : new_len;

	i64* result_array = HeapAllocEC(heap, 0, sizeof(i64) * new_len);

	for (int i = 0; i < curr_len; i++) {
		result_array[i] = array[i];
	}
	
	HeapFree(heap, 0, array);

	return result_array;

}