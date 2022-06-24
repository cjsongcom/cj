
#include <cj/cj.h>
#include <cj/cjyaml.h>

#include <cj/cjsch/cjsch.h>

#include <cj/cjsch/cjsch_trigger.h>
#include <cj/cjsch/cjsch_action.h>

#include <cj/cjsch/cjsch_parse_json.h>


CJEXTERNC static cjbool _cjsch_parse_json_sch_triggers(cjsch_parse_json_sch_ctx* ctx,
	cjson* json, cjsch_parse_err* out_err, const cjmc** out_err_msg);

CJEXTERNC static cjbool _cjsch_parse_json_sch_trigger_dict_period(
	cjsch_trigger* out_trig, cjson* json);

CJEXTERNC static cjbool _cjsch_parse_json_sch_trigger_dict_start(
	cjsch_trigger* out_trig, cjson* json);

CJEXTERNC static cjbool _cjsch_parse_json_sch_trigger_dict_advance(
	cjsch_trigger* out_trig, cjson* json);

CJEXTERNC static cjbool _cjsch_parse_json_sch_actions(cjsch_parse_json_sch_ctx* ctx,
	cjson* json_action, cjsch_parse_err* out_err, const cjmc** out_err_msg);


//
// entry
//


CJEXTERNC cjbool cjsch_parse_json_sch(cjsch_parse_json_sch_ctx* ctx,
	cjson* json, cjsch_parse_err* out_err, const cjmc** out_err_msg) {

	cjsch_parse_err err = CJSCH_PARSE_ERR_OK;
	const cjmc* err_msg = "";

	if (!ctx || !json)
		return cjfalse;

	cjson* cjsch = cjson_get_key(json, "cjsch");

	if (cjsch) {
		cjson* triggers = cjson_get_key(cjsch, "triggers");

		if (!triggers || triggers->type != cJSON_Array) {
			err = CJSCH_PARSE_ERR_INVALID_TOKEN;

			goto done_exit;
		}

		cjson* actions = cjson_get_key(cjsch, "actions");

		if (!actions || actions->type != cJSON_Array) {
			err = CJSCH_PARSE_ERR_INVALID_TOKEN;

			goto done_exit;
		}

		if (!_cjsch_parse_json_sch_triggers(ctx, triggers, &err, &err_msg))
			goto done_exit;

		if (!_cjsch_parse_json_sch_actions(ctx, actions, &err, &err_msg))
			goto done_exit;
	}


done_exit:

	if (out_err)
		*out_err = err;

	if (out_err_msg)
		*out_err_msg = err_msg;

	return (err == CJSCH_PARSE_ERR_OK) ? cjtrue : cjfalse;
}


//
// parse triggers
//

CJEXTERNC static cjbool _cjsch_parse_json_sch_triggers(cjsch_parse_json_sch_ctx* ctx,
	cjson* json_trig, cjsch_parse_err* out_err, const cjmc** out_err_msg) {

	cji cnt = cjson_ary_siz(json_trig);

	for (cji i = 0; i < cnt; i++) {
		cjson* cur = cjson_ary_at(json_trig, i);

		cjsch_trigger* trig = cjmzalloc(sizeof(cjsch_trigger));
		cjbool parsed = cjfalse;

		do {

			if (!cjson_get_kv_str(trig->name, CJA_SIZ(trig->name), cur, "name"))
				break;

			if (!cjson_get_kv_str(trig->class, CJA_SIZ(trig->class), cur, "class"))
				break;

			if (!cjson_get_kv_bool(&trig->enable, cur, "enable"))
				break;

			if (!_cjsch_parse_json_sch_trigger_dict_start(trig, cur))
				break;

			if (!_cjsch_parse_json_sch_trigger_dict_period(trig, cur))
				break;

			if (!_cjsch_parse_json_sch_trigger_dict_advance(trig, cur))
				break;

			parsed = cjtrue;
			break;

		} while (1);

		if (parsed) {
			if (!cjary_push_ptr(&ctx->triggers, trig)) {
				cjmfree(trig);

				return cjfalse;
			}

		} else {
			cjmfree(trig);

			return cjfalse;
		}

	}

	return cjtrue;
}

