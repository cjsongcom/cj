
#include <cj/cj.h>
#include <cj/cjsch/cjsch.h>

#include <cj/cjyaml.h>

#include <cj/cjsch/cjsch.h>
#include <cj/cjsch/cjsch_trigger.h>
#include <cj/cjsch/cjsch_action.h>
#include <cj/cjsch/cjsch_parse_json.h>

#include <cj/cjsch/cjsch_loader.h>



static cjbool cjsch_yaml_parser(struct cjyaml* yaml,
	cjyaml_parse_evt evt, void* evt_prm, void* user_prm) {

	switch (evt) {
	case CJYAML_PARSE_EVT_BEGIN: break;
	case CJYAML_PARSE_EVT_END: break;
    case CJYAML_PARSE_EVT_ON_PARSE:
		//print_event(evt_prm);
	    break;

	case CJYAML_PARSE_EVT_ERR_PARSE_FAILED: break;
    default: break;
	}

	return cjtrue;
}


//
// cjsch create
//

CJEXTERNC cjbool cjsch_loader_create(cjsch_loader** out_loader, const cjmc* yaml_path) {
		
	cjsch_loader* loader = cjNULL;
			
	loader = cjmzalloc(sizeof(cjsch_loader));

	cjstrcpy_s(loader->yaml_path, sizeof(loader->yaml_path) 
		/ sizeof(loader->yaml_path[0]), yaml_path);
	
	if (out_loader)
		*out_loader = loader;
	
	return cjtrue;
}

CJEXTERNC void cjsch_loader_destroy(cjsch_loader* loader) {

	if (!loader)
		return; 

	if(loader->sch_json)
		cjson_destroy(loader->sch_json);

	cjmfree(loader);
}

CJEXTERNC cjbool cjsch_loader_load(cjsch_loader* loader) {
	
	if (!loader)
		return cjfalse;

	if (!cjson_load_from_yaml(&loader->sch_json, loader->yaml_path))
		return cjfalse;

	cjsch_parse_json_sch_ctx ctx = { 0 };

	cjsch_parse_err parse_err = CJSCH_PARSE_ERR_OK;
	const cjmc* parse_err_msg = cjNULL;

	if (!cjsch_init_parse_json_sch_ctx(&ctx)) {		
		return cjfalse;
	}

	if (!cjsch_parse_json_sch(&ctx, loader->sch_json, &parse_err, &parse_err_msg)) {
		cjsch_finalize_parse_json_sch_ctx(&ctx);
		return cjfalse;
	}

	return cjtrue;
}


