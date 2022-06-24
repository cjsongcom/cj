
#include <cj/cj.h>

#define CJ_TEST_CJ_STRING

#if defined(CJ_TEST) && defined(CJ_TEST_CJ_STRING)

#include <cj/cj_string.h>


static cjrst cjtest_cj_string_parse_str(const cjmc* src) {

	cji64 v;

	if (!cj_parse_duration_str(&v, src)) {
		cjstd_printf("[ERR][parse] failed to parse src=%s\n", src);

		return cjrstfail;
	}

	cjstd_printf("[INFO][parse] src=%s, parsed value=" CJ_PRFMT_I64 "\n", src, v);

	return cjrstok;
}

CJEXTERNC cjrst cjtest_cj_string_main(void* prm) {

	cjstd_printf("\n");
	cjstd_printf("=== [cjtest_cj_string_parse_str] begin === \n");

	cjtest_cj_string_parse_str("1");

	cjtest_cj_string_parse_str("200");

	cjtest_cj_string_parse_str("1s");
	
	cjtest_cj_string_parse_str("10m");

	cjtest_cj_string_parse_str("10h");

	cjtest_cj_string_parse_str("1d");

	// fail case
	cjtest_cj_string_parse_str("1 s");
	cjtest_cj_string_parse_str("10.2m");
	cjtest_cj_string_parse_str("30d ");

	cjtest_cj_string_parse_str("10x");
	cjtest_cj_string_parse_str("22.5");


	cjstd_printf("=== [cjtest_cj_string_parse_str] end === \n");
	cjstd_printf("\n");

	return cjrstok;
}


#endif