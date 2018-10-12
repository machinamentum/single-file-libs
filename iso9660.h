/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 94):
 * <joshuahuelsman@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Josh Huelsman
 * ----------------------------------------------------------------------------
 */
#ifndef HSF_H
#define HSF_H

#include <stdint.h>
typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef char strA;
typedef char strD;

#define HSF_DEFAULT_PRIMARY_VOLUME_NAME "CD_IMAGE"

#define HSF_PATH_SEPARATOR '/'

#define HSF_SECTOR_SIZE 2048

#define HSF_VD_ID ("CD001")

#define HSF_VD_TYPE_BR      (0)
#define HSF_VD_TYPE_PVD     (1)
#define HSF_VD_TYPE_SVD     (2)
#define HSF_VD_TYPE_VPD     (3)
#define HSF_VD_TYPE_VDST  (255)

#ifdef __GNUC__
#define HSF_PACKED __attribute__((__packed__))
#else
#define HSF_PACKED
#pragma pack(push, 1)
#endif

typedef struct
{
    u8 years; //since 1900
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;
    u8 gmt_offset; //15 min intervals
} HSF_PACKED Hsf_Time_Stamp;

typedef struct
{
    char year[4];
    char month[2];
    char day[2];
    char hour[2];
    char minute[2];
    char second[2];
    char hundreths_of_second[2];
    u8 gmt_offset;
} HSF_PACKED Hsf_Date;

#define HSF_FILE_FLAG_HIDDEN          (1 << 0)
#define HSF_FILE_FLAG_IS_DIR          (1 << 1)
#define HSF_FILE_FLAG_ASSOCIATED_FILE (1 << 2)
#define HSF_FILE_FLAG_EAR             (1 << 3)
#define HSF_FILE_FLAG_EAR_PERMISSIONS (1 << 4)
#define HSF_FILE_FLAG_NOT_FINAL_DIR   (1 << 7)

typedef struct
{
    u8 length;
    u8 ear_length;
    u32 data_location_le;
    u32 data_location_be;
    u32 data_length_le;
    u32 data_length_be;
    Hsf_Time_Stamp time_stamp;
    u8 file_flags;
    u8 file_unit_size_interleaved;
    u8 interleave_gap;
    u16 volume_sequence_number_le;
    u16 volume_sequence_number_be;
    u8 filename_length;
    strD filename[1];
} HSF_PACKED Hsf_Directory_Entry;

typedef struct
{
    u8 type;
    strA id[5];
    u8 version;
    u8 data[0];
} HSF_PACKED Hsf_Volume_Descriptor;

typedef struct
{
    u8 type;
    strA id[5];
    u8 version;
    
    u8 unused0;
    char system_identifier[32];
    char volume_identifier[32];
    u8 unused1[8];
    u32 volume_space_size_le;
    u32 volume_space_size_be;
    u8 unused2[32];
    u16 volume_set_size_le;
    u16 volume_set_size_be;
    u16 volume_sequence_number_le;
    u16 volume_sequence_number_be;
    u16 logical_block_size_le;
    u16 logical_block_size_be;
    u32 path_table_size_le;
    u32 path_table_size_be;
    u32 path_table_location_le;
    u32 optional_path_table_location_le;
    u32 path_table_location_be;
    u32 optional_path_table_location_be;
    Hsf_Directory_Entry root_directory_entry;
    char volume_set_identifier[128];
    char publisher_identifier[128];
    char data_preparer_identifier[128];
    char application_identifier[128];
    char copyright_file_identifier[38];
    char abstrace_file_identifier[36];
    char bibliographic_file_identifier[37];
    Hsf_Date volume_creation_date;
    Hsf_Date volume_modification_date;
    Hsf_Date volume_expiration_date;
    Hsf_Date volume_effective_date;
    u8 file_structure_version;
    u8 unused3;
    u8 application_used[512];
    u8 reserved[653];
} HSF_PACKED Hsf_Primary_Volume_Descriptor;

