project(Box2D)


include_directories(./)

file(GLOB_RECURSE Box2D_SRC 
    *.h *.cpp)

add_library(Box2D STATIC ${Box2D_SRC})
set_property(TARGET Box2D PROPERTY CXX_STANDARD 11)
