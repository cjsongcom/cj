
#include "cj.h"
#include "cjuuid.h"


//
// cjuuid
//

extern int _impl_cjuuid4_str_gen(char* out, int out_siz);

static cjuuid4_str cjuuid4_str_err;

CJEXTERNC cjuuid4_str cjuuid4_str_gen() {
	cjuuid4_str uuid_str;
	
	if(_impl_cjuuid4_str_gen(uuid_str.id, CJUUID4_SIZ)){
		return cjuuid4_str_err;
	}

	return uuid_str;
}

CJEXTERNC cjbool cjuuid4_str_is_equal(cjuuid4_str* l, cjuuid4_str* r) {
	if (!l || !r) 
		return cjfalse;

	if (!cjmemcmp(l, r, sizeof(CJUUID4_SIZ)))
		return cjtrue;

	return cjfalse;
}
