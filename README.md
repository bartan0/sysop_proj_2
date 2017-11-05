# Operating Systems Course - project 2

Hunters and Cookers game (multithreaded).

## Building and running

After cloning this repo, run

```
$ make
```

to build the project. You can then run the program with

```
$ ./x <n_hunters> <n_cookers> <n_bag> <n_food> <n_days>
```

where:

	* <n_hunters> - initial number of hunters
	* <n_cookers> - initial number of cookers
	* <n_bag> - initial size of bag (number of hunted animals)
	* <n_food> - initial number of food units
	* <n_days> - number of days to simulate

It's also possible to build and run the program with default arguments values with one command:

```
$ make run
```

Object (.o) files can be removed with `make clean`.  
Object files and the executable can be removed with `make distclean`.
