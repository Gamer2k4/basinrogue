FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(SDL_net REQUIRED)
INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR} ${SDLNET_INCLUDE_DIR} ../network ../lua)
LINK_LIBRARIES(${SDL_LIBRARY} ${SDLNET_LIBRARY} ${SDLMAIN_LIBRARY} lua)

FILE(GLOB source_files "*.cpp")
FILE(GLOB network_files "../network/*.cpp")

ADD_EXECUTABLE(brserver ${source_files} ${network_files})

