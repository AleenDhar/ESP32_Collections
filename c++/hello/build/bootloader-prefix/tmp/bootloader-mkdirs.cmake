# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/aleen/esp/v5.3.1/esp-idf/components/bootloader/subproject"
  "C:/Users/aleen/Desktop/ESP32/c++/hello/build/bootloader"
  "C:/Users/aleen/Desktop/ESP32/c++/hello/build/bootloader-prefix"
  "C:/Users/aleen/Desktop/ESP32/c++/hello/build/bootloader-prefix/tmp"
  "C:/Users/aleen/Desktop/ESP32/c++/hello/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/aleen/Desktop/ESP32/c++/hello/build/bootloader-prefix/src"
  "C:/Users/aleen/Desktop/ESP32/c++/hello/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/aleen/Desktop/ESP32/c++/hello/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/aleen/Desktop/ESP32/c++/hello/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
