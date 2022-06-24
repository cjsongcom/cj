
#ifndef _CJHASH_H_
#define _CJHASH_H_

//
// type
//

typedef cji								cjhash_idx, cjhash_siz;
typedef cjui							cjhash_val;
typedef struct cjhash_t					cjhash;

enum {
	CJHASH_HASH_VAL_INVALID				= (cjhash_val)(-1),

	CJ_MAX_CJHASH_SMALL_KEY_STR_SIZ		= 32,
	CJ_MAX_CJHASH_KEY_STR_SIZ			= 64,
	CJ_MAX_CJHASH_LONG_KEY_STR_SIZ		= 256,
};

typedef cjpair_type	cjhash_pair_type;
typedef cjpair cjhash_pair;


CJEXTERNC void* cjhash_malloc(cjsiz siz);
CJEXTERNC void cjhash_mfree(void* p);


//
// hasher
//

CJEXTERNC typedef cjhash_val(*cjhash_hasher_fn)(const void* prm);

CJEXTERNC inline cjhash_val cjhash_mbcs_hasher(const void* _str) {

	const cjmc* str = (const cjmc*)_str;
	cjui hash = 0; cji c;

	while ((c = *str++))
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash;
}

CJEXTERNC inline cjhash_val cjhash_ptr_hasher(const void* p) {
	
	// http://stackoverflow.com/questions/20953390/what-is-the-fastest-hash-function-for-pointers
	return (cjhash_val)(((size_t)p) >> 1); // 3 on 64 bit, 1 on 32 bit
}


//
// compare
//

// 0:equal   1:left greater  -1:right greater
CJEXTERNC typedef cjrst(*cjhash_cmp_fn)(const void* lhs, const void* rhs);

CJEXTERNC inline cjrst cjhash_mbcs_cmp(const void* lhs, const void* rhs) {
	return cjstrcmp((const cjmc*)lhs, (const cjmc*)rhs);
}

CJEXTERNC inline cjrst cjhash_ptr_cmp(const void* lhs, const void* rhs) {
	return lhs == rhs ? 0 : lhs > rhs ? 1 : -1;
}


//
// remover
//

CJEXTERNC typedef void(*cjhash_remover_fn)(void* pair);

CJEXTERNC inline void cjhash_remover_default(void* pair) {
	CJ_UNUSED(pair)
}

CJEXTERNC inline void cjhash_remover_str_ptr(void* _pair) {
	cjhash_pair* pair = (cjhash_pair*)_pair;
	cjhash_mfree(pair->key);
}

//
// cjhash
//

typedef struct cjhash_t {
	cjhash_pair_type					type;

	// cjhash_pair
	void**								bucket;
	cjhash_siz							bucket_siz;

	void*								last_ins_pair;
	cjhash_idx							pair_cnt;

	cjhash_hasher_fn					hasher;
	cjhash_cmp_fn						cmp;
	cjhash_remover_fn					remover;

	cjsiz								key_siz;
	cjsiz								val_siz;

	cjbool								is_placed;

} cjhash;


CJEXTERNC void cjhash_set_func(cjhash* hash, cjhash_hasher_fn hasher, 
	cjhash_cmp_fn cmp, cjhash_remover_fn remover);


// default: bucket_siz=24

// cjmc*, void*
CJEXTERNC cjbool cjhash_new_strptr_ptr(cjhash** out_hash, cjhash_siz bucket_siz);
CJEXTERNC cjbool cjhash_new_strptr_ptr_placed(cjhash* out_hash, cjhash_siz bucket_siz);

// void*, void*
CJEXTERNC cjbool cjhash_new_ptr_ptr(cjhash** out_hash, cjhash_siz bucket_siz);
CJEXTERNC cjbool cjhash_new_ptr_ptr_placed(cjhash* out_hash, cjhash_siz bucket_siz);

// "key", void*
CJEXTERNC cjbool cjhash_new_str_ptr(cjhash** out_hash, cjhash_siz bucket_siz);
CJEXTERNC cjbool cjhash_new_str_ptr_placed(cjhash* out_hash, cjhash_siz bucket_siz);

CJEXTERNC void cjhash_del(cjhash* hash);

CJEXTERNC void cjhash_clear(cjhash* hash);

CJEXTERNC cjhash_val cjhash_hash(cjhash* hash, const void* key);

CJEXTERNC cjbool cjhash_is_empty(cjhash* hash);

CJEXTERNC cjhash_pair* cjhash_find(cjhash* hash, const void* key);

CJEXTERNC cjhash_siz cjhash_get_siz(cjhash* hash);

// key=void*, val=void*
CJEXTERNC cjhash_pair* cjhash_push(cjhash* hash, const void* ptr_key, void* ptr_val);

// key="key", val=void*
CJEXTERNC cjhash_pair* cjhash_push_str_ptr(cjhash* hash, const cjmc* str_key, void* ptr_val);


CJEXTERNC cjbool cjhash_remove(cjhash* hash, const void* ptr_key);
CJEXTERNC cjbool cjhash_remove_by_ptr(cjhash* hash, void* ptr_key);
CJEXTERNC cjbool cjhash_remove_by_str(cjhash* hash, const cjmc* str_key);



/*

CJEXTERNC cjbool cjhash_set_val_ptr(cjhash* hash, void* key);
CJEXTERNC cjbool cjhash_set_val(cjhash* hash, void* key, void* val);

CJEXTERNC cjbool cjhash_remove(cjhash* hash, void* key);
CJEXTERNC cjbool cjhash_clear(cjhash* hash);

*/

#endif
