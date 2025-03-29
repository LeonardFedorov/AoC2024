# AoC2024

Solutions for Advent of Code 2024 written in C. No particular paradigm or approach is targeted, beyond striving to have solutions sufficiently general to be able to read input data from any user and not be overfitted to my own. I have not used the standard library for anything - as an academic exercise I have directly used the Win32 APIs for I/O and memory management, on which I have built my own data structures. 

## Day 1
A straight forward enough start. The only interesting trick here was realising that instead of allocating two arrays of length n for the left and right columns respectively, I could allocate an array of length 2n and set the right column to be a pointer into the middle of the array. This change (from my limited testing bordering on anecdotal) doesn't change the best case runtime but makes it a lot less volatile. The q_sort implementation was heavily inspired by the K&R book, which is my C reference text, although it has my own twist of using bubble sort at the root of the recursion. Since the sort is done in place, it persists into part 2 where it speeds up the search.

## Day 2
Part 1 was quite straight forward, but part 2 required a bit of thinking. I am happy that the final solution is decently efficient as it makes good use of re-using a handful allocated memory blocks which are sproadically re-allocated to larger blocks when required (the solution uses no fore-knowledge of maximum line lengths, for example). The array extension mechanism went through a few iterations, but the final form where the array pointer is itself passed by reference is what I settled on after tracing a bug resulting from my not updating the pointer in the calling function correctly - so passing the pointer itself by reference allows the extension function to handle the update and remove the risk of this oversight.

