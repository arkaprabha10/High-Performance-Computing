## Shin HPC reuse

### Usage
- write your code in serial.c and parallel.c in the specified region
- edit the config.py file as per needed
- the script deletes the executables on each run and recompiles
- put the partial runs into a folder named 'fragments' and run merge.py, output file will be named 'data-merged.txt'

### Warnings
- don't reuse if the config file changed in between the runs
- if there are more runs in the data.txt file then the script doesn't delete the extra runs
- don't remove 0 in procs list, the serial code will not run otherwise
