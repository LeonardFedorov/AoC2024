#include "AoC.h"
#include "oStrings.h"

#define BUBBLE_LIMIT 4

//Memory allocation function which crudely error checks by throwing an
//unrecoverable exception if the allocation fails
void* HeapAllocEC(HANDLE heap, DWORD zero_init, u64 bytes) {

	if (zero_init) zero_init = 8;

	void* target = HeapAlloc(heap, zero_init + HEAP_GENERATE_EXCEPTIONS, bytes);
	if (target) {
		return target;
	}
	else {
		//This will occur if the memory allocation fails
		RaiseException(1, EXCEPTION_NONCONTINUABLE, 0, NULL);
	}
	return NULL;
}

//Performs bubble_sort - only intended to be called on small lists
void bubble_sort(i64* list, u64 len) {

	if (len == 1) {
		return;
	}

	u64 swaps = 1;
	i64 temp;

	//Loop forever until a pass where no swaps are required is performed
	while (swaps > 0) {
		swaps = 0;
		for (int i = 1; i < len; i++) {
			if (list[i] < list[i - 1]) {
				swaps++;
				temp = list[i];
				list[i] = list[i - 1];
				list[i - 1] = temp;
			}
		}
	}

	return;
}

//Performs in place sort on an integer array
void q_sort(i64* list, u64 len) {

	register i64 temp;
	i64 pivot;
	u64 upper;

	if (len < BUBBLE_LIMIT) {
		//If the list is small, perform bubble sort on it
		bubble_sort(list, len);
		return;
	}
	else {
		pivot = list[0]; //Use left-most element as pivot
		upper = len - 1;

		//If the upper value is already greater than the pivot, then decrement upper
		//This loop will halt because list[0] is the pivot itself
		while (list[upper] > pivot) { upper--; }

		for (int i = 1; i <= upper; i++) {
			//Swap the element to the top part of the array if it's greater than the pivot
			if (list[i] > pivot) {
				temp = list[i];
				list[i] = list[upper];
				list[upper] = temp;
				upper--;
				//Check again that the next upper isn't greater than the pivot
				while (list[upper] > pivot) { upper--; }
			}
		}

		//Once the loop is finished, swap the pivot to below the upper part
		list[0] = list[upper];
		list[upper] = pivot;

		//If they exist, recursively call the function on the portions above and below the pivot
		if (upper > 0) {
			q_sort(&list[0], upper);
		}
		if (upper < len - 1) {
			q_sort(&list[upper + 1], len - 1 - upper);
		}
		return;
	}
}

oString format_time(LARGE_INTEGER* start, LARGE_INTEGER* end, LARGE_INTEGER* freq, HANDLE heap) {

	u64 start_tick = start->QuadPart;
	u64 end_tick = end->QuadPart;
	u64 frequency = freq->QuadPart;

	//Since frequency is ticks per second, dividing by a million is ticks per microsecond
	u64 micros = (end_tick - start_tick) / (frequency / 1'000'000);

	char unit;
	i64 quantity;
	u64 unit_len;

	if (micros < 10'000) {
		unit = 230; //This isn't the standard extended ASCII codepoint for mu, but this is what works in my Windows 11 terminal apparently
		quantity = micros;
		unit_len = 3;
	}
	else if (micros < 10'000'000) {
		unit = 'm';
		quantity = micros / 1'000;
		unit_len = 3;
	}
	else {
		unit = '\0';
		quantity = micros / 1'000'000;
		unit_len = 2;
	}

	oString number = ostr_from_int(quantity, heap);
	oString text = ostr_new(number.len + unit_len, 0, heap);

	//Write the string data into text
	for (int i = 0; i < number.len; i++) {
		text.str[i] = number.str[i];
	}
	text.str[number.len] = ' ';
	if (unit) text.str[number.len + 1] = unit;
	text.str[number.len + unit_len - 1] = 's';

	return text;

}