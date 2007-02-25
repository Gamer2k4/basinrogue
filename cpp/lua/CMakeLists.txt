
FILE(GLOB source_files "*.cpp")

ADD_LIBRARY(lua ${source_files} "lua.hpp" "lunar.hpp")

