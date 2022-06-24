
#include <cj/cj.h>
#include "cjfile.h"


//
// file flag
//




//
// cjfile 
//

CJEXTERNC cjbool cjfile_is_valid(cjfile* file) {

    if (!file)
        return cjfalse;

    return (file->handle != CJFILE_HANDLE_INVALID) ? cjtrue : cjfalse;
}


CJEXTERNC cjbool cjfile_create_for_read(cjfile** out_file, const cjmc* path,
  cjfile_err* out_err) {

    cjfile_fl open_flag = CJFILE_FL_READ;
    cjflag share_flag = 0;
    cjfile_path_hint hint = CJFILE_PATH_HINT_BASE;

    cjfile_err err 
        = _cjfile_create_ex(out_file, path, open_flag, share_flag, hint, cjNULL);

    if (out_err)
        *out_err = err;

    return err == CJFILE_ERR_OK ? cjtrue : cjfalse;
}

CJEXTERNC cjbool cjfile_create_for_write(cjfile** out_file, const cjmc* path,
    cjfile_err* out_err) {

    cjfile_fl open_flag = CJFILE_FL_WRITE;
    cjflag share_flag = 0;
    cjfile_path_hint hint = CJFILE_PATH_HINT_BASE;

    open_flag |= CJFILE_FL_CREATE;
    open_flag |= CJFILE_FL_CREATE_PERM_READ | CJFILE_FL_CREATE_PERM_WRITE;

    cjfile_err err
        = _cjfile_create_ex(out_file, path, open_flag, share_flag, hint, cjNULL);

    if (out_err)
        *out_err = err;

    return err == CJFILE_ERR_OK ? cjtrue : cjfalse;
}

CJEXTERNC cjbool cjfile_create_for_write_ex(cjfile** out_file, const cjmc* path,
    cjbool is_append, cjbool is_truncate, cjfile_err* out_err) {

    cjfile_fl open_flag = CJFILE_FL_WRITE;
    cjflag share_flag = 0;
    cjfile_path_hint hint = CJFILE_PATH_HINT_BASE;

    if (is_append)
        open_flag |= CJFILE_FL_WRITE_APPEND;

    if (is_truncate)
        open_flag |= CJFILE_FL_TRUNCATE;

    open_flag |= CJFILE_FL_CREATE;
    open_flag |= CJFILE_FL_CREATE_PERM_READ | CJFILE_FL_CREATE_PERM_WRITE;

    cjfile_err err
        = _cjfile_create_ex(out_file, path, open_flag, share_flag, hint, cjNULL);

    if (out_err)
        *out_err = err;

    return err == CJFILE_ERR_OK ? cjtrue : cjfalse;
}

CJEXTERNC cjfile_err _cjfile_create(cjfile** out_file, const cjmc* path,
    cji read_write_opt, cjbool create_file_not_exist,
    cjfile_path_hint hint, void* prm) {
    
    cjfile_fl open_flag = 0;
    cjflag share_flag = 0;

    if (create_file_not_exist) {
        if (read_write_opt != 1) {
            // can't use CJFILE_FL_READ option with CJFILE_FL_CREATE
            return CJFILE_ERR_INVALID_PARAM;
        }
    }

    switch (read_write_opt) {
    case 0: open_flag = CJFILE_FL_READ; break;
    case 1: open_flag = CJFILE_FL_WRITE; break;
    case 2: open_flag = CJFILE_FL_READ | CJFILE_FL_WRITE; break;
    default: break;
    }

    if (create_file_not_exist) {
        open_flag |= CJFILE_FL_CREATE;
        open_flag |= CJFILE_FL_CREATE_PERM_READ | CJFILE_FL_CREATE_PERM_WRITE;
    }

    //if (open_truncate) {
    //    open_flag |= CJFILE_FL_TRUNCATE;
    //}

    return _cjfile_create_ex(out_file, path, open_flag, share_flag, hint, prm);
}


CJEXTERNC cjfile_err _cjfile_create_ex(cjfile** out_file, const cjmc* path,
    cjfile_fl open_flag, cjflag share_flag, cjfile_path_hint hint, void* prm)
{
    cjfile_err  err = CJFILE_ERR_OK;

    const cjmc* resolved_path = cjNULL;

    if (path) {
        resolved_path = cjfile_resolve_full_path(path);

    } else {
        err = CJFILE_ERR_INVALID_PARAM_PATH;
        goto err_exit;
    }

    if (!resolved_path || (*resolved_path == cjNULL)) {
        err = CJFILE_ERR_APP_RESOLVE_PATH;
        goto err_exit;
    }

    cjfile* file = cjmzalloc(sizeof(cjfile));

    file->handle = CJFILE_HANDLE_INVALID;
    file->open_flag = open_flag;
    file->hint = hint;

    cjfile_strcpy_s(file->path, CJFILE_MAX_PATH, resolved_path);

    file->name = cjfile_get_filename_from_path(file->path);
    file->prm = prm;

    if (out_file)
        *out_file = file;

    return err;

err_exit:

    if (out_file)
        *out_file = cjNULL;

    return err;
}

