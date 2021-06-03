# About
This code generates supercells with either a combination of displacement modes to sample the electron-phonon coupling (within the frozen phonon approximation). Part of this code is inspired by work published in PRS 2, 013357 (2020) (DOI:https://doi.org/10.1103/PhysRevResearch.2.013357)

This code can be cited as: https://zenodo.org/badge/latestdoi/371131829
# Manual

## How to start

In the directory, there is a Makefile which will generate the executables for constructing the supercells (name: first), and for linear combination for those files (name: second). You can run:

### `make`

to produce two executables in current directory. You can change the name in Makefile

## Making supercells

You can simply run executable directly in your command line for individual files, but recommend to use bash file if the number of your output files is large. To generate supercells, you need 10 arguments for the first command.\
\
argv[1] -- Original unit cell file name (Should only contain elements and their position in cartesians, unit in bohr or Angstrom)\
argv[2] -- Matdyn file name (contain your phonon modes)\
argv[3] -- Cell parameter file (Should only contain cell parameter in cartesian, make sure have same unit as argv[1])\
argv[4] -- Typical frequency number in matdyn file you want to find (not freqeuncy)\
argv[5] -- Supercell size in x\
argv[6] -- Supercell size in y\
argv[7] -- Supercell size in z\
argv[8] -- temperature you interested in Kelvin\
argv[9] -- Output file name of your supercell\
argv[10] -- Unit of your input file, in angstrom or bohr (for angstrom type 'a', for bohr type 'b')\
\
To generate an unperturbed supercell, i.e. no phonon included, simply make: argv[2] == 0.\
Example bash file is also included in the example directory (Two folders, Example contains for graphene, Example2 contains for diamond).

## Monta Carlo Sampling

The next step is making linear combination of your phonon states. You need 4 arguments for the second command.\
\
argv[1] -- Atom number in your supercell\
argv[2] -- Original Supercell (meaning no perturbation)\
argv[3] -- All perturbated files you generate, they should under the same quotation mark, and each file separate by space.\
argv[4] -- Output file name\
\
Example bash file is also included in the example directory.

## Quantum Espresso

I also include the files, in folder QE, that generates my matdyn file. You can see documentations for [pw.x](https://www.quantum-espresso.org/Doc/INPUT_PW.html), [ph.x](https://www.quantum-espresso.org/Doc/INPUT_PH.html) for modification. It is also helpful to use [parallelism](https://www.quantum-espresso.org/Doc/ph_user_guide/node14.html) for large unit cell calculation.
