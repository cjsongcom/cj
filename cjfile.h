
#ifndef _CJFILE_H_
#define _CJFILE_H_

// O_RDWR
#include <fcntl.h>

//////////////////////////////////////////////////////////////
// because cjfile always save text as utf8 or multibyte
// wide character implementation is not supported
// >  ms utf16 LE BOM issue
//////////////////////////////////////////////////////////////

//typedef cjmc						cjfile_ch;
#define cjfile_strcpy_s				cjstrcpy_s

typedef cji							cjfile_handle;


typedef cjlong64					cjfile_off;
typedef cjlong64					cjfile_siz;


//
// cjfile define
//

enum { 
	CJFILE_HANDLE_INVALID			= -1,

	CJFILE_MAX_PATH					= CJ_MAX_PATH,
};

typedef enum cjfile_path_hint_t
{
	CJFILE_PATH_HINT_BASE			= 0,	// path in startup process
	CJFILE_PATH_HINT_PAK			= 1,	// path in resource pak
		  	   
	CJFILE_PATH_HINT_MAX,
		  	   
	CJFILE_PATH_HINT_DEFAULT		= CJFILE_PATH_HINT_BASE,
	CJFILE_PATH_HINT_ABSOLUTE		= -1,
	CJFILE_PATH_HINT_PASSTHRU		= -2

} cjfile_path_hint;


typedef enum cjfile_fl_t
{
	//
	// open existing file, fail not exist file
	//
	CJFILE_FL_READ							= 0x00000001,
	CJFILE_FL_WRITE							= 0x00000002,
	CJFILE_FL_RW							= CJFILE_FL_READ | CJFILE_FL_WRITE,

	// CJFILE_FL_WRITE_APPEND must be used CJFILE_FL_WRITE or CJFILE_FL_RW
	CJFILE_FL_WRITE_APPEND					= 0x00000004,
		
	CJFILE_FL_TRUNCATE						= 0x00000008,

	CJFILE_FL_CREATE						= 0x00000010,	
	CJFILE_FL_CREATE_ONLY_NOT_EXIST			= 0x00000020,
	
	CJFILE_FL_MASK							= 0x000000ff,


	// 
	// open mode - create file permission
	//

	CJFILE_FL_CREATE_PERM_WRITE				= 0x00000100,
	CJFILE_FL_CREATE_PERM_READ				= 0x00000200,
	CJFILE_FL_CREATE_PERM_EXECUTE			= 0x00000400,

	CJFILE_FL_CREATE_MASK					= 0x0000ff00,


} cjfile_fl;


typedef enum cjfile_seek_t
{
	CJFILE_SEEK_BEGIN,
	CJFILE_SEEK_CUR,
	CJFILE_SEEK_END

} cjfile_seek;


typedef enum cjfile_err_t
{
	// errno: 0 ~ 1000
	CJFILE_ERR_OPEN_FAIL_PERM							= 1,   // EPERM(1)
	CJFILE_ERR_OPEN_FAIL_INVALID_FILE_PATH				= 2,   // ENOENT(2)
	CJFILE_ERR_OPEN_FAIL_TRY_TO_READONLY_FILE_TO_WRITE	= 13,  // EACCESS(13)
	CJFILE_ERR_OPEN_FAIL_ALREADY_EXIST_FILE				= 17,  // EEXIST(17)
	CJFILE_ERR_OPEN_FAIL_INVALID_OPEN_FLAG				= 22,  // EINVAL(22)
	CJFILE_ERR_OPEN_FAIL_TOO_MANY_FILE_OPEN				= 24,  // EMFILE(24)
	
	CJFILE_ERR_OK = 0,

	CJFILE_ERR_INVALID_HANDLE							= -1,
	CJFILE_ERR_INVALID_PARAM							= -20,
	CJFILE_ERR_INVALID_PARAM_PATH						= -21,
	CJFILE_ERR_INVALID_PARAM_OPEN_FLAG					= -22,
	CJFILE_ERR_REACHED_EOF								= -30,	 // FILE: EOF
	CJFILE_ERR_PARTIAL_WRITE							= -40,   // FILE: DISK FULL ?	
	CJFILE_ERR_CLOSED									= -60,


	// IN APP(IOS,ANDROID)
	CJFILE_ERR_APP_RESOLVE_PATH			= -5001,

	CJFILE_ERR_UNKNOWN					= -9999
	
} cjfile_err;


//
// cjfilesystem
//

typedef enum cjfilesys_os_t {
	CJFILESYS_OS_UNIX,
	CJFILESYS_OS_WIN32
} cjfilesys_os;

#define CJFILESYS_PATH_SEP_UNIX			'/'
#define CJFILESYS_PATH_SEP_WIN32		'\\'

typedef struct cjfile_t cjfile;


typedef cjbool(*cjfile_open_pfn)(cjfile*);
typedef cjbool(*cjfile_close_pfn)(cjfile*);
typedef cjbool(*cjfile_get_cur_dir_pfn)(cjmc* out_path, cjstr_siz out_path_siz);
typedef cjbool(*cjfile_get_proc_path_pfn)(cjmc* out_path, cjstr_siz out_path_siz);
typedef cjbool(*cjfile_is_exist_file_pfn)(const cjmc* file_path);


