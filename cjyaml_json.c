
#include <cj/cj.h>
#include <cj/cjyaml.h>

#include <cj/cjyaml_json.h>


typedef struct yaml_parse_as_json_prm_t {

	cjson* json;

} yaml_parse_as_json_prm;


static cjbool recur_parse_yaml_as_json(cjyaml_parse_ctx* parse_ctx,
	cjyaml_token_type opener_token, cjson* json_par) {

	switch (opener_token) 
	{

	case CJYAML_TOKEN_BLOCK_SEQUENCE_START:
	{
		// block-sequence-start-token
		// block-entry-token
		// block-mapping-start-token 

		cjson* json_child = cjNULL;
		cji seq_idx = 0;

		do {
			cjyaml_token_info token_info;
			cjyaml_token_type token_type;
		
			// block-entry-token
			if (!cjyaml_parse_token(parse_ctx, &token_info, cjNULL, 0,
				cjNULL, cjNULL))
				return cjfalse;

			token_type = token_info.token_type;

			if (token_type == CJYAML_TOKEN_BLOCK_END)
				return cjtrue;

			cjyaml_token_info next_token_info;
			cjyaml_token_type next_token_type;

			if (token_type == CJYAML_TOKEN_BLOCK_ENTRY) {

				if (!cjyaml_parse_token(parse_ctx, &next_token_info, cjNULL, 0,
					cjNULL, cjNULL)) {
					return cjfalse;
				}

				next_token_type = next_token_info.token_type;

				if (next_token_type == CJYAML_TOKEN_BLOCK_END)
					return cjtrue;

				{
					json_child = cJSON_CreateObject();
					cJSON_AddItemToArray(json_par, json_child);
				}

				// not json_child
				if (!recur_parse_yaml_as_json(parse_ctx, next_token_type, json_child))
					return cjfalse;
			}
			else {
				return cjfalse;
			}

		} while (1);
	}
	break;

	case CJYAML_TOKEN_BLOCK_MAPPING_START:
	{
		// key
		// scalar
		// val
		// scalar   or other
		cjbool skip_key_token_parse = cjfalse;
		cjson* json_child = cjNULL;

		do {
			cjmc key[CJ_MAX_CJYAML_KEY_SIZ] = { 0 };
			cjmc val[CJ_MAX_CJYAML_VAL_SIZ] = { 0 };
			cjyaml_token_type val_token_type;

			if (!cjyaml_parse_key_val_mapping(parse_ctx, skip_key_token_parse, key,
				CJ_MAX_CJYAML_KEY_SIZ, val, CJ_MAX_CJYAML_VAL_SIZ, &val_token_type, 
				json_par, &json_child)) {

				// parsed 'BLOCK_END_TOKEN' instead KEY_TOKEN
				if (parse_ctx->last_err_parsed_token == CJYAML_TOKEN_BLOCK_END) {
					return cjtrue;
				}

				return cjfalse;
			}

			if (val_token_type == CJYAML_TOKEN_BLOCK_END)
				return cjtrue;

			if (val_token_type == CJYAML_TOKEN_KEY) {
				// key-token
				// scalar-token
				// value-token
				// --omitted scalar-token--
				// key-token <<
				/*
				  aaa:
					bbb:  <<
					ccc:
				*/
				skip_key_token_parse = cjtrue;
				continue;
			}

			if (val_token_type != CJYAML_TOKEN_SCALAR_TOKEN) {
				if (!recur_parse_yaml_as_json(parse_ctx, val_token_type, json_child))
					return cjfalse;
			}

			skip_key_token_parse = cjfalse;

		} while (1);
	}
	break;

	case CJYAML_TOKEN_FLOW_SEQUENCE_START:
	{
		// scalar
		// flow-entry
		// CJYAML_TOKEN_FLOW_SEQUENCE_END
		// [ 1, 2, 3]
		cjson* json_flow_ary = cjNULL;

		{
			if (json_par->type != cJSON_Array) {
				cjassert(0);
				return cjfalse;
			}

			json_flow_ary = json_par;
		}

		do {
			cjmc val[CJ_MAX_CJYAML_VAL_SIZ] = { 0 };
			cjyaml_token_info scalar_token_info;
			cjyaml_token_type scalar_token_type;

			// scalar
			if (!cjyaml_parse_token(parse_ctx, &scalar_token_info, val,
				CJ_MAX_CJYAML_VAL_SIZ, cjNULL, cjNULL)) {
				return cjfalse;
			}

			scalar_token_type = scalar_token_info.token_type;

			if (scalar_token_type == CJYAML_TOKEN_FLOW_ENTRY)
				continue;

			if (scalar_token_type == CJYAML_TOKEN_FLOW_SEQUENCE_END)
				return cjtrue;

			cjyaml_token_info next_token_info;
			cjyaml_token_type next_token_type;

			if (scalar_token_type == CJYAML_TOKEN_SCALAR_TOKEN) {
				cjson* o;

				if (scalar_token_info.is_quoted_value) {
					o = cJSON_CreateString(val);

				} else {
					if (cj_is_number(val, cjtrue, cjfalse))
						o = cJSON_CreateNumber(atoi(val));
					else
						o = cJSON_CreateString(val);
				}
				
				if (!cJSON_AddItemToArray(json_flow_ary, o))
					return cjfalse;

				// expect flow-entry-token
				if (!cjyaml_parse_token(parse_ctx, &next_token_info, cjNULL, 0,
					cjNULL, cjNULL)) {
					return cjfalse;
				}

				next_token_type = next_token_info.token_type;

				if (next_token_type == CJYAML_TOKEN_FLOW_ENTRY)
					continue;

				if (next_token_type == CJYAML_TOKEN_FLOW_SEQUENCE_END)
					return cjtrue;

				cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_INVALID_TOKEN,
					"invalid token found, prev=%d, cur=%d",
					CJYAML_TOKEN_FLOW_SEQUENCE_START, next_token_type);

				return cjfalse;
			}


		} while (1);

	}
	break;

	case CJYAML_TOKEN_FLOW_MAPPING_START:
	{
		cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_IMPL_NOT_YET,
			"implement not yet, opener=CJYAML_TOKEN_FLOW_MAPPING_START");

		return cjfalse;
	}
	break;

	default:
	{
		cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_IMPL_NOT_YET,
			"implement not yet, opener=%d", opener_token);

		return cjfalse;
	}
	break;

	}
	
	return cjtrue;
}

