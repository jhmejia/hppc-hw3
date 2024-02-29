#!/bin/bash

#----------------------------------------------------

# Sample Slurm job script

#

#   -- Serial codes run on a single node (upper case N = 1).

#        A serial code ignores the value of lower case n,

#        but slurm needs a plausible value to schedule the job.

#

#   -- For a good way to run multiple serial executables at the

#        same time, execute "module load launcher" followed

#        by "module help launcher".

 

#----------------------------------------------------

 

#SBATCH -J jhmpingpong         # Job name

#SBATCH -p classroom           # Queue (partition) name

#SBATCH -o /home/jhmejia/hppc-hw3/jhmpingpong.o%j       # Name of stdout output file

#SBATCH -e /home/jhmejia/hppc-hw3/jhmpingpong.e%j       # Name of stderr error file

 

#SBATCH -N 1               # Total # of nodes (must be 1 for serial)

#SBATCH -n 2               # Total # of mpi tasks (should be 1 for serial)

#SBATCH -t 01:30:00        # Run time (hh:mm:ss)

 

# #SBATCH --mail-user=username@tacc.utexas.edu

# #SBATCH --mail-type=all    # Send email at begin and end of job

# #SBATCH -A myproject       # Allocation name (req'd if you have more than 1)

 

# Other commands must follow all #SBATCH directives...

 

module list

pwd

date

hostname

 

 

srun --mpi=pmi2 /home/jhmejia/hppc-hw3/a.out

 

# ---------------------------------------------------