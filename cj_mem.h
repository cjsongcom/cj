
#ifndef _CJ_MEM_H_
#define _CJ_MEM_H_

#ifdef __cplusplus 

#ifdef WIN32
#include <new>    // std::bad_alloc
#endif

#endif



enum cjm_etag { cjm_tag=0 };
typedef size_t  cjm_siz_t;


CJEXTERNC void cjmemset(void* dst, unsigned char c, cjmsiz_t count);
CJEXTERNC void cjmemzero(void* dst, cjmsiz_t count);
CJEXTERNC void cjmemcpy(void* dst, const void* src, cjmsiz_t count);
CJEXTERNC cjmsiz_t cjmemncpy(void* dst, cjmsiz_t dstSiz, const void* src, cjmsiz_t count);
CJEXTERNC void* cjmemmove(void* dst, const void* src, cjmsiz_t count);
CJEXTERNC cji cjmemcmp(const void* cmp1, const void* cmp2, cjmsiz_t count);

//
// Malloc/Free
//


#ifdef _CJDEBUG_MEM
CJEXTERNC void* _cjmalloc(cjmsiz_t siz, void* p, const cjc* tag,
	const cjc* filename, cjui fileline, const cjc* func_name);

CJEXTERNC void* _cjmzalloc(cjmsiz_t siz, void* p, const cjc* tag,
	const cjc* filename, cjui fileline, const cjc* func_name);

CJEXTERNC typedef void* (*cjmalloc_pfn)(cjmsiz_t siz, void* p, const cjc* tag,
	const cjc* filename, cjui fileline, const cjc* func_name);

#else
CJEXTERNC void* _cjmalloc(cjmsiz_t siz, void* p=cjcjNULL, const cjc* tag=cjcjNULL);

CJEXTERNC typedef void* (*cjmalloc_pfn)(cjmsiz_t siz, void* p = cjcjNULL, const cjc* tag = cjcjNULL);

#endif

CJEXTERNC void  _cjmfree(void* mem, void* p);
CJEXTERNC typedef void(*cjmfree_pfn)(void*, void* p);


#define cjmalloc(siz)				_cjmalloc(siz, 0, 0,   __FILE__, __LINE__, __FUNCTION__)
#define CJMALLOC					_cjmalloc
#define CJMALLOC_P(alloc_, siz_)	alloc_(siz_, 0, 0,  __FILE__, __LINE__, __FUNCTION__)

#define cjmzalloc(siz)				_cjmzalloc(siz, 0, 0,   __FILE__, __LINE__, __FUNCTION__)

#define cjmalloc_tag(siz, tag)		_cjmalloc(siz, 0, tag, __FILE__, __LINE__, __FUNCTION__)

#define cjmfree(mem)				_cjmfree(mem, 0);
#define CJMFREE						_cjmfree
#define CJMFREE_P(free_, mem)		free_(mem, 0)

#define cjmfree_p(x, p)				_cjmfree(x, p)




#ifdef __cplusplus 

template<class T>
T* cjNew() {
	return new(cjNULL, __FILE__, __LINE__, __FUNCTION__) T();
}


struct cjmem {

#ifdef _CJDEBUG_MEM
	static void* operator new(cjm_siz_t siz, const cjc* tag = cjNULL, const cjc* dbg_file = cjNULL, cjui dbg_line = 0, const cjc* dbg_func = cjNULL);
	static void* operator new[](cjm_siz_t siz, const cjc* tag = cjNULL, const cjc* dbg_file = cjNULL, cjui dbg_line = 0, const cjc* dbg_func = cjNULL);
#else
	static void* operator new(cjm_siz_t stSize, const cjc* tag = cjNULL);
	static void* operator new[](cjm_siz_t stSize, const cjc* tag = cjNULL);
#endif

#ifdef _CJDEBUG_MEM
	static void operator delete(void* mem, const cjc* tag, const cjc* dbg_file, cjui dbg_line, const cjc* dbg_func);
	static void operator delete[](void* mem, const cjc* tag, const cjc* dbg_file, cjui dbg_line, const cjc* dbg_func);
#else
	static void operator delete(void* mem, const cjc* tag);
	static void operator delete[](void* mem, const cjc* tag);
#endif

	static void operator delete(void* mem);
	static void operator delete[](void* mem);

	// placement new/delete
	inline static void* operator new(cjm_siz_t, void* p) { return p; }
	inline static void* operator new[](cjm_siz_t, void* p) { return p; }

	inline static void operator delete(void*, void*) {}
	inline static void operator delete[](void*, void*) {}

	// Malloc/Free
#ifdef _CJDEBUG_MEM
	static void* _MAlloc(cjm_siz_t siz, void* p = 0, const cjc* tag = cjNULL, const cjc* dbg_file = 0, cjui dbg_line = 0, const cjc* dbg_func = 0);
#else
	static void* _MAlloc(cjm_siz_t siz, void* p = 0, const cjc* tag = cjNULL);
#endif
	static void  _MDeAlloc(void* mem, void* p = 0);

};


#endif


#endif

