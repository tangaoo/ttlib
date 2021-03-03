/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       file.h
 * @ingroup    platform
 * @author     tango
 * @date       2021-01-07 
 * @brief      file.h file
 */

#ifndef TT_PLATFORM_FILE_H
#define TT_PLATFORM_FILE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the aligned size for direct mode
#define TT_FILE_DIRECT_ASIZE                (512)

// the cached size for direct mode
#define TT_FILE_DIRECT_CSIZE                (1 << 14)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the file mode type
typedef enum __tt_file_mode_t
{
    TT_FILE_MODE_RO         = 1,    //!< read only
    TT_FILE_MODE_WO         = 2,    //!< write only
    TT_FILE_MODE_RW         = 4,    //!< write and read
    TT_FILE_MODE_CREAT      = 8,    //!< creat
    TT_FILE_MODE_APPEND     = 16,   //!< append
    TT_FILE_MODE_TRUNC      = 32,   //!< truncate
    TT_FILE_MODE_DIRECT     = 64    //!< direct, no cache, @note data &size must be aligned by TT_FILE_DIRECT_ASIZE
}tt_file_mode_t;

// the file seek type
typedef enum __tt_file_seek_flag_t
{
    TT_FILE_SEEK_BEG        = 0,
    TT_FILE_SEEK_CUR        = 1,
    TT_FILE_SEEK_END        = 2
}tt_file_seek_flag_t;

// the file type
typedef enum __tt_file_type_t
{
    TT_FILE_TYPE_NONE       = 0,
    TT_FILE_TYPE_DIRECTORY  = 1,
    TT_FILE_TYPE_FILE       = 2,
    TT_FILE_TYPE_DOT        = 3,
    TT_FILE_TYPE_DOT2       = 4
}tt_file_type_t;

// the file info
typedef struct __tt_file_info_t
{
    // the file type
    tt_size_t               type;

    // the file size
    tt_hize_t               size;

    // the last access time 
    tt_time_t               atime;

    // the last modify time
    tt_time_t               mtime;
}tt_file_info_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init the file
 *
 * @param path          the path
 * @param mode          the file mode
 *
 * @return              the file
 */
tt_file_ref_t           tt_file_init(tt_char_t const* path, tt_size_t mode);

/*! exit the file
 *
 * @param file          the file
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_file_exit(tt_file_ref_t file);

/*! read the file data
 *
 * @param file          the file
 * @param data          the data
 * @param size          the size
 *
 * @return              the real size or -1
 */
tt_long_t               tt_file_read(tt_file_ref_t file, tt_byte_t* data, tt_size_t size);

/*! write data to the file
 *
 * @param file          the file
 * @param data          the data
 * @param size          the size
 *
 * @return              the real size or -1
 */
tt_long_t               tt_file_writ(tt_file_ref_t file, tt_byte_t const* data, tt_size_t size);

/*! pread the file data
 *
 * @param file          the file 
 * @param data          the data 
 * @param size          the size 
 * @param offset        the offset, the file offset will not be changed 
 *
 * @return              the real size or -1
 */
tt_long_t               tt_file_pread(tt_file_ref_t file, tt_byte_t* data, tt_size_t size, tt_hize_t offset);

/*! pwrite the file data
 *
 * @param file          the file 
 * @param data          the data 
 * @param size          the size 
 * @param offset        the offset, the file offset will not be changed 
 *
 * @return              the real size or -1
 */
tt_long_t               tt_file_pwrit(tt_file_ref_t file, tt_byte_t const* data, tt_size_t size, tt_hize_t offset);

/*! seek the file offset
 *
 * @param file          the file 
 * @param offset        the file offset 
 * @param where         the seek flag, @ref tt_file_seek_flag_t 
 *
 * @return              tt_void
 */
tt_hong_t               tt_file_seek(tt_file_ref_t file, tt_hong_t offset, tt_size_t where);

/*! fsync the file
 *
 * @param file          the file
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_file_sync(tt_file_ref_t file);

/*! the file size
 *
 * @param file          the file
 *
 * @return              file size
 */
tt_hize_t               tt_file_size(tt_file_ref_t file);

/*! get the file offset
 *
 * @param file          the file
 *
 * @return              the file offset
 */
tt_hong_t               tt_file_offset(tt_file_ref_t file);

/*! get the file info
 *
 * @param file          the file
 * @param info          the file info
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_file_info(tt_char_t const* path, tt_file_info_t* info);

/*! copy the file
 *
 * @param path          the file path
 * @param dest          the dest path
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_file_copy(tt_char_t const* path, tt_char_t const* dest);

/*! create file
 *
 * @param path          the file path
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_file_create(tt_char_t const* path);

/*! remove file
 *
 * @param path          the file path 
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_file_remove(tt_char_t const* path);

/*! rename the file
 *
 * @param path          the source file path
 * @param dest          the destination file path
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_file_rename(tt_char_t const* path, tt_char_t const* dest);

/*! link the file
 *
 * @param path          the source file path
 * @param dest          the destination file path
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_file_link(tt_char_t const* path, tt_char_t const* dest);


/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif
