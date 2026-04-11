# Queue

- Realization files `contqueue.h` and `contqueue.c`.  
- And `main.c` to demonstrate how does it work.  
  
## Details
- `void*` as a type of data in structure.  
- **User** bears responsibility for **type casting** and **memory manipulations**.  
- FIFO (first in, first out) method.  
- One way only linked.  
  
## Functions
### Grade "Satisfactory": 
- constructor
- destructor
- enqueue (add to the end)
- dequeue (get first element and delete it)
  
### Grade "Good":
- size (to get it)
- peek (peek element w/o delete it)
- clear
  
### Grade "Excellent":
- fenqueue (add to the beggining)
- copy 
- merge 
Iterators:  
- get positions (start, end, next element)
- compare (equal, if points to specific queue)
Other features:
- memory backup with `capacity * sqrt(2)`
