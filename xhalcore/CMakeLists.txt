find_package(wiscrpcsvc REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_library(xhal-base SHARED
#     src/common/XHALDevice.cpp    # FIXME Obsolete?
#     src/common/XHALInterface.cpp # FIXME Obsolete?
    src/common/LMDB.cpp
    src/common/rpc/exceptions.cpp
#     src/common/utils/XHALXMLParser.cpp # FIXME Only on CTP7?
)

set_target_properties(
    xhal-base
    PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

# Library
install(
    TARGETS xhal-base
    COMPONENT base
    EXPORT xHALBaseConfig
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Headers
install(
    FILES
    include/lmdb++.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT server-dev)
install(
    FILES
    include/xhal/LMDB.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xhal
    COMPONENT server-dev)
install(
    FILES
#     include/xhal/XHALDevice.h
#     include/xhal/XHALInterface.h
    include/xhal/rpc/call.h
    include/xhal/rpc/common.h
    include/xhal/rpc/compat.h
    include/xhal/rpc/exceptions.h
    include/xhal/rpc/helper.h
    include/xhal/rpc/register.h
    include/xhal/rpc/wiscRPCMsg.h
    include/xhal/rpc/wiscrpcsvc.h
#     include/xhal/utils/Exception.h
#     include/xhal/utils/XHALXMLNode.h
#     include/xhal/utils/XHALXMLParser.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xhal/rpc
    COMPONENT base-dev)

# CMake configuration files
install(
    EXPORT xHALBaseConfig
    NAMESPACE xHAL::
    DESTINATION ${CMAKE_INSTALL_DATADIR}/xHAL
    COMPONENT base-dev)

# Dependencies of the dev package (not autodetected by the tools)
# RPM
set(CPACK_RPM_BASE-DEV_PACKAGE_REQUIRES
    "xhal-base == ${PROJECT_VERSION}, libstdc++-devel, wiscrpcsvc-devel"
    PARENT_SCOPE)
set(CPACK_RPM_BASE-DEV_PACKAGE_SUGGESTS "cmake >= 3.4" PARENT_SCOPE)

# DEB
set(CPACK_DEBIAN_BASE-DEV_PACKAGE_DEPENDS
    "xhal-base == ${PROJECT_VERSION}, libstdc++-dev" # wiscrpcsvc isn't packaged
    PARENT_SCOPE)
set(CPACK_DEBIAN_BASE-DEV_PACKAGE_SUGGESTS "cmake >= 3.4" PARENT_SCOPE)
