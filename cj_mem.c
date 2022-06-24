
#include "cj.h"
#include "cj_mem.h"


CJEXTERNC void cjmemset(void* dst, unsigned char c, cjmsiz_t count)
{
	memset(dst, c, count);
}

CJEXTERNC void cjmemzero(void* dst, cjmsiz_t count)
{
	memset(dst, 0, count);
}

CJEXTERNC void cjmemcpy(void* dst, const void* src, cjmsiz_t count)
{
	memcpy(dst, src, count);
}

CJEXTERNC cjmsiz_t cjmemncpy(void* dst, cjmsiz_t dstSiz, const void* src, cjmsiz_t count)
{
	if (dstSiz < count)
		count = dstSiz;

	memcpy(dst, src, count);
	return count;
}

CJEXTERNC void* cjmemmove(void* dst, const void* src, cjmsiz_t count)
{
	return memmove(dst, src, count);
}

CJEXTERNC cji cjmemcmp(const void* cmp1, const void* cmp2, cjmsiz_t count)
{
	return memcmp(cmp1, cmp2, count);
}


//
// Malloc/Free
//

#ifdef _CJDEBUG_MEM

CJEXTERNC void* _cjmalloc(cjmsiz_t siz, void* p, const char* tag,
	const char* filename, unsigned int fileline, const char* func_name)
{
	if (!siz)
		return cjnull;

	return malloc(siz);
}

CJEXTERNC void* _cjmzalloc(cjmsiz_t siz, void* p, const char* tag,
	const char* filename, unsigned int fileline, const char* func_name)
{
	if (!siz)
		return cjnull;

	void *m = malloc(siz);

	if (m)
		memset(m, 0, siz);

	return m;
}


#else
CJEXTERNC void* _cjmalloc(cjmsiz_t siz, void* p, const char* tag)
{
	if (!siz)
		return cjnull;

	return malloc(siz);
}

#endif

CJEXTERNC void  _cjmfree(void* mem, void* p)
{
	if (!mem)
		return;

	free(mem);
}

