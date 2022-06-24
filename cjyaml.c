
#include <cj/cj.h>
#include <yaml.h>

#include <cJSON.h>
#include <cJSON_Utils.h>

#include <cj/cjyaml.h>


#define INDENT "  "
#define STRVAL(x) ((x) ? (char*)(x) : "")

static void indent(int level) {
	int i;
	for (i = 0; i < level; i++) {
		printf("%s", INDENT);
	}
}

static void print_token(yaml_token_t* token) {
	static int level = 0;

	switch (token->type) {

	case YAML_NO_TOKEN:
		indent(level);
		printf("no-token (%d)\n", token->type);
		break;

	case YAML_VERSION_DIRECTIVE_TOKEN:
		indent(level);
		printf("version-directive-token (%d)\n", token->type);
		break;

	case YAML_TAG_DIRECTIVE_TOKEN:
		indent(level);
		printf("tag-directive-token (%d)\n", token->type);
		break;

	case YAML_STREAM_START_TOKEN:
		indent(level++);
		printf("stream-start-token (%d)\n", token->type);
		break;
	case YAML_STREAM_END_TOKEN:
		indent(--level);
		printf("stream-end-token (%d)\n", token->type);
		break;

	case YAML_DOCUMENT_START_TOKEN:
		indent(level++);
		printf("document-start-token (%d)\n", token->type);
		break;
	case YAML_DOCUMENT_END_TOKEN:
		indent(--level);
		printf("document-end-token (%d)\n", token->type);
		break;

		// block
	case YAML_BLOCK_SEQUENCE_START_TOKEN:
		indent(level++);
		printf("block-sequence-start-token (%d)\n", token->type);
		break;
	case YAML_BLOCK_MAPPING_START_TOKEN:
		indent(level++);
		printf("block-mapping-start-token (%d)\n", token->type);
		break;
	case YAML_BLOCK_END_TOKEN:
		indent(--level);
		printf("block-end-token (%d)\n", token->type);
		break;

		//
		// flow 
		//
	case YAML_FLOW_SEQUENCE_START_TOKEN:
		indent(level++);
		printf("flow-sequnece-start-token (%d)\n", token->type);
		break;
	case YAML_FLOW_SEQUENCE_END_TOKEN:
		indent(--level);
		printf("flow-sequnece-end-token (%d)\n", token->type);
		break;
	case YAML_FLOW_MAPPING_START_TOKEN:
		indent(level++);
		printf("flow-mapping-start-token (%d)\n", token->type);
		break;
	case YAML_FLOW_MAPPING_END_TOKEN:
		indent(--level);
		printf("flow-mapping-end-token (%d)\n", token->type);
		break;

		//
		// entry token
		//
	case YAML_BLOCK_ENTRY_TOKEN:
		indent(level);
		printf("block-entry-token (%d)\n", token->type);
		break;
	case YAML_FLOW_ENTRY_TOKEN:
		indent(level);
		printf("flow-entry-token (%d)\n", token->type);
		break;

		//
		// key-value token
		//
	case YAML_KEY_TOKEN:
		indent(level);
		printf("key-token (%d)\n", token->type);
		break;
	case YAML_VALUE_TOKEN:
		indent(level);
		printf("value-token (%d)\n", token->type);
		break;

		//
		// alias / anchor / tag / scalar token
		//

	case YAML_ALIAS_TOKEN:
		indent(level);
		printf("alias-token (%d)\n", token->type);
		break;

	case YAML_ANCHOR_TOKEN:
		indent(level);
		printf("anchor-token (%d)\n", token->type);
		break;

	case YAML_TAG_TOKEN:
		indent(level);
		printf("tag-token (%d)\n", token->type);
		break;

	case YAML_SCALAR_TOKEN:
	{
		cjmc mc_val[1024] = { 0 };
		cj_conv_u8c_to_mc(mc_val, 1024, token->data.scalar.value);

		indent(level);

		//printf("scalar-token (%d) = {value=\"%s\", length=%d}\n",
		//	token->type,
		//	STRVAL(token->data.scalar.value),
		//	(int)token->data.scalar.length);

		printf("scalar-token (%d) = {value=\"%s\", length=%d}\n",
			token->type,
			mc_val,
			(int)token->data.scalar.length);
	}
	break;
	}

	if (level < 0) {
		fprintf(stderr, "indentation underflow!\n");
		level = 0;
	}
}

