#include "AoC.h"
#include "oStrings.h"

Day_Result Day_1(oString* file_data, HANDLE day_heap) {

	//Prep
	u64 line_count = 0;
	oString* as_lines = ostr_split(*file_data, NEWLINE, day_heap, &line_count);

	i64* left_col = HeapAllocEC(day_heap, 0, 2 * sizeof(i64) * line_count);
	i64* right_col = &left_col[line_count];

	u64 num_length;

	//Parse the numbers out of the file data. There are 3 spaces between the columns
	for (int i = 0; i < line_count; i++) {
		left_col[i] = str_parse_int(&as_lines[i].str[0], &num_length);
		right_col[i] = str_parse_int(&as_lines[i].str[num_length + 3], NULL);
	}

	//Part 1

	//Sort the two columns in-place
	q_sort(left_col, line_count);
	q_sort(right_col, line_count);

	i64 distance;
	i64 distance_sum = 0;

	for (int i = 0; i < line_count; i++) {
		distance = left_col[i] - right_col[i];
		distance_sum += distance > 0 ? distance : -distance;
	}

	//Part 2
	//Given the lists are already in order, the search can be made easier for part 2.
	i64 similarity_score;
	i64 similarity_total = 0;

	for (int i = 0; i < line_count; i++) {
		similarity_score = 0;

		for (int j = 0; j < line_count; j++) {

			if (right_col[j] == left_col[i]) {
				similarity_score += 1;
			}
			else if (right_col[j] > left_col[i]) {
				break; //Can abort here since the list is in order
			}

		}

		similarity_score *= left_col[i];
		similarity_total += similarity_score;
	}

	oString part1 = ostr_from_int(distance_sum, day_heap);
	oString part2 = ostr_from_int(similarity_total, day_heap);

	Day_Result result = { part1, part2 };
	return result;

}