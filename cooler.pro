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
	QT_VERSION	= Qt System (4.5.2)
	OTHERS_PLATFORM_TARGET_DEBUG	= cooler_debug
	OTHERS_PLATFORM_TARGET_RELEASE	= cooler
}

TEMPLATE	= app
LANGUAGE	= C++/Qt4
TARGET	= $$quote(cooler)
CONFIG	+= debug_and_release
QT	= core gui opengl xml
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
	src \
	src/engine/states \
	src/engine/states/gameintroduction \
	src/engine/states/gamemode \
	src/engine/states/padsettings \
	src/engine/states/multiplayerchoice \
	src/engine/states/multigameplay \
	src/engine/states/multiplayersetup \
	src/engine/states/multimapchoice \
	src/engine/states/multiplayground \
	src/engine/items \
	src/engine/menus \
	src/engine/settings \
	src/engine/tiles \
	src/engine

#	src/obsolete/editor \
#	src/obsolete/pads \
#	src/obsolete/tilesripper \
#	src/obsolete \

#FORMS	= src/obsolete/editor/UIMapEditor.ui \
#	src/obsolete/pads/UIPadConfiguration.ui \
#	src/obsolete/pads/UIPadSettings.ui \
#	src/obsolete/tilesripper/UITilesRipper.ui \
#	src/obsolete/UIMain.ui

HEADERS	= src/engine/GameEngine.h \
	src/engine/Globals.h \
	src/engine/PadSettings.h \
	src/engine/Player.h \
	src/engine/states/AbstractGameState.h \
	src/engine/menus/GSBattleItem.h \
	src/engine/menus/GSCOMLevelItem.h \
	src/engine/menus/GSFaceItem.h \
	src/engine/menus/GSGenericStateItem.h \
	src/engine/menus/GSMapItem.h \
	src/engine/menus/GSMenu.h \
	src/engine/menus/GSMenuItem.h \
	src/engine/menus/GSOnOffItem.h \
	src/engine/menus/GSStateItem.h \
	src/engine/menus/GSTimeItem.h \
	src/engine/settings/pXmlSettings.h \
	src/engine/tiles/AbstractTile.h \
	src/engine/tiles/BombTile.h \
	src/engine/tiles/ObjectTile.h \
	src/engine/tiles/PlayerTile.h \
	src/engine/tiles/TilesManager.h \
	src/engine/items/BombItem.h \
	src/engine/items/MapObjectItem.h \
	src/engine/items/PlayerItem.h \
	src/engine/states/gameintroduction/GSIntroduction.h \
	src/engine/states/gamemode/GSMode.h \
	src/engine/states/multigameplay/GSMultiGamePlay.h \
	src/engine/states/multimapchoice/GSMultiMapChoice.h \
	src/engine/states/multiplayerchoice/GSMultiPlayerChoice.h \
	src/engine/states/multiplayersetup/GSMultiPlayerSetup.h \
	src/engine/states/multiplayground/GSMultiPlayGround.h \
	src/engine/states/padsettings/GSPadSettings.h \
	src/engine/Map.h \
	src/engine/MapItem.h

#	src/obsolete/editor/GraphicsViewEditor.h \
#	src/obsolete/editor/MapEditorItem.h \
#	src/obsolete/editor/TreeWidgetLayers.h \
#	src/obsolete/editor/TreeWidgetTiles.h \
#	src/obsolete/editor/UIMapEditor.h \
#	src/obsolete/pads/UIPadConfiguration.h \
#	src/obsolete/pads/UIPadSettings.h \
#	src/obsolete/tilesripper/TilesRipperPreview.h \
#	src/obsolete/tilesripper/UITilesRipper.h \
#	src/obsolete/BombInformations.h \
#	src/obsolete/GraphicsView.h \
#	src/obsolete/UIMain.h \

SOURCES	= src/engine/GameEngine.cpp \
	src/engine/Globals.cpp \
	src/engine/PadSettings.cpp \
	src/engine/Player.cpp \
	src/engine/menus/GSBattleItem.cpp \
	src/engine/menus/GSCOMLevelItem.cpp \
	src/engine/menus/GSFaceItem.cpp \
	src/engine/menus/GSGenericStateItem.cpp \
	src/engine/menus/GSMapItem.cpp \
	src/engine/menus/GSMenu.cpp \
	src/engine/menus/GSMenuItem.cpp \
	src/engine/menus/GSOnOffItem.cpp \
	src/engine/menus/GSStateItem.cpp \
	src/engine/menus/GSTimeItem.cpp \
	src/engine/settings/pXmlSettings.cpp \
	src/engine/tiles/AbstractTile.cpp \
	src/engine/tiles/BombTile.cpp \
	src/engine/tiles/ObjectTile.cpp \
	src/engine/tiles/PlayerTile.cpp \
	src/engine/tiles/TilesManager.cpp \
	src/engine/items/BombItem.cpp \
	src/engine/items/MapObjectItem.cpp \
	src/engine/items/PlayerItem.cpp \
	src/engine/states/gameintroduction/GSIntroduction.cpp \
	src/engine/states/gamemode/GSMode.cpp \
	src/engine/states/multigameplay/GSMultiGamePlay.cpp \
	src/engine/states/multimapchoice/GSMultiMapChoice.cpp \
	src/engine/states/multiplayerchoice/GSMultiPlayerChoice.cpp \
	src/engine/states/multiplayersetup/GSMultiPlayerSetup.cpp \
	src/engine/states/multiplayground/GSMultiPlayGround.cpp \
	src/engine/states/padsettings/GSPadSettings.cpp \
	src/engine/Map.cpp \
	src/engine/MapItem.cpp \
	src/main.cpp

#	src/obsolete/editor/GraphicsViewEditor.cpp \
#	src/obsolete/editor/MapEditorItem.cpp \
#	src/obsolete/editor/TreeWidgetLayers.cpp \
#	src/obsolete/editor/TreeWidgetTiles.cpp \
#	src/obsolete/editor/UIMapEditor.cpp \
#	src/obsolete/pads/UIPadConfiguration.cpp \
#	src/obsolete/pads/UIPadSettings.cpp \
#	src/obsolete/tilesripper/TilesRipperPreview.cpp \
#	src/obsolete/tilesripper/UITilesRipper.cpp \
#	src/obsolete/GraphicsView.cpp \
#	src/obsolete/UIMain.cpp \
#	src/obsolete/BombInformations.cpp \