CJEXTERNC static cjbool _cjsch_parse_json_sch_trigger_dict_period(
	cjsch_trigger* out_trig, cjson* json) {

	if (!json || !out_trig)
		return cjfalse;

	cjson* dict = cjNULL;

	if (!cjson_get_dict(&dict, json, "period"))
		return cjfalse;

	cjmc buf[CJ_MAX_CJSON_VAL_LONG] = { 0 };
	cjmc buf_opt[CJ_MAX_CJSON_VAL_LONG] = { 0 };

	//
	// period & opt
	//

	if (!cjson_get_kv_str(buf, CJA_SIZ(buf), dict, "type"))
		return cjfalse;

	cjson* period_opt_dict;

	if (!cjson_get_dict(&period_opt_dict, dict, "opt"))
		return cjfalse;

	const cjmc* period_type = buf;

	out_trig->period.type = CJ_PERIOD_NONE;

	if (!cjstrcmp(period_type, "once")) {
		out_trig->period.type = CJ_PERIOD_ONCE;

	} else if (!cjstrcmp(period_type, "every_day")) {
		out_trig->period.type = CJ_PERIOD_EVERY_DAY;

		cjson* every_day_dict;

		if (!cjson_get_dict(&every_day_dict, period_opt_dict, "every_day"))
			return cjfalse;

		if (!cjson_get_kv_int(&out_trig->period.dat.every_day.per_day, 
			every_day_dict, "per_day"))
			return cjfalse;

	} else if (!cjstrcmp(period_type, "every_week")) {
		out_trig->period.type = CJ_PERIOD_EVERY_WEEK;

		cjson* every_week_dict;

		if (!cjson_get_dict(&every_week_dict, period_opt_dict, "every_week"))
			return cjfalse;

		if (!cjson_get_kv_int(&out_trig->period.dat.every_week.per_week,
			every_week_dict, "per_week"))
			return cjfalse;

		if (!cjson_get_kv_wdays(&out_trig->period.dat.every_week.wdays,
			every_week_dict, "wdays"))
			return cjfalse;

	} else if (!cjstrcmp(period_type, "every_month")) {
		out_trig->period.type = CJ_PERIOD_EVERY_MONTH;

		cjson* every_month_dict;

		if (!cjson_get_dict(&every_month_dict, period_opt_dict, "every_month"))
			return cjfalse;

		if (!cjson_get_kv_month(&out_trig->period.dat.every_month.month,
			every_month_dict, "months"))
			return cjfalse;
					
		// day, week
		if (!cjson_get_kv_str(buf_opt, CJA_SIZ(buf_opt), every_month_dict, "per_type"))
			return cjfalse;

		if (!cjstrcmp(buf_opt, "day")) {
			out_trig->period.dat.every_month.per_type = CJ_PERIOD_EVERY_DAY;

		} else if (!cjstrcmp(buf_opt, "week")) {
			out_trig->period.dat.every_month.per_type = CJ_PERIOD_EVERY_WEEK;
		}

		// parse per opt
		{
			cjson* every_month_per_opt_dict;

			if (!cjson_get_dict(&every_month_per_opt_dict, every_month_dict, "per_opt"))
				return cjfalse;

			if (out_trig->period.dat.every_month.per_type == CJ_PERIOD_EVERY_DAY) {

				cjson* em_popt_day_dict;

				if (!cjson_get_dict(&em_popt_day_dict, every_month_per_opt_dict, "day"))
					return cjfalse;
																				
				if (!cjson_get_kv_days(&out_trig->period.dat.every_month.days,
					em_popt_day_dict, "days"))
					return cjfalse;

			} else if (out_trig->period.dat.every_month.per_type == CJ_PERIOD_EVERY_WEEK) {

				cjson* em_popt_week_dict;

				if (!cjson_get_dict(&em_popt_week_dict, every_month_per_opt_dict, "week"))
					return cjfalse;

				if (!cjson_get_kv_weeks(&out_trig->period.dat.every_month.per_week.weeks,
					em_popt_week_dict, "weeks"))
					return cjfalse;

				if (!cjson_get_kv_wdays(&out_trig->period.dat.every_month.per_week.wdays,
					em_popt_week_dict, "wdays"))
					return cjfalse;

			} else {
				return cjfalse;
			}
		}

	} // end of every_month

	return cjtrue;
}