CJEXTERNC cjbool _cjyaml_is_quoted_scalar(void* _token){ 
	yaml_token_t* token = _token;

	if (token && (token->data.scalar.style == YAML_SINGLE_QUOTED_SCALAR_STYLE ||
		token->data.scalar.style == YAML_DOUBLE_QUOTED_SCALAR_STYLE))
		return cjtrue;

	return cjfalse;
}


static cjbool _cjyaml_load_file(cjyaml* yaml, cjmc** out_src,
	cjfile_off* out_src_siz) {

	if (!yaml)
		return cjfalse;

	cjfile* yaml_file = cjNULL;
	cjmc* src = cjNULL;
	cjfile_off src_siz = 0;

	if (!cjfile_create_for_read(&yaml_file, yaml->path, cjNULL)) {
		yaml->err = CJYAML_ERR_INVALID_YAML_FILE_PATH;
		goto done_exit;
	}

	if (!cjfile_open(yaml_file)) {
		yaml->err = CJYAML_ERR_OPEN_YAML_FILE;
		goto done_exit;
	}

	// libyaml input source does not allow null terminator
	src = cjfile_read_all_utf8_mem(yaml_file, &src_siz,
		cjfalse, cjfalse, CJMALLOC, CJMFREE);

	if (!src)
		yaml->err = CJYAML_ERR_LOAD_YAML_FILE;

done_exit:
	*out_src = src;
	*out_src_siz = src_siz;

	cjfile_destroy(yaml_file);

	return yaml->err == CJYAML_ERR_OK ? cjtrue : cjfalse;
}

CJEXTERNC cjbool cjyaml_create(cjyaml** out_yaml, const cjmc* path, 
	cjbool load_file) {

	if (!_cjyaml_create(out_yaml, path, cjNULL, CJMALLOC, CJMFREE))
		return cjfalse;

	if (load_file)
		return cjyaml_open(*out_yaml);

	return cjtrue;
}

CJEXTERNC cjbool _cjyaml_create(cjyaml** out_yaml, const cjmc* path,
	cjyaml_err* out_err, cjmalloc_pfn alloc_m, cjmfree_pfn free_m) {

	cjyaml* rst_yaml = cjNULL;
	cjyaml_err err = CJYAML_ERR_OK;

	if (!out_yaml || !path) {
		err = CJYAML_ERR_INVALID_PARAM;
		goto done_exit;
	}

	const cjmc* resolved_path 
		= cjfile_resolve_full_path(path);

	if (!resolved_path) {
		err = CJYAML_ERR_INVALID_PATH;
		goto done_exit;
	}

	cjyaml* yaml = CJMALLOC_P(alloc_m, sizeof(cjyaml));
	cjmemzero(yaml, sizeof(cjyaml));

	yaml->alloc_m = alloc_m;
	yaml->free_m = free_m;

	yaml->err = err;
	yaml->hint = CJFILE_PATH_HINT_BASE;

	cjstrcpy_s(yaml->path, CJFILE_MAX_PATH, path);
	yaml->name = cjfile_get_filename_from_path(yaml->path);

	rst_yaml = yaml;


done_exit:
	*out_yaml = rst_yaml;

	if (out_err)
		*out_err = err;

	return (err == CJYAML_ERR_OK) ? cjtrue : cjfalse;
}

CJEXTERNC void cjyaml_destroy(cjyaml* yaml) {

	if (!yaml)
		return;

	if (yaml->yaml_src)
		CJMFREE_P(yaml->free_m, yaml->yaml_src);

	CJMFREE_P(yaml->free_m, yaml);
}

CJEXTERNC cjbool cjyaml_open(cjyaml* yaml) {

	if (!yaml)
		return cjfalse;

	if (yaml->is_opened)
		return cjtrue;

	cjassert(!yaml->yaml_src);
	cjassert(!yaml->yaml_src_siz);

	cjmc* yaml_src = cjNULL;
	cjfile_off yaml_src_siz = 0;

	if (!_cjyaml_load_file(yaml, &yaml->yaml_src, &yaml->yaml_src_siz))
		return cjfalse;

	yaml->is_opened = cjtrue;

	return cjtrue;
}


//
// cjyaml_parse
//

