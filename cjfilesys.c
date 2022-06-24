
#include <cj/cj.h>
#include "cjfile.h"

#include <fcntl.h>
#include <sys/stat.h>   // S_IREAD


CJEXTERNC struct cjfilesys* cjfilesys_get();


#ifdef CJPF_WIN32

#include "cjfilesys_win32.c"



static struct cjfilesys_w32_dep {
    char path_sep;
} gs_filesys_win32_dep;

static struct cjfilesys_w32_dep* cjfilesys_get_plat_dep() {
    return &gs_filesys_win32_dep;
}

static struct cjfilesys  gs_filesys = {

    .filesys_os = CJFILESYS_OS_WIN32,

    .platform_dep = (void*)cjfilesys_get_plat_dep,

    .open_file = cjfilesys_win32_open_file,
    .close_file = cjfilesys_win32_close_file,

    .get_cur_dir = cjfilesys_win32_get_cur_dir,
    .get_proc_path = cjfilesys_win32_get_proc_path

};

static cjbool gs_cjfilesys_is_init;

static void cjfilesys_init(struct cjfilesys* fs) {
    fs->get_proc_path(fs->base_path[CJFILE_PATH_HINT_BASE], CJFILE_MAX_PATH);
}


#else
#error "impl, not yet"

#include "cjfilesys_unix.c"

static struct cjfilesys		gs_filesys;


#endif


CJEXTERNC struct cjfilesys* cjfilesys_get()
{
    if (!gs_cjfilesys_is_init) {
        cjfilesys_init(&gs_filesys);
        gs_cjfilesys_is_init = cjtrue;
    }

	return (struct cjfilesys*)&gs_filesys;
}


CJEXTERNC const cjmc* cjfile_resolve_full_path(const cjmc* path)
{
    // passthrue, meaningfull under aos, ios
	return path;
}

CJEXTERNC cjbool cjfile_is_exist_file(const cjmc* path) {        	
    struct cjfilesys* fs = cjfilesys_get();
    return fs->is_exist_file(path);
}

CJEXTERNC const cjmc* cjfile_get_filename_from_path(const cjmc* path) {

    struct cjfilesys* fs = cjfilesys_get();

    const cjmc* c;

    c = cjstrrchr(path, CJFILESYS_PATH_SEP_UNIX);

    if(!c && fs->filesys_os == CJFILESYS_OS_WIN32)
        c = cjstrrchr(path, CJFILESYS_PATH_SEP_WIN32);
    	
    if (c)
        return c + 1;
    else
        return path;
}


/*
www.gnu.org/software/libc/manual/html_node/Opening-and-Closing-Files.html
www.gnu.org/software/libc/manual/html_node/Access-Modes.html
open (filename, O_WRONLY | O_CREAT | O_TRUNC, mode)
*/

CJEXTERNC cjbool cjfile_translate_unix_open_flag(cjflag* out_unix_open_flag,
    cjflag* out_unix_open_mode, cjfile_fl open_file_flag) {

    // _O_BINARY = untranslate mode

    /*
        because win32 process LF('\n',0xa) to CRLF(0xd0xa) in text mode(translate mode).
        only binary mode(untranslat mode) is used.

        CR='\r'=0xd  ,  LF='\n'=0xa
        writing LF '\n' in file
        > win32: 0d0a
        > linux: 0a
    */

    cjflag trans_open_flag = _O_BINARY;
    cjflag trans_open_mode = 0;

    cjbool rst = cjtrue;

#ifdef CJPF_WIN32

    if (open_file_flag & CJFILE_FL_MASK) {
        // fail when file does not exist

        if (open_file_flag & CJFILE_FL_WRITE) {
            if (open_file_flag & CJFILE_FL_READ) {
                trans_open_flag |= _O_RDWR;     // open existing file for read/write
            } else {
                trans_open_flag |= _O_WRONLY;   // open existing file for writing only
            }

            if (open_file_flag & CJFILE_FL_WRITE_APPEND)
                trans_open_flag |= _O_APPEND;    // 'O_RDWR | O_APPEND' or 'O_WRONLY | O_APPEND'

        } else if (open_file_flag & CJFILE_FL_READ) {
            trans_open_flag |= _O_RDONLY;  // open existing file for read only            
        }

        if (open_file_flag & CJFILE_FL_TRUNCATE)
            trans_open_flag |= _O_TRUNC;

        if (open_file_flag & CJFILE_FL_CREATE)
            trans_open_flag |= _O_CREAT;

        if (open_file_flag & CJFILE_FL_CREATE_ONLY_NOT_EXIST)
            trans_open_flag |= _O_CREAT | _O_EXCL;
    }

    // created file permission
    if (open_file_flag & CJFILE_FL_CREATE_MASK) {

        if (open_file_flag & CJFILE_FL_CREATE_PERM_READ)
            trans_open_mode |= _S_IREAD;

        if (open_file_flag & CJFILE_FL_CREATE_PERM_WRITE)
            trans_open_mode |= _S_IWRITE;

        if (open_file_flag & CJFILE_FL_CREATE_PERM_EXECUTE)
            trans_open_mode |= _S_IEXEC;
    }


#else

#error "impl not yet"
     

    /*
     O_RDONLY : read access
     O_WRONLY : write access
     O_RDWR   : read & write

     O_CREAT : If set, the file will be created if it doesn't already exist.
     O_EXCL  : If both O_CREAT and O_EXCL are set, then open fails if the specified file already exists. This is guaranteed to never clobber an existing file.
     O_TRUNC : Truncate the file to zero length. This option is only useful for regular files, not special files such as directories or FIFOs. POSIX.1 requires that you open the file for writing to use O_TRUNC. In BSD and GNU you must have permission to write the file to truncate it, but you need not open for write access.
                This is the only open-time action flag specified by POSIX.1. There is no good reason for
                truncation to be done by open, instead of by calling ftruncate afterwards. The O_TRUNC flag
                existed in Unix before ftruncate was invented, and is retained for backward compatibility.

     O_SHLOCK : Acquire a shared lock on the file, as with flock
                If O_CREAT is specified, the locking is done atomically when creating the file. You are guaranteed that no other process will get the lock on the new file first.
     O_EXLOCK : Acquire an exclusive lock on the file, as with flock

     O_SYNC   : no needs flush io buffer
     O_APPEND : If you want to append to the file, setting the file position to the current end of file with SEEK_END is not sufficient. Another process may write more data after you seek but before you write, extending the file so the position you write onto clobbers their data. Instead, use the O_APPEND operating mode
    */
#endif

    if (out_unix_open_flag)
        *out_unix_open_flag = trans_open_flag;

    if (out_unix_open_mode)
        *out_unix_open_mode = trans_open_mode;

    return rst;
}