CJEXTERNC static cjbool _cjsch_parse_json_sch_trigger_dict_start(
	cjsch_trigger* out_trig, cjson* json) {

	if (!json || !out_trig)
		return cjfalse;

	cjson* dict = cjNULL;

	if (!cjson_get_dict(&dict, json, "start"))
		return cjfalse;

	cjmc buf[CJ_MAX_CJSON_VAL_LONG] = { 0 };

	//
	// parse start-type
	//

	if (!cjson_get_kv_str(buf, CJA_SIZ(buf), dict, "type"))
		return cjfalse;

	const cjmc* start_type = buf;

	out_trig->st_type = CJSCH_TRIGGER_START_TYPE_NONE;

	if (!cjstrcmp(start_type, "schedule")) {
		out_trig->st_type = CJSCH_TRIGGER_START_TYPE_SCHEDULE;
	} else if (!cjstrcmp(start_type, "on_start")) {
		out_trig->st_type = CJSCH_TRIGGER_START_TYPE_PROC_START;
	} else if (!cjstrcmp(start_type, "tick")) {
		out_trig->st_type = CJSCH_TRIGGER_START_TYPE_PROC_TICK;
	} else if (!cjstrcmp(start_type, "event")) {
		out_trig->st_type = CJSCH_TRIGGER_START_TYPE_PROC_EVENT;
	}

	//
	// parse start date_time
	//

	if (!cjson_get_kv_str(buf, CJA_SIZ(buf), dict, "datetime"))
		return cjfalse;

	const cjmc* datetime = buf;

	if (!cj_parse_datetime_str(&out_trig->st_datetime, datetime))
		return cjfalse;

	//
	// expire
	//

	cjson* expire_dict = cjNULL;

	if (!cjson_get_dict(&expire_dict, dict, "expire"))
		return cjfalse;

	{
		if (!cjson_get_kv_bool(&out_trig->expire_enable, expire_dict, "enable"))
			return cjfalse;

		if (!cjson_get_kv_str(buf, CJA_SIZ(buf), dict, "datetime"))
			return cjfalse;

		const cjmc* expire_datetime = buf;

		if (!cj_parse_datetime_str(&out_trig->st_datetime, expire_datetime))
			return cjfalse;
	}

	return cjtrue;
}

CJEXTERNC static cjbool _cjsch_parse_json_sch_trigger_dict_advance(
	cjsch_trigger* out_trig, cjson* json) {

	if (!json || !out_trig)
		return cjfalse;

	cjson* dict = cjNULL;

	if (!cjson_get_dict(&dict, json, "advance"))
		return cjfalse;

	cjmc buf[CJ_MAX_CJSON_VAL_LONG] = { 0 };

	//
	// parse start-type
	//

	if (!cjson_get_kv_bool(&out_trig->advance_enable, dict, "enable"))
		return cjfalse;


	//
	// delay start
	//

	cjson* delay_start_dict;

	if (!cjson_get_dict(&delay_start_dict, dict, "delay_start"))
		return cjfalse;

	// 0 30s 1m
	if (!cjson_get_kv_duration(&out_trig->advance.delay_start_duration, 
		delay_start_dict, "duration"))
		return cjfalse;


	//
	// loop option
	//

	cjson* loop_dict;

	if (!cjson_get_dict(&loop_dict, dict, "loop"))
		return cjfalse;

	if (!cjson_get_kv_bool(&out_trig->advance.is_loop_enable, 
		loop_dict, "enable"))
		return cjfalse;

	// 0 30s 1m
	if (!cjson_get_kv_interval(&out_trig->advance.loop_interval, 
		loop_dict, "interval"))
		return cjfalse;

	// 0 30s 1m
	if (!cjson_get_kv_duration(&out_trig->advance.loop_duration, 
		loop_dict, "duration"))
		return cjfalse;

	if (!cjson_get_kv_bool(&out_trig->advance.terminate_all_at_loop_exit, 
		loop_dict, "terminate_all"))
		return cjfalse;


	//
	// max_execution
	//

	cjson* max_exec_dict;

	if (!cjson_get_dict(&max_exec_dict, dict, "max_execution"))
		return cjfalse;

	if (!cjson_get_kv_bool(&out_trig->advance.is_max_execution_enable, 
		max_exec_dict, "enable"))
		return cjfalse;

	// 0 30s 1m
	if (!cjson_get_kv_duration(&out_trig->advance.max_execution_duration,
		max_exec_dict, "duration"))
		return cjfalse;

	if (!cjson_get_kv_bool(&out_trig->advance.terminate_at_max_execution_exit,
		max_exec_dict, "terminate"))
		return cjfalse;


	return cjtrue;

}


