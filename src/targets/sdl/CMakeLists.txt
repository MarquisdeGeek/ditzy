cmake_minimum_required(VERSION 3.7)
project (zx81 VERSION 0.1.0 LANGUAGES C CXX)

# The name 'Ditzy Chess' came much later and, because I like the binaries
# appearing at the end of ls (so I can check datestamps), the target
# remains 'misnamed'
set (EMU_TARGET zx81)
set (EMU_TARGET_CHESS zx81_chess)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_BUILD_TYPE Debug)

set (EMF_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)

find_package(SDL2 REQUIRED)


# A general-purpose build method, allowing for binaries
# with alternate settings
function(add_sdl_target TARGET_NAME)

    add_executable(${TARGET_NAME})

    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${EMF_ROOT}"
    )


    target_include_directories (${TARGET_NAME} PUBLIC
        ${SDL2_INCLUDE_DIRS}
        ${EMF_ROOT}
    )


    target_sources (${TARGET_NAME}
    PUBLIC
    ${EMF_ROOT}/targets/common/cpp/click_interface.cpp
    ${EMF_ROOT}/targets/common/cpp/screen.cpp
    ${EMF_ROOT}/targets/sdl/sdl_main.cpp
    ${EMF_ROOT}/targets/sdl/sdl_screen.cpp
    ${EMF_ROOT}/targets/sdl/sdl_update.cpp

    ${EMF_ROOT}/platforms/zx81/1k/cpp/zxmemory.cpp
    ${EMF_ROOT}/platforms/zx81/common/cpp/zxkeyboard.cpp
    ${EMF_ROOT}/platforms/zx81/common/cpp/zxio.cpp

    ${EMF_ROOT}/emulator/z80/c/zxccpu.c
    ${EMF_ROOT}/platforms/zx81/common/c/zxcio.c
    ${EMF_ROOT}/platforms/zx81/1k/c/mem1k.c
    ${EMF_ROOT}/platforms/zx81/common/zxmemory.c
    ${EMF_ROOT}/platforms/zx81/common/c/zxckeyboard.c

    ${EMF_ROOT}/framework/comparator.c
    )

    target_link_libraries(${TARGET_NAME} ${SDL2_LIBRARIES})

endfunction()


# Start building various versions of the game
add_sdl_target("${EMU_TARGET_CHESS}_comparator")

target_compile_definitions("${EMU_TARGET_CHESS}_comparator"
PUBLIC
CHESS_ONLY_OPTIMISATIONS=1

EMF_INC_EMU_C=1
EMF_INC_EMU_CPP=1
EMF_DUAL_EMULATION=1
EMF_FIRST_EMULATOR_CPP=1
EMF_CPU_DEBUG_STATE=0
)


# C version, as used on the Dragon
add_sdl_target("${EMU_TARGET_CHESS}_c")

target_compile_definitions("${EMU_TARGET_CHESS}_c"
PUBLIC
CHESS_ONLY_OPTIMISATIONS=1

EMF_INC_EMU_C=1
EMF_SINGLE_EMULATOR_C=1
)


# CPP version, maybe it's own project ;)
add_sdl_target("${EMU_TARGET_CHESS}_cpp")

target_compile_definitions("${EMU_TARGET_CHESS}_cpp"
PUBLIC
CHESS_ONLY_OPTIMISATIONS=1

EMF_INC_EMU_CPP=1
EMF_SINGLE_EMULATOR_CPP=1
)


# Full-on debug, for development
add_sdl_target("${EMU_TARGET_CHESS}_debug")

target_compile_definitions("${EMU_TARGET_CHESS}_debug"
PUBLIC
CHESS_ONLY_OPTIMISATIONS=1

EMF_INC_EMU_C=1
EMF_INC_EMU_CPP=1
EMF_DUAL_EMULATION=1
EMF_FIRST_EMULATOR_CPP=1
EMF_CPU_TRACER_PRINTF=0
EMF_CPU_DEBUG_STATE=1
EMF_COMPARATOR_BUFFER_SIZE=10
EMF_COMPARATOR_CPU_STATE=1
EMF_COMPARATOR_CPU_REPORTS=1
EMF_CPU_COMPARATOR_TRACE_1=0
EMF_CPU_COMPARATOR_TRACE_2=0
)


# ZX81 BASIC interpreter
add_sdl_target("${EMU_TARGET}_cpp")

target_compile_definitions("${EMU_TARGET}_cpp"
PUBLIC
EMF_INC_EMU_CPP=1
EMF_SINGLE_EMULATOR_CPP=1
)


# Brief notes on definitions:
# EMF_CPU_DEBUG_STATE = include the CPU getState method(s) in code. Needed for comparison checking
# EMF_COMPARATOR_BUFFER_SIZE = store this many comparison strings, to be dumped upon differences detected
# EMF_CPU_COMPARATOR_TRACE_1 = Output the CPU state each frame. Can generate super-large files, so EMF_COMPARATOR_BUFFER_SIZE is often preferred
