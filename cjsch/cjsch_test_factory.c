
#include <cj/cj.h>

#define CJ_TEST_CJSCH_FACTORY
#if defined(CJ_TEST) && defined(CJ_TEST_CJSCH_FACTORY)

#include <cj/cjsch/cjsch.h>
#include <cj/cjsch/cjsch_trigger.h>
#include <cj/cjsch/cjsch_action.h>
#include <cj/cjsch/cjsch_factory.h>

#include <cj/cjsch/cjsch_parse_json.h>


CJEXTERNC static cjrst cjtest_cjsch_factory_load_trigger_action() {

	cjsch_factory* f = cjNULL;
	cjson* sch_json = cjNULL;

	cjsch_factory_prm prm = {
		.hash_bucket_siz = 24
	};

	const cjmc* json_file_path = "cjsch_test.yaml";

	cjstd_printf("\n");
	cjstd_printf("===========================================\n");
	cjstd_printf("[INFO][cjsch][begin] cjsch_factory_create \n");
	cjstd_printf("===========================================\n");
	cjstd_printf("\n");

	if (!cjsch_factory_create(&f, &prm)) {
		goto err_exit;
	}

	if (!cjson_load_from_yaml(&sch_json, json_file_path))
		return cjrstfail;

	cjsch_parse_json_sch_ctx ctx = { 0 };

	cjsch_parse_err parse_err = CJSCH_PARSE_ERR_OK;
	const cjmc* parse_err_msg = cjNULL;

	if (!cjsch_init_parse_json_sch_ctx(&ctx)) {		
		return cjfalse;
	}

	if (!cjsch_parse_json_sch(&ctx, sch_json, &parse_err, &parse_err_msg)) {
		cjstd_printf("[ERR][cjsch][cjsch_parse_json_sch] failed to load json=%s , err=%d, err_msg=%s \n",
			json_file_path, parse_err, parse_err_msg);
		cjsch_finalize_parse_json_sch_ctx(&ctx);

		return cjrstfail;
	}

	//
	// register
	//

	cjstd_printf("\n");
	cjstd_printf("===========================================\n");
	cjstd_printf("[INFO][cjsch] register trigger/action  \n");
	cjstd_printf("===========================================\n");
	cjstd_printf("\n");

	for (cjary_idx i = 0; i < cjary_siz(&ctx.triggers); i++) {

		cjsch_trigger* trig 
			= cjary_get_ptr(&ctx.triggers, i);

		cjstd_printf("[INFO][cjsch][cjsch_factory_register_trigger] register trigger=%s\n", trig->name);
		
		if (!cjsch_factory_register_trigger(f, trig, trig->name)) {
			cjstd_printf("[ERR][cjsch][cjsch_factory_register_trigger] failed to register trigger=%s\n", trig->name);
			goto err_exit;
		}
	}

	for (cjary_idx i = 0; i < cjary_siz(&ctx.actions); i++) {

		cjsch_action* action 
			= cjary_get_ptr(&ctx.actions, i);

		cjstd_printf("[INFO][cjsch][cjsch_factory_register_action] register action=%s\n", action->name);

		if (!cjsch_factory_register_action(f, action, action->name)) {
			cjstd_printf("[ERR][cjsch][cjsch_factory_register_action] failed to register action=%s\n", action->name);
			goto err_exit;
		}
	}

	//
	// find
	//

	cjstd_printf("\n");
	cjstd_printf("===========================================\n");
	cjstd_printf("[INFO][cjsch] find trigger/action  \n");
	cjstd_printf("===========================================\n");
	cjstd_printf("\n");

	for (cjary_idx i = 0; i < cjary_siz(&ctx.triggers); i++) {

		cjsch_trigger* found_trigger = cjNULL;
		cjsch_trigger* trig 
			= cjary_get_ptr(&ctx.triggers, i);
		
		if (!cjsch_factory_find_trigger(&found_trigger, f, trig->name)) {
			cjstd_printf("[ERR][cjsch][cjsch_factory_find_trigger] failed to found trigger=%s\n", trig->name);
			goto err_exit;
		}

		cjstd_printf("[INFO][cjsch][cjsch_factory_find_trigger] found trigger=%s\n", found_trigger->name);
	}

	for (cjary_idx i = 0; i < cjary_siz(&ctx.actions); i++) {

		cjsch_action* found_action = cjNULL;
		cjsch_action* action 
			= cjary_get_ptr(&ctx.actions, i);
		
		if (!cjsch_factory_find_action(&found_action, f, action->name)) {
			cjstd_printf("[ERR][cjsch][cjsch_factory_find_action] failed to found action=%s\n", action->name);
			goto err_exit;
		}

		cjstd_printf("[INFO][cjsch][cjsch_factory_find_action] found action=%s\n", found_action->name);
	}

	
	//
	// destroy
	//

	cjson_destroy(sch_json);
	cjsch_factory_destroy(f);

	cjstd_printf("\n");
	cjstd_printf("===========================================\n");
	cjstd_printf("[INFO][cjsch][end] cjsch_factory_destroy \n");
	cjstd_printf("===========================================\n");
	cjstd_printf("\n");

	return cjrstok;


err_exit:	
	cjsch_factory_destroy(f);
	cjson_destroy(sch_json);

	return cjrstfail;
}


CJEXTERNC cjrst cjtest_cjsch_factory_main(void* prm) {

	cjtest_cjsch_factory_load_trigger_action();

	return cjrstok;
}


#endif


