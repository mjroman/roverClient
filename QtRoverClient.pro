CONFIG += qt
DESTDIR = build
# QT-= gui to build project without gui library
QT += opengl # add network, xml, xmlpatterns for more qt modules
QT += network

TARGET = QtRoverClient
CONFIG(debug, debug|release){
	mac: TARGET = $$join(TARGET,,,_debug)
}

unix:QMAKE_DEL_FILE=rm -rf
 
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
UI_DIR = build/ui
RCC_DIR = build/rcc
 
QMAKE_DISTCLEAN += build/obj build/moc build/ui build/rcc build

TEMPLATE = app \
    /src
INCLUDEPATH += . \
    src
SOURCES += main.cpp \
    src/clientGUI.cpp 

HEADERS += src/clientGUI.h 

FORMS += src/clientGUI.ui
# LIBS += 
# RESOURCES += images.qrc
RC_FILE = RedSatellite.icns
