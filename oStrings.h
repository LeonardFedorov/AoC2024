#pragma once
#include "AoC.h"

//Defines a string-like struct for handling character arrays.
//Because these strings can reference subsets of other strings, the char array
//should NOT be assumed to be null terminated after len chars in general, function comments
//will signpost where this behaviour is guaranteed
//Null string is indicated by len=0. The contents of the pointer are not guaranteed in this case

extern oString NEWLINE;

//Creates an oString object from a null terminated string.
//oString points to original string address, does not copy original
oString ostr_from_NT(char* source);

//String creation routines - these create new strings. It is guaranteed that:
//The result will be null terminated
//The pointer to the string will be heap allocation pointer, so can be freed later if needed
	
	//Creates a copy of the source string in the target heap
	//Result will be null terminated
	oString ostr_copy(oString source, HANDLE heap);

	//Creates a new string of a specific length initalised to a constant char
	//Result will be null terminated
	oString ostr_new(u64 length, char init, HANDLE heap);

	//Concatenates two strings and places the result in the target heap
	oString ostr_conc(oString str1, oString str2, HANDLE heap);

	//Concatenates an array of oStrings and places the result in the target heap
	oString ostr_conc_arr(oString* strs, u64 count, HANDLE heap);

	//Converts a signed integer into a string
	oString ostr_from_int(i64 number, HANDLE heap);

//String processing - these create oString(s) which relate into existing strings. These are not
//(and indeed are actively unlikely) to be null terminated and the pointers will not be valid heap targets
//It is incumbent on the caller to track the underlying string object to manage safe memory management

	//Counts how many pieces (separated by delimiter) are present in the oString
	u64 ostr_piece_count(oString src, oString delimiter);

	//Breaks an oString into pieces by a delimiter, returning an array of oStrings with each one representing the pieces
	//These pointers reference the original string in memory, no copy is created
	//The oString lengths will be set to exclude the delimiting character(s)
	oString *ostr_split(oString src, oString delimiter, HANDLE heap, u64 *piece_count);


//Parses an integer from the text starting at the pointer. Can return how many chars made up the int
//via optional pointer argument (pass NULL if not needed). 
//The number is assumed to contain no place separators
//If there are no numeric chars at the address, then the return is 0 along with 0 chars_read
//A lone minus sign will be read as 0.
i64 str_parse_int(char* text, u64 *chars_read);

//Tests if the txt at pointer txt is in fact target. It is incumbent on the caller to ensure there is significant length in txt
//to avoid a potential buffer overrun
char is_string(char* txt, oString target);