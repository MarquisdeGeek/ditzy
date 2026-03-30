cmake_minimum_required(VERSION 3.7)
project (zx81_d32 VERSION 0.1.0 LANGUAGES C)


set(EMU_TARGET_D32 zx1kmain)
set(EMF_ROOT ${CMAKE_CURRENT_LIST_DIR}/../..)
set(CMAKE_EXECUTABLE_SUFFIX ".bin")

# Remove this for tidier tree
set(CMOC_LEAVE_INTERMEDIATE 0)


# Define your source files
set(SOURCES 
${EMF_ROOT}/targets/d64/d64.c
${EMF_ROOT}/targets/d64/d64cmain.c
${EMF_ROOT}/targets/d64/d64zxcscreen.c

${EMF_ROOT}/emulator/z80/c/zxccpu.c
${EMF_ROOT}/platforms/zx81/1k/c/mem1k.c
${EMF_ROOT}/platforms/zx81/common/zxmemory.c
${EMF_ROOT}/platforms/zx81/common/c/zxcio.c
${EMF_ROOT}/platforms/zx81/common/c/zxckeyboard.c
)


# We ensue the usual cmake trappings to simply execute this:
# 	cmoc --dragon -DD32_COMPILE_WITH_STDLIB=0 -DEMF_CPU_TRACER_SECRET=0 -DEMF_CPU_DEBUG_STATE=0 -DEMF_DEBUG_KEYPRESSES=1 -Wgives-byte -Wsign-compare -Wshadow --limit=Bf90 -i -DMACHINE=DRAGON -o main.bin zxcmain.c zxcmemory.c zxcscreen.c zxccpu.c zxcio.c zxckeyboard.c ../mandelbrot/d32.c
# More cmoc flags at:
#   http://gvlsywt.cluster051.hosting.ovh.net/dev/cmoc.html

# (but split up the options into nice block...)
set(CMOC_COMPILE_WARNINGS
-Wgives-byte -Wsign-compare -Wshadow 
)

set(CMOC_COMPILE_SETTINGS
-DMACHINE=DRAGON
-DCHESS_ONLY_OPTIMISATIONS=1
-DD32_COMPILE_WITH_STDLIB=0
-DEMF_CPU_TRACER_SECRET=0
-DEMF_CPU_DEBUG_STATE=0
-DEMF_DEBUG_KEYPRESSES=1 
-DEMF_TIGHT_PRUNE=1
-DEMF_GRAPHICS_WITH_DIRTY_GRID=0
)


set(CMOC_COMPILE_BUILD
--dragon
--org=25D0      # 9680 - could push lower, but need to PCLEAR 4 before loading
# --org=3A98      # 15000
--limit=Bf90    # a guess at how much headroom we need, before hitting the ROM at C000
)


set(CMOC_COMPILE_OPTIONS
-I${EMF_ROOT}
${CMOC_COMPILE_WARNINGS}
${CMOC_COMPILE_SETTINGS}
${CMOC_COMPILE_BUILD}
)


if(CMOC_LEAVE_INTERMEDIATE)
  list(APPEND CMOC_COMPILE_OPTIONS -i)
endif()


set(CMOC_BINARY
${EMU_TARGET_D32}${CMAKE_EXECUTABLE_SUFFIX}
)


add_custom_command(
    OUTPUT ${CMOC_BINARY}
    COMMAND cmoc ${CMOC_COMPILE_OPTIONS} -o ${EMF_ROOT}/${CMOC_BINARY} ${SOURCES}
    DEPENDS ${SOURCES}
)


add_custom_target(dragon ALL
    DEPENDS ${CMOC_BINARY}
)
