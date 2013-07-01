#include <stdlib.h>
#include <assert.h>
#include "vector.h"

struct vector* vector_init(void (*freefunc) (void *))
{
	struct vector* vect=malloc(sizeof(struct vector));
	vect->capacity=INIT_CAPCITY;
	vect->size=0;
	vect->data=malloc(sizeof(void *)*vect->capacity);
	vect->freefunc=freefunc;
	return vect;
}

void vector_push(struct vector* vect, void *data)
{
	if(vect->size == vect->capacity) {
		assert(GROWTH_FACTOR > 1);
		vect->capacity=vect->capacity*GROWTH_FACTOR;
	       	vect->data=realloc(vect->data, sizeof(void *)*vect->capacity);
	}
	vect->data[vect->size++]=data;
}

void *vector_pop(struct vector *vect)
{
	if(vect->size == 0) return NULL;
	void *ret=vect->data[vect->size-1];
	vect->data[--vect->size]=NULL;
	return ret;
}

void vector_delete(struct vector *vect)
{
	if(vect == NULL) return;
	if(vect->freefunc != NULL) {
		int i;
		for(i=0;i<vect->size;i++) vect->freefunc(vect->data[i]);
		
	}
	free(vect->data);
	free(vect);
}