//
// parse actions
//

CJEXTERNC static cjbool _cjsch_parse_json_sch_action_dict_setting(
	cjsch_action* out_action, cjson* json) {

	if (!json || !out_action)
		return cjfalse;

	cjson* dict = cjNULL;

	if (!cjson_get_dict(&dict, json, "setting"))
		return cjfalse;

	//cjmc buf[CJ_MAX_CJSON_VAL_LONG] = { 0 };

	if (!cjson_get_kv_bool(&out_action->setting.instant_run_able, dict, 
		"instant_run_able"))
		return cjfalse;

	return cjtrue;
}

CJEXTERNC static cjbool _cjsch_parse_json_sch_action_dict_actor(
	cjsch_action* out_action, cjson* json) {

	if (!json || !out_action)
		return cjfalse;

	cjsch_action_actor_type actor_type = CJSCH_ACTION_ACTOR_TYPE_NONE;
	cjmc buf[CJ_MAX_CJSON_VAL_LONG] = { 0 };

	if (!cjson_get_kv_str(buf, CJA_SIZ(buf), json, "actor_type"))
		return cjfalse;

	cjson* dict = cjNULL;

	if (!cjson_get_dict(&dict, json, "actor"))
		return cjfalse;


	if (!cjstrcmp(buf, "none")) {
		actor_type = CJSCH_ACTION_ACTOR_TYPE_NONE;

	} else if (!cjstrcmp(buf, "program")) {
		actor_type = CJSCH_ACTION_ACTOR_TYPE_PROGRAM;

		cjson* prog_dict;

		if (!cjson_get_dict(&prog_dict, dict, "program"))
			return cjfalse;

		if (!cjson_get_kv_bool(&out_action->actor.dat.prog.enable,
			prog_dict, "enable"))
			return cjfalse;

		if (!cjson_get_kv_str(out_action->actor.dat.prog.path, 
			CJA_SIZ(out_action->actor.dat.prog.path), prog_dict, "path"))
			return cjfalse;

		if (!cjson_get_kv_str(out_action->actor.dat.prog.arg,
			CJA_SIZ(out_action->actor.dat.prog.arg), prog_dict, "arg"))
			return cjfalse;

		if (!cjson_get_kv_str(out_action->actor.dat.prog.work_path,
			CJA_SIZ(out_action->actor.dat.prog.work_path), prog_dict, "work_path"))
			return cjfalse;

	} else if (!cjstrcmp(buf, "mail")) {
		actor_type = CJSCH_ACTION_ACTOR_TYPE_MAIL;

		cjson* mail_dict;

		if (!cjson_get_dict(&mail_dict, dict, "mail"))
			return cjfalse;

		if (!cjson_get_kv_bool(&out_action->actor.dat.mail.enable, mail_dict, "enable"))
			return cjfalse;

		if (!cjson_get_kv_str(out_action->actor.dat.mail.mail_addr,
			CJA_SIZ(out_action->actor.dat.mail.mail_addr), mail_dict, "address"))
			return cjfalse;

	} else if (!cjstrcmp(buf, "message")) {
		actor_type = CJSCH_ACTION_ACTOR_TYPE_MESSAGE;

		cjson* msg_dict;

		if (!cjson_get_dict(&msg_dict, dict, "message"))
			return cjfalse;

		if (!cjson_get_kv_bool(&out_action->actor.dat.msg.enable, msg_dict, "enable"))
			return cjfalse;

		if (!cjson_get_kv_str(out_action->actor.dat.msg.text,
			CJA_SIZ(out_action->actor.dat.msg.text), msg_dict, "text"))
			return cjfalse;

	} else if (!cjstrcmp(buf, "functor")) {
		actor_type = CJSCH_ACTION_ACTOR_TYPE_FUNCTOR;

		cjson* functor_dict;

		if (!cjson_get_dict(&functor_dict, dict, "functor"))
			return cjfalse;

		if (!cjson_get_kv_bool(&out_action->actor.dat.functor.enable,
			functor_dict, "enable"))
			return cjfalse;

		if (!cjson_get_kv_str(out_action->actor.dat.functor.class,
			CJA_SIZ(out_action->actor.dat.functor.class), functor_dict, "class"))
			return cjfalse;

		if (!cjson_get_kv_str(out_action->actor.dat.functor.arg,
			CJA_SIZ(out_action->actor.dat.functor.arg), functor_dict, "arg"))
			return cjfalse;

	} else {
		return cjfalse;
	}

	out_action->actor.type = actor_type;

	return cjtrue;
}


