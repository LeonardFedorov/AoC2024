#pragma once

//Common headers required for all files
#include <windows.h>

//Convenience typedefs
typedef signed long long i64;
typedef unsigned long long u64;
typedef unsigned char bool;

//String construct used throughout - functions laid out in oStrings.h
typedef struct ostr {
	char* str;
	u64 len;
} oString;

typedef struct day_o {
	oString partA;
	oString partB;
} Day_Result;

//Misc utility functions not part of a bigger library
extern void* HeapAllocEC(HANDLE heap, DWORD flags, u64 bytes);
extern void q_sort(i64* list, u64 len);

extern Day_Result Day_1(oString* file_data, HANDLE day_heap);
extern Day_Result Day_2(oString* file_data, HANDLE day_heap);
extern Day_Result Day_3(oString* file_data, HANDLE day_heap);
extern Day_Result Day_4(oString* file_data, HANDLE day_heap);
extern Day_Result Day_5(oString* file_data, HANDLE day_heap);
extern Day_Result Day_6(oString* file_data, HANDLE day_heap);
extern Day_Result Day_7(oString* file_data, HANDLE day_heap);
extern Day_Result Day_8(oString* file_data, HANDLE day_heap);
extern Day_Result Day_9(oString* file_data, HANDLE day_heap);
extern Day_Result Day_10(oString* file_data, HANDLE day_heap);
extern Day_Result Day_11(oString* file_data, HANDLE day_heap);
extern Day_Result Day_12(oString* file_data, HANDLE day_heap);
extern Day_Result Day_13(oString* file_data, HANDLE day_heap);
extern Day_Result Day_14(oString* file_data, HANDLE day_heap);
extern Day_Result Day_15(oString* file_data, HANDLE day_heap);
extern Day_Result Day_16(oString* file_data, HANDLE day_heap);
extern Day_Result Day_17(oString* file_data, HANDLE day_heap);
extern Day_Result Day_18(oString* file_data, HANDLE day_heap);
extern Day_Result Day_19(oString* file_data, HANDLE day_heap);
extern Day_Result Day_20(oString* file_data, HANDLE day_heap);
extern Day_Result Day_21(oString* file_data, HANDLE day_heap);
extern Day_Result Day_22(oString* file_data, HANDLE day_heap);
extern Day_Result Day_23(oString* file_data, HANDLE day_heap);
extern Day_Result Day_24(oString* file_data, HANDLE day_heap);
extern Day_Result Day_25(oString* file_data, HANDLE day_heap);


extern oString format_time(LARGE_INTEGER* start, LARGE_INTEGER* end, LARGE_INTEGER* freq, HANDLE heap);