struct cjfilesys {
	cjfilesys_os					filesys_os;   // 0:unix/linux,  1:window
	void*							platform_dep;

	cjfile_open_pfn					open_file;
	cjfile_close_pfn				close_file;

	cjfile_get_cur_dir_pfn			get_cur_dir;
	cjfile_get_proc_path_pfn		get_proc_path;

	cjfile_is_exist_file_pfn		is_exist_file;

	cjmc							base_path[CJFILE_PATH_HINT_MAX][CJFILE_MAX_PATH];	
};

CJEXTERNC struct cjfilesys* cjfilesys_get();


//
// cjfile
//

typedef struct cjfile_t 
{
	cjfile_err				err;

	cjfile_handle			handle;
	cjfile_off				pos;

	cjui					open_flag;

	cji						open_err_no;  // errno

	cjfile_path_hint	    hint;
	cjmc					path[CJFILE_MAX_PATH];

	const cjmc*				name; // filename
	
	void*					prm;
} cjfile;

CJEXTERNC cjbool cjfile_is_valid(cjfile* file);


/////////////////////////////////////////////////////////////////////////////
// because win32 changes LF('\n', 0xa) to CRLF(0xd0xa) in text mode(translate mode),
// only binary mode(untranslat mode) is used.
// 
// CR = '\r' = 0xd, LF = '\n' = 0xa
// writing LF '\n' in file
// > win32: 0d0a
// > linux: 0a
/////////////////////////////////////////////////////////////////////////////

// read_write_opt: 0: read , 1: write, 2 : read/write
CJEXTERNC cjbool cjfile_create_for_read(cjfile** out_file, const cjmc* path, cjfile_err* out_err);

CJEXTERNC cjbool cjfile_create_for_write(cjfile** out_file, const cjmc* path, cjfile_err* out_err);
CJEXTERNC cjbool cjfile_create_for_write_ex(cjfile** out_file, const cjmc* path,
	cjbool is_append, cjbool is_truncate, cjfile_err* out_err);

CJEXTERNC cjfile_err _cjfile_create(cjfile** out_file, const cjmc* path,
	cji read_write_opt, cjbool create_file_not_exist, cjfile_path_hint hint, void* prm);

CJEXTERNC cjfile_err _cjfile_create_ex(cjfile** out_file, const cjmc* path,
	cjfile_fl open_flag, cjflag share_flag, cjfile_path_hint hint, void* prm);

CJEXTERNC void cjfile_destroy(cjfile* file);


CJEXTERNC cjbool cjfile_open(cjfile* file);
CJEXTERNC cjbool cjfile_close(cjfile* file);


CJEXTERNC void* cjfile_read_all_mem(cjfile* file, cjfile_off* out_siz, 
	cjbool add_null_term, cjmalloc_pfn alloc_m, cjmfree_pfn free_m);

CJEXTERNC cjmc* cjfile_read_all_utf8_mem(cjfile* file, cjfile_off* out_siz,
	cjbool convert_mc, cjbool add_null_term, cjmalloc_pfn alloc_m, cjmfree_pfn free_m);

CJEXTERNC cjbool cjfile_read_ex(cjfile* file, void* buf, cjfile_off count,
	cjfile_off* in_read_pos, cjfile_off* out_readed_count);

CJEXTERNC cjbool cjfile_write_utf8_from_mc(cjfile* file, const cjmc* src, cjstr_siz count);

CJEXTERNC cjbool cjfile_write(cjfile* file, const void* buf, cjfile_off count);

CJEXTERNC cjbool cjfile_write_ex(cjfile* file, const void* buf, cjfile_off count,
	cjfile_off* in_write_pos, cjfile_off* out_writed_count, cjbool flush);

CJEXTERNC cjbool cjfile_flush(cjfile* file);

CJEXTERNC cjfile_off cjfile_get_pos(cjfile* file);
CJEXTERNC cjbool cjfile_set_pos(cjfile* file, cjfile_off pos);

CJEXTERNC cjbool cjfile_get_siz(cjfile* file, cjfile_off* out_siz);



//
// cjfile util
//


CJEXTERNC cjbool cjfile_is_exist_file(const cjmc* path);
CJEXTERNC const cjmc* cjfile_get_filename_from_path(const cjmc* path);

CJEXTERNC cjbool cjfile_translate_unix_open_flag(cjflag* out_unix_open_flag, 
	cjflag* out_unix_open_mode, cjfile_fl open_file_flag);


//
// path
//

// default hint: CJFILE_PATH_HINT_BASE
CJEXTERNC const cjmc* cjfile_get_basepath(cjfile_path_hint hint);
CJEXTERNC cjbool cjfile_get_cur_path(cjmc* out_path, cjstr_siz out_path_siz);

CJEXTERNC const cjmc* cjfile_resolve_full_path(const cjmc* path);

CJEXTERNC cjbool cjfile_is_absolute_path(const cjmc* path);

// normalize path seprator: '/'
CJEXTERNC cjbool cjfile_normalize_path(cjmc* out, cjstr_siz out_siz, const cjmc* path);

CJEXTERNC cjmc* cjfile_resolve_path(cjmc* out, cjstr_siz siz_buf, const cjmc* file_path,
	cjfile_path_hint hint);


#endif
