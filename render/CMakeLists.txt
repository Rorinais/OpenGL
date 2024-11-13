file(GLOB_RECURSE RENDER ./  *.cpp  *.c)

add_library(render_lib  ${RENDER})