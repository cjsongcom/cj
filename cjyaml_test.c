
#if defined(CJ_TEST) && defined(CJ_TEST_CJYAML)

#include <cj/cj.h>
#include <cj/cjyaml.h>
#include <yaml.h>
#include <cj/cjyaml_json.h>


//static void print_event(yaml_event_t* event) {
//    static int level = 0;
//
//    switch (event->type) {
//    case YAML_NO_EVENT:
//        indent(level);
//        printf("no-event (%d)\n", event->type);
//        break;
//    case YAML_STREAM_START_EVENT:
//        indent(level++);
//        printf("stream-start-event (%d)\n", event->type);
//        break;
//    case YAML_STREAM_END_EVENT:
//        indent(--level);
//        printf("stream-end-event (%d)\n", event->type);
//        break;
//    case YAML_DOCUMENT_START_EVENT:
//        indent(level++);
//        printf("document-start-event (%d)\n", event->type);
//        break;
//    case YAML_DOCUMENT_END_EVENT:
//        indent(--level);
//        printf("document-end-event (%d)\n", event->type);
//        break;
//    case YAML_ALIAS_EVENT:
//        indent(level);
//        printf("alias-event (%d)\n", event->type);
//        break;
//    case YAML_SCALAR_EVENT:
//    {
//        cjmc mc_val[1024] = { 0 };
//        cj_conv_u8c_to_mc(mc_val, 1024, event->data.scalar.value);
//
//        indent(level);
//
//        //printf("scalar-event (%d) = {value=\"%s\", length=%d}\n",
//        //	event->type,
//        //	STRVAL(event->data.scalar.value),
//        //	(int)event->data.scalar.length);
//
//        printf("scalar-event (%d) = {value=\"%s\", length=%d}\n",
//            event->type,
//            mc_val,
//            (int)event->data.scalar.length);
//    }
//    break;
//    case YAML_SEQUENCE_START_EVENT:
//        indent(level++);
//        printf("sequence-start-event (%d)\n", event->type);
//        break;
//    case YAML_SEQUENCE_END_EVENT:
//        indent(--level);
//        printf("sequence-end-event (%d)\n", event->type);
//        break;
//    case YAML_MAPPING_START_EVENT:
//        indent(level++);
//        printf("mapping-start-event (%d)\n", event->type);
//        break;
//    case YAML_MAPPING_END_EVENT:
//        indent(--level);
//        printf("mapping-end-event (%d)\n", event->type);
//        break;
//    }
//    if (level < 0) {
//        fprintf(stderr, "indentation underflow!\n");
//        level = 0;
//    }
//}
//
//static cjbool parser_evt_hdr(struct cjyaml* yaml,
//    cjyaml_parse_evt evt, void* evt_prm, void* user_prm) {
//
//    switch(evt) {
//    case CJYAML_PARSE_EVT_BEGIN:
//        break;
//
//    case CJYAML_PARSE_EVT_END:
//        break;
//
//    case CJYAML_PARSE_EVT_ON_PARSE:
//    {
//        print_event(evt_prm);
//    }
//    break;
//
//    case CJYAML_PARSE_EVT_ERR_PARSE_FAILED:
//        break;
//
//    default:
//        break;
//    }
//
//    return cjrstok;
//}

