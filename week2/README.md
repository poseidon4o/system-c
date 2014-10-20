```target``` is a demo executable which allocates memory on the heap and read/prints it every second.

```magic``` expects one param as an input and this will be ```target```'s pid. It will look at the selected process memory map in /proc/<pid>/maps and then rewrite all readable characters from the heap part of the target.
