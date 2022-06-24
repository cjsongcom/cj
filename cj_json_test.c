
//#define CJ_TEST_CJ_JSON

#if defined(CJ_TEST) && defined(CJ_TEST_CJ_JSON)

#include <cj/cj.h>
#include <cj/cj_json.h>



static void* CJSON_CDECL failing_malloc(size_t size) {
	(void)size;
	return NULL;
}

/* work around MSVC error C2322: '...' address of dillimport '...' is not static */
static void CJSON_CDECL normal_free(void* pointer) {
	free(pointer);
}

static cJSON_Hooks failing_hooks = {
	failing_malloc,
	normal_free
};



//static void cjson_add_null_should_fail_on_allocation_failure(void) {
//	cJSON* root = cJSON_CreateObject();
//
//	cJSON_InitHooks(&failing_hooks);
//
//	TEST_ASSERT_NULL(cJSON_AddNullToObject(root, "null"));
//
//	cJSON_InitHooks(NULL);
//
//	cJSON_Delete(root);
//}


static cjbool cjtest_cj_json_type1() {

	cJSON* root = cJSON_CreateObject();


	cJSON_AddNumberToObject(root, "number", 42);

	cJSON_AddStringToObject(root, "type", "fruit");

	cJSON_AddBoolToObject(root, "is_ok", 1);

	cJSON_AddFalseToObject(root, "is_done");

	cjson* fruit_ary 
		= cJSON_AddArrayToObject(root, "list");
	//  = cJSON_GetObjectItemCaseSensitive(root, "fruit_lst");

	cjson* item1 = cJSON_CreateString("apple");
	cjson* item2 = cJSON_CreateNumber(1);
	cjson* item3 = cJSON_CreateNumber(3.14159);

	cJSON_AddItemToArray(fruit_ary, item1);
	cJSON_AddItemToArray(fruit_ary, item2);
	cJSON_AddItemToArray(fruit_ary, item3);
	cjson* fruit_ary_child_lst
		= cJSON_AddArrayToObject(fruit_ary, "child_lst");

	cjson* child_item1 = cJSON_CreateString("child_apple");
	cjson* child_item2 = cJSON_CreateNumber(99);
	cjson* child_item3 = cJSON_CreateNumber(55.14);

	cJSON_AddItemToArray(fruit_ary_child_lst, child_item1);
	cJSON_AddItemToArray(fruit_ary_child_lst, child_item2);
	cJSON_AddItemToArray(fruit_ary_child_lst, child_item3);


	//cji nums[] = { 1,2,3 };
	//cJSON_CreateIntArray(nums, 3);


	//cjson* fruit_name_ary;

	//{
	//	cjmc * fruit_names[] = { "apple", "grape", "peach"};

	//	fruit_name_ary = cJSON_CreateStringArray(fruit_names, 3);
	//}

	//cJSON_Add



	//cJSON_AddNumberToObject()


	//cjson* ary = cjson_new_ary();

	//cJSON_AddNumberToObject(ary, "val", 1);

	//cJSON_AddStringToObject(ary, "msg", "test");


	cjstd_printf("\n%s\n", cJSON_Print(root));
	cJSON_Delete(root);

	return cjtrue;
}



static cjbool cjtest_cj_json_int_array() {

	cji nums[] = { 1,2,3 };

	cjson* num_ary = cJSON_CreateIntArray(nums, 3);

	cjstd_printf("\n%s\n", cJSON_Print(num_ary));

	cJSON_Delete(num_ary);

	return cjtrue;
}

/*
{
   "name" : "È«±æµ¿",
   "age" : 20,
   "Å°" : 180,
   "ability" : [ "°ø°Ý", "¼öºñ" ],
   "speical" : "ûõ"
}
*/


static cjbool cjtest_cj_json_load_utf8(const cjmc* path) {

	cjmc* src = "";
	struct cjfile* file;

	cjmc file_path[CJ_MAX_PATH] = { 0 };

	cjfile_resolve_path(file_path, CJ_MAX_PATH,
		path, CJFILE_PATH_HINT_DEFAULT);

	if (!cjfile_create_for_read(&file, file_path, cjNULL))
		return cjfalse;

	if (!cjfile_open(file))
		return cjfalse;

	src = cjfile_read_all_mem(file, cjNULL, cjtrue, CJMALLOC, CJMFREE);

	cjfile_destroy(file);

	cjson* parsed = cJSON_Parse(src);	
	cjmfree(src);

	printf("%s", cJSON_Print(parsed));

	printf("\n\n\n");

	cjmc* s;
	cjmc val[256];

	s = cJSON_GetObjectItem(parsed, "name")->valuestring;
	cj_conv_u8c_to_mc(val, 256, s);

	printf("name: %s \n", val);

	cJSON_Delete(parsed);

	return cjtrue;
}




CJEXTERNC cjrst cjtest_cj_json_main(void* file_path) {

	cjtest_cj_json_load_utf8("test.json");

//	cjtest_cj_json_int_array();
//	cjtest_cj_json_type1();
	
	return cjrstok;
}








#endif
