# === Fetch the external test data ===

# Yes, I know. I should do a proper algorithm + data storage somewhere. I do not have access to the Numerics88
# website so this is the best I can do for now.

find_package(Subversion)

set(testing_url "https://github.com/Bonelab/BonelabData/trunk/data/")
set(data_dir "${CMAKE_BINARY_DIR}/data")

function(fetch_data DEST_DIR)
  # Gather all files passed through
  set(input_files ${ARGN})

  # Create the destination directory
  set( fetch_cmd COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_DIR} )

  # Get all files
  set(output_files "")
  foreach ( input_file ${input_files} )
    LIST( APPEND fetch_cmd COMMAND svn export ${testing_url}/${input_file} ${DEST_DIR}/${input_file})
    set(output_files ${output_files} ${DEST_DIR}/${input_file})
  endforeach( input_file ${targetsToCopy} )

  # Add all commands
  add_custom_command(
    OUTPUT ${output_files}
    PRE_BUILD ${fetch_cmd}
    COMMENT "Fetching data"
  )

  # Final target for linking data
  add_custom_target (
    testing_data
    DEPENDS ${output_files}
  )
endfunction(fetch_data)

fetch_data(
  ${data_dir}
  I0020027.M01
  I0020002.M01
)

# === Actual tests (single google test binary) ===

include_directories ("${GTEST_INCLUDE_DIR}")
include_directories (${Boost_INCLUDE_DIR})

add_executable (pQCTIOTests
  pQCTIOTests.cxx
  ../source/pQCTIO.cxx)

target_link_libraries (pQCTIOTests
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

add_dependencies (pQCTIOTests testing_data)