CJEXTERNC cjbool cjyaml_parse_worker_json(cjyaml_parse_ctx* parse_ctx,
	cjyaml_parse_evt evt, void* evt_prm) {

	yaml_parse_as_json_prm* user_prm 
		= parse_ctx->prm;

	switch (evt) {

	case CJYAML_PARSE_EVT_BEGIN:
	{
		// create root json object
		cjassert(!user_prm->json);

		user_prm->json = cJSON_CreateObject();

		// parse token until matching first block-mapping-start-token
		do {
			cjyaml_token_info token_info;

			if (!cjyaml_parse_token(parse_ctx, &token_info, cjNULL, 0, cjNULL, cjNULL))
				return cjfalse;

			if (token_info.token_type == CJYAML_TOKEN_STREAM_START)
				return cjtrue;

		} while (1);

	}
	break;

	case CJYAML_PARSE_EVT_ON_PARSE:
	{
		cjyaml_token_info val_token_info;
		
		do {
			if (!cjyaml_parse_token(parse_ctx, &val_token_info, cjNULL, 0, cjNULL, cjNULL))
				return cjfalse;

			if (val_token_info.token_type == CJYAML_TOKEN_STREAM_END)
				return cjtrue;

			if (!recur_parse_yaml_as_json(parse_ctx, val_token_info.token_type, user_prm->json))
				return cjfalse;

			//if (!cjyaml_parse_key_val_mapping(parse_ctx, key, CJ_MAX_CJYAML_KEY_SIZ,
			//	val, CJ_MAX_CJYAML_VAL_SIZ, val_token_type))
			//	return cjfalse;

			//if (val_token_type != CJYAML_TOKEN_SCALAR_TOKEN) {
			//	recur_parse_yaml_as_json(parse_ctx, val_token_type);
			//}

		} while (1);

	}
	break;

	case CJYAML_PARSE_EVT_END:
	{

	}
	break;

	case CJYAML_PARSE_EVT_ERR_PARSE_FAILED:
		// passthru
	default:
		return cjfalse;
	}

	return cjtrue;

}

CJEXTERNC cjbool cjyaml_conv_json(cjson** out_json, cjyaml* yaml) {

	if (!out_json)
		return cjfalse;

	yaml_parse_as_json_prm prm = { 0 };
	cjyaml_err err;

	if (!cjyaml_parse(yaml, cjyaml_parse_worker_json, &prm, &err)) {
		return cjfalse;
	}

	if (out_json)
		*out_json = prm.json;

	return cjtrue;
}

CJEXTERNC cjbool cjyaml_load_as_json(cjson** out_json, const cjmc* yaml_path,
	cjyaml_err* out_err) {

	cjyaml* yaml = cjNULL;
	cjson* json = cjNULL;
	cjyaml_err err = CJYAML_ERR_OK;

	if (!yaml_path) {
		err = CJYAML_ERR_INVALID_PATH;
		goto done_exit;
	}

	if (!cjyaml_create(&yaml, yaml_path, cjtrue)) {
		err = CJYAML_ERR_INVALID_PATH;
		goto done_exit;
	}

	if (!cjyaml_open(yaml)) {
		err = CJYAML_ERR_OPEN_YAML_FILE;
		goto done_exit;
	}

	if (!cjyaml_conv_json(&json, yaml)) {
		err = CJYAML_ERR_CONV_FAIL_TO_JSON;
		goto done_exit;
	}

	if (out_json)
		*out_json = json;

done_exit:
	if (yaml)
		cjyaml_destroy(yaml);

	if (out_err)
		*out_err = err;

	return (err == CJYAML_ERR_OK) ? cjtrue : cjfalse;
}