CJEXTERNC void cjfile_destroy(cjfile* file) {

    if (!file)
        return;

    cjmfree(file);
}


CJEXTERNC cjbool cjfile_open(cjfile* file)
{
    struct cjfilesys* fs = cjfilesys_get();
    return fs->open_file(file);
}

CJEXTERNC cjbool cjfile_close(cjfile* file)
{
    struct cjfilesys* fs = cjfilesys_get();
    return fs->close_file(file);
}

CJEXTERNC cjmc* cjfile_resolve_path(cjmc* out, cjstr_siz siz_buf, const cjmc* file_path, 
    cjfile_path_hint hint) {

    out[0] = cjNULL;

    if (!file_path || *file_path == 0)
        return out;

    if (hint == CJFILE_PATH_HINT_PASSTHRU || cjfile_is_absolute_path(file_path)) {
        cjstrcpy_s(out, siz_buf, file_path);

    } else {

        const cjmc* base_path = cjfile_get_basepath(hint);

        //cjopti
        if (base_path && *base_path)
        {
            const char* fmt = NULL;

            if (*file_path == CJFILESYS_PATH_SEP_WIN32 || *file_path == CJFILESYS_PATH_SEP_UNIX) {
                fmt = "%s%s";
            } else {
                #ifdef CJPF_WIN32
                    fmt = "%s\\%s";
                #else           
                    fmt = "%s/%s";
                #endif          
            }

            cjsnprintf(out, siz_buf, fmt, base_path, file_path);
        }
        else
        {
            cjstrcpy_s(out, siz_buf, file_path);
        }
    }

    return out;
}

CJEXTERNC cjbool cjfile_is_absolute_path(const cjmc* path) {

#ifdef CJPF_WIN32
    cjmc c = *(path + 1);

    if (!c || c != ':')
        return cjfalse;

    return cjtrue;

#else
    return path[0] == CJFILESYS_PATH_SEP_UNIX ? cjtrue : cjfalse;

#endif
}

CJEXTERNC const cjmc* cjfile_get_basepath(cjfile_path_hint hint) 
{ 
    return cjfilesys_get()->base_path[hint];
}

CJEXTERNC void cjfile_set_basepath(cjfile_path_hint hint, const cjmc* path) {

    struct cjfilesys* fs = cjfilesys_get();

    if (!path || !*path) {
        fs->base_path[hint][0] = cjNULL;
        return;
    }
        
    cjstrcpy_s(fs->base_path[hint], CJFILE_MAX_PATH, path);
}

CJEXTERNC cjbool cjfile_get_cur_path(cjmc* out_path, cjstr_siz out_path_siz) {
    struct cjfilesys* fs = cjfilesys_get();

    return fs->get_cur_dir(out_path, out_path_siz);
}

CJEXTERNC cjbool cjfile_get_proc_path(cjmc* out_path, cjstr_siz out_path_siz) {
    struct cjfilesys* fs = cjfilesys_get();

    return fs->get_proc_path(out_path, out_path_siz);
}

CJEXTERNC cjbool cjfile_normalize_path(cjmc* out, cjstr_siz out_siz, const cjmc* path) {

    cjmc d = '/';

    if (!out || !path)
        return cjfalse;

    out[0] = cjNULL;

    const cjmc* c = path;
    cjstr_siz cur_pos = 0;

    while (*c && out_siz > 0)
    {
        if (d == '/')
        {
            if (*c == '\\')
                out[cur_pos] = '/';
            else
                out[cur_pos] = *c;
        }
        else
        {
            if (*c == '/')
                out[cur_pos] = '\\';
            else
                out[cur_pos] = *c;
        }

        out_siz--;
        cur_pos++;

        c++;
    }

    if (out_siz <= 0)
    {
        cjassert(0 && "out index in buf..");
        return cjfalse;
    }

    out[cur_pos] = cjNULL;

    return cjtrue;
}

