```target``` is a demo executable which allocates memory on the heap and read/prints it every second.

```magic``` expects three params: pid, search, replace. Search and replace should be the same length. It will look at the selected process memory map in /proc/PID/maps and then rewrite all search occurances with replace in the heap memory of the target.
