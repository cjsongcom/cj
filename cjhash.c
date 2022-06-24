
#include <cj/cj.h>
#include <cj/cjhash.h>


CJEXTERNC static cjbool _cjhash_new_ex(cjhash* out_hash, cjhash_pair_type type,
	cjhash_siz bucket_siz, cjhash_hasher_fn hasher, cjhash_cmp_fn cmp,
	cjhash_remover_fn remover, cjbool is_placed, cjsiz_t key_siz, cjsiz_t val_siz);


CJEXTERNC void* cjhash_malloc(cjsiz siz) {
	return cjmzalloc(siz);
}

CJEXTERNC void cjhash_mfree(void* p) {
	cjmfree(p);
}


inline cjbool _cjhash_is_ptr_key(cjhash* hash) {

	if (!hash || (hash->type != CJPAIR_TYPE_PTR_PTR))
		return cjfalse;

	return cjtrue;
}

inline cjbool _cjhash_is_str_key(cjhash* hash) {

	if (!hash ||
		(hash->type != CJPAIR_TYPE_STRPTR_PTR && hash->type != CJPAIR_TYPE_STR_PTR))
		return cjfalse;

	return cjtrue;
}

CJEXTERNC void cjhash_set_func(cjhash* hash, cjhash_hasher_fn hasher,
	cjhash_cmp_fn cmp, cjhash_remover_fn remover) {

	if (!hash)
		return;

	if (hasher)
		hash->hasher = hasher;

	if (cmp)
		hash->cmp = cmp;

	if (remover)
		hash->remover = remover;
}

CJEXTERNC cjbool cjhash_new_ptr_ptr(cjhash** out_hash, cjhash_siz bucket_siz) {

	cjhash* hash = cjhash_malloc(sizeof(cjhash));

	return _cjhash_new_ex(hash, CJPAIR_TYPE_PTR_PTR, bucket_siz,
		cjhash_ptr_hasher, cjhash_ptr_cmp, cjhash_remover_default, cjfalse, 0, 0);
}

CJEXTERNC cjbool cjhash_new_ptr_ptr_placed(cjhash* out_hash, cjhash_siz bucket_siz) {

	cjmemzero(out_hash, sizeof(cjhash));

	return _cjhash_new_ex(out_hash, CJPAIR_TYPE_PTR_PTR, bucket_siz,
		cjhash_ptr_hasher, cjhash_ptr_cmp, cjhash_remover_default, cjtrue, 0, 0);
}

CJEXTERNC cjbool cjhash_new_strptr_ptr(cjhash** out_hash, cjhash_siz bucket_siz) {

	cjhash* hash = cjhash_malloc(sizeof(cjhash));

	cjbool rst = _cjhash_new_ex(hash, CJPAIR_TYPE_STRPTR_PTR, bucket_siz,
		cjhash_mbcs_hasher, cjhash_mbcs_cmp, cjhash_remover_default, cjfalse, 0, 0);

	if (!rst) {
		cjhash_mfree(hash);

		return cjfalse;
	}

	*out_hash = hash;

	return rst;
}

CJEXTERNC cjbool cjhash_new_strptr_ptr_placed(cjhash* out_hash, cjhash_siz bucket_siz) {

	cjmemzero(out_hash, sizeof(cjhash));

	return _cjhash_new_ex(out_hash, CJPAIR_TYPE_STRPTR_PTR, bucket_siz,
		cjhash_mbcs_hasher, cjhash_mbcs_cmp, cjhash_remover_default, cjtrue, 0, 0);
}


CJEXTERNC cjbool cjhash_new_str_ptr(cjhash** out_hash, cjhash_siz bucket_siz) {

	cjhash* hash = cjhash_malloc(sizeof(cjhash));

	cjbool rst = _cjhash_new_ex(hash, CJPAIR_TYPE_STR_PTR, bucket_siz,
		cjhash_mbcs_hasher, cjhash_mbcs_cmp, cjhash_remover_str_ptr, cjfalse, 0, 0);

	if (!rst) {
		cjhash_mfree(hash);

		return cjfalse;
	}

	*out_hash = hash;

	return cjtrue;
}

CJEXTERNC cjbool cjhash_new_str_ptr_placed(cjhash* out_hash, cjhash_siz bucket_siz) {

	cjmemzero(out_hash, sizeof(cjhash));

	return _cjhash_new_ex(out_hash, CJPAIR_TYPE_STR_PTR, bucket_siz,
		cjhash_mbcs_hasher, cjhash_mbcs_cmp, cjhash_remover_str_ptr, cjtrue, 0, 0);
}

