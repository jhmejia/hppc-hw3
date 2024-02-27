Hello, World!

HPPC HW1.

1. is a hello world

2. Is a ring process

3. is an RPS game

4. Is a ping-pong test


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