CJEXTERNC cjfile_off cjfile_get_pos(cjfile* file) {

    if (!cjfile_is_valid(file))
        return (cjfile_off)-1;

    if (file->open_flag & CJFILE_FL_WRITE) {

        if (!cjfile_flush(file))
            return (cjfile_off)-1;
    }

#ifdef CJPF_WIN32
    return (cjfile_off)_lseeki64(file->handle, 0, SEEK_CUR);
#else
    return (cjfile_off)::lseek(m_handle, 0, SEEK_CUR);
#endif
}

CJEXTERNC cjbool cjfile_set_pos(cjfile* file, cjfile_off pos) {

    if (!cjfile_is_valid(file))
        return cjfalse;

    if (pos == file->pos)
        return cjtrue;

#ifdef CJPF_WIN32
    // _lseek: 32bit 
    if (-1 == _lseeki64(file->handle, pos, SEEK_SET))
        return cjfalse;
#else
    if (-1 == ::lseek(file->handle, pos, SEEK_SET))
        return cjfalse;
#endif

    file->pos = pos;

    return cjtrue;
}

CJEXTERNC cjbool cjfile_get_siz(cjfile* file, cjfile_off* out_siz) {

    if (!cjfile_is_valid(file))
        return cjfalse;

    if (file->open_flag & CJFILE_FL_WRITE) {
        if (!cjfile_flush(file))
            return cjfalse;
    }

    cjfile_off cur_pos = cjfile_get_pos(file);

#ifdef CJPF_WIN32
    // long : always 32bit under win32 / win64
    _lseeki64(file->handle, 0, SEEK_END);
    cjfile_off end_pos = (cjfile_off)_lseeki64(file->handle, 0, SEEK_CUR);
    _lseeki64(file->handle, cur_pos, SEEK_SET);
#else
    // long : 64bit under unix/linux 64bit
    // long : 32bit under unix/linux 32bit
    lseek(file->handle, 0, SEEK_END);
    cjfile_off end_pos = (cjfile_off)lseek(file->handle, 0, SEEK_CUR);
    lseek(file->handle, cur, SEEK_SET);
#endif

    * out_siz = end_pos;

    return cjtrue;
}

CJEXTERNC void* cjfile_read_all_mem(cjfile* file, cjfile_off* out_siz, 
    cjbool add_null_term, cjmalloc_pfn alloc_m, cjmfree_pfn free_m) {

    void* rst = cjNULL;

    cjfile_off count = 0;
    cjfile_off read_pos = 0;
    cjfile_off readed_count = 0;

    if (!cjfile_get_siz(file, &count))
        return rst;

    void* buf = CJMALLOC_P(alloc_m, add_null_term ? (cjm_siz_t)count+1 : (cjm_siz_t)count);

    if (!buf)
        return rst;

    if (!cjfile_read_ex(file, buf, count, &read_pos, &readed_count))
        goto err_exit;

    if (out_siz)
        *out_siz = add_null_term ? readed_count +1 : readed_count;

    if (add_null_term)
        ((cjmc*)buf)[count] = cjNULL;

    rst = buf;

    return rst;

err_exit:
    if (buf)
        CJMFREE_P(free_m, buf);

    if (out_siz)
        *out_siz = 0;

    return rst;
}

CJEXTERNC cjmc* cjfile_read_all_utf8_mem(cjfile* file, cjfile_off* out_siz,
    cjbool convert_mc, cjbool add_null_term, cjmalloc_pfn alloc_m, cjmfree_pfn free_m) {

    cjmc* rst = cjNULL;

    cjfile_off raw_siz;
    cjmc* raw = cjfile_read_all_mem(file, &raw_siz, add_null_term, alloc_m, free_m);

    if (convert_mc) {
        cjstr_siz conv_siz;

        rst = cj_conv_u8c_to_mc_mem(&conv_siz, raw, (cjstr_siz)raw_siz, alloc_m, free_m);
        CJMFREE_P(free_m, raw);

        if (out_siz)
            *out_siz = conv_siz;
         
    } else {        
        if (out_siz)
            *out_siz = raw_siz;

        rst = raw;
    }   

    return rst;
}

CJEXTERNC cjbool cjfile_read_ex(cjfile* file, void* buf, cjfile_off count, 
    cjfile_off* in_read_pos, cjfile_off* out_readed_count) {

    if (!cjfile_is_valid(file))
        return cjfalse;

    cjfile_off readed = 0;

    if (count) {
        if (in_read_pos) {
            cjfile_off cur = cjfile_get_pos(file);

            if (cur != *in_read_pos) {

                if (!cjfile_set_pos(file, *in_read_pos))
                    return cjfalse;

                file->pos = *in_read_pos;
            }
        }

#ifdef CJPF_WIN32
        readed = _read(file->handle, buf, (unsigned int)count);
#else
        readed = read(file->handle, buf, count);
#endif

        if (readed == -1) {
            file->err = CJFILE_ERR_INVALID_PARAM;
            return cjfalse;
        }
    }

    file->pos += readed;

    if (out_readed_count != cjNULL)
        *out_readed_count = readed;

    if (readed != count) {        
        // reached End of File
        file->err = CJFILE_ERR_REACHED_EOF; 
        return cjfalse;
    }

    return cjtrue;
}

