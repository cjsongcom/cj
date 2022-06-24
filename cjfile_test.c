
#define CJ_TEST_CJFILE

#ifdef CJ_TEST_CJFILE

#include "cj.h"
#include "cjfile.h"

//#include <fcntl.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <io.h>
//#include <stdio.h>

CJEXTERNC cjbool cjtest_cjfile_main(cjrst* rst);


static cjbool cjtest_cjfile_write(void) {

    cjfile* write_file = cjNULL;

    cjmc write_file_path[CJFILE_MAX_PATH] = { 0 };

    cjfile_resolve_path(write_file_path, CJFILE_MAX_PATH, "cjfile_test_write.txt", CJFILE_PATH_HINT_BASE);

    //if (0) {
    //    //     _O_WRONLY | _O_CREAT, _S_IREAD | _S_IWRITE
    //    int fd = _open(write_file_path, _O_WRONLY | _O_CREAT | _O_APPEND, _S_IREAD | _S_IWRITE);

    //    cjmc buf[1024] = { 0 };

    //    cjsnprintf(buf, 1024, "THIS IS CJFILE TEST\n");

    //    _write(fd, buf, strlen(buf));

    //    _close(fd);
    //    return 0;
    //}

    if (1) {

        if (!cjfile_create_for_write(&write_file, write_file_path, cjNULL)) {
            return cjfalse;
        }

        if (!cjfile_open(write_file)) {
            return cjfalse;
        }

        for (int i = 0; i < 10; i++) {
            cjmc write_buf[1024] = { 0 };

            cjsnprintf(write_buf, 1024, "%04d-ABCDEF-가나다라마바사\n", i);

            cjfile_off write_count = 0;

            if (!cjfile_write_utf8_from_mc(write_file, write_buf, cjstrlen(write_buf)))
                return cjfalse;
        }

        cjfile_close(write_file);

    }
    
    return cjtrue;

}

static cjbool cjtest_cjfile_read(void) {
    
    cjfile* read_file = cjNULL;

    cjmc read_file_path[CJFILE_MAX_PATH] = { 0 };

    cjfile_resolve_path(read_file_path, CJFILE_MAX_PATH, "cjfile_test_write.txt", CJFILE_PATH_HINT_BASE);

    if (1) {

        if (!cjfile_create_for_read(&read_file, read_file_path, cjNULL)) {
            return cjfalse;
        }

        if (!cjfile_open(read_file)) {
            return cjfalse;
        }
    }

    cji type = 1;

    if (type == 0) {

        cjmc read_buf[4096] = { 0 };

        if (!cjfile_read_ex(read_file, read_buf, 4095, cjNULL, cjNULL))
            return cjfalse;


    }
    else if (type == 1) {

        cjfile_off str_siz = 0;
        cjmc* mc_src;

        mc_src = cjfile_read_all_utf8_mem(read_file, &str_siz,
            cjtrue, cjtrue, CJMALLOC, CJMFREE);

        if (!mc_src)
            return cjfalse;

        fprintf(stdout, "%s", mc_src);

        cjmfree(mc_src);
    }
    
    cjfile_close(read_file);

    return cjtrue;
}

CJEXTERNC cjbool cjtest_cjfile_main(cjrst* rst) {
	
    cjtest_cjfile_write();
    cjtest_cjfile_read();

	return cjrstok;
}













#endif