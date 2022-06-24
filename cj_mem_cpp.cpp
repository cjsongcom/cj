
#include "cj.h"
#include "cj_mem.h"


#ifdef _CJDEBUG_MEM

static void* cjm_allocate(cjui ary, cjm_siz_t siz, const cjc* tag = cjNULL,
	const cjc* name = cjNULL, cjui line = 0, const cjc* func = cjNULL){
	if (!siz)
		return cjNULL;

	return cjmalloc(siz);
}

#else

static void* cjm_allocate(cjui ary, cjm_siz_t siz, const cjc* tag = cjNULL){
	if (!siz)
		return cjNULL;

	return malloc(siz);
}

#endif


// Deallocate
static void cjm_deallocate(cjui /*ary*/, void* mem){
	if (!mem)
		return;

#ifdef _CJDEBUG_MEM
	cjmfree(mem);
#else
	cjmfree(mem);
#endif
}


#ifdef _CJDEBUG_MEM

void* cjmem::operator new(cjm_siz_t siz, const cjc* tag, const cjc* dbg_file, cjui dbg_line, const cjc* dbg_func){
	return cjm_allocate(0, siz, tag, dbg_file, dbg_line, dbg_func);
}

void* cjmem::operator new[](cjm_siz_t siz, const cjc* tag, const cjc* dbg_file, cjui dbg_line, const cjc* dbg_func){
	return cjm_allocate(1, siz, tag, dbg_file, dbg_line, dbg_func);
}

void cjmem::operator delete(void* mem, const cjc* /*tag*/, const cjc* /*dbg_file*/, cjui /*dbg_line*/, const cjc* /*dbg_func*/){
	if (!mem)
		return;

	cjm_deallocate(0, mem);
}

void cjmem::operator delete[](void* mem, const cjc* /*tag*/, const cjc* /*dbg_file*/, cjui /*dbg_line*/, const cjc* /*dbg_func*/){
	// On GCC the allocated size is passed into operator delete[] 
	// so there is no need for the allocator to save the size of
	// the allocation.
	if (!mem)
		return;

	cjm_deallocate(1, mem);
}

void cjmem::operator delete(void* mem){
	if (!mem)
		return;

	cjm_deallocate(0, mem);
}

void cjmem::operator delete[](void* mem){
	if (!mem)
		return;

	cjm_deallocate(1, mem);
}


#else

void* cjmem::operator new(cjm_siz_t siz, const cjc* tag){
	return cjm_allocate(0, siz);
}

void* cjmem::operator new[](cjm_siz_t siz, const cjc* tag){
	return cjm_allocate(1, siz);
}

void cjmem::operator delete(void* mem, cjm_siz_t siz, const cjc* tag){
	if (!mem)
		return;

	cjm_deallocate(0, mem);
}

void cjmem::operator delete[](void* mem, cjm_siz_t /*stElementSize*/, const cjc* tag){

	// On GCC the allocated size is passed into operator delete[] 
	// so there is no need for the allocator to save the size of
	// the allocation.
	if (!mem)
		return;

	cjm_deallocate(1, mem);
}

void cjmem::operator delete(void* mem) {
	if (!mem)
		return;

	cjm_deallocate(0, mem);
}

void cjmem::operator delete[](void* mem) {
	if (!mem)
		return;

	cjm_deallocate(1, mem);
}

#endif

