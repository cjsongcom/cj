
#ifndef _CJSCH_PARSE_JSON_H_
#define _CJSCH_PARSE_JSON_H_

#include <cj/cjsch/cjsch_trigger.h>


typedef struct {
	cjmc						name[CJ_MAX_CJSCH_TRIGGER_NAME];
	cjmc						class_name[CJ_MAX_CJSCH_CLASS_NAME];
	cji							rev;

} _cjschp_triggers;


typedef struct cjsch_parse_json_sch_ctx_t {

	cjary               triggers;
	cjary				actions;

} cjsch_parse_json_sch_ctx;


typedef enum cjsch_parse_err_t {

	CJSCH_PARSE_ERR_OK = 0,

	CJSCH_PARSE_ERR_INVALID_TOKEN		= -100,


} cjsch_parse_err;



CJEXTERNC cjbool cjsch_parse_json_sch(cjsch_parse_json_sch_ctx* ctx,
	cjson* json, cjsch_parse_err* out_err, const cjmc** out_err_msg);




#endif

