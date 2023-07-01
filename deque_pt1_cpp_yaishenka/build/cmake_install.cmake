# Install script for directory: /home/katerina/mipt/4 семестр/Cpp/cpp_course22_Ekaterina_Prohorchuk_2022/deque_pt1_cpp_yaishenka

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/katerina/mipt/4 семестр/Cpp/cpp_course22_Ekaterina_Prohorchuk_2022/deque_pt1_cpp_yaishenka/bin/deque_cpp_yaishenka" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/katerina/mipt/4 семестр/Cpp/cpp_course22_Ekaterina_Prohorchuk_2022/deque_pt1_cpp_yaishenka/bin/deque_cpp_yaishenka")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/katerina/mipt/4 семестр/Cpp/cpp_course22_Ekaterina_Prohorchuk_2022/deque_pt1_cpp_yaishenka/bin/deque_cpp_yaishenka"
         RPATH "/home/katerina/mipt/4 семестр/Cpp/cpp_course22_Ekaterina_Prohorchuk_2022/deque_pt1_cpp_yaishenka/bin")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/katerina/mipt/4 семестр/Cpp/cpp_course22_Ekaterina_Prohorchuk_2022/deque_pt1_cpp_yaishenka/bin/deque_cpp_yaishenka")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/home/katerina/mipt/4 семестр/Cpp/cpp_course22_Ekaterina_Prohorchuk_2022/deque_pt1_cpp_yaishenka/bin" TYPE EXECUTABLE FILES "/home/katerina/mipt/4 семестр/Cpp/cpp_course22_Ekaterina_Prohorchuk_2022/deque_pt1_cpp_yaishenka/build/deque_cpp_yaishenka")
  if(EXISTS "$ENV{DESTDIR}/home/katerina/mipt/4 семестр/Cpp/cpp_course22_Ekaterina_Prohorchuk_2022/deque_pt1_cpp_yaishenka/bin/deque_cpp_yaishenka" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/katerina/mipt/4 семестр/Cpp/cpp_course22_Ekaterina_Prohorchuk_2022/deque_pt1_cpp_yaishenka/bin/deque_cpp_yaishenka")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/home/katerina/mipt/4 семестр/Cpp/cpp_course22_Ekaterina_Prohorchuk_2022/deque_pt1_cpp_yaishenka/bin/deque_cpp_yaishenka"
         OLD_RPATH ":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
         NEW_RPATH "/home/katerina/mipt/4 семестр/Cpp/cpp_course22_Ekaterina_Prohorchuk_2022/deque_pt1_cpp_yaishenka/bin")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/katerina/mipt/4 семестр/Cpp/cpp_course22_Ekaterina_Prohorchuk_2022/deque_pt1_cpp_yaishenka/bin/deque_cpp_yaishenka")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/katerina/mipt/4 семестр/Cpp/cpp_course22_Ekaterina_Prohorchuk_2022/deque_pt1_cpp_yaishenka/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
