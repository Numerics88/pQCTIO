cmake_minimum_required (VERSION 3.2)
project (pQCTIO)

# === Versioning

set(PQCTIO_MAJOR_VERSION 1)
set(PQCTIO_MINOR_VERSION 0)
set(PQCTIO_PATCH_VERSION 0)
set(PQCTIO_VERSION
  ${PQCTIO_MAJOR_VERSION}.${PQCTIO_MINOR_VERSION}.${PQCTIO_PATCH_VERSION})

# === Standard CMake options

option (BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Ensure that CMake behaves predictably
set (CMAKE_EXPORT_NO_PACKAGE_REGISTRY ON)
set (CMAKE_FIND_PACKAGE_NO_PACKAGE_REGISTRY ON)
set (CMAKE_FIND_PACKAGE_NO_SYSTEM_PACKAGE_REGISTRY ON)

# === Source code files

set (SRC
  source/pQCTIO.cxx)

# === Testing

option (ENABLE_TESTING "Build unit and other tests" OFF)
if (ENABLE_TESTING)
    enable_testing()
    find_package (GTest REQUIRED)
endif (ENABLE_TESTING)

# == Dependencies

find_package (Boost 1.57.0 COMPONENTS filesystem system REQUIRED)
if (MSVC)
  add_definitions (-D_CRT_SECURE_NO_WARNINGS)
endif (MSVC)
if (WIN32)
  # Disable auto-linking
  add_definitions (-DBOOST_ALL_NO_LIB)
endif()

include_directories (${Boost_INCLUDE_DIR})

# == Create library

include_directories (${CMAKE_BINARY_DIR})  # for pqctio_export.h
include_directories (${CMAKE_SOURCE_DIR}/include)
include (GenerateExportHeader)

add_library (pQCTIO ${SRC})
set_target_properties(pQCTIO
  PROPERTIES
    VERSION ${PQCTIO_VERSION}
    SOVERSION ${PQCTIO_MAJOR_VERSION}.${PQCTIO_MINOR_VERSION})

generate_export_header (pQCTIO)

target_link_libraries (pQCTIO
	PUBLIC
		Boost::filesystem Boost::system
)

# === Install

install (TARGETS pQCTIO
         EXPORT pQCTIOTargets
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)

install (DIRECTORY "${CMAKE_SOURCE_DIR}/include/pQCTIO" DESTINATION include)
install(FILES ${PROJECT_BINARY_DIR}/pqctio_export.h DESTINATION include/pQCTIO)

# === Testing

if (ENABLE_TESTING)
  add_subdirectory (tests)
endif (ENABLE_TESTING)

# === CMake config files for importing this project

# Set some locations for the install
set(INSTALL_LIB_DIR lib)
set(INSTALL_BIN_DIR bin)
set(INSTALL_INCLUDE_DIR include)
if(WIN32 AND NOT CYGWIN)
    set(INSTALL_CMAKE_DIR CMake)
else()
    set(INSTALL_CMAKE_DIR "lib/cmake/pQCTIO-${PQCTIO_MAJOR_VERSION}.${PQCTIO_MINOR_VERSION}")
endif()

# Make relative paths absolute
foreach(p LIB BIN INCLUDE CMAKE)
    set(var INSTALL_${p}_DIR)
    if(NOT IS_ABSOLUTE "${${var}}")
      set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
    endif()
endforeach()

# Add all targets to the build-tree export set
export (TARGETS pQCTIO
    NAMESPACE pQCTIO::
    FILE "${PROJECT_BINARY_DIR}/pQCTIOTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE pQCTIO)

# Create the pQCTIOConfig.cmake and pQCTIOConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_BINARY_DIR}")
configure_file(pQCTIOConfig.cmake.in
    "${PROJECT_BINARY_DIR}/pQCTIOConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${PQCTIO_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(pQCTIOConfig.cmake.in
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/pQCTIOConfig.cmake" @ONLY)
# ... for both
configure_file(pQCTIOConfigVersion.cmake.in
    "${PROJECT_BINARY_DIR}/pQCTIOConfigVersion.cmake" @ONLY)

# Install the pQCTIOConfig.cmake and pQCTIOConfigVersion.cmake
install(FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/pQCTIOConfig.cmake"
    "${PROJECT_BINARY_DIR}/pQCTIOConfigVersion.cmake"
    DESTINATION "${INSTALL_CMAKE_DIR}")

# Install the export set for use with the install-tree
install(EXPORT pQCTIOTargets
	NAMESPACE
		pQCTIO::
	DESTINATION
  	"${INSTALL_CMAKE_DIR}"
)