CJEXTERNC cjbool _cjhash_new_ex(cjhash* out_hash, cjhash_pair_type type,
	cjhash_siz bucket_siz, cjhash_hasher_fn hasher, cjhash_cmp_fn cmp, 
	cjhash_remover_fn remover, cjbool is_placed, cjsiz_t key_siz, cjsiz_t val_siz) {

	if (!out_hash || bucket_siz < 1)
		return cjfalse;

	out_hash->type = type;

	out_hash->bucket = cjhash_malloc(sizeof(void*) * bucket_siz);
	out_hash->bucket_siz = bucket_siz;

	out_hash->hasher = hasher;
	out_hash->cmp = cmp;
	out_hash->remover = remover;

	out_hash->key_siz = key_siz;
	out_hash->val_siz = val_siz;

	out_hash->is_placed = is_placed;

	return cjtrue;
}
 
CJEXTERNC void cjhash_del(cjhash* hash) {

	if (!hash)
		return;

	cjhash_clear(hash);

	cjhash_mfree(hash->bucket);
	
	if(!hash->is_placed)
		cjhash_mfree(hash);
}

CJEXTERNC void cjhash_clear(cjhash* hash) {
	
	if (!hash)
		return;
	 
	cjhash_pair** pairs = (cjhash_pair**)hash->bucket;

	for (cjhash_siz i = 0; i < hash->bucket_siz; i++) {
		cjhash_pair* pair = pairs[i];

		if (pair) {
			hash->remover(pair);

			//if (hash->type == CJPAIR_TYPE_STR_PTR) {
			//	// hash->remover(pair) will set pair->key to zero
			//	;
			//	//cjassert(!pair->key);
			//}

			cjhash_mfree(pair);
		}
	}

	cjmemzero(hash->bucket, sizeof(void*) * hash->bucket_siz);

	hash->pair_cnt = 0;
	hash->last_ins_pair = cjNULL;	
}

CJEXTERNC cjhash_val cjhash_hash(cjhash* hash, const void* key) {

	cjhash_val v = hash->hasher(key);

	return v % (hash->bucket_siz - 1);
}

CJEXTERNC cjbool cjhash_is_empty(cjhash* hash) {

	if (!hash)
		return cjfalse;

	return  (hash->pair_cnt == 0) ? cjtrue : cjfalse;
}

CJEXTERNC inline static cjhash_pair* _cjhash_lookup_pair(cjhash* hash,
	cjhash_val bucket_idx, const void* key) {

	cjhash_pair** bucket
		= (cjhash_pair**)hash->bucket;

	cjhash_pair* pair = bucket[bucket_idx];

	while (pair) {

		if (0 == hash->cmp(pair->key, key))
			return pair;

		// stop at last valid pair
		if (!pair->_next)
			return cjNULL;

		pair = pair->_next;
	}

	return cjNULL;
}

CJEXTERNC cjhash_pair* cjhash_find(cjhash* hash, const void* key) {

	if (!hash)
		return cjNULL;

	if (hash->pair_cnt == 0)
		return cjNULL;

	cjhash_val bucket_idx = cjhash_hash(hash, key);

	return _cjhash_lookup_pair(hash, bucket_idx, key);
}

CJEXTERNC cjhash_siz cjhash_get_siz(cjhash* hash) {
	if (!hash)
		return (cjhash_siz)-1;

	return hash->pair_cnt;
}

CJEXTERNC static void _cjhash_update_pair_seq(cjhash* hash, cjhash_pair* pair) {

	if (hash->last_ins_pair) {
		((cjhash_pair*)hash->last_ins_pair)->_seq_prev = pair;
		pair->_seq_next = hash->last_ins_pair;
	}

	hash->last_ins_pair = pair;
}

CJEXTERNC cjhash_pair* cjhash_push_ptr_ptr(cjhash* hash, void* ptr_key, void* ptr_val) {

	if (!hash || !_cjhash_is_ptr_key(hash))
		return cjNULL;
	
	cjhash_val hash_val = cjhash_hash(hash, ptr_key);

	cjhash_pair** bucket
		= (cjhash_pair**)hash->bucket;

	// get start pair
	cjhash_pair* pair = bucket[hash_val];
	cjhash_pair* new_pair = cjNULL;

	if (pair) {

		if (_cjhash_lookup_pair(hash, hash_val, ptr_key)) {
			// key pair is already exist
			// change only value
			pair->val = ptr_val;

		} else {
			// add new pair at end of pair in current bucket
			new_pair = cjhash_malloc(sizeof(cjhash_pair));

			cjassert(!pair->_next);
			pair->_next = new_pair;
		}

	} else {
		new_pair = cjhash_malloc(sizeof(cjhash_pair));
		bucket[hash_val] = new_pair;
	}

	if (new_pair) {
		new_pair->type = hash->type;
		new_pair->key = ptr_key;
		new_pair->val = ptr_val;

		_cjhash_update_pair_seq(hash, new_pair);
		hash->pair_cnt++;
	}

	return new_pair ? new_pair : pair;
}

