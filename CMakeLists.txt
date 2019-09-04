# 3.4 is required for CPack components
cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

cmake_policy(SET CMP0048 NEW) # VERSION
project(xhal VERSION 3.2.2 DESCRIPTION "xHAL interface library")

# Prevent in-source builds
include(PreventInSourceBuilds)

# Standard directories
include(GNUInstallDirs)

# Compile
add_subdirectory(xhalcore)
# add_subdirectory(xhalarm)

# Install README.md as part of xhal-base
install(
    FILES README.md
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
    COMPONENT base)

# Package configuration
set(CPACK_PACKAGING_INSTALL_PREFIX /opt/${PROJECT_NAME})
set(CPACK_COMPONENTS_ALL base)

# Package metadata
set(CPACK_PACKAGE_CONTACT "Louis Moureaux <lmoureau@ulb.ac.be>")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/README.md)
set(CPACK_PACKAGE_VENDOR "CMS GEM Collaboration")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

# Enable component-based packages
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_RPM_COMPONENT_INSTALL ON)

# Enable automatic discovery of dependencies
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# Relocateable RPM
set(CPACK_RPM_PACKAGE_RELOCATABLE ON)

# Generated package file name
foreach(c ${CPACK_COMPONENTS_ALL})
    list(APPEND CPACK_COMPONENTS_ALL ${c}-dev)
    string(TOUPPER ${c} _uppc)

    # Archive
    set(CPACK_ARCHIVE_${_uppc}_FILE_NAME
        ${PROJECT_NAME}-${c}_${PROJECT_VERSION}_${CMAKE_SYSTEM_PROCESSOR})

    # Debian
    # FIXME ${CMAKE_SYSTEM_PROCESSOR} is x86_64 while the convention is to use amd64
    set(CPACK_DEBIAN_${_uppc}_FILE_NAME
        ${PROJECT_NAME}-${c}_${PROJECT_VERSION}_${CMAKE_SYSTEM_PROCESSOR}.deb)
    set(CPACK_DEBIAN_${_uppc}-DEV_FILE_NAME
        ${PROJECT_NAME}-${c}-dev_${PROJECT_VERSION}_${CMAKE_SYSTEM_PROCESSOR}.deb)

    # RPM
    set(CPACK_RPM_${_uppc}_FILE_NAME
        ${PROJECT_NAME}-${c}_${PROJECT_VERSION}_${CMAKE_SYSTEM_PROCESSOR}.rpm)
    set(CPACK_RPM_${_uppc}-DEV_FILE_NAME
        ${PROJECT_NAME}-${c}-devel_${PROJECT_VERSION}_${CMAKE_SYSTEM_PROCESSOR}.rpm)
    set(CPACK_RPM_${_uppc}-DEV_PACKAGE_NAME
        ${PROJECT_NAME}-${c}-devel)

    unset(_uppc)
endforeach()

include(CPack)
