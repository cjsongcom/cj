
#include <cj/cj.h>

#define CJ_TEST_CJHASH

#if defined(CJ_TEST) && defined(CJ_TEST_CJHASH)

#include <cj/cjhash.h>



CJEXTERNC static cjrst cjtest_cjhash_str_ptr_placed() {

	cjstd_printf("\n");
	cjstd_printf("===========================================\n");
	cjstd_printf("[INFO][cjhash][begin] cjhash_new_strptr_ptr_placed \n");
	cjstd_printf("===========================================\n");
	cjstd_printf("\n");

	cjhash hash;

	if (!cjhash_new_strptr_ptr_placed(&hash, 24))
		return cjfalse;

	if (!cjhash_push_str_ptr(&hash, "test_key111", "test_val111")) {
		cjstd_printf("[ERR][cjhash][cjhash_push_str_ptr] \"test_key111\",\"test_val111\" failed \n");
		return cjfalse;
	}

	if (!cjhash_push_str_ptr(&hash, "test_key222", "test_val222")) {
		cjstd_printf("[ERR][cjhash][cjhash_push_str_ptr] \"test_key222\",\"test_val222\" failed \n");
		return cjfalse;
	}

	cjstd_printf("[INFO][cjhash][cjhash_get_siz] size=%d \n", cjhash_get_siz(&hash));

	cjstd_printf("[INFO][cjhash][cjhash_find] key=test_key111, val=%s \n",
		cjhash_find(&hash, "test_key111")->val);

	cjstd_printf("[INFO][cjhash][cjhash_find] key=test_key222, val=%s \n",
		cjhash_find(&hash, "test_key222")->val);

	cjhash_pair* pair = cjhash_find(&hash, "test_key333");
	cjstd_printf("[INFO][cjhash][cjhash_find] key=test_key333, val=%s \n",
		pair ? pair->val : "ERR=invalid key");

	if (cjhash_remove_by_str(&hash, "test_key222")) {
		cjstd_printf("[INFO][cjhash][cjhash_remove_by_str] deleting key=test_key222 is success\n");
	} else {
		cjstd_printf("[ERR][cjhash][cjhash_remove_by_str] deleting key=test_key222 is failed\n");
	}

	cjhash_del(&hash);

	cjstd_printf("\n");
	cjstd_printf("===========================================\n");
	cjstd_printf("[INFO][cjhash][end] cjhash_new_strptr_ptr_placed  \n");
	cjstd_printf("===========================================\n");
	cjstd_printf("\n");

	return cjrstok;
}



CJEXTERNC static cjrst cjtest_cjhash_str_ptr() {

	cjstd_printf("\n");
	cjstd_printf("===========================================\n");
	cjstd_printf("[INFO][cjhash][begin] cjhash_new_str_ptr \n");
	cjstd_printf("===========================================\n");
	cjstd_printf("\n");

	cjhash* hash;

	if (!cjhash_new_str_ptr(&hash, 24))
		return cjfalse;

	if (!cjhash_push_str_ptr(hash, "test_key111", "test_val111")) {
		cjstd_printf("[ERR][cjhash][cjhash_push_str_ptr] \"test_key111\",\"test_val111\" failed \n");
		return cjfalse;
	}

	if (!cjhash_push_str_ptr(hash, "test_key222", "test_val222")) {
		cjstd_printf("[ERR][cjhash][cjhash_push_str_ptr] \"test_key222\",\"test_val222\" failed \n");
		return cjfalse;
	}

	cjstd_printf("[INFO][cjhash][cjhash_get_siz] size=%d \n", cjhash_get_siz(hash));

	cjstd_printf("[INFO][cjhash][cjhash_find] key=test_key111, val=%s \n",
		cjhash_find(hash, "test_key111")->val);

	cjstd_printf("[INFO][cjhash][cjhash_find] key=test_key222, val=%s \n",
		cjhash_find(hash, "test_key222")->val);

	cjhash_pair* pair = cjhash_find(hash, "test_key333");
	cjstd_printf("[INFO][cjhash][cjhash_find] key=test_key333, val=%s \n",
		pair ? pair->val : "ERR=invalid key");

	if (cjhash_remove_by_str(hash, "test_key222")) {
		cjstd_printf("[INFO][cjhash][cjhash_remove_by_str] deleting key=test_key222 is success\n");
	} else {
		cjstd_printf("[ERR][cjhash][cjhash_remove_by_str] deleting key=test_key222 is failed\n");
	}

	cjhash_del(hash);

	cjstd_printf("\n");
	cjstd_printf("===========================================\n");
	cjstd_printf("[INFO][cjhash][end] cjhash_new_str_ptr  \n");
	cjstd_printf("===========================================\n");
	cjstd_printf("\n");

	return cjrstok;
}


CJEXTERNC static cjrst cjtest_cjhash_strptr_ptr() {

	cjstd_printf("\n");
	cjstd_printf("===========================================\n");
	cjstd_printf("[INFO][cjhash][begin] cjhash_new_strptr_ptr \n");
	cjstd_printf("===========================================\n");
	cjstd_printf("\n");

	cjhash* hash;

	if (!cjhash_new_strptr_ptr(&hash, 24))
		return cjfalse;

	if (!cjhash_push_str_ptr(hash, "test_key111", "test_val111")) {
		cjstd_printf("[ERR][cjhash][cjhash_push_str_ptr] \"test_key111\",\"test_val111\" failed \n");
		return cjfalse;
	}

	if (!cjhash_push_str_ptr(hash, "test_key222", "test_val222")) {
		cjstd_printf("[ERR][cjhash][cjhash_push_str_ptr] \"test_key222\",\"test_val222\" failed \n");
		return cjfalse;
	}	

	cjstd_printf("[INFO][cjhash][cjhash_get_siz] size=%d \n", cjhash_get_siz(hash));

	cjstd_printf("[INFO][cjhash][cjhash_find] key=test_key111, val=%s \n",
		cjhash_find(hash, "test_key111")->val);

	cjstd_printf("[INFO][cjhash][cjhash_find] key=test_key222, val=%s \n",
		cjhash_find(hash, "test_key222")->val);

	cjhash_pair* pair = cjhash_find(hash, "test_key333");
	cjstd_printf("[INFO][cjhash][cjhash_find] key=test_key333, val=%s \n",
		pair ? pair->val : "ERR=invalid key");

	if (cjhash_remove_by_str(hash, "test_key222")) {
		cjstd_printf("[INFO][cjhash][cjhash_remove_by_str] deleting key=test_key222 is success\n");
	} else {
		cjstd_printf("[ERR][cjhash][cjhash_remove_by_str] deleting key=test_key222 is failed\n");
	}

	cjhash_del(hash);

	cjstd_printf("\n");
	cjstd_printf("===========================================\n");
	cjstd_printf("[INFO][cjhash][end] cjhash_new_strptr_ptr  \n");
	cjstd_printf("===========================================\n");
	cjstd_printf("\n");


	return cjrstok;
}


CJEXTERNC cjrst cjtest_cjhash_main(void* prm) {

	cjtest_cjhash_str_ptr_placed();
//	cjtest_cjhash_ptr_ptr();
//	cjtest_cjhash_str_ptr();


	return cjrstok;
}



#endif
