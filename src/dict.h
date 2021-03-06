#ifndef _DICT_H_
#define _DICT_H_
#include "types.h"
#include "vector.h"
#include <glib.h>
#include <SDL.h>


/*
 * Following code is based on Professor DJ Kehoe's MoGul Engine from NJIT
 * source can be found at https://github.com/engineerOfLies/MoGUL
*/

#define LINE_LENGTH 128
#define GFOREACH(item, list) for(GList *__glist = list; __glist && (item = __glist->data, true); __glist = __glist->next)
typedef enum Dict_types_S
{
	DICT_VOID,
	DICT_INT,
	DICT_FLOAT,
	DICT_STRING,
	DICT_LIST,
	DICT_HASH,
	DICT_SOUND,
	DICT_MUSIC,
	DICT_CUSTOM,	
}Dict_Type;

typedef void (*Dict_Free)(void *data);
typedef char Line[128];
typedef Uint8 Bool; 
/*
 dictionary data container
 key is pointer to data
*/
typedef struct Dict_S
{
	Dict_Type data_type;
	int item_count; /*<In case DICT holds a list or a hash*/
	void (*keyFree)( void * data);
	void *keyValue;
}Dict;

/*
* @brief destroys the pointer and frees content it points to
* @param pointer to a typed pointer
*/
void dict_destroy(Dict * dict);
/**
* @brief frees the Dict and sets the passed in pointer to NULL
* @param a pointer to a pointer to a typed pointer.
*/
void dict_free(Dict ** dict);
/**
* @brief frees the GString type
* @param string the string to free 
*/
void dict_g_string_free(char * string);

/**
* @brief returns a newly allocated dictionary
* @return returns a pointer to new dictionary
*/
Dict * dict_new();
/*
* @brief allocates and and creates a pointer to a new Ghash
* @return NULL on error or a Dictionary of a GHashTable
*/
Dict * dict_new_hash();

/**
* @brief allocates a GString and sets up a pointer to new GString
* @param The text to contain.
* @return NULL on error or Dict containing a GString
*/
Dict *dict_new_string(char *text);

/*creates new key values based on inputs of different types*/
Dict * dict_new_bool(Bool n);
Dict * dict_new_int(int n);
Dict * dict_new_uint(int n);
Dict * dict_new_float(float n);
Dict * dict_new_vec2d(Vec2d n);
Dict * dict_new_vec3d(Vec3d n);
Dict * dict_new_rect(SDL_Rect n);
Dict * dict_new_rectf(Rect_f n);

/**
* @brief inserts a new key value pair into the dictionary hash table
* @param hash, the dictionary hash table data container
* @param char *key, the key to identify the new value
* @param dict * value, the value to insert into the hash table
*/
void dict_hash_insert(Dict *hash, const char *key, Dict *value);
/**
* @brief gets the number of values into the hash table
* @param dict hash, the hash table to check
* @return int the number of entries in the hash table
*/
int dict_get_hash_count(Dict * hash);
/**
* @brief returns the nth value in a hash table
* @param Line key, the output of the nth values key
* @param dict hash, the hash table to search through
* @param int n, the nth value
* @return dict value of the nth entry in hash
*/
Dict * dict_get_hash_nth(Line key, Dict *hash, int n);

Dict * dict_get_hash_value(Dict*hash, Line key);

#endif