//
//static cjrst parse_worker_recur_show_parse_tree_lv1(cjyaml_parse_ctx* parse_ctx) {
//
//    yaml_parser_t* parser
//        = parse_ctx->parser;
//
//    yaml_event_t event;
//    yaml_event_type_t event_type;
//
//    do {
//
//        if (!yaml_parser_parse(parser, &event)) {
//            cjyaml_parse_set_err(parse_ctx, CJYAML_ERR_PARSE_FAIL,
//                parser->problem);
//
//            return -1;
//        }
//
//        print_event(&event);
//
//        if (event.type == YAML_MAPPING_START_EVENT) {
//            parse_worker_recur_show_parse_tree_lv1(parse_ctx);
//        }
//
//        if (event.type == YAML_MAPPING_END_EVENT) {
//            return cjrstok;
//        }
//
//        event_type = event.type;
//        yaml_event_delete(&event);
//
//    } while (1);
//
//    return cjrstok;
//}
//
//CJEXTERNC cjrst cjyaml_parse_worker_show_parse_tree(cjyaml_parse_ctx* parse_ctx,
//    cjyaml_parse_evt evt, void* evt_prm) {
//
//    yaml_parser_t* parser
//        = parse_ctx->parser;
//
//    yaml_event_t event;
//    yaml_event_type_t event_type;
//
//    do {
//        if (!yaml_parser_parse(parser, &event)) {
//            yaml_event_delete(&event);
//
//            cjstrcpy_s(parse_ctx->last_parse_err, CJ_MAX_ERR_MSG,
//                parser->problem);
//
//            return -1;
//        }
//
//        print_event(&event);
//
//        if (event.type == YAML_MAPPING_START_EVENT) {
//            parse_worker_recur_show_parse_tree_lv1(parse_ctx);
//        }
//
//        event_type = event.type;
//        yaml_event_delete(&event);
//
//    } while (event_type != YAML_STREAM_END_EVENT);
//
//
//    return cjrstok;
//}
//
//static cjrst cjtest_cjyaml_show_parse_tree(cjyaml* yaml)     {
//
//    cjyaml_err  parse_err = 0;
//
//    if (!cjyaml_parse(yaml, cjyaml_parse_worker_show_parse_tree, cjNULL, &parse_err)) {
//        cjstd_printf("[cjtest][cjyaml] parsing is succeed, parse_err=%d", parse_err);
//    }
//
//    return cjrstok;
//}


 
CJEXTERNC cjrst cjtest_cjyaml_main(void* file_path) {

    if (!file_path)
        return cjrstfail;

    cjyaml* yaml = cjNULL;

    if (!cjyaml_create(&yaml, file_path, cjtrue))
        goto err_exit;

    if (!cjyaml_open(yaml))
        goto err_exit;

    //////////////////////////////////////////////

    //cjyaml_show_parsed_token(yaml);
 //   cjyaml_show_parsed_token(yaml, cjNULL);

//   cjtest_cjyaml_parse_cjsch(yaml);

     cjson* json = cjNULL;
     cjyaml_conv_json(&json, yaml);

     cjmc* p = cJSON_Print(json);
     printf("%s",p);

     cjson_destroy(json);

    //cjtest_cjyaml_parse_semi(yaml);

    //////////////////////////////////////////////

    cjyaml_destroy(yaml);
    return 0;

err_exit:
    cjyaml_destroy(yaml);
    return cjrstfail;
}


#endif






//
//CJEXTERNC cjrst cjyaml_parse_worker_semi(cjyaml_parse_ctx* parse_ctx,
//    cjyaml_parse_evt evt, void* evt_prm) {
//
//	switch (evt) {
//	case CJYAML_PARSE_EVT_BEGIN:
//        return cjyaml_parse_worker_semi(parse_ctx, CJYAML_PARSE_EVT_ON_PARSE, evt_prm);
//
//	case CJYAML_PARSE_EVT_END:
//		break;
//
//	case CJYAML_PARSE_EVT_ON_PARSE:
//    {
//        if (cjyaml_parse_find_mapping_scalar(parse_ctx, "cjsch")) {
//            cjstd_printf("[INFO] found mapping scalar 'cjsch' \n");
//
//            // find 'trigger'
//            if (cjyaml_parse_find_mapping_scalar(parse_ctx, "triggers")) {
//                cjstd_printf("[INFO] found mapping scalar 'triggers' \n");
//            } else {
//                cjstd_printf("[ERR] failed to find mapping scalar 'triggers' \n");
//            }
//
//
//        } else {
//            cjstd_printf("[ERR] failed to find mapping scalar 'cjsch' \n");
//            return cjrstfail;
//        }
//
//    }
//    break;
//        
//    case CJYAML_PARSE_EVT_ERR_PARSE_FAILED:
//       	break;
//        
//    default:
//       	return cjrstfail;
//    }
//        
//    return cjrstok;
//}

//static cjrst cjtest_cjyaml_parse_semi(cjyaml* yaml) {
//
//    cjyaml_err  parse_err = 0;
//
//    if (!cjyaml_parse(yaml, cjyaml_parse_worker_semi, cjNULL, &parse_err)) {
//        cjstd_printf("[cjtest][cjyaml] parsing is succeed, parse_err=%d", parse_err);
//    }
//
//    return cjrstok;
//}



