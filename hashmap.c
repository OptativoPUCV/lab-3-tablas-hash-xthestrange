#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    long pos = hash(key, map->capacity);

    while (map->buckets[pos] != NULL && map->buckets[pos]->key != NULL) {
        if (is_equal(map->buckets[pos]->key, key)) return;
        pos = (pos + 1) % map->capacity;
    }

    Pair * nuevoPar = createPair(_strdup(key), value);
    map->buckets[pos] = nuevoPar;
    map->size++;
    map->current = pos;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair ** oldBuckets = map->buckets;
    long oldCapacity = map->capacity;

    map->capacity *= 2;
    map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
    map->size = 0;

    for (long i = 0; i < oldCapacity; i++) {
        if (oldBuckets[i] != NULL && oldBuckets[i]->key != NULL) {
            insertMap(map, oldBuckets[i]->key, oldBuckets[i]->value);
        }
    }
}



HashMap * createMap(long capacity) {
    HashMap * M = (HashMap *) malloc(sizeof(HashMap));
    M->buckets = (Pair **) calloc(capacity, sizeof(Pair *));
    M->capacity = capacity;
    M->size = 0;
    M->current = -1;
    return M;
}

void eraseMap(HashMap * map,  char * key) {    
    Pair * par = searchMap(map, key);
    if (par != NULL){
        par->key = NULL;
        map->size--;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    long pos = hash(key, map->capacity);

    while (map->buckets[pos] != NULL) {
        if (map->buckets[pos]->key != NULL && is_equal(map->buckets[pos]->key, key)) {
            map->current = pos;
            return map->buckets[pos];
        }
        pos = (pos + 1) % map->capacity;
    }

    return NULL;
}

Pair * firstMap(HashMap * map) {
    for (long i = 0; i < map->capacity; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    for (long i = map->current + 1; i < map->capacity; i++) {
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) {
            map->current = i;
            return map->buckets[i];
        }
    }
    return NULL;
}
