option(ZLIB_CRC32_POWER "Enable building POWER-CRC32 implementation" ON)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

#
# Check for IBM Power
#
if(ZLIB_CRC32_POWER)
    # preset the compiler specific flags
    set(POWER_TEST
        "#ifndef _ARCH_PPC \n\
            #error Target is not Power \n\
        #endif \n\
        #if !(defined(__PPC64__) || defined(__powerpc64__)) \n\
            #error Target is not 64 bits \n\
        #endif \n\
        #if !(defined(__BUILTIN_CPU_SUPPORTS__)) && !(__has_builtin(__builtin_cpu_supports)) \n\
            #error Target doesn't support __builtin_cpu_supports() \n\
        #endif ")

    # check for power 8 support
    set(CMAKE_REQUIRED_FLAGS "-mcpu=power8")
        check_c_source_compiles("${POWER_TEST}" HAS_POWER8_SUPPORT)
    unset(CMAKE_REQUIRED_FLAGS)

    # check for power 9 support
    set(CMAKE_REQUIRED_FLAGS "-mcpu=power9")
        check_c_source_compiles("${POWER_TEST}" HAS_POWER9_SUPPORT)
    unset(CMAKE_REQUIRED_FLAGS)

    # prepare compiling for power
    if(HAS_POWER8_SUPPORT OR HAS_POWER9_SUPPORT)
        add_library(zlib_crc32_power OBJECT 
            crc32_z_power8.c
            crc32_z_power_hooks.h
            crc32_constants.h
            clang_workaround.h)

        # set the compile flags
        if(HAS_POWER8_SUPPORT)
            target_compile_definitions(zlib_crc32_power PUBLIC Z_POWER8=1)
			target_compile_options(zlib_crc32_power PRIVATE -mcpu=power8)
        elseif(HAS_POWER9_SUPPORT)
            target_compile_definitions(zlib_crc32_power PUBLIC Z_POWER9=1)
			target_compile_options(zlib_crc32_power PRIVATE -mcpu=power9)
        endif(HAS_POWER8_SUPPORT)
    endif(HAS_POWER8_SUPPORT OR HAS_POWER9_SUPPORT)
endif(ZLIB_CRC32_POWER)
