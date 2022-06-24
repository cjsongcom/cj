
#ifndef _CJUUID_H_
#define _CJUUID_H_


// 37 => 36(guid) + 1byte(NULL)
// "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxx-" to 
// "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxNULL"
#define CJUUID4_SIZ  (int)sizeof("xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx")


typedef struct cjuuid4_str_t {
	char id[CJUUID4_SIZ];
} cjuuid4_str;


CJEXTERNC cjuuid4_str cjuuid4_str_gen();

CJEXTERNC cjbool cjuuid4_str_is_equal(cjuuid4_str* l, cjuuid4_str* r);




#endif

