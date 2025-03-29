#include "IO.h"

//Writes an oString to the standard output. No new line is inserted
void write_ostr(oString data) {

	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsoleA(out, data.str, (DWORD)data.len, NULL, NULL);
	return;

}

oString get_std_input(u64 chars_to_read, HANDLE heap) {

	HANDLE in = GetStdHandle(STD_INPUT_HANDLE);

	//Double the buffer size because the data will come double-byte formatted
	WCHAR* data_in = HeapAllocEC(heap, 0, sizeof(WCHAR) * chars_to_read);
	u64 actual_read = 0;

	ReadConsole(in, data_in, (DWORD)chars_to_read, &actual_read, NULL);

	//Store the chars we were interested in
	char* data_filter = HeapAllocEC(heap, 0, actual_read);

	for (int i = 0; i < actual_read; i++) {
		//Discard any crazy unicode nonsense and keep only ASCII
		data_filter[i] = data_in[i] > 255 ? 0 : (char)data_in[i];
	}

	//If our read request was fully fulfilled, there might be more in the buffer, so check for it
	if (actual_read >= chars_to_read) {
		if (data_filter[actual_read - 1] == '\n') {
			//If the last read character was newline, then the buffer is empty so no further action needed
		}
		else {
			while (data_in[0] != '\n') {
				ReadConsole(in, data_in, 1, &actual_read, NULL);
			}
		}
	}

	HeapFree(heap, 0, data_in);

	oString result = { data_filter, actual_read };
	return result;

}

oString get_file(oString path, HANDLE heap) {

	HANDLE file_handle = CreateFileA(
		path.str,
		GENERIC_READ,
		FILE_SHARE_READ,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	LARGE_INTEGER file_sizeLI;
	GetFileSizeEx(file_handle, &file_sizeLI);

	char* fileData = HeapAllocEC(heap, 0, file_sizeLI.QuadPart);
	BOOL read_outcome = ReadFile(file_handle, fileData, (DWORD)file_sizeLI.QuadPart, NULL, NULL);

	//Throw an exception if the file read reports an error
	if (!read_outcome) { RaiseException(2, EXCEPTION_NONCONTINUABLE, 0, NULL); }

	oString result = { fileData, file_sizeLI.QuadPart };
	return result;

}