project(savitar)
cmake_minimum_required(VERSION 2.8.12)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(BUILD_PYTHON "Build " ON)
option(BUILD_STATIC "Build as a static library" OFF)
option(BUILD_PUGIXML "Build internal pugixml library" ON)

if (BUILD_PUGIXML)
    add_subdirectory(pugixml)
endif()

if(BUILD_PYTHON)
    set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
    find_package(SIP REQUIRED)
    if(NOT DEFINED LIB_SUFFIX)
        set(LIB_SUFFIX "")
    endif()
    if(EXISTS /etc/debian_version)
        # Running on a debian-based system, which requires special handling for python modules.
        set(PYTHON_SITE_PACKAGES_DIR lib${LIB_SUFFIX}/python3/dist-packages CACHE STRING "Directory to install Python bindings to")
    else()
        set(PYTHON_SITE_PACKAGES_DIR lib${LIB_SUFFIX}/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages CACHE STRING "Directory to install Python bindings to")
    endif()
    include_directories(python/ src/ ${SIP_INCLUDE_DIR} ${PYTHON_INCLUDE_DIR})
endif()

if(NOT ${CMAKE_VERSION} VERSION_LESS 3.1)
    set(CMAKE_CXX_STANDARD 11)
else()
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

if(APPLE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

if(NOT APPLE AND NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
endif()


set(savitar_SRCS
    src/ThreeMFParser.cpp
    src/SceneNode.cpp
    src/Scene.cpp
    src/MeshData.cpp
    src/Vertex.cpp
    src/Face.cpp
)

set(savitar_HDRS
    src/ThreeMFParser.h
    src/Types.h
    src/SceneNode.h
    src/Scene.h
    src/MeshData.h
    src/Vertex.h
    src/Face.h
    src/SavitarExport.h
)

set(SAVITAR_VERSION 0.1.0)
set(SAVITAR_SOVERSION 0)

set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_FULL_LIBDIR})

if(BUILD_STATIC)
    add_library(Savitar STATIC ${savitar_SRCS})
else()
    add_library(Savitar SHARED ${savitar_SRCS})
endif()

target_link_libraries(Savitar PUBLIC pugixml)

if(NOT WIN32 OR CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(Savitar PROPERTIES COMPILE_FLAGS -fPIC)
endif()


if(BUILD_PYTHON)
    set(SIP_EXTRA_FILES_DEPEND python/Types.sip python/MeshData.sip python/SceneNode.sip python/Scene.sip)
    #set(SIP_EXTRA_SOURCE_FILES python/Types.cpp)
    set(SIP_EXTRA_OPTIONS -g -n PyQt5.sip) # -g means always release the GIL before calling C++ methods.
    add_sip_python_module(Savitar python/ThreeMFParser.sip Savitar)
endif()

target_include_directories(Savitar PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(${CMAKE_BUILD_TYPE})
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
        add_definitions(-DSAVITAR_DEBUG)
    endif()
endif()

set_target_properties(Savitar PROPERTIES
    FRAMEWORK FALSE
    VERSION ${SAVITAR_VERSION}
    SOVERSION ${SAVITAR_SOVERSION}
    PUBLIC_HEADER "${savitar_HDRS}"
    DEFINE_SYMBOL MAKE_SAVITAR_LIB
)

install(TARGETS Savitar
    EXPORT Savitar-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Savitar
)

install(EXPORT Savitar-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Savitar
)

configure_package_config_file(SavitarConfig.cmake.in ${CMAKE_BINARY_DIR}/SavitarConfig.cmake INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Savitar)
write_basic_package_version_file(${CMAKE_BINARY_DIR}/SavitarConfigVersion.cmake VERSION ${SAVITAR_VERSION} COMPATIBILITY SameMajorVersion)

install(FILES
    ${CMAKE_BINARY_DIR}/SavitarConfig.cmake
    ${CMAKE_BINARY_DIR}/SavitarConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Savitar
)

include(CPackConfig.cmake)