CJEXTERNC cjbool _cjyaml_create_parse_ctx(cjyaml_parse_ctx** out_parse_ctx, 
	cjyaml* yaml, void* prm) {

	if (!yaml)
		return cjfalse;

	if (!yaml->yaml_src || !yaml->yaml_src_siz) {
		yaml->err = CJYAML_ERR_EMPTY_YAML;

		return cjfalse;
	}

	cjyaml_parse_ctx* ctx =
		cjmzalloc(sizeof(cjyaml_parse_ctx));

	yaml_parser_t* parser = cjmzalloc(sizeof(yaml_parser_t));
	yaml_parser_initialize(parser);

	// yaml_src does not end with null terminator
	yaml_parser_set_input_string(parser, yaml->yaml_src, (size_t)yaml->yaml_src_siz);

	ctx->parser = parser;
	ctx->prm = prm;

	*out_parse_ctx = ctx;

	return cjtrue;
}

CJEXTERNC void _cjyaml_destroy_parse_ctx(cjyaml_parse_ctx* parse_ctx) 	{

	if (!parse_ctx)
		return;

	cjmfree(parse_ctx->parser);
	cjmfree(parse_ctx);
}


CJEXTERNC cjbool cjyaml_parse(cjyaml* yaml, cjyaml_parse_worker_pfn worker,
	void* prm, cjyaml_err* out_parse_err) {

	cjyaml_err parse_err = CJYAML_ERR_OK;
		
	if (!yaml || !worker) {
		parse_err = CJYAML_ERR_PARSE_INVALID_PARAM;

		goto err_exit;
	}

	cjyaml_parse_ctx* parse_ctx = cjNULL;
	
	if (!_cjyaml_create_parse_ctx(&parse_ctx, yaml, prm)) {
		parse_err = CJYAML_ERR_PARSE_CREATE_CTX;

		goto err_exit;
	}

	if (!worker(parse_ctx, CJYAML_PARSE_EVT_BEGIN, cjNULL)) {
		parse_err = CJYAML_ERR_PARSE_ABORTED;

		goto err_exit;
	}

	if (!worker(parse_ctx, CJYAML_PARSE_EVT_ON_PARSE, cjNULL)) {
		parse_err = CJYAML_ERR_PARSE_ABORTED;

		goto err_exit;
	}

	worker(parse_ctx, CJYAML_PARSE_EVT_END, cjNULL);

	_cjyaml_destroy_parse_ctx(parse_ctx);

	return cjtrue;

err_exit:
	if (out_parse_err)
		*out_parse_err = parse_err;

	_cjyaml_destroy_parse_ctx(parse_ctx);

	return cjfalse;
}

CJEXTERNC void cjyaml_parse_set_err(cjyaml_parse_ctx* parse_ctx,
	cjyaml_err err, const cjmc* err_msg_fmt, ...) {

	if (!parse_ctx)
		return;

	parse_ctx->last_err = err;

	if (err_msg_fmt) {
		va_list ap;
		va_start(ap, err_msg_fmt);

		vsnprintf(parse_ctx->last_err_msg,
			sizeof(parse_ctx->last_err_msg) / sizeof(parse_ctx->last_err_msg[0]),
			err_msg_fmt, ap);

		va_end(ap);

	} else {
		parse_ctx->last_err_msg[0] = cjNULL;
	}
}


