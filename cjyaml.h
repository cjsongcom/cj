
#ifndef _CJYAML_H_
#define _CJYAML_H_


// https://llvm.org/docs/YamlIO.html

//[yaml form]
// > "scalars", "mappings", "sequences"

//[scalar]
// > any number, string
// > not a sequence or mapping

//[block scalar]
// > block scalars are string literals that are represented 
//   in YAML using the literal block notation
//  text: |
//    First line
//   Second line
//

//[mapping]
// > a set of key-value pairs where the key ends with a colon
//   name: Tom
//   hat_size : 7

//[sequence]
// > list of items where each item starts with a leading dash (бо-бо).
// > - x86
//   - x86_64
//   - PowerPC


typedef enum cjyaml_err_t {

	CJYAML_ERR_OK = 0,

	CJYAML_ERR_INVALID_PATH						= -10,
	CJYAML_ERR_INVALID_PARAM					= -20,
	CJYAML_ERR_OPEN_YAML_FILE					= -30,
	CJYAML_ERR_INVALID_YAML_FILE_PATH			= -40,
	CJYAML_ERR_LOAD_YAML_FILE					= -50,
	CJYAML_ERR_EMPTY_YAML						= -51,


	CJYAML_ERR_PARSE_INVALID_PARAM				= -110,
	CJYAML_ERR_PARSE_INVALID_STATE				= -111,
	CJYAML_ERR_PARSE_INVALID_TOKEN				= -112,
	CJYAML_ERR_PARSE_IMPL_NOT_YET				= -113,
	CJYAML_ERR_PARSE_CREATE_CTX					= -120,
	CJYAML_ERR_PARSE_ABORTED					= -130,

	CJYAML_ERR_PARSE_FAIL						= -140,
	CJYAML_ERR_PARSE_FAIL_FIND_TOKEN			= -141,
	CJYAML_ERR_PARSE_FAIL_KEY_VAL				= -142,

	CJYAML_ERR_PARSE_NOT_KEY_TOKEN				= -200,
	CJYAML_ERR_PARSE_NOT_VALUE_SCALAR_TOKEN		= -210,

	CJYAML_ERR_PARSE_DIFF_MAP_SCALAR_TAG		= -300,
	CJYAML_ERR_PARSE_REACH_END					= -400,

	CJYAML_ERR_CONV_FAIL_TO_JSON				= -500


} cjyaml_err;

typedef enum cjyaml_max_t {

	CJ_MAX_CJYAML_KEY_SIZ = 256,
	CJ_MAX_CJYAML_VAL_SIZ = 256,

	CJ_MAX_CJYAML_MAP_SCALAR_TAG = 32,

} cjyaml_max;


typedef enum cjyaml_token_type_t {
	CJYAML_TOKEN_NO,
	CJYAML_TOKEN_STREAM_START,
	CJYAML_TOKEN_STREAM_END,
	CJYAML_TOKEN_VERSION_DIRECTIVE,
	CJYAML_TOKEN_TAG_DIRECTIVE,
	CJYAML_TOKEN_DOCUMENT_START,
	CJYAML_TOKEN_DOCUMENT_END,
	CJYAML_TOKEN_BLOCK_SEQUENCE_START,
	CJYAML_TOKEN_BLOCK_MAPPING_START,
	CJYAML_TOKEN_BLOCK_END,
	CJYAML_TOKEN_FLOW_SEQUENCE_START,
	CJYAML_TOKEN_FLOW_SEQUENCE_END,
	CJYAML_TOKEN_FLOW_MAPPING_START,
	CJYAML_TOKEN_FLOW_MAPPING_END,
	CJYAML_TOKEN_BLOCK_ENTRY,
	CJYAML_TOKEN_FLOW_ENTRY,
	CJYAML_TOKEN_KEY,
	CJYAML_TOKEN_VALUE,
	CJYAML_TOKEN_ALIAS,
	CJYAML_TOKEN_ANCHOR,
	CJYAML_TOKEN_TAG,
	CJYAML_TOKEN_SCALAR_TOKEN
} cjyaml_token_type;


