# Spare
Here I am trying out some different ways of comparing strings for equality and benchmarking those.

## Build
`make`    

## Test
`make && make tests`    
You can also run the tests with *Valgrind* by directly calling the **runtests.sh** script.   
`VALGRIND=valgrind ./runtests.sh`    
The output will be stored in **./tests/error_log.txt**.

## Usage
`./build/spare <file1> <file2>`    
