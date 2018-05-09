# Allocate 2D arrays all at once

It is easy to allocate a 2-D array as so:
```
double** array = malloc(arrrows * sizeof(double*));
for (int i = 0; i < arrrows; i++) {
    array[i] = malloc(arrcols * sizeof(double));
}
```

However, this distributes the pieces of the array all over memory, to wherever they are allocated. But keeping the parts separate, no not only make memory access messier for large arrays, but you remove the ability to apply optimized algorithms to a tightly organized array structure. The correct way to allocate a 2D array is:

```
double* array = malloc(sizeof(double[arrrows][arrcols]));
```

The problem with this is that you can no longer access your array as `array[i][j]` since the object `array` is now a single pointer. But you will make up for the difficulty in _writing_ the code with increased speed _executing_ the code.

Find a good explanation here: https://stackoverflow.com/questions/42094465/correctly-allocating-multi-dimensional-arrays

# Don't `malloc` in a function and return the result

This is, in my short experience, the best way to lose track of a pointer and get a memory leak. If you re-use the function for something else, you have to remember to properly de-allocate the memory.

Thus instead of:
```
double* getNums(double* data, int arrlen) {
    double* response = malloc(arrlen * sizeof(double));
    ...
    response = <add some data>
    ...
    return response;
}
```

You should do:
```
void getNums(double* data, double** response, int arrlen) {
    ...
    (*response)= <add some data>
    ...
}
```

# Use the same iterators for the same matrix dims

For matrix math, if you are dealing with an (n x k) matrix, then let i = (1, 2, ..., n) and j = (1, 2, ..., k). Keep this the same through all your code! You will thank me when it is time to debug.

# Test different sizes to stress test `malloc`

The `malloc` function is critical, and also the biggest potential source of failure. One big potential failure source is that if you `malloc` small bits of data, you might not see problems from bad code. Memory is assigned in (relatively) big chunks by operating systems. If you request the memory for 10 doubles, you probably will get much more memory. This can prevent you from finding errors in your code.

Say you allocate 10 doubles, but then read 11. It is entirely possible that there will be something logical to read (or write) in the next bytes past what you have allocted; since this memory is assigned to the process of your C excutable, this won't necessarily be a problem. However, later down the line when you allocate 10,000 doubles, running of the end of your alloted memory will cause a memory corruption failure at runtime.

If this error occurs in a piece of code you have run several times before, this can throw off your bug hunting efforts as you concentrate on other pieces of code. Therefore, when you test subroutines, ensure that you test them with many sizes of inputs to iron out memory allocation problems at the get go.

# Ensure writeover of anything passed by reference!

Basically, make sure that whenever you do a `+=` you have a corresponding `=0`. 

If you are allocating vectors containing a lot of numbers that look a lot the same, it can sometimes be very hard to notice in troubleshooting that you have the wrong value in the wrong bytes. 
