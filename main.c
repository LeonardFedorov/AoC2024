#include "AoC.h"
#include "oStrings.h"
#include "IO.h"

#define INTRO "###############################\n" \
			  "##### ADVENT OF CODE 2024 #####\n" \
              "##### by Oliver Ingamells #####\n" \
			  "###############################\n\n"

#define PATH_PIECES 4

char* path_prefix = "DayData\\Day";
char* path_suffix = "Input.txt";

void string_tests();
oString get_data_file(oString* path_pieces, i64 day_text, HANDLE heap);
oString* setup_path_pieces(oString exe_path, HANDLE heap);

Day_Result(*day_funcs[25])(oString*, HANDLE) = { Day_1, Day_2, Day_3, Day_4, NULL, \
												 NULL, NULL, NULL, NULL, NULL, \
												 NULL, NULL, NULL, NULL, NULL, \
												 NULL, NULL, NULL, NULL, NULL, \
												 NULL, NULL, NULL, NULL, NULL };

int main(u64 argc, char* argv[]) {

	HANDLE default_heap = GetProcessHeap();

	//Initial set up, print welcome message
	char* intro_str = INTRO;
	oString intro_txt = {intro_str, sizeof(INTRO)};
	write_ostr(intro_txt);

	//Set up the static parts of the exe path
	oString exe_path = ostr_from_NT(argv[0]);
	oString* path_pieces = setup_path_pieces(exe_path, default_heap);
	oString input_prompt = ostr_from_NT("Please select day: ");

	//Standard output messages
	oString part_a = ostr_from_NT("Part A Result: ");
	oString part_b = ostr_from_NT("Part B Result: ");
	oString time_str = ostr_from_NT("Time taken: ");

	//Variables relating to timing
	LARGE_INTEGER perf_freq;
	QueryPerformanceFrequency(&perf_freq);
	LARGE_INTEGER start_time;
	LARGE_INTEGER end_time;

	//Loop to allow as many day calculations to be run as desired.
	while (1) {

		//640k ought to be enough for anyone
		HANDLE day_heap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 640 * 1024, 0);
		
		if (!day_heap) {
			write_ostr(ostr_from_NT("Could not create heap, terminating execution"));
			break;
		}

		write_ostr(input_prompt);
		oString response = get_std_input(2, day_heap);

		i64 day_num = str_parse_int(response.str, NULL);
		//Abort if the day number is out of bounds, or there's a null pointer in that array position
		char proceed = day_num > 0 && day_num < 26 && day_funcs[day_num - 1];
		
		if (proceed) {

			oString data_file = get_data_file(path_pieces, day_num, day_heap);

			//Time only the execution itself - exclude the disk IO of the initial file read
			QueryPerformanceCounter(&start_time);
			Day_Result result = day_funcs[day_num-1](&data_file, day_heap);
			QueryPerformanceCounter(&end_time);

			//Prepare the result output strings
			i64 delta = result.partA.len - result.partB.len;
			u64 mod_delta = delta > 0 ? delta : -delta;
			oString pad = ostr_new(mod_delta, ' ', day_heap);

			//Add left padding to the shorter string
			if (delta > 0) {
				result.partB = ostr_conc(pad, result.partB, day_heap);
			}
			else if (delta < 0) {
				result.partA = ostr_conc(pad, result.partA, day_heap);
			}

			//Get the time string
			oString time_taken = format_time(&start_time, &end_time, &perf_freq, day_heap);

			//Print the results
			write_ostr(part_a); write_ostr(result.partA); write_ostr(NEWLINE);
			write_ostr(part_b); write_ostr(result.partB); write_ostr(NEWLINE);
			write_ostr(time_str); write_ostr(time_taken); write_ostr(NEWLINE);
			write_ostr(NEWLINE);

		}

		//Destroy the heap to fully clean up all the day's processing
		HeapDestroy(day_heap);

		if (!proceed) break;

	}
	
	return 0;

}

void string_tests(){

	oString test1 = ostr_from_NT("Hello World\n");
	oString test2 = ostr_copy(test1, GetProcessHeap());
	oString test3 = ostr_from_NT("Goodbye World\n");
	oString test4 = ostr_conc(test1, test3, GetProcessHeap());

	oString test5 = ostr_from_NT("He\r\nLine2\r\n\r\nline4\r\nEOF\r\n");
	u64 lines = ostr_piece_count(test5, NEWLINE);

	u64 lines_output;
	oString* test6 = ostr_split(test5, NEWLINE, GetProcessHeap(), &lines_output);

	i64 test7 = str_parse_int("123", NULL);
	u64 test8len;
	i64 test8 = str_parse_int("-12", &test8len);

	oString test9 = ostr_from_int(1534, GetProcessHeap());
	oString test10 = ostr_from_int(-23644531, GetProcessHeap());

	write_ostr(test10);

	return;

}

oString* setup_path_pieces(oString exe_path, HANDLE heap) {

	oString* path_pieces = HeapAllocEC(heap, 0, sizeof(oString) * PATH_PIECES);
	
	u64 cut_index = exe_path.len;
	u64 backslash_count = 0;

	for (; cut_index > 0; cut_index--) {
		backslash_count += (exe_path.str[cut_index - 1] == '\\');
		if (backslash_count == 3) break;
	}

	exe_path.len = cut_index;
	path_pieces[0] = exe_path;
	path_pieces[1] = ostr_from_NT(path_prefix);
	path_pieces[3] = ostr_from_NT(path_suffix);

	return path_pieces;

}

oString get_data_file(oString* path_pieces, i64 day_text, HANDLE heap) {

	path_pieces[2] = ostr_from_int(day_text, heap);

	oString file_path = ostr_conc_arr(&path_pieces[0], PATH_PIECES, heap);
	oString result = get_file(file_path, heap);

	return result;

}