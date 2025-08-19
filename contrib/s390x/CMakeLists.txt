option(ZLIB_CRC32VX "Enable building S390-CRC32VX implementation" ON)
option(ZLIB_DFLTCC  "Enable building S390-DFLTCC deflate/inflate accelerator implementation" OFF)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

#
# check if we compile for IBM s390x
#
CHECK_C_SOURCE_COMPILES("
#ifndef __s390x__
    #error
#endif
" HAS_S390X_SUPPORT)

#
# Check for IBM S390X - VX extensions
#
if(ZLIB_CRC32VX AND HAS_S390X_SUPPORT)
    # preset the compiler specific flags
    if (CMAKE_C_COMPILER_ID STREQUAL "Clang")
        set(VGFMAFLAG "-fzvector")
    else()
        set(VGFMAFLAG "-mzarch")
    endif(CMAKE_C_COMPILER_ID STREQUAL "Clang")

    set(S390X_VX_TEST
        "#ifndef __s390x__ \n\
        #error \n\
        #endif \n\
        #include <vecintrin.h> \n\
        int main(void) { \
           unsigned long long a __attribute__((vector_size(16))) = { 0 }; \
           unsigned long long b __attribute__((vector_size(16))) = { 0 }; \
           unsigned char c __attribute__((vector_size(16))) = { 0 }; \
           c = vec_gfmsum_accum_128(a, b, c); \
           return c[0]; \
        }")

    # cflags already contains a valid march
    set(CMAKE_REQUIRED_FLAGS "${VGFMAFLAG}")
    check_c_source_compiles("${S390X_VX_TEST}" HAS_S390X_VX_SUPPORT)
    unset(CMAKE_REQUIRED_FLAGS)

    # or set march for our compile units
    if(NOT HAS_S390X_VX_SUPPORT)
        set(CMAKE_REQUIRED_FLAGS "${VGFMAFLAG} -march=z13")
        check_c_source_compiles("${S390X_VX_TEST}" HAS_Z13_S390X_VX_SUPPORT)
        unset(CMAKE_REQUIRED_FLAGS )
        list(APPEND VGFMAFLAG "-march=z13")
    endif(NOT HAS_S390X_VX_SUPPORT)

    # prepare compiling for s390x
    if(HAS_S390X_VX_SUPPORT OR HAS_Z13_S390X_VX_SUPPORT)
        add_library(zlib_crc32_vx OBJECT crc32-vx.c)
        set_source_files_properties(
            crc32-vx.c
            PROPERTIES COMPILE_OPTIONS "${VGFMAFLAG}")
        target_compile_definitions(zlib_crc32_vx PUBLIC HAVE_S390X_VX=1)
    endif(HAS_S390X_VX_SUPPORT OR HAS_Z13_S390X_VX_SUPPORT)
endif(ZLIB_CRC32VX AND HAS_S390X_SUPPORT)

#
# Check for IBM S390X - DFLTCC extensions
#
if(ZLIB_DFLTCC AND HAS_S390X_SUPPORT)
    # check if we have static_assert
    check_c_source_compiles("
        #include <assert.h>
        static_assert(1==1,\"true\");
        " HAS_STATIC_ASSERT)

    # check if we have secure_getenv
    set(CMAKE_REQUIRED_FLAGS -D_GNU_SOURCE=1)
    check_c_source_compiles("
        #include <stdlib.h>
        int main() { char* _foo = secure_getenv(\"PWD\");return 0; }
        " HAS_SECURE_GETENV)
    unset(CMAKE_REQUIRED_FLAGS)

    # check for specific headers
    check_include_file(sys/sdt.h HAS_SYS_SDT_H)

    # prepare compiling for s390x
    add_library(zlib_dfltcc OBJECT
        dfltcc.c
        ../functable/hooks.h)
    target_link_libraries(zlib_dfltcc PUBLIC $<TARGET_OBJECTS:zlib_s390x_functable>)
    target_compile_definitions(zlib_dfltcc PUBLIC HAVE_S390X_DFLTCC=1)
    target_compile_definitions(zlib_s390x_functable PUBLIC HAVE_S390X_DFLTCC=1)
    if(HAS_STATIC_ASSERT)
        target_compile_definitions(zlib_dfltcc PRIVATE HAVE_STATIC_ASSERT=1)
    endif()
    if(HAS_SECURE_GETENV)
        target_compile_definitions(zlib_dfltcc PRIVATE
                                   HAVE_SECURE_GETENV=1 _GNU_SOURCE=1)
    endif()
    if(HAS_SYS_SDT_H)
        target_compile_definitions(zlib_dfltcc PRIVATE HAVE_SYS_SDT_H=1)
    endif()
endif(ZLIB_DFLTCC AND HAS_S390X_SUPPORT)
