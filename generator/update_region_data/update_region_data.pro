# Generator binary

ROOT_DIR = ../..

DEPENDENCIES = generator routing traffic routing_common search storage indexer editor platform geometry \
               coding base freetype expat jansson protobuf \
               minizip succinct pugixml icu stats_client
include($$ROOT_DIR/common.pri)

CONFIG += console warn_on
CONFIG -= app_bundle
TEMPLATE = app

# needed for Platform::WorkingDir() and unicode combining
QT *= core

macx-* {
  LIBS *= "-framework IOKit" "-framework SystemConfiguration"
}

SOURCES += \
    update_region_data.cpp \

HEADERS += \
