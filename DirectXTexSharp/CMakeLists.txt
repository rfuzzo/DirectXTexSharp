cmake_minimum_required(VERSION 3.5)
project(texconv)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/CMake")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/CMake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/CMake")

set(CMAKE_CXX_STANDARD 17)
# disable on win build
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

function(add_cxx_compiler_flags)
    set(CMAKE_REQUIRED_QUIET true)
    include(CheckCXXCompilerFlag)
    set(flags ${CMAKE_CXX_FLAGS})
    foreach(flag ${ARGV})
        string(REGEX REPLACE "[^_A-Za-z0-9]" "_" stripped_flag "${flag}")
        set(var "CXX_COMPILER_FLAG_${stripped_flag}")
        check_cxx_compiler_flag("${flag}" "${var}")
        if(${var})
            set(flags "${flags} ${flag}")
        endif()
    endforeach()
    set(CMAKE_CXX_FLAGS ${flags} PARENT_SCOPE)
endfunction()
add_cxx_compiler_flags(-pedantic -fno-permissive -pedantic-errors -Wall -Wextra -Wshadow -Wstrict-aliasing -Wno-misleading-indentation "-ftemplate-depth=1024")


include_directories(.)


# DXTEX options

option(BC_USE_OPENMP "Build with OpenMP support" ON)



set(LIBRARY_HEADERS
        ../DirectXTex/DirectXTex/DirectXTex.h
        ../DirectXTex/DirectXTex/DirectXTex.inl)

set(LIBRARY_SOURCES
        ../DirectXTex/DirectXTex/BC.h
        ../DirectXTex/DirectXTex/DDS.h
        ../DirectXTex/DirectXTex/DirectXTexP.h
        ../DirectXTex/DirectXTex/filters.h
        ../DirectXTex/DirectXTex/scoped.h
        ../DirectXTex/DirectXTex/BC.cpp
        ../DirectXTex/DirectXTex/BC4BC5.cpp
        ../DirectXTex/DirectXTex/BC6HBC7.cpp
        ../DirectXTex/DirectXTex/DirectXTexCompress.cpp
        ../DirectXTex/DirectXTex/DirectXTexConvert.cpp
        ../DirectXTex/DirectXTex/DirectXTexDDS.cpp
        ../DirectXTex/DirectXTex/DirectXTexHDR.cpp
        ../DirectXTex/DirectXTex/DirectXTexImage.cpp
        ../DirectXTex/DirectXTex/DirectXTexMipmaps.cpp
        ../DirectXTex/DirectXTex/DirectXTexMisc.cpp
        ../DirectXTex/DirectXTex/DirectXTexNormalMaps.cpp
        ../DirectXTex/DirectXTex/DirectXTexPMAlpha.cpp
        ../DirectXTex/DirectXTex/DirectXTexResize.cpp
        ../DirectXTex/DirectXTex/DirectXTexTGA.cpp
        ../DirectXTex/DirectXTex/DirectXTexUtil.cpp)

if(WIN32)
    set(LIBRARY_SOURCES ${LIBRARY_SOURCES}
            ../DirectXTex/DirectXTex/DirectXTexFlipRotate.cpp
            ../DirectXTex/DirectXTex/DirectXTexWIC.cpp)
endif()

add_library(texconv SHARED
        DirectXTexEnums.h
        texconv.h
        # texconv_lib.cpp
        Texconv.cpp
        Format.cpp
        Metadata.cpp
        ${LIBRARY_HEADERS}
        ${LIBRARY_SOURCES})
# add_library(texconv_static STATIC ${SOURCE})
add_executable(main
        DirectXTexEnums.h
        texconv.h
        # texconv_lib.cpp
        Texconv.cpp
        Format.cpp
        Metadata.cpp
        ${LIBRARY_HEADERS}
        ${LIBRARY_SOURCES} main.cpp)


if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /permissive- /JMC- /Zc:__cplusplus)


    if(BC_USE_OPENMP)
        target_compile_options(${PROJECT_NAME} PRIVATE /openmp /Zc:twoPhase-)
        target_compile_options(texconv PRIVATE /openmp /Zc:twoPhase-)
        target_compile_options(main PRIVATE /openmp /Zc:twoPhase-)
    endif()


endif()