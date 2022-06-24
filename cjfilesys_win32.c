
#ifdef WIN32

#include <cj/cj.h>

#include "cjfile.h"

// _getcwd()
#include <direct.h>


CJEXTERNC static cjbool cjfilesys_win32_open_file(cjfile* file) {
	
	if (!file || file->err != CJFILE_ERR_OK)
		return cjfalse;

	cjflag unix_open_flag;
	cjflag unix_open_mode;

	if (!cjfile_translate_unix_open_flag(&unix_open_flag, &unix_open_mode, 
		file->open_flag)) {
		file->err = CJFILE_ERR_INVALID_PARAM_OPEN_FLAG;

		return cjfalse;
	}

#ifdef CJFILE_USE_WIDE
#error "impl not yet !"
#else
	file->handle = _open(file->path, unix_open_flag, unix_open_mode);
#endif
	
	if (file->handle == CJFILE_ERR_INVALID_HANDLE) {
		file->open_err_no = errno;
		/*
		EPERM (1):  file permission
		ENOENT(2):  can't find file with given path
		EACCES(13): trying readonly file to write or failed shared opening
		EEXIST(17): file is already exist
		EINVAL(22): invalid open parameter
		EMFILE(24): too many file opened
		*/
		file->err = errno;

		return cjfalse;
	}
	 
	return cjtrue;
}

CJEXTERNC static cjbool cjfilesys_win32_close_file(cjfile* file) {

	if (!file || file->err != CJFILE_ERR_OK)
		return cjfalse;

	_close(file->handle);

	file->handle = CJFILE_HANDLE_INVALID;
	file->err = CJFILE_ERR_CLOSED;

	return cjtrue;
}

CJEXTERNC static cjbool cjfilesys_win32_get_cur_dir(cjmc* out_path, cjstr_siz out_path_siz) {

	if (!_getcwd(out_path, out_path_siz))
		return cjfalse;

	return cjtrue;
}

CJEXTERNC static cjbool cjfilesys_win32_get_proc_path(cjmc* out_path, cjstr_siz out_path_siz) {

	if (0 == GetModuleFileNameA(GetModuleHandleA(cjNULL), out_path, out_path_siz))
		return cjfalse;

	cjmc* p = strrchr(out_path, CJFILESYS_PATH_SEP_WIN32);

	if (p)
		*p = cjNULL;

	return cjtrue;
}

static CJEXTERNC cjbool cjfilesys_win32_is_exist_file(const cjmc* path) {

	cjmc resolved_path[CJFILE_MAX_PATH] = { 0 };

	cjfile_resolve_path(resolved_path, CJFILE_MAX_PATH, path, CJFILE_PATH_HINT_BASE);

	DWORD flag = GetFileAttributesA(resolved_path);

	if (flag == INVALID_FILE_ATTRIBUTES || (flag & FILE_ATTRIBUTE_DIRECTORY))
		return cjfalse;

	return cjtrue;
}


#endif   // end of ifdef WIN32