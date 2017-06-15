project(update_region_data)

set(SRC update_region_data.cpp)

omim_add_executable(${PROJECT_NAME} ${SRC})

omim_link_libraries(
  ${PROJECT_NAME}
  generator
  routing
  traffic
  routing_common
  search
  storage
  indexer
  editor
  platform
  geometry
  coding
  base
  opening_hours
  freetype
  expat
  icu
  jansson
  minizip
  succinct
  pugixml
  ${LIBZ}
)

link_qt5_core(${PROJECT_NAME})
