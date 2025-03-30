#include "AoC.h"
#include "oStrings.h"

oString MUL_PREFIX = { "mul(", 4 };
u64 MIN_MUL = sizeof("MUL(1,1)") - 1; //subtract one because this sizeof includes \0 terminator

oString DO = { "do()", 4 };
oString DONOT = { "don't()", 7 };

bool is_mult(char* str, u64 len, i64* result, u64* mul_len);

Day_Result Day_3(oString* file_data, HANDLE day_heap) {

	i64 part_1 = 0;
	i64 part_2 = 0;
	u64 cursor = 0;
	i64 mul_res;
	u64 mul_len;

	bool no_try = 1; //Only options are do or do not

	//Halt the loop once we are close enough to the end of the string that there could not possibly be another MUL
	while (cursor < file_data->len - MIN_MUL) {

		u64 cursor_address = file_data->str + cursor;

		//Checking them in size order will save time overall. If a positive match is found, push the cursor along
		//by the length of the relevant item as no need to keep searching inside it
		if (is_string(cursor_address, DO)) {
			no_try = 1;
			cursor += DO.len;
		}
		else if (is_string(cursor_address, DONOT)) {
			no_try = 0;
			cursor += DONOT.len;
		} 
		else if (is_mult(cursor_address, file_data->len - cursor, &mul_res, &mul_len)) {
			part_1 += mul_res;
			part_2 += mul_res * no_try;
			cursor += mul_len;
		}
		else
		{
			cursor++; //If nothing interesting found, go forward one char
		}
	}

	oString part1 = ostr_from_int(part_1, day_heap);
	oString part2 = ostr_from_int(part_2, day_heap);

	Day_Result result = { part1, part2 };
	return result;

}

//Test if the text from str contains a valid MUL expression. If it does,
//the MUL result is written into the result pointer.
//The length of the MUL express is written to mul_len only if a valid MUL is found
bool is_mult(char* str, u64 len, i64* result, u64* mul_len) {

	i64 int1;
	i64 int2;

	//Each time cursor is incremented, need to check we haven't run off the end of the string by comparing to len
	u64 cursor = 0;

	u64 chars_read;

	//Test if the prefix is present
	if (!is_string(&str[cursor], MUL_PREFIX)) {
		return 0;
	}

	cursor += MUL_PREFIX.len;
	if (cursor >= len) return 0;
	
	//Check for first number
	int1 = str_parse_int(&str[cursor], &chars_read);
	
	//If zero chars were read, then this wasn't really a number so abort here
	if (!chars_read) {
		return 0;
	}

	cursor += chars_read;
	if (cursor >= len) return 0;

	//Check for the comma separator
	if (str[cursor++] != ',') {
		return 0;
	}
	if (cursor >= len) return 0;

	//Check for the second number
	int2 = str_parse_int(&str[cursor], &chars_read);

	//If zero chars were read, then this wasn't really a number so abort here
	if (!chars_read) {
		return 0;
	}

	cursor += chars_read;
	if (cursor >= len) return 0;

	//Check for the closing paranthesis
	if (str[cursor++] != ')') {
		return 0;
	}

	//Valid mul - prepare results
	if (mul_len) *mul_len = cursor;
	if (result) *result = int1 * int2;
	return 1;

}