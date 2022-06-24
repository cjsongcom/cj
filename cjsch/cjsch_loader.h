
#ifndef _CJSCH_LOADER_H_
#define _CJSCH_LOADER_H_

#include <cj/cjyaml.h>

typedef struct cjsch_loader_t {

	cjmc    yaml_path[CJ_MAX_PATH];
	cjmc	last_err_msg[CJ_MAX_ERR_MSG];

	cjson* sch_json;

} cjsch_loader;


CJEXTERNC cjbool cjsch_loader_create(cjsch_loader** out_loader, const cjmc* yaml_path);
CJEXTERNC void cjsch_loader_destroy(cjsch_loader* loader);

CJEXTERNC cjbool cjsch_loader_load(cjsch_loader* loader);


#endif
