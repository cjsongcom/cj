
#include <cj/cj.h>

#include <cj/cjyaml_json.h>

#include <cJSON.h>
#include <cJSON_Utils.h>

#include <cj/cj_json.h>


CJEXTERNC cjbool cjson_load_from_yaml(cjson** out_json, const cjmc* yaml_path) {
	
	cjassert(out_json);
	cjassert(yaml_path);

	if (yaml_path) {
		if (!cjyaml_load_as_json(out_json, yaml_path, cjNULL))
			return cjfalse;
	}

	return cjtrue;
}

CJEXTERNC cjbool cjson_get_kv_str(cjmc* out, cjstr_siz out_siz, cjson* json,
	const cjmc* key) {

	if (!json || !key)
		return cjfalse;
		
	cjson* o = cJSON_GetObjectItemCaseSensitive(json, key);

	if (!o)
		return cjfalse;

	const cjmc* v = cJSON_GetStringValue(o);

	if (!v)
		return cjfalse;

	if(out)
		cjstrcpy_s(out, out_siz, v);

	return cjtrue;
}

CJEXTERNC cjbool cjson_get_kv_num(cjson_num* out, cjson* json, const cjmc* key) {

	if (!json || !key)
		return cjfalse;

	cjson* o = cJSON_GetObjectItemCaseSensitive(json, key);

	if (!o)
		return cjfalse;

	if (o->type != cJSON_Number)
		return cjfalse;

	if (out)
		*out = cJSON_GetNumberValue(o);

	return cjtrue;
}


CJEXTERNC cjbool cjson_get_kv_int(cjson_int* out, cjson* json, const cjmc* key) {
	
	cjson_num v;

	if (!cjson_get_kv_num(&v, json, key))
		return cjfalse;

	if (out)
		*out = (cjson_int)v;

	return cjtrue;
}

CJEXTERNC cjbool cjson_get_kv_int64(cjson_int64* out, cjson* json, const cjmc* key) {

	cjson_num v;

	if (!cjson_get_kv_num(&v, json, key))
		return cjfalse;

	if (out)
		*out = (cjson_int64)v;

	return cjtrue;
}

CJEXTERNC cjbool cjson_get_dict(cjson** out_dict, cjson* json, const cjmc* key) {

	if (!json || !key)
		return cjfalse;

	cjson* o = cJSON_GetObjectItemCaseSensitive(json, key);

	if (!o)
		return cjfalse;

	if (out_dict)
		*out_dict = o;

	return cjtrue;
}

CJEXTERNC cjbool cjson_get_kv_bool(cjson_bool* out_bool, cjson* json, 
	const cjmc* key) {

	if (!json || !key)
		return cjfalse;

	cjson* o = cJSON_GetObjectItemCaseSensitive(json, key);

	if (!o)
		return cjfalse;

	cjbool rst = cjfalse;

	switch (o->type) {

	case cJSON_True:
		rst = cjtrue;
		break;

	case cJSON_False:
		break;

	case cJSON_String: {
		const cjmc* v = cJSON_GetStringValue(o);

		if (!cjstrcmp(v, "true") || !cjstrcmp(v, "True"))
			rst = cjtrue;
		else if (!cjstrcmp(v, "false") || !cjstrcmp(v, "False"))
			rst = cjfalse;
		else
			return cjfalse;
	}
	break;

	case cJSON_Number: {
		cjdouble v = cJSON_GetNumberValue(o);

		if (v == 0)
			rst = cjfalse;
		else if (v == 1)
			rst = cjtrue;
		else
			return cjfalse;
	}
	break;

	default:
		return cjfalse;
	}

	if (out_bool)
		*out_bool = rst;

	return cjtrue;
}

CJEXTERNC cjbool cjson_get_kv_ary(cjson** out_json_ary, cjson* json, const cjmc* key) {

	if (!json || !key)
		return cjfalse;

	cjson* o = cJSON_GetObjectItemCaseSensitive(json, key);

	if (!o)
		return cjfalse;

	if (o->type != cJSON_Array)
		return cjfalse;

	if (out_json_ary)
		*out_json_ary = o;

	return cjtrue;
}

//
// parse cjson ary
//

static CJEXTERNC cjbool _cjson_get_ary(cjson** out_json_ary, cjson* json, 
	const cjmc* key) {

	if (!json || !key)
		return cjfalse;

	cJSON* o = cJSON_GetObjectItemCaseSensitive(json, key);

	if (!o)
		return cjfalse;

	if (o->type != cJSON_Array)
		return cjfalse;

	if (out_json_ary)
		*out_json_ary = o;

	return cjtrue;
}


