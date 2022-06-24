
#ifndef _CJYAML_JSON_H_
#define _CJYAML_JSON_H_

#include <cj/cjyaml.h>


CJEXTERNC cjbool cjyaml_conv_json(cjson** out_json, cjyaml* yaml);

CJEXTERNC cjbool cjyaml_load_as_json(cjson** out_json, const cjmc* yaml_path, 
	cjyaml_err* err);


#endif
