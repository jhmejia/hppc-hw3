Hello, World!

HPPC HW3.

Notable files: 

- Problem 1: one.c
- Problem 2: two.c
- Problem 3: rps.c
- Problem 4: infiniband_jhm.pdf

  (notable problem 4 files are also included)



How to do MPI programs:

1. Load the MPI (and other) modules:
```bash
module load openmpi/gnu
```
2. Compile the program:
```bash
mpicc -o hello hello.c
```
3. Run the program:
```bash
mpirun -np 4 hello
```

OR run it using SLURM:

(make sure to configure the slurm file to use the correct number of nodes and tasks, and the correct program name and path)
```bash
sbatch hello.slurm
```
