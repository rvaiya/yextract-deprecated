#ifndef _VECTOR_
#define _VECTOR_

static const int INIT_CAPCITY=30; //Determines initial size of vector
static const float GROWTH_FACTOR=1.6; //Determines rate at which capacity increases whenever size hits the ceiling
typedef struct vector* Vector;
struct vector {
	void **data;
	size_t size;	
	size_t capacity;	
	void (*freefunc) (void *);
};

void vector_push(Vector vect, void *data);
void *vector_pop(Vector vect);
void vector_delete(Vector vect);
Vector vector_init(void (*freefunc) (void *));
#endif