CJEXTERNC cjhash_pair* cjhash_push_str_ptr(cjhash* hash, const cjmc* str_key, void* ptr_val) {

	if (!hash || !(_cjhash_is_str_key(hash)))
		return cjNULL;

	if (!str_key || *str_key == 0)
		return cjNULL;

	cjhash_val hash_val = cjhash_hash(hash, str_key);

	cjhash_pair** bucket
		= (cjhash_pair**)hash->bucket;

	// get start pair
	cjhash_pair* pair = bucket[hash_val];
	cjhash_pair* new_pair = cjNULL;

	if (pair) {

		if (_cjhash_lookup_pair(hash, hash_val, str_key)) {
			// key pair is already exist
			// change only value
			pair->val = ptr_val;

		} else {
			// add new pair at end of pair in current bucket
			new_pair = cjhash_malloc(sizeof(cjhash_pair));
			cjassert(!pair->_next);
			pair->_next = new_pair;
		}

	} else {
		new_pair = cjhash_malloc(sizeof(cjhash_pair));
		bucket[hash_val] = new_pair;
	}

	if (new_pair) {
		cjsiz key_siz = sizeof(cjmc) * (cjstrlen(str_key) + 1);

		new_pair->type = hash->type;
		new_pair->key_siz = key_siz;
		new_pair->key = cjhash_malloc(key_siz);
		cjstrcpy_s(new_pair->key, key_siz / sizeof(cjmc), str_key);

		new_pair->val = ptr_val;


		_cjhash_update_pair_seq(hash, new_pair);
		hash->pair_cnt++;
	}

	return new_pair ? new_pair : pair;
}


CJEXTERNC static void _cjhash_inner_delete(cjhash* hash, cjhash_pair* prev, 
	cjhash_pair* del) {

	if (prev)
		prev->_next = del->_next;

	if (del->_seq_prev)
		del->_seq_prev->_seq_next = del->_seq_next;

	if (del->_seq_next)
		del->_seq_next->_seq_prev = del->_seq_prev;

	if (hash->last_ins_pair == del)
	{
		cjassert(del->_seq_prev == cjNULL);
		hash->last_ins_pair = del->_seq_next;
	}

	hash->remover(del);
	cjhash_mfree(del);

	hash->pair_cnt--;
	cjassert(hash->pair_cnt > 0);
}

CJEXTERNC cjbool cjhash_remove(cjhash* hash, const void* ptr_key) {

	if (!hash || !ptr_key || hash->pair_cnt == 0)
		return cjfalse;

	cjhash_val bucket_idx = cjhash_hash(hash, ptr_key);

	cjhash_pair** bucket = (cjhash_pair**)hash->bucket;

	cjhash_pair* cur = bucket[bucket_idx];
	cjhash_pair* prev = cur;

	if (!cur)
		return cjfalse;

	// first hit
	if (0 == hash->cmp(cur->key, ptr_key)) {
		bucket[bucket_idx] = cur->_next;
		_cjhash_inner_delete(hash, cjNULL, cur);

		return cjtrue;
	}

	prev = cur;	
	cur = cur->_next;

	while (cur) {
		if (0 == hash->cmp(cur->key, ptr_key)) {
			if (prev)
				prev->_next = cur->_next;

			_cjhash_inner_delete(hash, prev, cur);

			return cjtrue;
		}

		prev = cur;	
		cur = cur->_next;
	}

	return cjfalse;
}

CJEXTERNC cjbool cjhash_remove_by_ptr(cjhash* hash, void* ptr_key) {

	if (!_cjhash_is_ptr_key(hash))
		return cjfalse;

	return cjhash_remove(hash, ptr_key);
}

CJEXTERNC cjbool cjhash_remove_by_str(cjhash* hash, const cjmc* str_key) {

	if (!_cjhash_is_str_key(hash))
		return cjfalse;

	return cjhash_remove(hash, str_key);
}