typedef struct
{
    u8 identifier_length;
    u8 ear_length;
    u32 extent_location;
    u16 parent_directory_index;
    strD identifier[1];
} HSF_PACKED Hsf_Path_Table_Entry;

#ifdef __GNUC__
#else
#pragma pack(pop)
#endif

#undef HSF_PACKED

typedef int (*hsf_read_sector_callback)(void *payload, void *buffer, u32 sector_start, u32 sector_count);

typedef int (*hsf_write_sector_callback)(void *payload, void *buffer, u32 sector_start, u32 sector_count);


#define HSF_IO_READ_ONLY  0
#define HSF_IO_READ_WRITE 1

typedef struct
{
    void *user_payload;
    hsf_read_sector_callback read_sector_cb;
    hsf_write_sector_callback write_sector_cb;
    Hsf_Primary_Volume_Descriptor *pvd;
    
    int io_mode;
} Hsf_Context;

typedef struct
{
    Hsf_Context *ctx;
    Hsf_Directory_Entry *directory_entry;
    u32 seek_position;
} Hsf_File;

#ifdef __cplusplus
extern "C" {
#endif
    void hsf_create_context(Hsf_Context *ctx, void *callback_payload, hsf_read_sector_callback read_cb, hsf_write_sector_callback write_cb, int io_mode);
    void hsf_destroy_context(Hsf_Context *ctx);
    
#ifdef HSF_INCLUDE_STDIO
    void hsf_create_from_fopen(Hsf_Context *ctx, const char *filename);
    void hsf_destruct_with_fclose(Hsf_Context *ctx);
#endif
    
    void *hsf_get_sector(Hsf_Context *ctx, u32 Sector);
    Hsf_Primary_Volume_Descriptor *hsf_get_primary_volume_descriptor(Hsf_Context *ctx);
    Hsf_Directory_Entry *hsf_get_directory_entry(Hsf_Context *ctx, const char *filename);
    
#define HSF_SEEK_SET 0
#define HSF_SEEK_CUR 1
#define HSF_SEEK_END 2
    
    Hsf_File *hsf_file_open(Hsf_Context *ctx, const char *filename);
    void hsf_file_close(Hsf_File *file);
    void hsf_file_seek(Hsf_File *file, u32 offset, int seek_type);
    u32  hsf_file_tell(Hsf_File *file);
    int hsf_file_read(void *buffer, u64 count_bytes, Hsf_File *file);
    
    
    typedef void (*hsf_visitor_callback)(Hsf_Context *ctx, const char *dir_path, Hsf_Directory_Entry *entry, void *user_payload);
    void hsf_visit_directory(Hsf_Context *ctx, const char *dir_path, hsf_visitor_callback visitor_cb, void *user_payload);
    
#ifdef __cplusplus
} // extern "C"
#endif

#endif


#ifdef HSF_IMPLEMENTATION

