
#define CJ_TEST_CJARY

#if defined(CJ_TEST) && defined(CJ_TEST_CJARY)


#include <cj/cj.h>
#include <cjary.h>


static void cjtest_cjary_show_stat(cjary* ary) {
	cjstd_printf(" is_ptr_ary = %d \n", ary->is_ptr_ary);
	cjstd_printf(" elem_siz = %d \n", ary->elem_siz);
	cjstd_printf(" capacity = %d \n", ary->capacity);
	cjstd_printf(" siz = %d \n", ary->siz);
}


static cjrst cjtest_cjary_ptr_test() {
	
	cjary* ptr_ary = cjary_new_ptr(3);
	cjtest_cjary_show_stat(ptr_ary);

	cji* rr = cjNULL;

	cji aa = 111;
	cji bb = 222;
	cji cc = 333;
	cji dd = 444;
	cji ee = 555;

	cji ff = 888;

	cjstd_printf("\n\n");
	cjstd_printf("====== cjary ptr begin ======\n");

	cjstd_printf("====== push ptr at 0, : %d ======\n", aa);
	cjary_push_ptr(ptr_ary, &aa);
	cjtest_cjary_show_stat(ptr_ary);
	rr = cjary_get_ptr(ptr_ary, 0);
	cjstd_printf("get ptr at 0: %d ======\n", *rr);

	cjstd_printf("====== push ptr at 1, : %d ======\n", bb);
	cjary_push_ptr(ptr_ary, &bb);
	cjtest_cjary_show_stat(ptr_ary);
	rr = cjary_get_ptr(ptr_ary, 1);
	cjstd_printf("get ptr at 1: %d ======\n", *rr);

	cjstd_printf("====== push ptr at 2, : %d ======\n", cc);
	cjary_push_ptr(ptr_ary, &cc);
	cjtest_cjary_show_stat(ptr_ary);
	rr = cjary_get_ptr(ptr_ary, 2);
	cjstd_printf("get ptr at 2: %d ======\n", *rr);

	cjstd_printf("====== push ptr at 3, : %d ======\n", dd);
	cjary_push_ptr(ptr_ary, &dd);
	cjtest_cjary_show_stat(ptr_ary);
	rr = cjary_get_ptr(ptr_ary, 3);
	cjstd_printf("get ptr at 3: %d ======\n", *rr);

	cjstd_printf("set ptr at 2: %d ======\n", ff);
	cjary_set_ptr(ptr_ary, 2, &ff);

	cjstd_printf("====== cjary show all ======\n");

	rr = cjary_get_ptr(ptr_ary, 0);
	cjstd_printf("get ptr at 0: %d \n", *rr);
	rr = cjary_get_ptr(ptr_ary, 1);
	cjstd_printf("get ptr at 1: %d \n", *rr);
	rr = cjary_get_ptr(ptr_ary, 2);
	cjstd_printf("get ptr at 2: %d \n", *rr);
	rr = cjary_get_ptr(ptr_ary, 3);
	cjstd_printf("get ptr at 3: %d \n", *rr);

	cjstd_printf("====== cjary erase ======\n");

	cjary_erase(ptr_ary, 1);
	cjtest_cjary_show_stat(ptr_ary);

	cjstd_printf("====== cjary show all after erase======\n");

	rr = cjary_get_ptr(ptr_ary, 0);
	cjstd_printf("get ptr at 0: %d \n", *rr);
	rr = cjary_get_ptr(ptr_ary, 1);
	cjstd_printf("get ptr at 1: %d \n", *rr);
	rr = cjary_get_ptr(ptr_ary, 2);
	cjstd_printf("get ptr at 2: %d \n", *rr);

	rr = cjary_get_ptr(ptr_ary, 3);

	if(rr)
		cjstd_printf("get ptr at 3: %d \n", *rr);


	cjstd_printf("====== cjary ptr end ======\n");
	cjstd_printf("\n\n");

	cjary_del(ptr_ary);

	return 0;
}

struct test_st_t {
	int a;
	char buf[10];
};

static cjrst cjtest_cjary_test() {

	cjary* ary = cjary_new(2, sizeof(struct test_st_t));

	struct test_st_t aa = {
		.a = 111,
		.buf = "aaa"
	};
		
	struct test_st_t bb = {
		.a = 222,
		.buf = "bbb"
	};

	struct test_st_t cc = {
		.a = 333,
		.buf = "ccc"
	};

	struct test_st_t dd = {
		.a = 444,
		.buf = "ddd"
	};

	struct test_st_t pp = {
	.a = 888,
	.buf = "ppp"
	};


	struct test_st_t* v;


	cjtest_cjary_show_stat(ary);

	cjstd_printf("\n\n");
	cjstd_printf("====== cjary begin ======\n");

	cjstd_printf("====== push struct aa ======\n");
	cjary_push_val(ary, &aa);
	cjtest_cjary_show_stat(ary);
	v = cjary_get_val(ary, 0, cjNULL);
	cjstd_printf("get val at 0: a=%d , buf=%s ======\n", v->a, v->buf);

	cjstd_printf("====== push struct bb ======\n");
	cjary_push_val(ary, &bb);
	cjtest_cjary_show_stat(ary);
	v = cjary_get_val(ary, 1, cjNULL);
	cjstd_printf("get val at 1: a=%d , buf=%s ======\n", v->a, v->buf);

	cjstd_printf("====== push struct cc ======\n");
	cjary_push_val(ary, &cc);
	cjtest_cjary_show_stat(ary);
	v = cjary_get_val(ary, 2, cjNULL);
	cjstd_printf("get val at 2: a=%d , buf=%s ======\n", v->a, v->buf);

	cjstd_printf("====== push struct dd ======\n");
	cjary_push_val(ary, &dd);
	cjtest_cjary_show_stat(ary);
	v = cjary_get_val(ary, 3, cjNULL);
	cjstd_printf("get val at 3: a=%d , buf=%s ======\n", v->a, v->buf);


	cjstd_printf("====== cjary erase at 1 ======\n");

	cjary_erase(ary, 1);
	cjtest_cjary_show_stat(ary);

	cjstd_printf("====== cjary show all after erase======\n");

	v = cjary_get_val(ary, 0, cjNULL);
	cjstd_printf("get val at 0: a=%d , buf=%s ======\n", v->a, v->buf);
	v = cjary_get_val(ary, 1, cjNULL);
	cjstd_printf("get val at 1: a=%d , buf=%s ======\n", v->a, v->buf);
	v = cjary_get_val(ary, 2, cjNULL);
	cjstd_printf("get val at 2: a=%d , buf=%s ======\n", v->a, v->buf);

	cjstd_printf("====== cjary set======\n");
	cjstd_printf("=== set val at 1: a=%d , buf=%s ======\n", pp.a, pp.buf);
	cjary_set_val(ary, 1, &pp);


	cjstd_printf("====== cjary show all after set======\n");

	v = cjary_get_val(ary, 0, cjNULL);
	cjstd_printf("get val at 0: a=%d , buf=%s ======\n", v->a, v->buf);
	v = cjary_get_val(ary, 1, cjNULL);
	cjstd_printf("get val at 1: a=%d , buf=%s ======\n", v->a, v->buf);
	v = cjary_get_val(ary, 2, cjNULL);
	cjstd_printf("get val at 2: a=%d , buf=%s ======\n", v->a, v->buf);


	cjary_del(ary);

	return 0;
}


CJEXTERNC cjrst cjtest_cjary_main(void* prm) {

	cjtest_cjary_ptr_test();
	cjtest_cjary_test();

	return 0;
}


#endif

