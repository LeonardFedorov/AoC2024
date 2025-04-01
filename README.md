# AoC2024

Solutions for Advent of Code 2024 written in C. No particular paradigm or approach is targeted, beyond striving to have solutions sufficiently general to be able to read input data from any user and not be overfitted to my own. I have not used the standard library for anything - as an academic exercise I have directly used the Win32 APIs for I/O and memory management, on which I have built my own data structures. 

Be warned that there are unmarked spoilers for the day puzzles below.

## General Commentary

### Memory Management
All memory management is handled by the Windows heap API. The basic idea is that the default process heap is used by the outer loop, with a new heap created for each run of a day's code. The main point of this construct is that once the day is finished, this heap can be explicitly deconstructed to prevent any leaking of memory that could accrue if multiple days are executed in one run of the application. To enable this design decision, all functions that perform memory allocations take a heap handle as an argument so that the caller can control where objects are stored.

### oString Type
The idiomatic way strings are handled in C is a pointer to a null-terminated character array. I felt this would be inefficient given the large amount of string processing inherent in many AoC problems. The oString type is a struct containing a pointer to a char array and an int stating how long the data payload is. This has particular use when, say, splitting a file input into lines. An oString struct can be set up for each line with a pointer into the raw data and the length of the line. This then requires no further copying/moving data around as it is transparent to the calling function where any of the data is actually stored.

### oMap Type
This aim of this type is provide a wrapper to an oString which is known to represent a 2d map (which are common in AoC puzzles). The first such puzzle was Day 4. My general approach here is to have all map functions (move, peeking, etc) always wrap around the edges, but report whether a wrap occurred via an optional pointer argument so calling processes that don't want to wrap know to discard the result. In addition, there are unsafe versions of the basic map functions which do not perform wrap checking which are inlined due to their simplicity (e.g. if one is doing a for loop over rows and columns, then all coordinate pairs produced are known to be valid).
Directions are in a single enum, with 0 representing no movement, 1-4 being the cardinal directions and 5-8 being the diagonals. Opposing directions are separated by 2. This allows for for statements to loop through all common sets of directions (all, all exc. no move, cardinals only, diagonals only).

## Individual Day Commentary

### Day 1
<details>
A straight forward enough start. The only interesting trick here was realising that instead of allocating two arrays of length n for the left and right columns respectively, I could allocate an array of length 2n and set the right column to be a pointer into the middle of the array. This change (from my limited testing bordering on anecdotal) doesn't change the best case runtime but makes it a lot less volatile. The q_sort implementation was heavily inspired by the K&R book, which is my C reference text, although it has my own twist of using bubble sort at the root of the recursion. Since the sort is done in place, it persists into part 2 where it speeds up the search.
</details>

### Day 2
<details>
Part 1 was quite straight forward, but part 2 required a bit of thinking. I am happy that the final solution is decently efficient as it makes good use of re-using a handful of allocated memory blocks which are sproadically reallocated to larger blocks when required (the solution uses no foreknowledge of maximum line lengths, for example). The array extension mechanism went through a few iterations, but the final form where the array pointer is itself passed by reference is what I settled on after tracing a heap corruption assertion resulting from my not updating the pointer in the calling function correctly - so passing the pointer itself by reference allows the extension function to handle the update and remove the risk of this oversight.
</details>

### Day 3
<details>
This day did not pose a challenge algorithmically, as I had already prepared a string checking function to test if the text from a given memory address equalled some other string constant and equipped the integer parse function with the ability to report if the target string was in fact not an integer, which does almost all of the heavy lifting here. Bolting on part 2 was incredibly easy given this infrastructure as well. The only thing that stopped this working instantly was some syntax around getting the right levels of indirection passed between the functions to get the desired effect (i.e. not a memory access violation exception). The solution executes very quickly as it simply does a linear read through the data stream with no pre-processing or dynamic data structures required.
</details>

### Day 4
<details>
This day was the first map themed puzzle and involved building out a first stab at the oMap library. The direction enum seemed to work well here. The solution takes the longest to execute of the days so far (~3.5ms), but the nature of the puzzle just requires for more data searching than the previous ones. Possible efficiency gains would be modifying the ray caster to halt the moment it wraps, rather than always performing a full wrapped ray cast and then letting the caller discard, but as the rays are only 3 long it didn't seem worth creating an extra function just for this. Part 2 involved a horrible boolean case analysis statement which is ugly, but probably reasonably efficient due to short-circuit evaluation.
</details>