// a set of key-value pairs where the key ends with a colon
// > name: tom
CJEXTERNC cjbool cjyaml_parse_key_val_mapping(cjyaml_parse_ctx* parse_ctx, 
	cjbool skip_key_token_parse, cjmc* out_key, cjstr_siz out_key_siz,
	cjmc* out_val, cjstr_siz out_val_siz, cjyaml_token_type* out_value_token,
	cjson* json_par, cjson** out_json_child) {

	if (!parse_ctx)
		return cjfalse;

	yaml_parser_t* parser = parse_ctx->parser;

	//
	// parse key token
	//

	if (!skip_key_token_parse) {

		yaml_token_t token_key;

		if (!yaml_parser_scan(parser, &token_key)) {
			yaml_token_delete(&token_key);
			cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_FAIL, parser->problem);

			return cjfalse;
		}

		if (token_key.type != YAML_KEY_TOKEN) {
			// YAML_BLOCK_END_TOKEN
			parse_ctx->last_err_parsed_token = token_key.type;
			parse_ctx->last_err = CJYAML_ERR_PARSE_NOT_KEY_TOKEN;

			yaml_token_delete(&token_key);

			return cjfalse;
		}

		yaml_token_delete(&token_key);
	}


	//
	// parse key scalar(string,num)
	//

	yaml_token_t token_key_scalar;

	if (!yaml_parser_scan(parser, &token_key_scalar)) {
		yaml_token_delete(&token_key_scalar);
		cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_FAIL, parser->problem);

		return cjfalse;
	}

	if (token_key_scalar.type != YAML_SCALAR_TOKEN) {
		cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_FAIL,
			"invalid token-type, wanted:scalar token, parsed=%d", token_key_scalar.type);

		yaml_token_delete(&token_key_scalar);

		return cjfalse;
	}

	if(out_key)
		cj_conv_u8c_to_mc(out_key, out_key_siz, token_key_scalar.data.scalar.value);

	yaml_token_delete(&token_key_scalar);


	//
	// parse value token
	//

	yaml_token_t token_value;

	if (!yaml_parser_scan(parser, &token_value)) {
		cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_FAIL, parser->problem);

		yaml_token_delete(&token_value);

		return cjfalse;
	}

	if (token_value.type != YAML_VALUE_TOKEN) {
		
		cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_FAIL,
			"invalid token-type, wanted:value token, parsed=%d", token_value.type);

		yaml_token_delete(&token_value);

		return cjfalse;
	}

	yaml_token_delete(&token_value);


	//
	// parse value scalar
	//

	yaml_token_t token_value_scalar;
	yaml_token_type_t token_value_scalar_type;

	if (!yaml_parser_scan(parser, &token_value_scalar)) {
		cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_FAIL, parser->problem);
		yaml_token_delete(&token_value_scalar);

		return cjfalse;
	}

	token_value_scalar_type = token_value_scalar.type;

	if (out_value_token)
		*out_value_token = token_value_scalar.type;

	if (out_val) {
		if (token_value_scalar_type == YAML_SCALAR_TOKEN) {
			cj_conv_u8c_to_mc(out_val, out_val_siz, token_value_scalar.data.scalar.value);
		}
	}

	{
		// cjstd_printf("[KEY][VAL] %s:%s\n", out_key, out_val);

		if (json_par) {
			cjson* child = cjNULL;

			switch (token_value_scalar_type) {
			case YAML_SCALAR_TOKEN:
			{
				cjson* o;

				if(_cjyaml_is_quoted_scalar(&token_value_scalar)) {
					o = cJSON_CreateString(out_val);
				} else {
					if (cj_is_number(out_val, cjtrue, cjfalse))
						o = cJSON_CreateNumber(atoi(out_val));
					else 
						o = cJSON_CreateString(out_val);
				}

				cJSON_AddItemToObject(json_par, out_key, o);
			}
			break;

			case YAML_FLOW_SEQUENCE_START_TOKEN:
				child = cJSON_AddArrayToObject(json_par, out_key);
				break;

			case CJYAML_TOKEN_BLOCK_SEQUENCE_START:
				child = cJSON_AddArrayToObject(json_par, out_key);
				break;

			default:
				child = cJSON_AddObjectToObject(json_par, out_key);
				break;
			}
			
			if (out_json_child)
				*out_json_child = child;
		}
	} 

	yaml_token_delete(&token_value_scalar);

	return cjtrue;
}


CJEXTERNC cjbool cjyaml_parse_token(cjyaml_parse_ctx* parse_ctx,
	cjyaml_token_info* out_token_info, cjmc* out_val, cjstr_siz out_val_siz,
	cjson* json_par, cjson** out_json_child) {

	if (!parse_ctx)
		return cjfalse;

	yaml_parser_t* parser = parse_ctx->parser;
	yaml_token_t token;

	if (!yaml_parser_scan(parser, &token)) {
		yaml_token_delete(&token);
		cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_FAIL, parser->problem);

		return cjfalse;
	}

	if (out_token_info) {
		cjmemzero(out_token_info, sizeof(cjyaml_token_info));
		out_token_info->token_type = token.type;
	}

	if (out_val) {
		if (token.type == YAML_SCALAR_TOKEN) {
			cj_conv_u8c_to_mc(out_val, out_val_siz, token.data.scalar.value);
			//cjstd_printf("[SINGLE] %s\n", out_val);

			if (out_token_info) {
				out_token_info->is_value = cjtrue;
				out_token_info->is_quoted_value = _cjyaml_is_quoted_scalar(&token);
			}

		} else {
			*out_val = cjNULL;
		}
	}

	{
		if (json_par) {

			cjson* child = cjNULL;

			if (token.type == YAML_SCALAR_TOKEN) {
				child = cJSON_AddObjectToObject(json_par, out_val);

			} else {
				child = cJSON_AddObjectToObject(json_par, out_val);
			}

			if (out_json_child)
				*out_json_child = child;
		}
	}

	yaml_token_delete(&token);

	return cjtrue;
}