CJEXTERNC cjbool cjfile_write(cjfile* file, const void* buf, cjfile_off count) {
    return cjfile_write_ex(file, buf, count, cjNULL, cjNULL, cjfalse);
}

CJEXTERNC cjbool cjfile_write_utf8_from_mc(cjfile* file, const cjmc* src, cjstr_siz count) {
    
    cjstr_siz str_siz;

    cjmc* u8c = cj_conv_mc_to_u8c_mem(&str_siz, src, count, &CJMALLOC, &CJMFREE);

    if (!u8c)
        return cjfalse;

    cjfile_off writed_count = 0;
    cjfile_write_ex(file, u8c, sizeof(cjmc) * str_siz, cjNULL, &writed_count, cjfalse);

    cjmfree(u8c);

    return (writed_count == sizeof(cjmc) * str_siz) ? cjtrue : cjfalse;
}


CJEXTERNC cjbool cjfile_write_ex(cjfile* file, const void* buf, cjfile_off count,
    cjfile_off* in_write_pos, cjfile_off* out_writed_count, cjbool flush) {

    if (!cjfile_is_valid(file))
        return cjfalse;

    cjfile_off writed_count = 0;

    if (count) {
        if (in_write_pos) {
            cjfile_off cur = cjfile_get_pos(file);

            if (*in_write_pos != cur) {
                if (!cjfile_set_pos(file, cur))
                    return cjfalse;

                file->pos = *in_write_pos;
            }
        }

#ifdef CJPF_WIN32
        writed_count = _write(file->handle, buf, (unsigned int)count);
#else
        writed_count = write(file->handle, buf, count);
#endif

        if (writed_count == -1) {
            file->err = CJFILE_ERR_INVALID_PARAM;
            return cjfalse;
        }
    }

    file->pos += writed_count;

    if (out_writed_count)
        *out_writed_count = writed_count;

    if (count != writed_count) {
        // disk full ?
        file->err = CJFILE_ERR_PARTIAL_WRITE;
        return cjfalse;
    }

    if (flush)
        cjfile_flush(file);

    return cjtrue;
}

CJEXTERNC cjbool cjfile_flush(cjfile* file) {

    if (!cjfile_is_valid(file))
        return cjfalse;

#ifdef CJPF_WIN32
    // https://stackoverflow.com/questions/3989545/how-do-i-get-the-file-handle-from-the-fopen-file-structure
    
    HANDLE handle_w32 = (HANDLE)_get_osfhandle(file->handle);

    if (!FlushFileBuffers(handle_w32))
        return cjfalse;

#else

    // POSIX doesn't define a behavior for fsync() on a file descriptor that was opened O_RDONLY
    if (file->open_fl & CJFILE_FL_WRITE) {
        if (fsync(file->handle))
            return cjfalse;
    }

    /*
           fsync() transfers ("flushes") all modified in-core data of (i.e.,
           modified buffer cache pages for) the file referred to by the file
           descriptor fd to the disk device (or other permanent storage device)
           so that all changed information can be retrieved even after the
           system crashed or was rebooted.  This includes writing through or
           flushing a disk cache if present.  The call blocks until the device
           reports that the transfer has completed.  It also flushes metadata
           information associated with the file (see stat(2)).

           Calling fsync() does not necessarily ensure that the entry in the
           directory containing the file has also reached disk.  For that an
           explicit fsync() on a file descriptor for the directory is also
           needed.

           fdatasync() is similar to fsync(), but does not flush modified
           metadata unless that metadata is needed in order to allow a
           subsequent data retrieval to be correctly handled.  For example,
           changes to st_atime or st_mtime (respectively, time of last access
           and time of last modification; see stat(2)) do not require flushing
           because they are not necessary for a subsequent data read to be
           handled correctly.  On the other hand, a change to the file size
           (st_size, as made by say ftruncate(2)), would require a metadata
           flush.

           The aim of fdatasync() is to reduce disk activity for applications
           that do not require all metadata to be synchronized with the disk.
    */
#endif

    return cjtrue;
}