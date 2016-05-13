# mapshot binary

ROOT_DIR = ..
DEPENDENCIES = map storage indexer platform geometry coding base succinct protobuf

include($$ROOT_DIR/common.pri)

CONFIG += console warn_on
CONFIG -= app_bundle
TEMPLATE = app

# needed for Platform::WorkingDir() and unicode combining
QT *= core

SOURCES += feature_coords.cpp