CJEXTERNC static cjbool cjyaml_parse_worker_show_parse_tree_by_token(
	cjyaml_parse_ctx* parse_ctx, cjyaml_parse_evt evt, void* evt_prm) {

	if (evt != CJYAML_PARSE_EVT_ON_PARSE)
		return cjtrue;

	yaml_parser_t* parser
		= parse_ctx->parser;

	yaml_token_t token;
	yaml_token_type_t token_type;

	do {
		if (!yaml_parser_scan(parser, &token)) {
			yaml_token_delete(&token);

			cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_FAIL,
				parser->problem);

			return cjfalse;
		}

		token_type = token.type;

		print_token(&token);

		yaml_token_delete(&token);

	} while (token_type != YAML_STREAM_END_TOKEN);

	return cjtrue;
}

CJEXTERNC cjbool cjyaml_show_parsed_token(const cjmc* yaml_path, cjyaml_err* out_err) {

	cjyaml* yaml;
	cjyaml_err err = CJYAML_ERR_OK;

	if (!cjyaml_create(&yaml, yaml_path, cjtrue)) {
		if (out_err)
			*out_err = CJYAML_ERR_LOAD_YAML_FILE;

		return cjfalse;
	}

	cjyaml_parse(yaml, cjyaml_parse_worker_show_parse_tree_by_token, cjNULL, &err);

	if (out_err)
		*out_err = err;

	return err == CJYAML_ERR_OK ? cjtrue : cjfalse;
}
















