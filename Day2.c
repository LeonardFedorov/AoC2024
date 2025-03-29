#include "AoC.h"
#include "oStrings.h"

i64 MIN_STEP = 1;
i64 MAX_STEP = 3;

unsigned char safe_step(i64 a, i64 b, signed char* overall_direction);
unsigned char list_safe(i64* list, u64 list_len);

void extend_array(i64** array, u64* curr_len, u64 new_len, HANDLE heap);

Day_Result Day_2(oString* file_data, HANDLE day_heap) {

	u64 line_count;

	oString* data_as_lines = ostr_split(*file_data, NEWLINE, day_heap, &line_count);

	//Counter for the results
	u64 safe_count_1 = 0;
	u64 safe_count_2 = 0;

	//Working variables needed in the loop
	signed char overall_direction;
	u64 chars_read;
	u64 cursor;
	i64 curr_num;

	i64* number_list = HeapAllocEC(day_heap, 0, sizeof(i64));
	i64* number_list_2 = HeapAllocEC(day_heap, 0, sizeof(i64));
	u64 list_len = 1;
	u64 list_len_2 = 1;
	u64 line_len;

	unsigned char safe;

	for (u64 i = 0; i < line_count; i++) {

		//Read the line as an array of ints
		line_len = 0;
		cursor = 0;
		while (1) {
			curr_num = str_parse_int(&data_as_lines[i].str[cursor], &chars_read);
			cursor += chars_read + 1;

			//If no number was read, the end of the line has been reached
			if (!chars_read) break;

			line_len++;
			if (line_len > list_len) {
				extend_array(&number_list, &list_len, 0, day_heap);
			}
			number_list[line_len - 1] = curr_num;
		}

		//Perform part 1 safety test
		safe = list_safe(number_list, line_len);

		safe_count_1 += safe;

		//If part 1 was safe, then part 2 is also safe with no further testing needed
		safe_count_2 += safe;
		if (!safe) {

			if (line_len > list_len_2) {
				extend_array(&number_list_2, &list_len_2, line_len, day_heap);
			}

			//Start by copying the line list, less the first element
			for (int j = 1; j < line_len; j++) {
				number_list_2[j - 1] = number_list[j];
			}

			//Search by trying to skip each item and running the test until a valid combination is found
			for (int j = 0; j < line_len - 1; j++) {

				safe = list_safe(number_list_2, line_len - 1);

				if (safe) {
					break;
				}
				else {
					number_list_2[j] = number_list[j];
				}

			}

			//Test the final list config
			safe = list_safe(number_list_2, line_len - 1);
			safe_count_2 += safe;

		}

	}


	oString part1 = ostr_from_int(safe_count_1, day_heap);
	oString part2 = ostr_from_int(safe_count_2, day_heap);

	Day_Result result = { part1, part2 };
	return result;

}

//Given a list of numbers, test whether they form a safe list
unsigned char list_safe(i64* list, u64 list_len) {

	char overall_direction = 0;
	char safe = 1;

	for (u64 j = 0; j < list_len - 1; j++) {
		if (!safe_step(list[j], list[j + 1], &overall_direction)) {
			safe = 0;
			break;
		}
	}

	return safe;

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

//Extend an array to at least new_len. Array will always be at least doubled
//Receives the address of the existing pointer so it can be updated in place
//Newly added array elemenets will not be initialised
void extend_array(i64** array, u64* curr_len, u64 new_len, HANDLE heap) {

	u64 curr_len_local;
	i64* old_array = *array;

	if (!curr_len) {
		RaiseException(1, EXCEPTION_NONCONTINUABLE, 0, NULL);
	}
	else {
		curr_len_local = *curr_len << 1;

		new_len = curr_len_local > new_len ? curr_len_local : new_len;

		*array = HeapAllocEC(heap, 0, sizeof(i64) * new_len);

		for (int i = 0; i < *curr_len; i++) {
			(*array)[i] = old_array[i];
		}

		HeapFree(heap, 0, old_array);

		*curr_len = new_len;

	}
}