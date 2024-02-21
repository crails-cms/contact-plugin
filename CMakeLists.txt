cmake_minimum_required(VERSION 3.5)

project(crails-cms-contact-plugin)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")

find_package(PkgConfig)
pkg_check_modules(CRAILS_CMS REQUIRED
  libcrails-cms>=2.0.0
  libcrails-templates>=2.0.0
  libcrails-json-views>=2.0.0
  libcrails-html-views>=2.0.0
  libcrails-mail>=2.0.0
)

include_directories(. ../.. ${CRAILS_CMS_INCLUDE_DIRS})
link_directories(../../build)

set(ENV{CRAILS_CMS_INCLUDE_DIRS} "${CRAILS_CMS_INCLUDE_DIRS}")
execute_process(COMMAND ./prebuild.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE plugin_files
  app/*.cpp app/*.cxx
  lib/*.cpp lib/*.cxx)

add_library(contact SHARED ${plugin_files})
add_definitions(-DWITH_ODB)
set_target_properties(contact PROPERTIES ENABLE_EXPORTS ON)
set_target_properties(contact PROPERTIES PREFIX "")
target_link_libraries(contact
  ${CRAILS_CMS_LIBRARIES}
  crypto ssl
  odb-pgsql
  crails-app)
install(TARGETS contact LIBRARY DESTINATION lib/libcrails-cms)
install(FILES contact.description.json contact.description.png DESTINATION lib/libcrails-cms)
install(FILES contact.fr.json contact.en.json contact.es.json DESTINATION locales)
