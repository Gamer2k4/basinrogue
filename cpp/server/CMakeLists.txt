FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(SDL_net REQUIRED)
INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR} ${SDLNET_INCLUDE_DIR} ../network)
LINK_LIBRARIES(${SDL_LIBRARY} ${SDLNET_LIBRARY} ${SDLMAIN_LIBRARY})

SET(source_files main level tile player mobile sound ../network/networkcommandbuffer)
# SET_SOURCE_FILES_PROPERTIES(${source_files} PROPERTIES COMPILE_FLAGS "-g -W -Wall")
ADD_EXECUTABLE(brserver ${source_files})
