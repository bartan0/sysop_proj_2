# Operating Systems Course - project 2

Hunters and Cookers game (multithreaded).

## Building and running

After cloning this repo, run

	$ make

to build the project. You can then run the program with

	$ ./x <n_hunters> <n_cookers> <n_bag> <n_food> [<n_days> [<sleep_base>]]

where:
* \<n_hunters\> - initial number of hunters
* \<n_cookers\> - initial number of cookers
* \<n_bag\> - initial size of the bag (number of hunted animals)
* \<n_food\> - initial number of food units
* \<n_days\> - number of days to simulate
* \<sleep_base\> - base duration of hunters/cookers sleep phase

It's also possible to build and run the program with default (defined as `exmpl_args` in _Makefile_)
arguments values with one command:

	$ make run

By default, DEBUG flag is set (see below), you can disable this flag by removing `-DDEBUG`
from value of `c_opt` variable in _Makefile_.

Object (_.o_) files can be removed with `make clean`.  
Object files and the executable can be removed with `make distclean`.

## Program output

The output of the program consist of __status lines__ (only if DEBUG flag is set) and __final status__.

### Status lines
First status line has the form:

	0: <n_hunted>/<n_food>

where:
* \<n_hunted\> - initial size of the bag
* \<n_food\> - initial number of food units

Subsequent status lines are print by every agent (hunter/cooker) just before going to sleep and just before exit.  
Each of them has one of the forms:

	<i_day>: <id><x> <n_hunted>/<n_food>
	<i_day>: <id>(e)<x> <n_hunted>/<n_food>

where:
* \<i_day\> - current day (according to the agent's clock)
* \<id\> - type of agent: H for hunter, C for cooker
* (e) - if present, means the agent is about to go away due to starvation
* \<x\>
  * for hunter: + - hunt was successful, _blank_ - nothing's been hunted
  * for cooker: number of produced units of food
* \<n_hunted\> - size of the bag _just before_ the agent's sleep phase
* \<n_food\> - number of food units _just before_ the agent's sleep phase

### Final status
Final status shows the final state of the world; it's form is:

	#Hunters survived: <i_h>/<n_h>
	#Cookers survived: <i_c>/<n_c>
	#Survived total: <i_t>/<n_t>
	
where:
* \<i_h\>, \<i_c\>, \<i_t\> - final number of hunters, cookers, agents that hadn't gone away
* \<n_h\>, \<n_c\>, \<n_t\> - initial number of hunters, cookers, agents