//
// cjyaml
//

typedef struct cjyaml_t {
	cjyaml_err					err;

	cjfile_path_hint			hint;
	cjmc						path[CJFILE_MAX_PATH];

	const cjmc*					name; // filename

	cjmalloc_pfn				alloc_m;
	cjmfree_pfn					free_m;

	cjmc*						yaml_src;
	cjfile_off					yaml_src_siz;

	void*						prm;

	cjbool						is_opened;

} cjyaml;


CJEXTERNC cjbool cjyaml_create(cjyaml** out_yaml, const cjmc* path, cjbool load_file);

CJEXTERNC cjbool _cjyaml_create(cjyaml** out_yaml, const cjmc* path, 
	cjyaml_err* out_err, cjmalloc_pfn alloc_m, cjmfree_pfn free_m);

CJEXTERNC void cjyaml_destroy(cjyaml* yaml);

CJEXTERNC cjbool cjyaml_open(cjyaml* yaml);



//
// cjyaml parse
//

typedef enum cjyaml_parse_evt_t {

	CJYAML_PARSE_EVT_BEGIN = 100,
	CJYAML_PARSE_EVT_END = 200,

	CJYAML_PARSE_EVT_ON_PARSE = 300,

	CJYAML_PARSE_EVT_ERR_PARSE_FAILED = -1,

} cjyaml_parse_evt;


typedef struct cjyaml_parse_ctx_t {
	void* parser;
	void* prm;

	cjyaml_err last_err;
	cjmc last_err_msg[CJ_MAX_ERR_MSG];

	cjyaml_token_type last_err_parsed_token;

} cjyaml_parse_ctx;


typedef struct cjyaml_token_info_t {
	cjyaml_token_type token_type;
	cjbool is_value;
	cjbool is_quoted_value;
} cjyaml_token_info;


typedef cjbool(*cjyaml_parse_worker_pfn)(cjyaml_parse_ctx* parse_ctx, 
	cjyaml_parse_evt evt, void* evt_prm);

CJEXTERNC cjbool cjyaml_parse(cjyaml* yaml, cjyaml_parse_worker_pfn worker, void* prm,
	cjyaml_err* out_parse_err);

CJEXTERNC cjbool _cjyaml_create_parse_ctx(cjyaml_parse_ctx** out_parse_ctx, 
	cjyaml* yaml, void* prm);

CJEXTERNC void _cjyaml_destroy_parse_ctx(cjyaml_parse_ctx* parse_ctx);

inline cjbool cjyaml_is_scalar_token(cjyaml_token_type token_type) {
	return (token_type == CJYAML_TOKEN_SCALAR_TOKEN) ?
		cjtrue : cjfalse;
}

CJEXTERNC void cjyaml_parse_set_err(cjyaml_parse_ctx* parse_ctx,
	cjyaml_err err, const cjmc* err_msg_fmt, ...);

CJEXTERNC cjbool cjyaml_parse_token(cjyaml_parse_ctx* parse_ctx,
	cjyaml_token_info* out_token_info, cjmc* out_val, cjstr_siz out_val_siz,
	cjson* json_par, cjson** out_json_child);

CJEXTERNC cjbool cjyaml_parse_key_val_mapping(cjyaml_parse_ctx* parse_ctx,
	cjbool skip_key_token_parse, cjmc* out_key, cjstr_siz out_key_siz,
	cjmc* out_val, cjstr_siz out_val_siz, cjyaml_token_type* out_value_token,
	cjson* json_par, cjson** out_json_child);

CJEXTERNC cjbool cjyaml_show_parsed_token(const cjmc* yaml_path, cjyaml_err* out_err);

CJEXTERNC cjbool _cjyaml_is_quoted_scalar(void* _token);


#endif