// [ "sun", "mon", "tue", "wed", "thu", "fri", "sat" ]
CJEXTERNC cjbool cjson_get_kv_wdays(cj_wdays* out_wdays, cjson* json, const cjmc* key) {

	cjson* ary;
		
	if(!_cjson_get_ary(&ary, json, key))
		return cjfalse;

	if (!out_wdays)
		return cjtrue;

	cj_wdays wdays = { 0 };
	cji num_ary = cJSON_GetArraySize(ary);

	for (cji i = 0; i < num_ary; i++) {
		cJSON* item = cJSON_GetArrayItem(ary, i);

		if (item->type != cJSON_String)
			return cjfalse;

		const cjmc* v = cJSON_GetStringValue(item);

		if (!cjstrcmp(v, "sun")) {
			wdays.chk.sun = 1;
		} else if (!cjstrcmp(v, "mon")) {
			wdays.chk.mon = 1;
		} else if (!cjstrcmp(v, "tue")) {
			wdays.chk.tue = 1;
		} else if (!cjstrcmp(v, "wed")) {
			wdays.chk.wed = 1;
		} else if (!cjstrcmp(v, "thu")) {
			wdays.chk.thu = 1;
		} else if (!cjstrcmp(v, "fri")) {
			wdays.chk.fri = 1;
		} else if (!cjstrcmp(v, "sat")) {
			wdays.chk.sat = 1;
		} else {
			return cjfalse;
		}
	}

	if (out_wdays)
		*out_wdays = wdays;

	return cjtrue;
}

// [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0=last_month ]
CJEXTERNC cjbool cjson_get_kv_month(cj_months* out_month, cjson* json, const cjmc* key) {

	cjson* ary;

	if (!_cjson_get_ary(&ary, json, key))
		return cjfalse;

	if (!out_month)
		return cjtrue;

	cj_months month = { 0 };

	cji num_ary = cJSON_GetArraySize(ary);

	for (cji i = 0; i < num_ary; i++) {
		cJSON* item = cJSON_GetArrayItem(ary, i);

		if (item->type != cJSON_Number)
			return cjfalse;

		cji v = (cji)cJSON_GetNumberValue(item);

		if (v < 0 || v > 12)
			return cjfalse;
			
		month.dat.v[v] = 1;
	}

	*out_month = month;

	return cjtrue;
}

/*
 [  1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
   13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
   23, 24, 25, 26, 27, 28, 29, 30, 31, 0 ]
   0: last day
*/
CJEXTERNC cjbool cjson_get_kv_days(cj_days* out_days, cjson* json, const cjmc* key) {

	cjson* ary;

	if (!_cjson_get_ary(&ary, json, key))
		return cjfalse;

	if (!out_days)
		return cjtrue;

	cj_days days = { 0 };

	cji num_ary = cJSON_GetArraySize(ary);

	for (cji i = 0; i < num_ary; i++) {
		cJSON* item = cJSON_GetArrayItem(ary, i);

		if (item->type != cJSON_Number)
			return cjfalse;

		cji v = (cji)cJSON_GetNumberValue(item);

		if (v < 0 || v > 31)
			return cjfalse;

		days.dat.v[v] = 1;
	}

	*out_days = days;

	return cjtrue;
}

// [ 1, 2, 3, 4, 0 ]
// 1:first week, 2:second week, .. 0: last day
CJEXTERNC cjbool cjson_get_kv_weeks(cj_weeks* out_weeks, cjson* json, const cjmc* key) {

	cjson* ary;

	if (!_cjson_get_ary(&ary, json, key))
		return cjfalse;

	if (!out_weeks)
		return cjtrue;

	cj_weeks weeks = { 0 };
	cji num_ary = cJSON_GetArraySize(ary);

	for (cji i = 0; i < num_ary; i++) {

		cJSON* item = cJSON_GetArrayItem(ary, i);

		if (item->type != cJSON_Number)
			return cjfalse;

		cji v = (cji)cJSON_GetNumberValue(item);

		if (v < 0 || v > 4)
			return cjfalse;

		weeks.dat.v[v] = 1;
	}

	*out_weeks = weeks;

	return cjtrue;
}

CJEXTERNC cjbool cjson_get_kv_duration(cj_duration* out_duration, cjson* json, const cjmc* key) {

	if (!json || !key)
		return cjfalse;

	cjmc buf[CJ_MAX_CJSON_VAL] = { 0 };

	cj_duration duration;

	// 1s 10m 20h
	if (cjson_get_kv_str(buf, CJA_SIZ(buf), json, key)) {
		if (!cj_parse_duration_str(&duration.v, buf))
			return cjfalse;

	} else {
		if (!cjson_get_kv_int64(&duration.v, json, key))
			return cjfalse;

	}

	if (out_duration)
		*out_duration = duration;

	return cjtrue;
}

CJEXTERNC cjbool cjson_get_kv_interval(cj_interval* out_interval, cjson* json, const cjmc* key) {

	if (!json || !key)
		return cjfalse;

	cjmc buf[CJ_MAX_CJSON_VAL] = { 0 };

	cj_interval interval;

	// 1s 10m 20h
	if (cjson_get_kv_str(buf, CJA_SIZ(buf), json, key)) {
		if (!cj_parse_interval_str(&interval.v, buf))
			return cjfalse;

	} else {
		if (!cjson_get_kv_int64(&interval.v, json, key))
			return cjfalse;

	}

	if (out_interval)
		*out_interval = interval;

	return cjtrue;
}