CJEXTERNC static cjbool _cjsch_parse_json_sch_actions(cjsch_parse_json_sch_ctx* ctx,
	cjson* json_action, cjsch_parse_err* out_err, const cjmc** out_err_msg) {

	cji cnt = cjson_ary_siz(json_action);

	for (cji i = 0; i < cnt; i++) {
		cjson* cur = cjson_ary_at(json_action, i);

		cjsch_action* action = cjmzalloc(sizeof(cjsch_action));
		cjbool parsed = cjfalse;

		do {
			if (!cjson_get_kv_str(action->name, CJA_SIZ(action->name), cur, "name"))
				break;

			if (!cjson_get_kv_str(action->class, CJA_SIZ(action->class), cur, "class"))
				break;

			if (!cjson_get_kv_bool(&action->enable, cur, "enable"))
				break;

			if (!_cjsch_parse_json_sch_action_dict_setting(action, cur))
				break;

			if (!_cjsch_parse_json_sch_action_dict_actor(action, cur))
				break;

			parsed = cjtrue;

			break;

		} while (1);

		if (parsed) {
			if (!cjary_push_ptr(&ctx->actions, action)) {
				cjmfree(action);

				return cjfalse;
			}

		} else {
			cjmfree(action);

			return cjfalse;
		}

	}

	return cjtrue;
}






//
//static cjbool init_cjsch_yaml_parse_ctx(cjsch_yaml_parse_ctx* ctx) {
//
//	cjary_new_ptr_placed(&ctx->triggers, 2);
//
//
//
//
//	return cjtrue;
//}
//
//
//static void finalize_cjsch_yaml_parse_ctx(cjsch_yaml_parse_ctx* ctx) {
//
////	ctx->triggers
//
//
//
//}
////
////static cjrst parse_cjsch_yaml_parse_ctx(cjsch_yaml_parse_ctx* ctx, yaml_event_t* evt_prm) {
////
////	return 0;
////}
//
//
//CJEXTERNC cjrst cjsch_yaml_parser(struct cjyaml* yaml,
//	cjyaml_parse_evt evt, void* evt_prm, void* _user_prm) {
//
//	//cjsch_yaml_parse_ctx* ctx 
//	//	= (cjsch_yaml_parse_ctx*)_user_prm;
//
//	//if (!ctx)
//	//	return cjfail;
//
//	//switch (evt) {
//
//	//case CJYAML_PARSE_EVT_BEGIN: {
//	//	init_cjsch_yaml_parse_ctx(ctx);
//	//}
//	//break;
//
//	//case CJYAML_PARSE_EVT_ON_PARSE: {
//	//	//parse_cjsch_yaml_parse_ctx(ctx, (yaml_event_t*)evt_prm);
//	//}
//	//break;
//
//	//case CJYAML_PARSE_EVT_END: {
//	//	init_cjsch_yaml_parse_ctx(ctx);
//	//}
//	//break;
//
//	return cjrstfail;
//
//}

