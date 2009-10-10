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
	QT_VERSION	= Qt System (4.5.2)
	EXECUTE_RELEASE	= cooler
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
	src/editor \
	src/items \
	src/pads \
	src/reusable \
	src/tiles \
	src/tilesripper \
	src/engine \
	src/engine/menus

FORMS	= src/UIMain.ui \
	src/editor/UIMapEditor.ui \
	src/pads/UIPadSettings.ui \
	src/pads/UIPadConfiguration.ui \
	src/tilesripper/UITilesRipper.ui

HEADERS	= src/tiles/AbstractTile.h \
	src/tiles/BombTile.h \
	src/tiles/ObjectTile.h \
	src/tiles/PlayerTile.h \
	src/items/BombItem.h \
	src/items/PlayerItem.h \
	src/BombInformations.h \
	src/Globals.h \
	src/GraphicsView.h \
	src/MapItem.h \
	src/TilesManager.h \
	src/UIMain.h \
	src/editor/GraphicsViewEditor.h \
	src/editor/MapEditorItem.h \
	src/editor/TreeWidgetLayers.h \
	src/editor/TreeWidgetTiles.h \
	src/editor/UIMapEditor.h \
	src/items/MapObjectItem.h \
	src/pads/UIPadSettings.h \
	src/pads/PadSettings.h \
	src/pads/UIPadConfiguration.h \
	src/tilesripper/UITilesRipper.h \
	src/tilesripper/TilesRipperPreview.h \
	src/reusable/pXmlSettings.h \
	src/engine/states/AbstractGameState.h \
	src/engine/GameEngine.h \
	src/engine/states/gameintroduction/GSIntroduction.h \
	src/engine/states/gamemode/GSMode.h \
	src/engine/menus/GSMenuItem.h \
	src/engine/menus/GSMenu.h \
	src/engine/states/padsettings/GSPadSettings.h \
	src/engine/states/multiplayerchoice/GSMultiPlayerChoice.h \
	src/engine/menus/GSFaceItem.h \
	src/engine/Player.h \
	src/engine/menus/GSStateItem.h \
	src/engine/states/multigameplay/GSMultiGamePlay.h \
	src/engine/menus/GSGenericStateItem.h \
	src/engine/menus/GSCOMLevelItem.h \
	src/engine/menus/GSOnOffItem.h \
	src/engine/menus/GSBattleItem.h \
	src/engine/menus/GSTimeItem.h \
	src/engine/states/multiplayersetup/GSMultiPlayerSetup.h \
	src/engine/states/multimapchoice/GSMultiMapChoice.h \
	src/engine/menus/GSMapItem.h \
	src/engine/states/multiplayground/GSMultiPlayGround.h

SOURCES	= src/tiles/AbstractTile.cpp \
	src/tiles/BombTile.cpp \
	src/tiles/ObjectTile.cpp \
	src/tiles/PlayerTile.cpp \
	src/items/BombItem.cpp \
	src/items/PlayerItem.cpp \
	src/BombInformations.cpp \
	src/Globals.cpp \
	src/GraphicsView.cpp \
	src/main.cpp \
	src/MapItem.cpp \
	src/TilesManager.cpp \
	src/UIMain.cpp \
	src/editor/GraphicsViewEditor.cpp \
	src/editor/MapEditorItem.cpp \
	src/editor/TreeWidgetLayers.cpp \
	src/editor/TreeWidgetTiles.cpp \
	src/editor/UIMapEditor.cpp \
	src/items/MapObjectItem.cpp \
	src/pads/UIPadSettings.cpp \
	src/pads/PadSettings.cpp \
	src/pads/UIPadConfiguration.cpp \
	src/tilesripper/UITilesRipper.cpp \
	src/tilesripper/TilesRipperPreview.cpp \
	src/reusable/pXmlSettings.cpp \
	src/engine/GameEngine.cpp \
	src/engine/states/gameintroduction/GSIntroduction.cpp \
	src/engine/states/gamemode/GSMode.cpp \
	src/engine/menus/GSMenuItem.cpp \
	src/engine/menus/GSMenu.cpp \
	src/engine/states/padsettings/GSPadSettings.cpp \
	src/engine/states/multiplayerchoice/GSMultiPlayerChoice.cpp \
	src/engine/menus/GSFaceItem.cpp \
	src/engine/Player.cpp \
	src/engine/menus/GSStateItem.cpp \
	src/engine/states/multigameplay/GSMultiGamePlay.cpp \
	src/engine/menus/GSGenericStateItem.cpp \
	src/engine/menus/GSCOMLevelItem.cpp \
	src/engine/menus/GSOnOffItem.cpp \
	src/engine/menus/GSBattleItem.cpp \
	src/engine/menus/GSTimeItem.cpp \
	src/engine/states/multiplayersetup/GSMultiPlayerSetup.cpp \
	src/engine/states/multimapchoice/GSMultiMapChoice.cpp \
	src/engine/menus/GSMapItem.cpp \
	src/engine/states/multiplayground/GSMultiPlayGround.cpp