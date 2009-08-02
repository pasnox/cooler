###########################################################################################
##		Created using Monkey Studio v1.8.4.0b (1.8.4.0bsvn_release)
##
##	Author    : AZEVEDO Filipe aka Nox P@sNox <pasnox@gmail.com>
##	Project   : cooler
##	FileName  : cooler.pro
##	Date      : 2009-07-19T17:25:47
##	License   : GPL
##	Comment   : Creating using Monkey Studio IDE
##	Home Page   :
##
##	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################

XUPProjectSettings {
	EDITOR	= QMake
	EXECUTE_DEBUG	= cooler_debug
	QT_VERSION	= Qt System (4.5.0)
}

TEMPLATE	= app
LANGUAGE	= C++/Qt4
TARGET	= $$quote(cooler)
CONFIG	+= debug_and_release
BUILD_PATH	= ./build

CONFIG(debug, debug|release) {
	#Debug
	CONFIG	+= console
	unix:TARGET	= $$join(TARGET,,,_debug)
	else:TARGET	= $$join(TARGET,,,d)
	unix:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/debug/.obj/mac
	UI_DIR	= $${BUILD_PATH}/debug/.ui
	MOC_DIR	= $${BUILD_PATH}/debug/.moc
	RCC_DIR	= $${BUILD_PATH}/debug/.rcc
} else {
	#Release
	unix:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/unix
	win32:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/win32
	mac:OBJECTS_DIR	= $${BUILD_PATH}/release/.obj/mac
	UI_DIR	= $${BUILD_PATH}/release/.ui
	MOC_DIR	= $${BUILD_PATH}/release/.moc
	RCC_DIR	= $${BUILD_PATH}/release/.rcc
}

INCLUDEPATH	= . \
	src

FORMS	= src/UIMain.ui \
	src/editor/UIMapEditor.ui

HEADERS	= src/PlayerItem.h \
	src/BombInformations.h \
	src/UIMain.h \
	src/GraphicsView.h \
	src/MapItem.h \
	src/TilesManager.h \
	src/editor/TreeWidgetTiles.h \
	src/editor/GraphicsViewEditor.h \
	src/editor/MapEditorItem.h \
	src/editor/UIMapEditor.h \
	src/editor/TreeWidgetLayers.h \
	src/ObjectItem.h \
	src/Globals.h \
	src/BombItem.h \
	src/tile/PlayerTile.h \
	src/tile/ObjectTile.h \
	src/tile/BombTile.h

SOURCES	= src/main.cpp \
	src/PlayerItem.cpp \
	src/BombInformations.cpp \
	src/UIMain.cpp \
	src/GraphicsView.cpp \
	src/MapItem.cpp \
	src/TilesManager.cpp \
	src/editor/TreeWidgetTiles.cpp \
	src/editor/GraphicsViewEditor.cpp \
	src/editor/MapEditorItem.cpp \
	src/editor/UIMapEditor.cpp \
	src/editor/TreeWidgetLayers.cpp \
	src/ObjectItem.cpp \
	src/Globals.cpp \
	src/BombItem.cpp \
	src/tile/PlayerTile.cpp \
	src/tile/ObjectTile.cpp \
	src/tile/BombTile.cpp