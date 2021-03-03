/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       file.c
 * @ingroup    platform
 * @author     tango
 * @date       2021-03-03 
 * @brief      file.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "file.h"
#include "native_memcpy.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>

// tmp TODO 

#define TT_PATH_MAXN    (1024)

tt_char_t const* tt_path_absolute(tt_char_t const* path, tt_char_t* full, tt_size_t size)
{
    return path;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tt_file_ref_t tt_file_init(tt_char_t const* path, tt_size_t mode)
{
    // check
    tt_assert_and_check_return_val(path, tt_null);

    // the full path
    tt_char_t full[TT_PATH_MAXN];
    path = tt_path_absolute(path, full, TT_PATH_MAXN);
    tt_assert_and_check_return_val(path, tt_null);

    // set flags
    tt_size_t flags = 0;
    if(mode & TT_FILE_MODE_RO)      flags |= O_RDONLY;
    else if(mode & TT_FILE_MODE_WO) flags |= O_WRONLY; 
    else if(mode & TT_FILE_MODE_RW) flags |= O_RDWR; 

    if(mode & TT_FILE_MODE_CREAT)   flags |= O_CREAT;
    if(mode & TT_FILE_MODE_APPEND)  flags |= O_APPEND;
    if(mode & TT_FILE_MODE_TRUNC)   flags |= O_TRUNC;

#ifdef TT_OS_LINUX
    if(mode & TT_FILE_MODE_DIRECT)  flags |= O_DIRECT;
#endif    

    // non-block
    flags |= O_NONBLOCK;

    // modes
    tt_size_t modes = 0;
    if(mode & TT_FILE_MODE_CREAT)
    {
        // 0644: -rw-r--r--
        modes = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    }

    // open it, @note need absolute path
    tt_long_t fd = open(path, flags, modes);
    if(fd < 0 && (mode & TT_FILE_MODE_CREAT) && (errno != EPERM && errno != EACCES))
    {
        // TODO
        tt_trace_e("no dir");
    }
    tt_check_return_val(fd >= 0, tt_null);

    // trace
    tt_trace_d("open: %p", tt_fd2file(fd));

    // ok?
    return tt_fd2file(fd);
}

tt_bool_t tt_file_exit(tt_file_ref_t file)
{
    // check
    tt_assert_and_check_return_val(file, tt_false);

    //trace
    tt_trace_d("close: %p", file);

    // close 
    tt_bool_t ok = !close(tt_file2fd(file))? tt_true : tt_false;
    //failed
    if(!ok) tt_trace_e("close: %p failed, errno, %d", file, errno);

    // ok?
    return ok;
}

tt_long_t tt_file_read(tt_file_ref_t file, tt_byte_t* data, tt_size_t size)
{
    // check
    tt_assert_and_check_return_val(file && data, -1);

    // read it
    return read(tt_file2fd(file), data, size);
}

tt_long_t tt_file_writ(tt_file_ref_t file, tt_byte_t const* data, tt_size_t size)
{
    // check
    tt_assert_and_check_return_val(file && data, -1);

    // write it 
    return write(tt_file2fd(file), data, size);
}

tt_long_t tt_file_pread(tt_file_ref_t file, tt_byte_t* data, tt_size_t size, tt_hize_t offset)
{
    // check
    tt_assert_and_check_return_val(file && data, -1);

    // pread
    return pread(tt_file2fd(file), data, (size_t)size, offset);
}


tt_long_t tt_file_pwrit(tt_file_ref_t file, tt_byte_t const* data, tt_size_t size, tt_hize_t offset)
{
    // check
    tt_assert_and_check_return_val(file && data, -1);

    // pread
    return pwrite(tt_file2fd(file), data, (size_t)size, offset);

}

tt_hong_t tt_file_seek(tt_file_ref_t file, tt_hong_t offset, tt_size_t where)
{
    // check
    tt_assert_and_check_return_val(file, -1);

    // seek
    return lseek(tt_file2fd(file), offset, where);
}

tt_bool_t tt_file_sync(tt_file_ref_t file)
{
    // check
    tt_assert_and_check_return_val(file, tt_false);

    // sysn
    return !fsync(tt_file2fd(file))? tt_true : tt_false;
}

tt_hize_t tt_file_size(tt_file_ref_t file)
{
    // check
    tt_assert_and_check_return_val(file, -1);

    // the file size
    tt_hize_t size = 0;
    struct stat st = {0};
    if(!fstat(tt_file2fd(file), &st)) size = st.st_size;

    // ok?
    return size;
}

tt_hong_t tt_file_offset(tt_file_ref_t file)
{
    // check
    tt_assert_and_check_return_val(file, -1);

    // the offset
    return tt_file_seek(file, (tt_hong_t)0, TT_FILE_SEEK_CUR);
}

tt_bool_t tt_file_info(tt_char_t const* path, tt_file_info_t* info)
{
    // check
    tt_assert_and_check_return_val(path && info, tt_false);

    // the full path (need translate such as "~/")
    tt_char_t full[TT_PATH_MAXN];
    path = tt_path_absolute(path, full, TT_PATH_MAXN);
    tt_assert_and_check_return_val(path, tt_false);

    // exists?
    tt_check_return_val(!access(path, F_OK), tt_false);

    // get info
    if(info)
    {
        // clear info
        tt_memset(info, 0, sizeof(tt_file_info));

        // get stat
        struct stat st = {0};
        if(!stat(path, &st))
        {
            // file type
            if(S_ISDIR(st.st_mode)) info->type = TT_FILE_TYPE_DIRECTORY;
            else info->type = TT_FILE_TYPE_FILE;

            info->size  = st.st_size >= 0? (tt_hize_t)st.st_size : 0;
            info->atime = (tt_time_t)st.st_atime;
            info->mtime = (tt_time_t)st.st_mtime;
        }
    }

    // ok
    return tt_true;
}

tt_bool_t tt_file_copy(tt_char_t const* path, tt_char_t const* dest)
{
    // check
    tt_assert_and_check_return_val(path && dest, tt_false);

    // done
    tt_int_t    ifd = -1;
    tt_int_t    ofd = -1;
    tt_bool_t   ok  = tt_false;
    do
    {
        // the full path
        tt_char_t data[8192];
        path = tt_path_absolute(path, data, TT_PATH_MAXN);
        tt_assert_and_check_return_val(path, tt_false);

        // get stat.st_mode 
        struct stat st = {0};
        if(stat(path, &st)) break;

        // open source file
        ifd = open(path, O_RDONLY);
        tt_check_break(ifd);

        // the full path
        dest = tt_path_absolute(dest, data, TT_PATH_MAXN);
        tt_assert_and_check_return_val(dest, tt_false);

        // open dest file and copy source file mode
        ofd = open(dest, O_RDWR | O_CREAT | O_TRUNC, st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
        if(ofd < 0 && (errno != EPERM && errno != EACCES))
        {
            // attempt to open it again after creating directory
            // TODO
        }
        tt_check_break(ofd);

        // get source file size
        tt_hize_t size = tt_file_size(tt_fd2file(ifd));

        // init write size
        tt_hize_t writ = 0;

        // copy
        while(writ < size)
        {
            // read some data
            tt_int_t real = read(ifd, data, (size_t)tt_min(size - writ, sizeof(data)));
            if(real > 0)
            {
                real = write(ofd, data, real);
                if(real > 0) writ += real;
                else break;
            }
            else break;
        }
        
        // ok?
        ok = (write == size);

    } while (0);
    
    // close ifd
    if(ifd >= 0) close(ifd);
    ifd = -1;

    // close ofd
    if(ofd >= 0) close(ofd);
    ofd = -1;

    // ok?
    return ok;
}


tt_bool_t tt_file_create(tt_char_t const* path)
{
    // check
    tt_assert_and_check_return_val(path, tt_false);

    // make it
    tt_file_ref_t file = tt_file_init(path, TT_FILE_MODE_CREAT | TT_FILE_MODE_WO | TT_FILE_MODE_TRUNC);
    if(file) tt_file_exit(file);

    // ok?
    return file? tt_true : tt_false;
}

tt_bool_t tt_file_remove(tt_char_t const* path)
{
    // check
    tt_assert_and_check_return_val(path, tt_false);

    // the full path
    tt_char_t full[TT_PATH_MAXN];
    path = tt_path_absolute(path, full, TT_PATH_MAXN);
    tt_assert_and_check_return_val(path, tt_false);

    // remove it
    return !remove(path)? tt_true : tt_false;
}

tt_bool_t tt_file_rename(tt_char_t const* path, tt_char_t const* dest)
{
    // check
    tt_assert_and_check_return_val(path && dest, tt_false);

    // the full path
    tt_char_t full0[TT_PATH_MAXN];
    path = tt_path_absolute(path, full0, TT_PATH_MAXN);
    tt_assert_and_check_return_val(path, tt_false);

    // the full path
    tt_char_t full1[TT_PATH_MAXN];
    dest = tt_path_absolute(dest, full1, TT_PATH_MAXN);
    tt_assert_and_check_return_val(dest, tt_false);

    // attempt to rename it directly
    if(!rename(path, dest)) return tt_true;
    else if(errno != EPERM && errno != EACCES)
    {
        // attempt to make dir, then try rename
    }

    return tt_false;
}

tt_bool_t tt_file_link(tt_char_t const* path, tt_char_t const* dest)
{
    // check
    tt_assert_and_check_return_val(path && dest, tt_false);

    // the full path
    tt_char_t full0[TT_PATH_MAXN];
    path = tt_path_absolute(path, full0, TT_PATH_MAXN);
    tt_assert_and_check_return_val(path, tt_false);

    // the full path
    tt_char_t full1[TT_PATH_MAXN];
    dest = tt_path_absolute(dest, full1, TT_PATH_MAXN);
    tt_assert_and_check_return_val(dest, tt_false);

    // attempt to rename it directly
    if(!symlink(path, dest)) return tt_true;
    else if(errno != EPERM && errno != EACCES)
    {
        // attempt to make dir, then try rename
    }

    return tt_false;
}



