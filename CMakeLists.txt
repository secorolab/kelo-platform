cmake_minimum_required(VERSION 3.15)
project(kelo_platform VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INSTALL_DIR share/${PROJECT_NAME}/cmake)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/../build/)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/../build/)

LINK_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/lib/)

find_package(soem REQUIRED)

list(APPEND local_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include/)

include_directories(
  include
  ${local_INCLUDE_DIRS}
)

add_library(ethercat_comm SHARED
  src/EthercatCommunication.c
)
target_include_directories(ethercat_comm PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(ethercat_comm
  soem
  pthread
)

add_library(kelo_drive_comm SHARED
  src/KeloDriveCommunication.c
)
target_include_directories(kelo_drive_comm PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(kelo_drive_comm
  ethercat_comm
  m
)

add_library(kelo_pmu SHARED
  src/PowerManagementUnit.c
)
target_include_directories(kelo_pmu PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(kelo_pmu
  ethercat_comm
)

add_library(kelo_mediator SHARED
  src/Mediator.c
)
target_include_directories(kelo_mediator PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(kelo_mediator
  ethercat_comm
  kelo_drive_comm
  kelo_pmu
)

install(
  TARGETS ethercat_comm kelo_drive_comm kelo_pmu kelo_mediator
  EXPORT ${PROJECT_NAME}-targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Generate the CMake config file
include(CMakePackageConfigHelpers)
# Generate the version file accompanying the project's config file
configure_package_config_file(${PROJECT_NAME}-config.cmake.in
                              ${PROJECT_NAME}-config.cmake
                              INSTALL_DESTINATION ${CMAKE_INSTALL_DIR})

write_basic_package_version_file(${PROJECT_NAME}-config-version.cmake
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY AnyNewerVersion)

# Install the config and configversion
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_DIR})

# Make the targets accessible from this packages's build tree
export(
  EXPORT ${PROJECT_NAME}-targets
  NAMESPACE ${PROJECT_NAME}::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-targets.cmake"
)

# Make the targets accessible from this packages's install tree
install(
  EXPORT ${PROJECT_NAME}-targets
  FILE ${PROJECT_NAME}-targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${CMAKE_INSTALL_DIR})

install(DIRECTORY include/
  DESTINATION include
  FILES_MATCHING PATTERN "*.h"
)

add_subdirectory(src/example)