
#ifndef _CJ_JSON_H_
#define _CJ_JSON_H_

#include <cJSON.h>
#include <cJSON_Utils.h>

typedef cJSON								cjson;
typedef cJSON_Hooks							cjson_hook;


#define cjson_create_root					cJSON_CreateObject
#define cjson_destroy(json)					cJSON_Delete(json)

#define cjson_print(json)					cJSON_Print(json);

#define cjson_new_ary						cJSON_CreateArray
#define cjson_ary_siz(json)					cJSON_GetArraySize(json)
#define cjson_ary_at(json, idx)				cJSON_GetArrayItem(json, idx)

#define cjson_get_key(json, key)			cJSON_GetObjectItemCaseSensitive(json, key)


typedef cji									cjson_int;
typedef cji64								cjson_int64;

typedef cjdouble							cjson_num;
typedef cjbool								cjson_bool;


CJEXTERNC cjbool cjson_load_from_yaml(cjson** out_json, const cjmc* yaml_path);


// "key" : { .. }
CJEXTERNC cjbool cjson_get_dict(cjson** out_dict, cjson* json, const cjmc* key);


//
// key-value 
//

enum {
	CJ_MAX_CJSON_KEY			= 64,

	CJ_MAX_CJSON_VAL_SHORT		= 64,
	CJ_MAX_CJSON_VAL			= 256,
	CJ_MAX_CJSON_VAL_LONG		= 1024,

};

CJEXTERNC cjbool cjson_get_kv_str(cjmc* out, cjstr_siz out_siz, cjson* json,
	const cjmc* key);

CJEXTERNC cjbool cjson_get_kv_int(cjson_int* out, cjson* json, const cjmc* key);
CJEXTERNC cjbool cjson_get_kv_int64(cjson_int64* out, cjson* json, const cjmc* key);

CJEXTERNC cjbool cjson_get_kv_num(cjson_num* out, cjson* json, const cjmc* key);

// "key" : 1/true/"true" , 0/false/"false"
CJEXTERNC cjbool cjson_get_kv_bool(cjson_bool* out_bool, cjson* json, const cjmc* key);

CJEXTERNC cjbool cjson_get_kv_ary(cjson** out_json_ary, cjson* json, const cjmc* key);


// parse cjson ary into 

CJEXTERNC cjbool cjson_get_kv_wdays(cj_wdays* out_wdays, cjson* json, const cjmc* key);
CJEXTERNC cjbool cjson_get_kv_month(cj_months* out_month, cjson* json, const cjmc* key);
CJEXTERNC cjbool cjson_get_kv_days(cj_days* out_days, cjson* json, const cjmc* key);
CJEXTERNC cjbool cjson_get_kv_weeks(cj_weeks* out_weeks, cjson* json, const cjmc* key);

CJEXTERNC cjbool cjson_get_kv_duration(cj_duration* out_duration, cjson* json, const cjmc* key);
CJEXTERNC cjbool cjson_get_kv_interval(cj_interval* out_interval, cjson* json, const cjmc* key);


#endif