#ifdef __cplusplus
extern "C" {
#endif
    
#ifdef HSF_INCLUDE_STDIO
#include <stdio.h>
    
    int __stdio_read_sector(void *payload, void *buffer, u32 sector, u32 sector_count) {
        int result = fseek((FILE *)payload, sector * HSF_SECTOR_SIZE, SEEK_SET);
        if (result != 0) return -1;
        
        FILE *file = (FILE *)payload;
        
        u32 total = 0;
        for (;;) {
            result = fread(buffer, HSF_SECTOR_SIZE, sector_count-total, file);
            if (result != (sector_count-total)) {
                if (ferror(file) || feof(file)) {
                    clearerr(file);
                    return -1;
                }
            }
            
            total += result;
            
            if (total == sector_count) break;
        }
        
        return 0;
    }
    
    int __stdio_write_sector(void *payload, void *buffer, u32 sector, u32 sector_count) {
        int result = fseek((FILE *)payload, sector * HSF_SECTOR_SIZE, SEEK_SET);
        if (result != 0) return -1;
        
        FILE *file = (FILE *)payload;
        
        u32 total = 0;
        for (;;) {
            result = fwrite(buffer, HSF_SECTOR_SIZE, sector_count-total, file);
            if (result != (sector_count-total)) {
                if (ferror(file) || feof(file)) {
                    clearerr(file);
                    return -1;
                }
            }
            
            total += result;
            
            if (total == sector_count) break;
        }
        
        return 0;
    }
    
    void hsf_create_from_fopen(Hsf_Context *ctx, const char *filename) {
        FILE *file = fopen(filename, "r+b");
        if (!file) {
            // file doesnt exist so open it in create-mode
            file = fopen(filename, "w+b");
        }
        
        ctx->pvd = 0;
        if (!file) return;
        
        hsf_create_context(ctx, file, __stdio_read_sector, __stdio_write_sector, HSF_IO_READ_ONLY);
    }
    
    void hsf_destruct_with_fclose(Hsf_Context *ctx)
    {
        fclose((FILE *)ctx->user_payload);
        hsf_destroy_context(ctx);
    }
#endif
    
    int __hsf_is_dchar_set(char C) {
        if ( (C >= 'A') || (C <= 'Z') ) return 1;
        if ( (C >= '0') || (C <= '9') ) return 1;
        if (C == '_') return 1;
        
        
        return 0;
    }
    
    int __hsf_is_achar_set(char C) {
        if (__hsf_is_dchar_set(C)) return 1;
        if (   C == '!' || C == '\"' || C == '%'
            || C == '&' || C == '\'' || C == '('
            || C == ')' || C == '*' || C == '+'
            || C == ',' || C == '-' || C == '.'
            || C == '/' || C == ':' || C == ';'
            || C == '<' || C == '=' || C == '>'
            || C == '?') return 1;
        
        return 0;
    }
    
    int __hsf_strncmp(const char *str0, const char *str1, u32 length) {
        if (!str0 && str1) return -1;
        if (str0 && !str1) return 1;
        if (!str0 && !str1) return 0;
        
        for (u32 i = 0; i < length; ++i) {
            int c0 = str0[i];
            int c1 = str1[i];
            
            int result = c0-c1;
            if (c0 == 0 && c1) return result;
            if (c1 == 0 && c0) return result;        
            if (c0 == 0 && c1 == 0) break;
            
            if (result == 0) continue;
            return result;
        }
        
        return 0;
    }
    
    u32 __hsf_strlen(const char *path) {
        const char *end = path;
        while (*end) end++;
        return (u32)(end-path);
    }
    
    void __hsf_memcpy(void *_dst, const void *_src, u32 size) {
        u8 *dst = (u8 *)_dst;
        u8 *src = (u8 *)_src;
        
        while (size) {
            *dst = *src;
            dst++;
            src++;
            size--;
        }
    }
    
    void __hsf_memset(void *buffer, u8 value, u64 bytes) {
        u8 *data = (u8 *)buffer;
        for (u64  i = 0; i < bytes; ++i) {
            data[i] = value;
        }
    }
    
    void __hsf_zero_memory(void *buffer, u64 bytes) {
        char *data = (char *)buffer;
        for (u64  i = 0; i < bytes; ++i) {
            data[i] = 0;
        }
    }
    
    void hsf_create_context(Hsf_Context *ctx, void *callback_payload, hsf_read_sector_callback read_cb, hsf_write_sector_callback write_cb, int io_mode) {
        ctx->user_payload = callback_payload;
        ctx->read_sector_cb = read_cb;
        ctx->write_sector_cb = write_cb;
        ctx->pvd = hsf_get_primary_volume_descriptor(ctx);
        ctx->io_mode = io_mode;
    }
    
    void hsf_destroy_context(Hsf_Context *ctx) {
        if (ctx->pvd) HSF_FREE(ctx->pvd);
        __hsf_zero_memory(ctx, sizeof(Hsf_Context));
    }
    
    int __hsf_read_sectors(Hsf_Context *ctx, u32 sector, u32 sector_count, void *buffer)
    {
        return ctx->read_sector_cb(ctx->user_payload, buffer, sector, sector_count);
    }
    
    int __hsf_write_sectors(Hsf_Context *ctx, u32 sector, u32 sector_count, void *buffer) {
        if (ctx->io_mode == HSF_IO_READ_WRITE) {
            return ctx->write_sector_cb(ctx->user_payload, buffer, sector, sector_count); 
        }
        
        return -1;
    }
    
    /*
    typedef struct
    {
        u8 Type;
        strA Id[5];
        u8 Version;
        
        u8 Unused0;
        strA SystemIdentifier[32];
        strD VolumeIdentifier[32];
        u8 Unused1[8];
        u32 VolumeSpaceSizeLE;
        u32 VolumeSpaceSizeBE;
        u8 Unused2[32];
        u16 VolumeSetSizeLE;
        u16 VolumeSetSizeBE;
        u16 VolumeSequenceNumberLE;
        u16 VolumeSequenceNumberBE;
        u16 LogicalBlockSizeLE;
        u16 LogicalBlockSizeBE;
        u32 PathTableSizeLE;
        u32 PathTableSizeBE;
        u32 PathTableLocationLE;
        u32 OptionalPathTableLocationLE;
        u32 PathTableLocationBE;
        u32 OptionalPathTableLocationBE;
        hsf_directory_entry RootDirectoryEntry;
        char VolumeSetIdentifier[128];
        char PublisherIdentifier[128];
        char DataPreparerIdentifier[128];
        char ApplicationIdentifier[128];
        char CopyrightFileIdentifier[38];
        char AbstractFileIdentifier[36];
        char BibliographicFileIdentifier[37];
        hsf_date VolumeCreationDate;
        hsf_date VolumeModificationDate;
        hsf_date VolumeExpirationDate;
        hsf_date VolumeEffectiveDate;
        u8 FileStructureVersion;
        u8 Unused3;
        u8 ApplicationUsed[512];
        u8 Reserved[653];
    } HSF_PACKED hsf_primary_volume_descriptor;
    */
    
    u32 __hsf_max_u32(u32 a, u32 b) {
        return (a > b) ? a : b;
    }
    
    int hsf_format_image(Hsf_Context *ctx, const char *primary_volume_name, u64 total_disc_size_sectors) {
        if (ctx->io_mode == HSF_IO_READ_ONLY) return -1;
        
        if (ctx->pvd) {
            HSF_FREE(ctx->pvd);
            ctx->pvd = 0;
        }
        
        // zero all the disc sectors first, this makes it easy to be compatible with standard
        // fwrite APIs for ensuring the file size without the user worrying about first making sure
        // the file is the correct size. However, this probably shouldn't be used on a direct file
        // handle to a CD-R device since it may make the entire disc read-only
        void *zero_mem = HSF_ALLOC(HSF_SECTOR_SIZE);
        __hsf_zero_memory(zero_mem, HSF_SECTOR_SIZE);
        
        for (u64 i = 0; i < total_disc_size_sectors; ++i) {
            int result = __hsf_write_sectors(ctx, i, 1, zero_mem);
            if (result != 0) {
                HSF_FREE(zero_mem);
                return -1;
            }
        }
        
        if (!primary_volume_name) primary_volume_name = HSF_DEFAULT_PRIMARY_VOLUME_NAME;
        
        Hsf_Primary_Volume_Descriptor *pvd = (Hsf_Primary_Volume_Descriptor *)HSF_ALLOC(HSF_SECTOR_SIZE);
        __hsf_zero_memory(pvd, HSF_SECTOR_SIZE);
        
        pvd->type = HSF_VD_TYPE_PVD;
        __hsf_memcpy(&pvd->id[0], HSF_VD_ID, 5);
        pvd->version = 1;
        __hsf_memcpy(&pvd->system_identifier[0], "", 0); // @TODO see what grub expects here
        __hsf_memcpy(&pvd->volume_identifier[0], primary_volume_name, __hsf_max_u32(__hsf_strlen(primary_volume_name), 32));
        
        HSF_FREE(zero_mem);
        return 0;
    }
    
    int hsf_file_read(void *buffer, u64 count_bytes, Hsf_File *file) {
        Hsf_Context *ctx = file->ctx;
        
        u32 sector_start = file->directory_entry->data_location_le + (file->seek_position / HSF_SECTOR_SIZE);
        u32 num_sectors = (count_bytes / HSF_SECTOR_SIZE) + ((count_bytes % HSF_SECTOR_SIZE) ? 1 : 0);
        
        void *temp = HSF_ALLOC(HSF_SECTOR_SIZE * num_sectors);
        int result = __hsf_read_sectors(ctx, sector_start, num_sectors, temp);
        if (result != 0) {
            HSF_FREE(temp);
            return -1;
        }
        
        u32 start = file->seek_position % HSF_SECTOR_SIZE;
        __hsf_memcpy(buffer, ((char *)temp) + start, count_bytes);
        
        file->seek_position += count_bytes;
        HSF_FREE(temp);
        
        return 0;
    }
    
    void *hsf_get_sector(Hsf_Context *ctx, u32 sector) {
        void *buffer = HSF_ALLOC(HSF_SECTOR_SIZE);
        int result = __hsf_read_sectors(ctx, sector, 1, buffer);
        if (result != 0) {
            HSF_FREE(buffer);
            return 0;
        }
        return buffer;
    }
    
    Hsf_Primary_Volume_Descriptor *hsf_get_primary_volume_descriptor(Hsf_Context *ctx) {
        void *buffer = hsf_get_sector(ctx, 0x10);
        return (Hsf_Primary_Volume_Descriptor *)buffer;
    }
    
    int __hsf_parse_next_path_identifier(const char *path, int start_offset) {
        u32 path_len = __hsf_strlen(path);
        if (start_offset >= path_len) return path_len;
        
        const char *start = path + start_offset;
        u32 length = __hsf_strlen(start);
        
        for (u32 i = 0; i < length; ++i) {
            char c = start[i];
            
            if (c == HSF_PATH_SEPARATOR) {
                return start_offset + i;
            } else if (__hsf_is_dchar_set(c)) {
                continue;
            } else {
                return -1;
            }
        }
        
        return start_offset + length;
    }
    
    int __hsf_is_valid_path(const char *filename) {
        if (!filename) return -1;
        
        while (*filename) {
            char c = *filename;
            if (!(c == HSF_PATH_SEPARATOR || __hsf_is_dchar_set(c)) ) return -1;
            filename++;
        }
        
        return 0;
    }
    
    u32 __hsf_get_filename_length(Hsf_Directory_Entry *entry) {
        u32 max = entry->filename_length;
        
        for (u32 i = 0; i <= max; ++i) {
            char c = entry->filename[i];
            
            if (c == '.' && i < max) {
                char d = entry->filename[i+1];
                if (d == ';') return i;
            }
            
            // @FixMe -1 hwew is potentially very bad if i == 0, but maybe that never happens, i dunno man.
            if (c == ';') return i;
        }
        
        return max;
    }
    
    Hsf_Directory_Entry *hsf_get_directory_entry(Hsf_Context *ctx, const char *filename) {
        if (__hsf_is_valid_path(filename) == -1) return 0;
        
        int offset = __hsf_parse_next_path_identifier(filename, 0);
        if (offset == -1) return 0;
        
        int name_end = __hsf_parse_next_path_identifier(filename, offset + 2);
        
        void *buffer = hsf_get_sector(ctx, ctx->pvd->root_directory_entry.data_location_le);
        if (!buffer) return 0;
        
        Hsf_Directory_Entry *re = (Hsf_Directory_Entry *)buffer;
        
        if (name_end == 1) {
            return re;
        }
        
        offset = 1;
        
        int index_current = 0;
        // @TODO handle the case that you need > 2048 bytes
        int index_max = re->data_length_le;
        
        while (index_current < index_max) {
            if (re->length == 0) break;
            if (name_end == offset) break;
            
            u32 length = name_end - offset;
            u32 filename_len = __hsf_get_filename_length(re); 
            
            if (length == filename_len) {
                if (__hsf_strncmp(&re->filename[0], filename + offset, length) == 0) {
                    if (re->file_flags & HSF_FILE_FLAG_IS_DIR) {
                        offset = name_end + 1;
                        name_end = __hsf_parse_next_path_identifier(filename, offset);
                        
                        void *new_sector = hsf_get_sector(ctx, re->data_location_le);
                        HSF_FREE(buffer);
                        if (!new_sector) return 0;
                        
                        buffer = new_sector;
                        re = (Hsf_Directory_Entry *)buffer;
                        index_current = 0;
                        index_max = re->data_length_le;
                        
                        if (name_end <= offset) {
                            return re;
                        }
                        
                        continue;
                    } else {
                        offset = name_end + 1;
                        name_end = __hsf_parse_next_path_identifier(filename, offset);
                        
                        if (name_end <= offset) {
                            Hsf_Directory_Entry *out = (Hsf_Directory_Entry *)HSF_ALLOC(re->length);
                            __hsf_memcpy(out, re, re->length);
                            HSF_FREE(buffer);
                            return (Hsf_Directory_Entry *)out;
                        } else {
                            break; // there's more in the path so the path may be invalid after this point
                        }
                    }
                }
            }
            
            index_current += re->length;
            re = (Hsf_Directory_Entry *)((u8 *)re + re->length);
        }
        
        HSF_FREE(buffer);
        return 0;
    }
    
    Hsf_File *hsf_file_open(Hsf_Context *ctx, const char *filename) {
        Hsf_File *file = (Hsf_File *)HSF_ALLOC(sizeof(Hsf_File));
        file->ctx = ctx;
        file->directory_entry = hsf_get_directory_entry(ctx, filename);
        file->seek_position = 0;
        
        // File not found
        if (!file->directory_entry)  {
            HSF_FREE(file);
            return 0;
        }
        
        return file;
    }
    
    void hsf_file_close(Hsf_File *file) {
        HSF_FREE(file->directory_entry);
        HSF_FREE(file);
    }
    
    void hsf_file_seek(Hsf_File *file, u32 offset, int seek_type) {
        // @FixMe this is unsafe because we dont range-check Offset
        if (seek_type == HSF_SEEK_SET) {
            file->seek_position = offset;
        } else if (seek_type == HSF_SEEK_CUR) {
            file->seek_position += offset;
        } else if (seek_type == HSF_SEEK_END) {
            file->seek_position = file->directory_entry->data_length_le - offset;
        }
    }
    
    u32 hsf_file_tell(Hsf_File *file) {
        return file->seek_position;
    }
    
    void hsf_visit_directory(Hsf_Context *ctx, const char *dir_path, hsf_visitor_callback visitor_callback, void *user_payload) {
        Hsf_Directory_Entry *entry = hsf_get_directory_entry(ctx, dir_path);
        
        if (entry) {
            Hsf_Directory_Entry *entry_memory = entry;
            int index_current = 0;
            int index_max = entry->data_length_le;
            
            while (index_current < index_max) {
                if (entry->length == 0) break;
                
                visitor_callback(ctx, dir_path, entry, user_payload);
                
                index_current+= entry->length;
                entry = (Hsf_Directory_Entry *)((u8 *)entry + entry->length);
            }
            
            HSF_FREE(entry_memory);
        } else {
            // @TODO error
        }
    }
    
#ifdef __cplusplus
} // extern "C"
#endif


#endif // HSF_IMPLEMENTATION
