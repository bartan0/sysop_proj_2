#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <unistd.h>
#include <pthread.h>

// --------------------------------
// config
// --------------------------------

#define DEF_N_DAYS 365
#define DEF_SLEEP_BASE 20

// --------------------------------
// optional features
// --------------------------------

#ifdef DEBUG
#define log(...) printf(__VA_ARGS__)
#else
#define log(...) ;
#endif

// --------------------------------
// storage type and functions
// --------------------------------

// storage - an abstraction of infinitely large container
// capable of holding any non-negative number of items
// we care only about the number of items, not the items itselves
typedef struct storage
{
	int n_items;
	pthread_rwlock_t lock;
} storage;

// error codes
typedef enum storage_error
{
	STORAGE_OK,
	STORAGE_EMPTY
} storage_error;

// initialize given storage
void storage_init(storage *strg, int n_items)
{
	strg->n_items = n_items;
	pthread_rwlock_init(&strg->lock, NULL);
}

// if n_items:
//   >= 0: add n_items to storage
//         return STORAGE_OK
//   < 0: return STORAGE_EMPTY if the operation would make number of stored items negative
//        remove -n_items from storage
//        return STORAGE_OK
storage_error storage_mod(storage *strg, int n_items)
{
	storage_error err = STORAGE_OK;

	pthread_rwlock_wrlock(&strg->lock);
	if (strg->n_items < -n_items)
		err = STORAGE_EMPTY;
	else
		strg->n_items += n_items;
	pthread_rwlock_unlock(&strg->lock);

	return err;
}

// --------------------------------
// agents functions and data
// --------------------------------

// reasons of agents exits
typedef enum exit_reason
{
	TIME_ELAPSED,
	NO_FOOD
} exit_reason;

// resources struct contains resources needed by agents
typedef struct resources
{
	int n_days;
	useconds_t sleep_base;
	storage *hunted;
	storage *food;
} resources;

// hunter's lifecycle
void *hunter(void *arg)
{
	resources *res = (resources*)arg;
	int i_day, hunted;

	for (i_day = 0; i_day < res->n_days; i_day++)
	{
		// hunting
		if ((hunted = rand()%6 > rand()%6))
			storage_mod(res->hunted, 1);

		// dining
		if (storage_mod(res->food, -1) == STORAGE_EMPTY)
			break;

		log("%d: H%s %d/%d\n",
			i_day + 1,
			(hunted) ? "+" : " ",
			res->hunted->n_items,
			res->food->n_items);

		// sleeping
		usleep(res->sleep_base + (rand()%10000 - 5000));
	}

	if (i_day < res->n_days)
	{
		log("%d: H(e)%s %d/%d\n",
			i_day + 1,
			(hunted) ? "+" : " ",
			res->hunted->n_items,
			res->food->n_items);
		return (void*)NO_FOOD;
	}
	return (void*)TIME_ELAPSED;
}

// cooker's lifecycle
void *cooker(void *arg)
{
	resources *res = (resources*)arg;
	int i_day, n_cooked;

	for (i_day = 0; i_day < res->n_days; i_day++, n_cooked = 0)
	{
		// cooking
		if (storage_mod(res->hunted, -1) == STORAGE_OK)
			storage_mod(res->food, (n_cooked = rand()%6 + 1));

		// dining
		if (storage_mod(res->food, -1) == STORAGE_EMPTY)
			break;

		log("%d: C%d %d/%d\n",
			i_day + 1,
			n_cooked,
			res->hunted->n_items,
			res->food->n_items);

		// sleeping
		usleep(res->sleep_base + (rand()%10000 - 5000));
	}

	if (i_day < res->n_days)
	{
		log("%d: C(e)%d %d/%d\n",
			i_day + 1,
			n_cooked,
			res->hunted->n_items,
			res->food->n_items);
		return (void*)NO_FOOD;
	}
	return (void*)TIME_ELAPSED;
}

// --------------------------------
// main
// --------------------------------

int main(int argc, const char **argv)
{
	int n_days = (argc > 5) ? atoi(argv[5]) : DEF_N_DAYS;
	int n_hunted = atoi(argv[3]), n_food = atoi(argv[4]);
	int n_hunters = atoi(argv[1]), n_cookers = atoi(argv[2]);
	useconds_t sleep_base = ((argc > 6) ? atoi(argv[6]) : DEF_SLEEP_BASE)*1000;

	srand(time(NULL));

	// create hunted animals and food storage, init resources
	storage hunted, food;
	storage_init(&hunted, n_hunted);
	storage_init(&food, n_food);
	resources res = {n_days, sleep_base, &hunted, &food};

	log("0: %d/%d\n", hunted.n_items, food.n_items);

	// create and start agents
	pthread_t *hunters = malloc(n_hunters*sizeof(pthread_t));
	pthread_t *cookers = malloc(n_cookers*sizeof(pthread_t));
	for (int i = 0; i < n_hunters; i++)
		pthread_create(hunters + i, NULL, hunter, &res);
	for (int i = 0; i < n_cookers; i++)
		pthread_create(cookers + i, NULL, cooker, &res);

	// wait for agents to finish and collect stats
	int i_hunters = 0, i_cookers = 0;
	void *ret;
	for (int i = 0; i < n_hunters; i++)
	{
		pthread_join(hunters[i], &ret);
		i_hunters += (ret == TIME_ELAPSED) ? 1 : 0;
	}
	for (int i = 0; i < n_cookers; i++)
	{
		pthread_join(cookers[i], &ret);
		i_cookers += (ret == TIME_ELAPSED) ? 1 : 0;
	}

	printf("\n#Hunters survived: %d/%d\n#Cookers survived: %d/%d\n#Survived total: %d/%d\n",
		i_hunters,
		n_hunters,
		i_cookers,
		n_cookers,
		i_hunters + i_cookers,
		n_hunters + n_cookers);

	return 0;
}
