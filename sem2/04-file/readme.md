# File input/output

- all functions in `fiostruct.h` and `fiostruct.c`, where **struct** is name of structure from 3rd lab (here it's **queue**).  
- `makefile`

## Task  

For 03-queue lab make fileio operations, file movement and change 03-queue `main.c` (I made copy here, so original file is fine) to get arguments from command line like this:  
- `./main save filename` - to save objects to file.  
- `./main load filename` - to load objects from file.  
- `./main list filename` - to print list of objects with numbers from file.  
- `./main get I filename` - to get i-element from file.  
Also:
- Opportunity to choose regular or binary read/write.  
- Key feature of this demonstration is opportunity to demonstrate only one operation (program close after one of the functions).  

## Grade "Satisfactory"

1. `rand_gen_struct(size_t n)` to generate 10'000 elements for the structure.  
2. save data in `.txt` regularly.  
3. load data from `.txt` regularly.  
5. `get_element_from_text_file(const char* filename, int index)` to move in file reading lines till the index, return value or `NULL`.  

## Grade "Good"

**all from Grade "Satisfactory"**
1. save data in `.txt` binary.  
2. load data from `.txt` binary.  
Take into account data size while read/write (e.g. `sizeof` to get size of every element).  
3. `get_element_from_binary_file(const char* filename, int index, size_t element_size, void* result)` to calculate position of element as `index* element_size`. Use `fseek` to move cursor.  Read element with `fread`.  

## Grade "Excellent"  

**all from Grades "Satisfactory" and "Good"**
1. `rand_gen_struct_in_container(size_t n)` to gen n values of the structure and put them into the structure container.  
- for `queue`: every element in new line.  
2. save and load data regularly and binary, ability to instantly move data from structure to file if write and load from file to structure.  
3. demonstrate ouput i-element from structure to have been loaded from file.  
