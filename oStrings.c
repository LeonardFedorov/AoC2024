#include "oStrings.h"

oString NEWLINE = { "\r\n", 2 };
oString EMPTY = { NULL, 0 };
u64 MAX_INT_LEN = 32;

char is_string(char* txt, oString target) {

	char result = 1;

	for (int i = 0; i < target.len; i++) {
		if (txt[i] != target.str[i]) {
			result = 0;
			break;
		}
	}

	return result;

}

oString ostr_from_NT(char* source) {

	int length = 0;
	while (source[length++] != '\0');
	oString result = { source, length - 1 };
	return result;

}

oString ostr_new(u64 length, char init, HANDLE heap) {

	char* result_txt = HeapAllocEC(heap, 0, length + 1);
	oString* result = HeapAllocEC(heap, 0, sizeof(oString));

	for (int i = 0; i < length; i++) {
		result_txt[i] = init;
	}
	result_txt[length] = '\0';
	result->str = result_txt;
	result->len = length;

	return *result;

}

oString ostr_copy(oString source, HANDLE heap) {

	char* destination = HeapAllocEC(heap, 0, source.len + 1);
	
	for (int i = 0; i < source.len; i++) {
		destination[i] = source.str[i];
	}
	destination[source.len] = '\0';

	oString result = { destination, source.len };
	return result;

}

oString ostr_conc(oString str1, oString str2, HANDLE heap) {

	char* destination = HeapAllocEC(heap, 0, str1.len + str2.len + 1);

	int i;
	for (i = 0; i < str1.len; i++) {
		destination[i] = str1.str[i];
	}
	for (i = 0; i < str2.len; i++) {
		destination[i + str1.len] = str2.str[i];
	}
	destination[str1.len + str2.len] = '\0';

	oString result = { destination, str1.len + str2.len };
	return result;
}

oString ostr_conc_arr(oString* strs, u64 count, HANDLE heap) {

	u64 size = 0;
	for (int i = 0; i < count; i++) {
		size += strs[i].len;
	}

	char* result_txt = HeapAllocEC(heap, 0, size + 1);

	u64 cursor = 0;
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < strs[i].len; j++) {
			result_txt[cursor++] = strs[i].str[j];
		}
	}

	result_txt[cursor] = '\0';

	oString result = { result_txt, size };
	return result;

}

u64 ostr_piece_count(oString src, oString delimiter) {

	if (src.len == 0) return 0;

	int line_count = 1;
	u64 test_bound = src.len - (delimiter.len - 1);

	for (int i = 0; i < test_bound; i++) {
		if (is_string(&src.str[i], delimiter)) line_count++;
	}

	return line_count;
}

oString* ostr_split(oString src, oString delimiter, HANDLE heap, u64* piece_count) {

	//Get the number of pieces and store this value in the caller's pointer if provided
	u64 pieces = ostr_piece_count(src, delimiter);
	if (piece_count) *piece_count = pieces;

	if (pieces == 0) return NULL;

	oString* str_array = HeapAllocEC(heap, 1, pieces * sizeof(oString));

	u64 cursor = 0;
	u64 start_pos = cursor;
	u64 piece_counter = 0;

	while (cursor <= src.len - delimiter.len) {
		
		if (is_string(&src.str[cursor], delimiter)) {
			//Having found the end of the line, store this line's details
			str_array[piece_counter].str = &src.str[start_pos];
			str_array[piece_counter].len = cursor - start_pos;

			//Advance the counters
			piece_counter++;
			cursor += delimiter.len;
			start_pos = cursor;
		}
		else {
			cursor++;
		}

	}

	//At this point, there cannot be another new line
	str_array[piece_counter].len = src.len - start_pos;
	//If the string ends in a new line, then the last line will be a null string
	str_array[piece_counter].str = src.len - start_pos == 0 ? NULL : &src.str[start_pos];

	return str_array;
}

i64 str_parse_int(char* text, u64* chars_read) {

	i64 result = 0;
	u64 cursor = 0;
	signed char negative = 1; 

	//If a minus sign is present, record this and advance the cursor
	if (text[0] == '-') {
		negative = -1;
		cursor = 1;
	}

	while (text[cursor] >= '0' && text[cursor] <= '9') {
		result *= 10;
		result += text[cursor++] - '0';
	}

	result *= negative;
	
	//If a non-null pointer was passed, store the chars read count in it
	if (chars_read) *chars_read = cursor;

	return result;

}

//Converts a signed integer into a string
oString ostr_from_int(i64 number, HANDLE heap) {

	//Allocate some space. This is a temporary allocation since we need to write the number backwards initially
	char* num_temp = HeapAllocEC(heap, 0, MAX_INT_LEN);
	u64 cursor = 0;
	char negative = 0;

	//Handle negative number by storing this fact and 
	//flipping sign so the rest of the function can assume the number is positive
	if (number < 0) {
		negative = 1;
		number *= -1;
	}

	int remainder;

	while (number > 0) {
		remainder = number % 10;
		number /= 10;
		num_temp[cursor++] = remainder + '0';
		if (cursor % 4 == 3 && number > 0) num_temp[cursor++] = ',';
	}

	if (negative) num_temp[cursor++] = '-';

	//Assign the permanent space that the output will use
	char* num_out = HeapAllocEC(heap, 0, cursor);
	
	for (int i = 0; i < cursor; i++) {
		num_out[i] = num_temp[cursor - 1 - i];
	}

	//Discard the temporary space
	HeapFree(heap, 0, num_temp);

	oString result = {num_out , cursor};
	return result;

}