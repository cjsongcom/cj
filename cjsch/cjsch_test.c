
#define CJ_TEST_CJSCH

#if defined(CJ_TEST) && defined(CJ_TEST_CJSCH)

#include <cj/cj.h>
#include <cj/cjsch/cjsch.h>

#include <cj/cjsch/cjsch_loader.h>

#include <cj/cjsch/cjsch_trigger.h>
#include <cj/cjsch/cjsch_trigger_inst.h>
#include <cj/cjsch/cjsch_action.h>
#include <cj/cjsch/cjsch_action_inst.h>
#include <cj/cjsch/cjsch_factory.h>
#include <cj/cjsch/cjsch_manager.h>

#include <cj/cjsch/cjsch_parse_json.h>


static const cjmc* _tlp1 = "[cjsch_test]";


typedef struct cjsch_test_ctx_t {

    cjmc                    sch_yaml_path[CJ_MAX_PATH];

    cjsch_factory*          sch_factory;
    cjsch_factory_prm       sch_factory_prm;

    cjsch_manager*          sch_man;
    cjsch_manager_prm       sch_man_prm;

} cjsch_test_ctx;


CJEXTERNC cjbool cjsch_man_update(cjsch_manager* man, cjtime_ms curtime,
    cjtime_ms* out_elapsed) {
    
    cjsch_trigger_inst* trig = man->trigger_insts;

    while (trig) {
        trig = trig->_list_next_;
    }

    return cjtrue;
}


CJEXTERNC cjrst cjtest_cjsch_manager(cjsch_test_ctx* ctx) {

    const cjmc* _tlp2 = "cjtest_cjsch_manager";

    ctx->sch_man_prm.sch_factory = ctx->sch_factory;

    if (!cjsch_man_create(&ctx->sch_man, &ctx->sch_man_prm)) {
        return -2100;
    }

    if (!cjsch_man_create_trigger(ctx->sch_man, "trigger00", cjNULL)) {

        cjstd_printf("[%s][%s] failed to load \n", _tlp1, _tlp2);

        return -2120;
    }

    return cjrstok;
}

CJEXTERNC cjrst cjtest_cjsch_factory(cjsch_test_ctx* ctx) {

	cjson* sch_json = cjNULL;
    ctx->sch_factory_prm.hash_bucket_siz = 24;


    if (!cjsch_factory_create(&ctx->sch_factory, &ctx->sch_factory_prm))
        return -1001;


	if (!cjson_load_from_yaml(&sch_json, ctx->sch_yaml_path))
		return -1111;

    cjsch_parse_json_sch_ctx parse_json_ctx = { 0 };

	cjsch_parse_err parse_err = CJSCH_PARSE_ERR_OK;
	const cjmc* parse_err_msg = cjNULL;

	if (!cjsch_init_parse_json_sch_ctx(&parse_json_ctx)) {
        return -1120;
	}

	if (!cjsch_parse_json_sch(&parse_json_ctx, sch_json, &parse_err, &parse_err_msg)) {
		cjsch_finalize_parse_json_sch_ctx(&parse_json_ctx);

        return -1121;
	}

	cjson_destroy(sch_json);


    //
    // register trigger, action
    //

    for (cjary_idx i = 0; i < cjary_siz(&parse_json_ctx.triggers); i++) {

		cjsch_trigger* trig 
			= cjary_get_ptr(&parse_json_ctx.triggers, i);

		cjstd_printf("[INFO][cjsch][cjsch_factory_register_trigger] register trigger=%s\n", trig->name);
		
		if (!cjsch_factory_register_trigger(ctx->sch_factory, trig, trig->name)) {
			cjstd_printf("[ERR][cjsch][cjsch_factory_register_trigger] failed to register trigger=%s\n", trig->name);
            return -1201;
		}
	}

	for (cjary_idx i = 0; i < cjary_siz(&parse_json_ctx.actions); i++) {

		cjsch_action* action 
			= cjary_get_ptr(&parse_json_ctx.actions, i);

		cjstd_printf("[INFO][cjsch][cjsch_factory_register_action] register action=%s\n", action->name);

		if (!cjsch_factory_register_action(ctx->sch_factory, action, action->name)) {
			cjstd_printf("[ERR][cjsch][cjsch_factory_register_action] failed to register action=%s\n", action->name);
			return -1202;
		}
	}
    
    return cjrstok;
}
















// file_path="cjsch_test.yaml"
CJEXTERNC cjrst cjtest_cjsch_main(void* file_path) {

    if (!file_path)
        return cjrstfail;

    cjsch_test_ctx  test_ctx = { 0 };

    // "cjsch_test.yaml"
    cjstrcpy_s(test_ctx.sch_yaml_path, CJA_SIZ(test_ctx.sch_yaml_path), file_path);

    if (cjrstfail == cjtest_cjsch_factory(&test_ctx)) {
        cjstd_printf("[ERR][cjsch][cjtest_cjsch_factory] failed");

        return cjrstfail;
    }

    if (cjrstfail == cjtest_cjsch_manager(&test_ctx)) {
        cjstd_printf("[ERR][cjsch][cjtest_cjsch_manager] failed");

        return cjrstfail;
    }


done_exit:
    cjsch_man_destroy(test_ctx.sch_man);
	cjsch_factory_destroy(test_ctx.sch_factory);


	return 0;
}

#endif



//
//CJEXTERNC cjrst cjtest_cjsch_test_loader(const cjmc* file_path) {
//    cjsch_loader* loader = cjNULL;
//
//    //cjmc yaml_path[CJFILE_MAX_PATH] = { 0 };
//    //cjfile_resolve_path(yaml_path, CJFILE_MAX_PATH, "cjsch_test.yaml", CJFILE_PATH_HINT_BASE);
//
//    if (!cjsch_loader_create(&loader, file_path)) {
//        cjstd_printf("%s failed to create cjsch, file=%s", _tlp, file_path);
//        return cjrstfail;
//    }
//
//    if (!cjsch_loader_load(loader)) {
//        cjstd_printf("%s failed to load cjsch, file=%s", _tlp, file_path);
//        return cjrstfail;
//    }
//
//    cjsch_loader_destroy(loader);
//
//    return cjrstok;
//}
