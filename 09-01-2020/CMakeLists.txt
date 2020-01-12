set(CMAKE_CXX_STANDARD 17)
   
#include(GenerateExportHeader)

#SET(SOURCES
#src/main.cpp
#src/poligon_draw.h
#)

#add_library(radar_display_window STATIC ${SOURCES})

#target_include_directories(poligon_example/src)


ADD_EXECUTABLE(example_poligon src/main.cpp
  #    src/vidget.cpp
#    src/scene.cpp
    src/main_widget.cpp  src/_main.h src/draw.cpp src/form.ui
    )

target_link_libraries(example_poligon PUBLIC libpqxx
        Qt5::Core
        Qt5::Widgets
#        # CONFIG
)


