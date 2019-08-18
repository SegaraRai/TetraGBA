cmake_minimum_required(VERSION 3.12)

set(TARGET_NAME final)

project(${TARGET_NAME} NONE)

set(ROOT /usr/local/gcc-arm-none-eabi-8-2019-q3-update)
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})
set(APP_DIR ${SRC_DIR}/app)
set(GBA_DIR ${SRC_DIR}/gba)
set(RES_DIR ${SRC_DIR}/resources)

FILE(GLOB_RECURSE GBA_SOURCES ${GBA_DIR}/*.cpp)

set(CMAKE_C_COMPILER   ${ROOT}/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${ROOT}/bin/arm-none-eabi-g++)

set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# skip compiler check (especially about linking)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_SYSROOT ${ROOT}/arm-none-eabi)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE NEVER)

set(LINKER_SCRIPT ${GBA_DIR}/gba.ls)


enable_language(C)
enable_language(CXX)

# set -DGBA_USE_CRT if use crt
set(CUSTOM_COMMON_FLAGS "")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -g3")
#set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -Wl,-verbose")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -Wl,-gc-sections")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -Wl,-Map=${TARGET_NAME}.map")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -Wl,-T${LINKER_SCRIPT}")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -Wall -Wextra -Weffc++")
#set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -Os")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -O3")
#set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -flto")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -nostdlib")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -static")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -mthumb-interwork -mcpu=arm7tdmi -mlittle-endian -mfloat-abi=soft")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -mthumb")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -mtune=arm7tdmi")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -fdata-sections -ffunction-sections")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -fmerge-all-constants")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -fno-ident -fomit-frame-pointer")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -finline-small-functions -findirect-inlining")
set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -DNDEBUG")

if (RELEASE_BUILD)
  set(CUSTOM_COMMON_FLAGS "${CUSTOM_COMMON_FLAGS} -DRELEASE_BUILD")
endif()

set(CMAKE_C_FLAGS "${CUSTOM_COMMON_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

set(CMAKE_CXX_FLAGS "${CUSTOM_COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-non-call-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")


FILE(GLOB_RECURSE APP_SOURCES ${APP_DIR}/*.cpp ${APP_DIR}/*.c)

set(IMAGE_HEADERS
  ${RES_DIR}/image/bg.hpp
  ${RES_DIR}/image/bg_background.hpp
  ${RES_DIR}/image/obj.hpp
)

FILE(GLOB SONG_RESOURCES ${RES_DIR}/song/*.mid)
set(SONG_SOURCES ${SONG_RESOURCES})
list(TRANSFORM SONG_SOURCES REPLACE \.mid$ .cpp)

FILE(GLOB SOUND_RESOURCES ${RES_DIR}/sound/*.wav)
set(SOUND_SOURCES ${SOUND_RESOURCES})
list(TRANSFORM SOUND_SOURCES REPLACE \.wav$ .cpp)


add_executable(${TARGET_NAME}
  ${APP_SOURCES}
  ${GBA_SOURCES}
  ${SONG_SOURCES}
  ${SOUND_SOURCES}
)

target_include_directories(${TARGET_NAME}
  PRIVATE ${GBA_DIR}
  PRIVATE ${RES_DIR}
)

target_link_libraries(${TARGET_NAME} -lstdc++_nano -lm -lc_nano -lnosys -lgcc)

set_target_properties(${TARGET_NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

add_custom_command(TARGET ${TARGET_NAME}
  POST_BUILD
  COMMAND ${ROOT}/bin/arm-none-eabi-objdump -C -S -d "$<TARGET_FILE_NAME:${TARGET_NAME}>" > "$<TARGET_FILE_NAME:${TARGET_NAME}>_dump_src.txt"
  COMMAND ${ROOT}/bin/arm-none-eabi-objdump -C -d "$<TARGET_FILE_NAME:${TARGET_NAME}>" > "$<TARGET_FILE_NAME:${TARGET_NAME}>_dump_s.txt"
  COMMAND ${ROOT}/bin/arm-none-eabi-objdump -C -x "$<TARGET_FILE_NAME:${TARGET_NAME}>" > "$<TARGET_FILE_NAME:${TARGET_NAME}>_headers.txt"
  COMMAND ${ROOT}/bin/arm-none-eabi-objcopy -O binary "$<TARGET_FILE_NAME:${TARGET_NAME}>" "$<TARGET_FILE_NAME:${TARGET_NAME}>.bin"
  COMMAND ${ROOT}/bin/arm-none-eabi-objdump -D -b binary -marm "$<TARGET_FILE_NAME:${TARGET_NAME}>.bin" > "$<TARGET_FILE_NAME:${TARGET_NAME}>_dump.txt"
  COMMAND ${ROOT}/bin/arm-none-eabi-objcopy -S "$<TARGET_FILE_NAME:${TARGET_NAME}>" "$<TARGET_FILE_NAME:${TARGET_NAME}>.elf"
  COMMAND cp "$<TARGET_FILE_NAME:${TARGET_NAME}>.bin" "$<TARGET_FILE_NAME:${TARGET_NAME}>.mb"
  COMMAND du -bh "$<TARGET_FILE_NAME:${TARGET_NAME}>.bin"
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
)


# resources/image

function(add_image_command NAME TYPE)
  # ARGN: resources

  set(FULLPATH_RESOURCES ${ARGN})
  list(TRANSFORM FULLPATH_RESOURCES PREPEND ${RES_DIR}/image/)

  set(PNG_RESOURCES ${FULLPATH_RESOURCES})
  list(TRANSFORM PNG_RESOURCES APPEND .png)

  set(CSV_RESOURCES ${FULLPATH_RESOURCES})
  list(TRANSFORM CSV_RESOURCES APPEND .csv)

  add_custom_command(
    OUTPUT ${RES_DIR}/image/${NAME}.hpp
    WORKING_DIRECTORY ${RES_DIR}/image
    COMMAND node index.js ${NAME} ${TYPE} ${PNG_RESOURCES}
    DEPENDS
      ${RES_DIR}/image/index.js
      ${PNG_RESOURCES}
      ${CSV_RESOURCES}
  )
endfunction()

add_image_command(bg bg
  title
  frame
  pause
  unscii-8
  text
  block
)

add_image_command(bg_background bg
  background_gray
  background_flame
)

add_image_command(obj obj
  empty64x64
  effect
)

add_custom_target(${TARGET_NAME}_resource_image
  SOURCES
    ${IMAGE_HEADERS}
)

add_dependencies(${TARGET_NAME} ${TARGET_NAME}_resource_image)


# resources/song

foreach(SONG_RESOURCE ${SONG_RESOURCES})
  string(REGEX REPLACE \.mid$ .cpp SONG_SOURCE ${SONG_RESOURCE})

  add_custom_command(
    OUTPUT ${SONG_SOURCE}
    WORKING_DIRECTORY ${RES_DIR}/song
    COMMAND node index.js ${SONG_RESOURCE}
    DEPENDS
      ${RES_DIR}/song/index.js
      ${RES_DIR}/song/note.js
      ${RES_DIR}/song/register.js
      ${RES_DIR}/song/waveram.js
      ${SONG_RESOURCE}
  )
endforeach(SONG_RESOURCE)

add_custom_target(${TARGET_NAME}_resource_song
  SOURCES
    ${SONG_SOURCES}
)

add_dependencies(${TARGET_NAME} ${TARGET_NAME}_resource_song)


# resources/sound

math(EXPR SOUND_SAMPLING_RATE "32768 / 2" OUTPUT_FORMAT DECIMAL)

foreach(SOUND_RESOURCE ${SOUND_RESOURCES})
  string(REGEX REPLACE \.wav$ .cpp SOUND_SOURCE ${SOUND_RESOURCE})

  add_custom_command(
    OUTPUT ${SOUND_SOURCE}
    WORKING_DIRECTORY ${RES_DIR}/sound
    COMMAND node index.js ${SOUND_RESOURCE} ${SOUND_SAMPLING_RATE}
    DEPENDS
      ${RES_DIR}/sound/index.js
      ${SOUND_RESOURCE}
  )
endforeach(SOUND_RESOURCE)

add_custom_target(${TARGET_NAME}_resource_sound
  SOURCES
    ${SOUND_SOURCES}
)

add_dependencies(${TARGET_NAME} ${TARGET_NAME}_resource_sound)
