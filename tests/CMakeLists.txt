# === Fetch the external test data ===

include (ExternalData)

set (ExternalData_URL_TEMPLATES "http://numerics88.com/data/%(algo)/%(hash)")

ExternalData_Expand_Arguments(
  fetch_data_tarball
  DATATARBALL
  DATA{${CMAKE_SOURCE_DIR}/data/pqctio_testdata.tar.gz})
ExternalData_Add_Target (fetch_data_tarball)

# I0020027.M01 is just a random choice of one of the unpacked data files.
add_custom_command (OUTPUT ${CMAKE_BINARY_DIR}/data/I0020027.M01
  COMMAND ${CMAKE_COMMAND} -E tar "xvzf" "pqctio_testdata.tar.gz"
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/data
  DEPENDS fetch_data_tarball)
add_custom_target (test_data
  DEPENDS ${CMAKE_BINARY_DIR}/data/I0020027.M01)

# === Actual tests (single google test binary) ===

include_directories ("${GTEST_INCLUDE_DIR}")
include_directories (${Boost_INCLUDE_DIR})

add_executable (pQCTIOTests
  pQCTIOTests.cxx)

target_link_libraries (pQCTIOTests
	pQCTIO
  ${GTEST_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY})

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_link_libraries (pQCTIOTests pthread)
endif (CMAKE_SYSTEM_NAME STREQUAL "Linux")

add_test (
  NAME pQCTIOTests
  COMMAND $<TARGET_FILE:pQCTIOTests> "${data_dir}"
)

add_dependencies (pQCTIOTests test_data)
