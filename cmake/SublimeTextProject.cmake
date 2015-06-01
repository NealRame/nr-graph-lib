if(__generate_sublime_text_project)
  return()
endif()

set(__generate_sublime_text_project YES)

function(generate_sublime_text_project _cxx_flags _definitions _include_directories)

  if(APPLE)
    set(_include_directories
      # "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/c++/v1"
      # "/usr/local/include"
      # "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/5.0/include"
      # "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include"
      # "/usr/include"
      # "/System/Library/Frameworks"
      # "/Library/Frameworks"
      "/Library/Developer/CommandLineTools/usr/include/c++/v1/"
      ${_include_directories}
    )
  endif()

  # message("-- _include_directories: ${_include_directories}")
  # message("-- _definitions: ${_definitions}")
  # message("-- _cxx_flag: ${_cxx_flags}")

  foreach(DIR ${_include_directories})
    if(SUBLIBME_CLANG_FLAGS)
      set(SUBLIBME_CLANG_FLAGS "${SUBLIBME_CLANG_FLAGS}, \"-I${DIR}\"")
    else()
      set(SUBLIBME_CLANG_FLAGS "\"-I${DIR}\"")
    endif()
  endforeach()

  foreach(DEF ${_definitions})
      if (SUBLIBME_CLANG_FLAGS)
        set(SUBLIBME_CLANG_FLAGS "${SUBLIBME_CLANG_FLAGS}, \"-D${DEF}\"")
      else()
        set(SUBLIBME_CLANG_FLAGS "\"-D${DEF}\"")
      endif()
  endforeach()

  foreach(FLAG ${_cxx_flags})
    if (CXX_FLAGS)
      set(CXX_FLAGS "${CXX_FLAGS}, \"${FLAG}\"")
    else()
      set(CXX_FLAGS "\"${FLAG}\"")
    endif()
  endforeach()

  message("-- SUBLIBME_CLANG_FLAGS: ${SUBLIBME_CLANG_FLAGS}")
  message("-- CXX_FLAGS: ${CXX_FLAGS}")

  configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/SublimeTextProject.cmake.in
    ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.sublime-project
  )
endfunction()