//
//static cjrst cjsch_yaml_parse_tag(cjyaml_parse_ctx* parse_ctx, const cjmc* tag) {
//
//	yaml_parser_t* parser = parse_ctx->parser;
//
//	yaml_event_t event;
//	yaml_event_type_t event_type;
//
//	do {
//		if (!yaml_parser_parse(parser, &event)) {
//			cjstrcpy_s(parse_ctx->last_parse_err, CJ_MAX_ERR_MSG, parser->problem);
//			return -1;
//		}
//
//		if (event.type == YAML_MAPPING_START_EVENT) {
//
//			yaml_event_t evt2;
//			yaml_parser_parse(parser, &evt2);
//
//			if (evt2.type == YAML_SCALAR_EVENT) {
//
//				cjmc cate[256] = { 0 };
//				cj_conv_u8c_to_mc(cate, 256, evt2.data.scalar.value);
//
//				printf("scalar-event = {value=\"%s\", length=%d}\n", 
//					cate, (int)evt2.data.scalar.length);
//
//
//
//
//			}
//
//			yaml_event_delete(&evt2);
//
//
//
//
//			cjmc val[]
//
//			event.data
//
//
//			parse_worker_recur_show_parse_tree_lv1(parse_ctx);
//		}
//
//		event_type = event.type;
//		yaml_event_delete(&event);
//
//	} while (event_type != YAML_STREAM_END_EVENT);
//
//	return cjrstok;
//}

//
//CJEXTERNC cjrst cjsch_yaml_parse_worker(cjyaml_parse_ctx* parse_ctx,
//	cjyaml_parse_evt evt, void* evt_prm) {
//		
//	switch (evt) {
//	case CJYAML_PARSE_EVT_BEGIN:
//		init_cjsch_yaml_parse_ctx(parse_ctx);
//		break;
//
//	case CJYAML_PARSE_EVT_END:
//		finalize_cjsch_yaml_parse_ctx(parse_ctx);
//		break;
//
//	case CJYAML_PARSE_EVT_ON_PARSE: {
//
//		if (cjrstok != cjyaml_parse_find_mapping_scalar(parse_ctx, "cjsch")) {
//
//
//		}
//
//
//
//
//
//		//yaml_parser_t* parser = parse_ctx->parser;
//		//yaml_event_t evt;
//		//yaml_event_type_t evt_type;
//
//		//do {
//		//	if (!yaml_parser_parse(parser, &evt)) {
//		//		cjstrcpy_s(parse_ctx->last_parse_err, CJ_MAX_ERR_MSG, parser->problem);
//		//		yaml_event_delete(&evt);
//
//		//		return -100;
//		//	}
//
//		//	// find entry 'cjsch'
//		//	if (evt_type == YAML_MAPPING_START_EVENT) {
//
//		//		do {
//		//			yaml_event_t evt2;
//		//			yaml_event_type_t evt_type2;
//
//		//			if (!yaml_parser_parse(parser, &evt2)) {
//		//				cjstrcpy_s(parse_ctx->last_parse_err, CJ_MAX_ERR_MSG, parser->problem);
//		//				yaml_event_delete(&evt2);
//		//				break;
//		//			}
//
//		//			if (evt_type2 == YAML_SCALAR_EVENT) {
//		//				cjmc mc_val[256] = { 0 };
//		//				cj_conv_u8c_to_mc(mc_val, 256, evt2.data.scalar.value);
//
//		//				printf("scalar-event (%d) = {value=\"%s\", length=%d}\n",
//		//					evt_type2, mc_val, (int)evt2.data.scalar.length);
//
//		//				if (!cjstrcmp(mc_val, "cjsch"))
//		//					cjsch_yaml_parse_tag(parse_ctx, "cjsch");
//		//			}
//
//
//		//		} while( evt_type == YAML_MAPPING_END_EVENT)
//
//		//	}
//
//		//	evt_type = evt.type;
//		//	yaml_event_delete(&evt);
//
//		//} while (evt_type != YAML_STREAM_END_EVENT);
//
//	}
//	break;
//
//	case CJYAML_PARSE_EVT_ERR_PARSE_FAILED:
//		break;
//
//	default:
//		return cjrstfail;
//
//	}
//
//	return cjrstok;
//}
