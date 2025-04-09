#ifndef Z_HOOKS_H__
#define Z_HOOKS_H__

/**
 * DEFLATE HOOKS
 */
#define DEFLATE_BOUND_ADJUST_COMPLEN(strm, complen, sourceLen) do {} while (0)
#define DEFLATE_BOUND_COMPLEN(source_len) 0
#define DEFLATE_DONE(strm, flush) 1
#define DEFLATE_GET_DICTIONARY_HOOK(strm, dict, dict_len) do {} while (0)
#define DEFLATE_HOOK(strm, flush, bstate) 0
#define DEFLATE_NEED_CHECKSUM(strm) 1
#define DEFLATE_NEED_CONSERVATIVE_BOUND(strm) 0
#define DEFLATE_PARAMS_HOOK(strm, level, strategy, hook_flush) do {} while (0)
#define DEFLATE_RESET_KEEP_HOOK(strm) do {} while (0)
#define DEFLATE_SET_DICTIONARY_HOOK(strm, dict, dict_len) do {} while (0)

/**
 * INFLATE HOOKS
 */
#define INFLATE_GET_DICTIONARY_HOOK(strm, dict, dict_len) do {} while (0)
#define INFLATE_MARK_HOOK(strm) do {} while (0)
#define INFLATE_NEED_CHECKSUM(strm) 1
#define INFLATE_NEED_UPDATEWINDOW(strm) 1
#define INFLATE_PRIME_HOOK(strm, bits, value) do {} while (0)
#define INFLATE_RESET_KEEP_HOOK(strm) do {} while (0)
#define INFLATE_SET_DICTIONARY_HOOK(strm, dict, dict_len) do {} while (0)
#define INFLATE_SYNC_POINT_HOOK(strm) do {} while (0)
#define INFLATE_TYPEDO_HOOK(strm, flush) do {} while (0)

/**
 * MEMORY HOOKS
 */
#define TRY_FREE_WINDOW TRY_FREE
#define ZALLOC_STATE ZALLOC
#define ZALLOC_WINDOW ZALLOC
#define ZCOPY_STATE zmemcpy
#define ZCOPY_WINDOW zmemcpy
#define ZFREE_STATE ZFREE
#define ZFREE_WINDOW ZFREE
#endif
