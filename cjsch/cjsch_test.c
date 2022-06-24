
#define CJ_TEST_CJSCH

#if defined(CJ_TEST) && defined(CJ_TEST_CJSCH)

#include <cj/cj.h>
#include <cj/cjsch/cjsch.h>

#include <cj/cjsch/cjsch_loader.h>


static const cjmc* _tlp = "[cjsch_test]";

// file_path="cjsch_test.yaml"
CJEXTERNC cjrst cjtest_cjsch_main(void* file_path) {

    if (!file_path)
        return cjrstfail;

    cjsch_loader* loader = cjNULL;

    //cjmc yaml_path[CJFILE_MAX_PATH] = { 0 };
    //cjfile_resolve_path(yaml_path, CJFILE_MAX_PATH, "cjsch_test.yaml", CJFILE_PATH_HINT_BASE);

    if (!cjsch_loader_create(&loader, file_path)) {
        cjstd_printf("%s failed to create cjsch, file=%s", _tlp, file_path);
        return cjrstfail;
    }

    if (!cjsch_loader_load(loader)) {
        cjstd_printf("%s failed to load cjsch, file=%s", _tlp, file_path);
        return cjrstfail;
    }

    cjsch_loader_destroy(loader);

	return 0;
}

#endif