//
//CJEXTERNC cjbool cjyaml_parse_mapping(cjyaml_parse_ctx* parse_ctx, 
//	cjmc* out, cjstr_siz out_siz, 
//	
//	) 	{
//
//	if (!parse_ctx)
//		return cjfalse;
//
//	cjyaml_err err = CJYAML_ERR_OK;
//
//	yaml_event_t evt;
//	yaml_event_type_t evt_type;
//
//	do {
//		if (!yaml_parser_parse((yaml_parser_t*)parse_ctx->parser, &evt)) {
//			yaml_event_delete(&evt);
//			err = CJYAML_ERR_PARSE_FAIL;
//
//			break;
//		}
//
//		evt_type = evt.type;
//
//		if (evt_type == YAML_DOCUMENT_START_EVENT) {
//
//			_cjyaml_parse_mapping_scalar(parse_ctx)
//
//		}
//		
//
//
//
//
//
//
//		if (evt.type == YAML_MAPPING_START_EVENT) {
//			yaml_event_delete(&evt);
//
//			yaml_event_t sub_evt;
//
//			if (!yaml_parser_parse((yaml_parser_t*)parse_ctx->parser, &sub_evt)) {
//				yaml_event_delete(&sub_evt);
//				err = CJYAML_ERR_PARSE_FAIL;
//
//				break;
//			}
//
//			if (sub_evt.type == YAML_SCALAR_EVENT) {
//
//				// get mapping scalar
//				cjmc tag[CJ_MAX_CJYAML_MAP_SCALAR_TAG] = { 0 };
//
//				cj_conv_u8c_to_mc(tag, CJ_MAX_CJYAML_MAP_SCALAR_TAG,
//					sub_evt.data.scalar.value);
//
//				if (!cjstrcmp(tag, map_scalar_tag)) {
//					yaml_event_delete(&sub_evt);
//
//					return cjtrue;
//
//				} else {
//					// different mapping scalar tag, consume next scalar(value)
//					err = cjyaml_parse_consume_scalar(parse_ctx, cjtrue, cjNULL, cjNULL, 0);
//
//					if (err == CJYAML_ERR_OK) {
//
//					}
//
//				}
//
//			} else {
//				// invalid parse state
//				err = CJYAML_ERR_PARSE_INVALID_STATE;
//			}
//
//			yaml_event_delete(&sub_evt);
//
//			if (err == CJYAML_ERR_OK) {
//				continue;
//
//			} else {
//				break;
//			}
//
//		} else {
//			// non 'YAML_MAPPING_START_EVENT'
//
//			if (evt.type == YAML_DOCUMENT_END_EVENT ||
//				evt.type == YAML_STREAM_END_EVENT) {
//
//				yaml_event_delete(&evt);
//				err = CJYAML_ERR_PARSE_REACH_END;
//
//				break;
//			}
//
//			yaml_event_delete(&evt);
//		}
//
//	} while (1);
//
//	if (err != CJYAML_ERR_OK) {
//		parse_ctx->last_err = err;
//
//		return cjfalse;
//	}
//
//	return cjtrue;
//}
//
//CJEXTERNC cjyaml_err cjyaml_parse_recur_consume_non_scalar(cjyaml_parse_ctx* parse_ctx,
//	cjui yaml_evt_type_start) {
//
//	if (yaml_evt_type_start == YAML_SCALAR_EVENT)
//		return CJYAML_ERR_PARSE_INVALID_PARAM;
//
//	cjyaml_err err = CJYAML_ERR_OK;
//	yaml_event_t evt;
//
//	do {
//
//		if (!yaml_parser_parse((yaml_parser_t*)parse_ctx->parser, &evt)) {
//			yaml_event_delete(&evt);
//
//			return CJYAML_ERR_PARSE_FAIL;
//		}
//
//		if( (yaml_evt_type_start == YAML_SEQUENCE_START_EVENT && evt.type == YAML_SEQUENCE_END_EVENT) ||
//			(yaml_evt_type_start == YAML_MAPPING_START_EVENT && evt.type == YAML_MAPPING_END_EVENT)) {
//			yaml_event_delete(&evt);
//
//			return CJYAML_ERR_OK;
//		}
//
//		if (evt.type == YAML_STREAM_END_EVENT || evt.type == YAML_DOCUMENT_END_EVENT) {
//			yaml_event_delete(&evt);
//			
//			return CJYAML_ERR_PARSE_REACH_END;
//		}
//
//		if (evt.type == YAML_SEQUENCE_START_EVENT || evt.type == YAML_MAPPING_START_EVENT) {
//			cjyaml_err consume_rst
//				= cjyaml_parse_recur_consume_non_scalar(parse_ctx, evt.type);
//
//			if (CJYAML_ERR_OK != consume_rst) {
//				yaml_event_delete(&evt);
//
//				return consume_rst;
//			}
//		}
//
//		// just consume other element
//		yaml_event_delete(&evt);
//
//	} while (1);
//
//	return CJYAML_ERR_OK;
//}
//
//CJEXTERNC cjyaml_err cjyaml_parse_consume_scalar(cjyaml_parse_ctx* parse_ctx,
//	cjbool consume_child, cjui* out_scalar_type, cjmc* out_value, cjstr_siz out_siz) {
//
//	if (!parse_ctx)
//		return CJYAML_ERR_INVALID_PARAM;
//
//	cjyaml_err err = CJYAML_ERR_OK;
//
//	yaml_event_t evt;
//	yaml_event_type_t evt_type;
//
//	if (!yaml_parser_parse((yaml_parser_t*)parse_ctx->parser, &evt)) {
//		yaml_event_delete(&evt);
//
//		return CJYAML_ERR_PARSE_FAIL;
//	}
//
//	evt_type = evt.type;
//
//	if (out_scalar_type)
//		*out_scalar_type = evt_type;
//
//	if (evt_type == YAML_SCALAR_EVENT) {
//
//		if (out_value) {
//			cj_conv_u8c_to_mc(out_value, out_siz, evt.data.scalar.value);
//		}
//
//		yaml_event_delete(&evt);
//
//		return CJYAML_ERR_OK;
//	} 
//
//	if (consume_child) {
//
//		yaml_event_t evt_consume;
//		yaml_event_type_t evt_consume_type;
//
//		do {
//			if (!yaml_parser_parse((yaml_parser_t*)parse_ctx->parser, &evt_consume)) {
//				yaml_event_delete(&evt_consume);
//				err = CJYAML_ERR_PARSE_FAIL;
//
//				break;
//			}
//
//			evt_consume_type = evt_consume.type;
//			yaml_event_delete(&evt_consume);
//
//			if (evt_consume_type == YAML_SEQUENCE_START_EVENT ||
//				evt_consume_type == YAML_MAPPING_START_EVENT) {
//
//				cjyaml_err consume_rst
//					= cjyaml_parse_recur_consume_non_scalar(parse_ctx, evt_consume_type);
//
//				if (consume_rst != cjrstok) {
//					err = consume_rst;
//					break;
//				}
//
//			} else {
//				// not YAML_SEQUENCE_START_EVENT or YAML_MAPPING_START_EVENT
//				err = CJYAML_ERR_PARSE_INVALID_STATE;
//				break;
//			}
//
//		} while (1);
//
//	}
//
//	yaml_event_delete(&evt);
//
//	return CJYAML_ERR_OK;
//}