//static cjbool cjtest_cjyaml(cjrst* rst) {
//
//    struct cjfile* yaml_file;
//
//    cjmc yaml_path[CJFILE_MAX_PATH] = { 0 };
//    cjfile_resolve_path(yaml_path, CJFILE_MAX_PATH, "core.yaml", CJFILE_PATH_HINT_BASE);
//
//    if (!cjfile_create_for_read(&yaml_file, yaml_path, cjNULL))
//        return cjfalse;
//
//    if (!cjfile_open(yaml_file))
//        return cjfalse;
//
//    cjfile_off src_siz;
//
//    // libyaml input source does not allow null terminator
//    cjmc* src = cjfile_read_all_utf8_mem(yaml_file, &src_siz, cjfalse, cjfalse, CJMALLOC, CJMFREE);
//
//    if (!src) {
//        cjfile_destroy(yaml_file);
//        return cjfalse;
//    }
//
//
//    yaml_parser_t parser;
//    yaml_event_t event;
//    yaml_event_type_t event_type;
//
//    yaml_parser_initialize(&parser);
//    //yaml_parser_set_input_file(&parser, stdin);
//    yaml_parser_set_input_string(&parser, src, src_siz);
//
//    do {
//        if (!yaml_parser_parse(&parser, &event))
//            goto error;
//
//        print_event(&event);
//        event_type = event.type;
//        yaml_event_delete(&event);
//
//    } while (event_type != YAML_STREAM_END_EVENT);
//
//    yaml_parser_delete(&parser);
//
//    cjfile_destroy(yaml_file);
//
//    return EXIT_SUCCESS;
//
//error:
//    fprintf(stderr, "Failed to parse: %s\n", parser.problem);
//    yaml_parser_delete(&parser);
//
//    return EXIT_FAILURE;
//}

//CJEXTERNC cjbool _cjyaml_parse_key_val(cjyaml_parse_ctx* parse_ctx,
//    cjyaml_token_type token_type) {
//
//    if (token_type == CJYAML_TOKEN_BLOCK_MAPPING_START) {
//
//        do {
//            cjmc key[CJ_MAX_CJYAML_KEY_SIZ] = { 0 };
//            cjmc val[CJ_MAX_CJYAML_VAL_SIZ] = { 0 };
//            cjyaml_token_type val_token_type = 0;
//
//            if (!cjyaml_parse_key_val_mapping(parse_ctx,
//                key, CJ_MAX_CJYAML_KEY_SIZ,
//                val, CJ_MAX_CJYAML_VAL_SIZ,
//                &val_token_type)) {
//
//                cjyaml_parse_set_err(parse_ctx,
//                    CJYAML_ERR_PARSE_FAIL_KEY_VAL,
//                    "failed to parse key-value");
//
//                return cjfalse;
//            }
//
//            if (cjyaml_is_scalar_token(val_token_type)) {
//                cjstd_printf("[parsed] key=%s, val=%s \n", key, val);
//
//            } else {
//
//                if (val_token_type == CJYAML_TOKEN_BLOCK_MAPPING_START) {
//                    _cjyaml_parse_key_val(parse_ctx, val_token_type);
//
//                } else if (val_token_type == CJYAML_TOKEN_BLOCK_END) {
//
//                    return cjtrue;
//                } else {
//
//                    return cjfalse;
//                }
//            }
//
//        } while (1);
//    
//    } else if(token_type == CJYAML_TOKEN_FLOW_SEQUENCE_START)  {
//
//        //if (!cjyaml_parse_valuekey_val_mapping(parse_ctx,
//        //    key, CJ_MAX_CJYAML_KEY_SIZ,
//        //    val, CJ_MAX_CJYAML_VAL_SIZ,
//        //    &val_token_type)) {
//
//        //    cjyaml_parse_set_err(parse_ctx,
//        //        CJYAML_ERR_PARSE_FAIL_KEY_VAL,
//        //        "failed to parse key-value");
//
//        //    return cjfalse;
//        //}
//        ;
//
//
//    }
//
//    return cjfalse;
//
//}