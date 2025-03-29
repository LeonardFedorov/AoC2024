#pragma once

#include "AoC.h"
#include "oStrings.h"

//Writes an oString to the standard output. No new line is inserted
void write_ostr(oString data);

//Reads data from the standard input and puts it in an oString
//All buffered input beyond the specified chars to read will be discarded
//Read data will be placed in the specified heap
oString get_std_input(u64 chars_to_read, HANDLE heap);

//Read the contents of the specified file into a given heap, returned as an
//unprocessed oString. The whole file will be read, with the length stored in the oString
//Path must be null terminated.
oString get_file(oString path, HANDLE heap);