//if (rst != cjrstok) {
//	//cjstrcpy_s(parse_ctx->last_parse_err, CJ_MAX_ERR_MSG, parser->problem);
//}

//
//CJEXTERNC cjbool cjyaml_show_parse_tree(cjyaml* yaml) {
//
//	if (!yaml || !evt_hdr)
//		return cjfalse;
//
//	if (yaml->err != CJYAML_ERR_OK)
//		return cjfalse;
//
//	if (!yaml->yaml_src || !yaml->yaml_src_siz) {
//		yaml->err = CJYAML_ERR_LOAD_YAML_FILE;
//		return cjfalse;
//	}
//
//	yaml_parser_t parser;
//	yaml_parser_initialize(&parser);
//
//	// yaml_src does not end with null terminator
//	yaml_parser_set_input_string(&parser, yaml->yaml_src, (size_t)yaml->yaml_src_siz);
//
//	yaml_event_t event;
//	yaml_event_type_t event_type;
//
//	if (cjrstok != evt_hdr(yaml, CJYAML_PARSE_EVT_BEGIN, cjNULL, user_prm))
//		goto abort_parse_exit;
//
//	do {
//		if (!yaml_parser_parse(&parser, &event)) {
//			cjstrcpy_s(yaml->last_parse_err, CJYAML_PARSE_ERR_MAX_STR, parser.problem);
//
//			evt_hdr(yaml, CJYAML_PARSE_EVT_ERR_PARSE_FAILED, (void*)parser.problem, user_prm);
//
//			goto err_parse_exit;
//		}
//
//		// print_event(&event);
//		if (cjrstok != evt_hdr(yaml, CJYAML_PARSE_EVT_ON_PARSE, &event, user_prm))
//			goto abort_parse_exit;
//				
//		event_type = event.type;
//		yaml_event_delete(&event);
//
//	} while (event_type != YAML_STREAM_END_EVENT);
//
//	evt_hdr(yaml, CJYAML_PARSE_EVT_END, 0, user_prm);
//	yaml_parser_delete(&parser);
//
//	return cjtrue;
//
//
//abort_parse_exit:
//
//err_parse_exit:	
//	evt_hdr(yaml, CJYAML_PARSE_EVT_END, -1, user_prm);
//
//	cjstrcpy_s(yaml->last_parse_err, CJYAML_PARSE_ERR_MAX_STR, parser.problem);
//	yaml_parser_delete(&parser);
//
//	return cjfalse;
//}



//
//CJEXTERNC cjbool cjyaml_parse_find_block_scalar(cjyaml_parse_ctx* parse_ctx,
//	cjyaml_token_type dst_block_token) {
//
//	if (!parse_ctx)
//		return cjfalse;
//
//	yaml_token_t token;
//	yaml_token_type_t token_type;
//	yaml_parser_t* parser = parse_ctx->parser;
//
//	do {
//
//		if (!yaml_parser_scan(parser, &token)) {
//			yaml_token_delete(&token);
//
//			cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_FAIL,
//				"failed to parse");
//
//			return cjfalse;
//		}
//
//		token_type = token.type;
//		yaml_token_delete(&token);
//
//		if (dst_block_token == YAML_STREAM_END_TOKEN) {
//			cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_REACH_END,
//				cjNULL);
//
//			return cjfalse;
//		}
//
//		if (dst_block_token == YAML_BLOCK_SEQUENCE_START_TOKEN &&
//			token_type == YAML_BLOCK_END_TOKEN) {
//
//			cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_INVALID_STATE,
//				"invalid to parse state");
//
//			return cjfalse;
//		}
//
//		if (dst_block_token == YAML_BLOCK_MAPPING_START_TOKEN &&
//			token_type == YAML_BLOCK_END_TOKEN) {
//
//			cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_INVALID_STATE,
//				"invalid to parse state");
//
//			return cjfalse;
//		}
//
//		if (dst_block_token == token_type) {
//			return cjtrue;
//		}
//
//	} while (1);
//
//	return cjtrue;
//}
