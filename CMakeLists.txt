################################################################################
# Mohon jangan ubah
cmake_minimum_required( VERSION 3.0.2 )

GET_FILENAME_COMPONENT( NAMA_DIR_PROJEK ${CMAKE_CURRENT_SOURCE_DIR} NAME )
STRING( REPLACE " " "" PROJECT_NAME ${NAMA_DIR_PROJEK} )
STRING( REPLACE "-" "" PROJECT_NAME ${PROJECT_NAME} )
PROJECT( ${PROJECT_NAME} )
################################################################################
# local cmake modules
# SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")
################################################################################
# VERSION
STRING( TIMESTAMP PROJECT_VERSION_MAJOR %y )
STRING( TIMESTAMP PROJECT_VERSION_MINOR %m )
STRING( TIMESTAMP PROJECT_VERSION_RELEASE %d )
STRING( TIMESTAMP PROJECT_VERSION_BUILD %H )

SET( PROJECT_VERSION_MARKETTING 0 )
SET( PROJECT_SOVERSION ${PROJECT_VERSION_MARKETTING}.${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} )
SET( PROJECT_VERSION ${PROJECT_SOVERSION}.${PROJECT_VERSION_RELEASE}.${PROJECT_VERSION_BUILD} )
CONFIGURE_FILE( version.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/include/version.hpp )
################################################################################
# Build libraries as shared or static #######
# It is a bit messed up on Windows between STATIC and SHARED, if we don't want
# to add UtiLite_DEFINITIONS in FindUtiLite.cmake... to know from 3rd party application if it needs to
# link dynamically [UTILITE_EXP __declspec( dllimport )] or statically (UTILITE_EXP "nothing").
OPTION( BUILD_SHARED_LIBS "Set to OFF to build static libraries" ON )
################################################################################
# SET RPATH
# When RPATH is activated (supported on most UNIX systems),
# the user doesn't need to change LD_LIBRARY_PATH
# use, i.e. don't skip the full RPATH for the build tree
SET( CMAKE_SKIP_BUILD_RPATH  FALSE )
################################################################################
# when building, don't use the install RPATH already
# (but later on when installing)
SET( CMAKE_BUILD_WITH_INSTALL_RPATH FALSE )
################################################################################
# the RPATH to be used when installing
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME}" )
################################################################################
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE )
################################################################################
# OUTPUT DIR
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin )
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin )
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib )
################################################################################
# INSTALL DIR
# Offer the user the choice of overriding the installation directories
SET( INSTALL_LIB_DIR lib/${PROJECT_NAME} CACHE PATH "Installation directory for libraries" )
SET( INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
SET( INSTALL_INCLUDE_DIR include/${PROJECT_NAME} CACHE PATH "Installation directory for header files" )
if( WIN32 AND NOT CYGWIN )
SET( DEF_INSTALL_CMAKE_DIR CMake )
else()
SET( DEF_INSTALL_CMAKE_DIR lib/${PROJECT_NAME} )
endif()
MESSAGE( STATUS "DEF_INSTALL_CMAKE_DIR: ${DEF_INSTALL_CMAKE_DIR}" )
SET( INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files" )
MESSAGE( STATUS "INSTALL_CMAKE_DIR: ${INSTALL_CMAKE_DIR}" )
################################################################################
# FLAGS
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14" )
MESSAGE( STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )
#ADD_DEFINITIONS( -D_GLIBCXX_USE_CXX11_ABI=0 )
################################################################################
FIND_PACKAGE( PkgConfig REQUIRED )
PKG_CHECK_MODULES( BlueZ bluez )

IF( BlueZ_FOUND )
	MESSAGE( STATUS "Found BlueZ: ${BlueZ_VERSION}" )
	MESSAGE( STATUS "Include BlueZ: ${BlueZ_INCLUDE_DIRS}" )
ENDIF( BlueZ_FOUND )

SET(
	SRC_FILES
	main.cpp
)

SET(
	INCLUDE_DIRS
	${CMAKE_CURRENT_SOURCE_DIR}/
	${CMAKE_CURRENT_SOURCE_DIR}/include/
	${BlueZ_INCLUDE_DIRS}
)

MESSAGE( STATUS "INCLUDE_DIRS: ${INCLUDE_DIRS}" )

SET(
	LIBRARIES
	${BlueZ_LIBRARIES}
)

ADD_SUBDIRECTORY( src )

LINK_DIRECTORIES( ${LIBRARIES} )
INCLUDE_DIRECTORIES( ${INCLUDE_DIRS} )

ADD_EXECUTABLE( ${PROJECT_NAME} ${SRC_FILES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${PROJECT_NAME}Core ${LIBRARIES} )
################################################################################
# Install, jangan ubah!!!!
INSTALL(
TARGETS ${PROJECT_NAME}
RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT runtime
LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT devel
ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT devel
)

INSTALL(
DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ ${CMAKE_CURRENT_SOURCE_DIR}/GUI/
DESTINATION "${INSTALL_INCLUDE_DIR}"
COMPONENT devel
FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
PATTERN ".svn" EXCLUDE
PATTERN ".git" EXCLUDE
)

SET( HOME "$ENV{HOME}/.${PROJECT_NAME}" )
MESSAGE( STATUS "HOME: ${HOME}")
FILE( MAKE_DIRECTORY ${HOME} )
################################################################################
# Uninstall target, for "make uninstall"
CONFIGURE_FILE( "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY )
ADD_CUSTOM_TARGET( uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" )
################################################################################
# Setup RTABMapConfig.cmake
# Create the RTABMapConfig.cmake and RTABMapConfigVersion files
FILE( RELATIVE_PATH REL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_CMAKE_DIR}" "${CMAKE_INSTALL_PREFIX}/${INSTALL_INCLUDE_DIR}" )
FILE( RELATIVE_PATH REL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_CMAKE_DIR}" "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB_DIR}" )
################################################################################
# ... for the build tree
SET( CONF_INCLUDE_DIRS "${INCLUDE_DIRS}" )
SET( CONF_LIB_DIR "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}" )
CONFIGURE_FILE( Config.cmake.in "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY )
################################################################################
# ... for the install tree
SET( CONF_INCLUDE_DIRS "\${${PROJECT_NAME}_CMAKE_DIR}/${REL_INCLUDE_DIR}" )
SET( CONF_LIB_DIR "\${${PROJECT_NAME}_CMAKE_DIR}/${REL_LIB_DIR}" )
CONFIGURE_FILE( Config.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake" @ONLY )
################################################################################
# ... for both
CONFIGURE_FILE( ConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" @ONLY )
################################################################################
# Install the RTABMapConfig.cmake and RTABMapConfigVersion.cmake
INSTALL(
FILES
"${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
"${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
DESTINATION "${INSTALL_CMAKE_DIR}"
COMPONENT devel
)
