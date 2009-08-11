#include "MapItem.h"
#include "MapObjectItem.h"
#include "PlayerItem.h"

#include <QFile>
#include <QSettings>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>

MapItem::MapItem( QGraphicsItem* parent )
	: QGraphicsRectItem( parent )
{
	setPen( QPen( Qt::NoPen ) );
	mTiles = TilesManager::instance();
}

MapItem::~MapItem()
{
}

int MapItem::type() const
{
	return Type;
}

void MapItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
	QGraphicsRectItem::paint( painter, option, widget );
}

QString MapItem::name() const
{
	return mName;
}

QSize MapItem::size() const
{
	return mSize;
}

LayersMap MapItem::layers() const
{
	return mLayers;
}

LayerMap MapItem::layer( uint id ) const
{
	return mLayers.value( id );
}

void MapItem::clear()
{
	qDeleteAll( childItems() );
	mName.clear();
	mSize = QSize();
	mMapping.clear();
	mLayers.clear();
	mPlayersPosition.clear();
}

bool MapItem::load( const QString& fileName )
{
	if ( !QFile::exists( fileName ) )
	{
		return false;
	}
	
	QSettings settings( fileName, QSettings::IniFormat );
	
	if ( settings.status() == QSettings::FormatError )
	{
		return false;
	}
	
	clear();
	mName = settings.value( "Map/Name" ).toString();
	mSize = settings.value( "Map/Size" ).toSize();
	
	// tiles
	settings.beginGroup( "Tiles" );
	const QStringList tiles = settings.childKeys();
	foreach ( const QString& tile, tiles )
	{
		uint t = tile.toUInt();
		mMapping[ t ] = settings.value( tile ).toString();
	}
	settings.endGroup();
	
	// layers
	const QStringList groups = settings.childGroups();
	foreach ( const QString& group, groups )
	{
		if ( !group.toLower().startsWith( "layer" ) )
		{
			continue;
		}
		
		const uint layer = group.section( '_', 1, 1 ).toUInt();
		
		if ( layer < Globals::FirstLayer || layer > Globals::LastLayer )
		{
			if ( layer != Globals::SkyLayer )
			{
				Q_ASSERT( 0 );
			}
		}
		
		settings.beginGroup( group );
		const QStringList keys = settings.childKeys();
		foreach ( const QString& key, keys )
		{
			const int y = key.toInt();
			const QString line = settings.value( key ).toString();
			const QStringList parts = line.split( MAP_SPLIT_CHAR, QString::SkipEmptyParts );
			
			for ( int x = 0; x < parts.count(); x++ )
			{
				if ( parts.at( x ).toLower() == "x" )
				{
					continue;
				}
				
				const uint id = parts.at( x ).toUInt();
				AbstractTile* tile = mappedTile( id );
				
				switch ( tile->Type )
				{
					case Globals::InvalidTile:
						Q_ASSERT( 0 );
						continue;
						break;
					case Globals::BlockTile:
					case Globals::BoxTile:
					case Globals::FloorTile:
					case Globals::SkyTile:
					case Globals::BonusTile:
						break;
					case Globals::PlayerTile:
						mPlayersPosition[ tile->name() ] = QPoint( x, y );
					case Globals::BombTile:
					case Globals::BombExplosionTile:
					case Globals::PlayerExplosionTile:
					case Globals::TextTile:
						continue;
						break;
				}
				
				MapObjectItem* object = new MapObjectItem( tile, this );
				object->setZValue( layer );
				mLayers[ layer ][ QPoint( x, y ) ] = object;
			}
		}
		settings.endGroup();
	}
	
	updateMap();
	
	return true;
}

const PlayersPositionMap& MapItem::playersPosition() const
{
	return mPlayersPosition;
}

int MapItem::maxPlayers() const
{
	return mPlayersPosition.count();
}

QPoint MapItem::canStrokeTo( PlayerItem* player, Globals::PadStroke stroke ) const
{
	const int stepBy = 1;
	const QRect bbr = player->mapRectToScene( player->boundingRect() ).toRect();
	const int mw = bbr.width() /8;
	const int mh = bbr.height() /8;
	QPoint p;
	QRect sr;
	
	// check map bounding rect
	switch ( stroke )
	{
		case Globals::PadStrokeUp:
			if ( !( bbr.y() -stepBy >= y() ) )
				return p;
			sr = QRect( QPoint( bbr.left() +mw, bbr.top() -1 ), QSize( mw *6, 1 ) );
			break;
		case Globals::PadStrokeDown:
			if ( !( bbr.y() +stepBy +bbr.height() <= boundingRect().height() ) )
				return p;
			sr = QRect( QPoint( bbr.left() +mw, bbr.bottom() +1 ), QSize( mw *6, 1 ) );
			break;
		case Globals::PadStrokeLeft:
			if ( !( bbr.x() -stepBy >= x() ) )
				return p;
			sr = QRect( QPoint( bbr.left() -1, bbr.top() +mh ), QSize( 1, mh *6 ) );
			break;
		case Globals::PadStrokeRight:
			if ( !( bbr.x() +stepBy +bbr.width() <= boundingRect().width() ) )
				return p;
			sr = QRect( QPoint( bbr.right() +1, bbr.top() +mh ), QSize( 1, mh *6 ) );
			break;
		default:
			break;
	}
	
	QMap<QPoint, MapObjectItem*> objects;
	QSet<MapObjectItem*> walkableObjects;
	
	// minimized objects map
	foreach ( QGraphicsItem* item, scene()->items( sr ) )
	{
		MapObjectItem* object = qgraphicsitem_cast<MapObjectItem*>( item );
		
		if ( !object || !object->isValid() || object == player )
		{
			continue;
		}
		
		const QPoint pos = gridPos( object );
		
		if ( objects.contains( pos ) )
		{
			MapObjectItem* curObject = objects[ pos ];
			
			if ( curObject->isWalkable() )
			{
				walkableObjects.remove( curObject );
				objects[ pos ] = object;
				
				if ( object->isWalkable() )
				{
					walkableObjects << object;
				}
			}
		}
		else
		{
			objects[ pos ] = object;
			
			if ( object->isWalkable() )
			{
				walkableObjects << object;
			}
		}
	}
	
	// determine the item to walk to
	MapObjectItem* walkToObject = nearestObject( sr.center(), stroke, walkableObjects );
	
	// return if can't walk
	if ( !walkToObject )
	{
		return p;
	}
	
	// calculate the step to move to
	switch ( stroke )
	{
		case Globals::PadStrokeUp:
		case Globals::PadStrokeDown:
			if ( bbr.x() != walkToObject->x() )
			{
				if ( bbr.x() < walkToObject->x() )
					p.rx() += stepBy;
				else
					p.rx() -= stepBy;
			}
			else
			{
				if ( stroke == Globals::PadStrokeUp )
					p.ry() -= stepBy;
				else
					p.ry() += stepBy;
			}
			break;
		case Globals::PadStrokeLeft:
		case Globals::PadStrokeRight:
			if ( bbr.y() != walkToObject->y() )
			{
				if ( bbr.y() < walkToObject->y() )
					p.ry() += stepBy;
				else
					p.ry() -= stepBy;
			}
			else
			{
				if( stroke == Globals::PadStrokeLeft )
					p.rx() -= stepBy;
				else
					p.rx() += stepBy;
			}
			break;
		default:
			break;
	}
	
	// return possible walk
	return p;
}

QPoint MapItem::gridToPos( const QPoint& gridPos ) const
{
	const QSize tileSize = mTiles->tileSize();
	return QPoint( gridPos.x() *tileSize.width(), gridPos.y() *tileSize.height() );
}

QPoint MapItem::posToGrid( const QPoint& pos ) const
{
	const QSize tileSize = mTiles->tileSize();
	return QPoint( pos.x() /tileSize.width(), pos.y() /tileSize.height() );
}

QPoint MapItem::gridPos( MapObjectItem* object ) const
{
	const QPoint invalidPos( -1, -1 );
	const int layer = object->zValue();
	QPoint p = mLayers.value( layer ).key( object, invalidPos );
	
	if ( p == invalidPos )
	{
		const QSize tileSize = mTiles->tileSize();
		p = QPoint( object->pos().x() /tileSize.width(), object->pos().y() /tileSize.height() );
	}
	
	return p;
}

QPoint MapItem::closestPos( const QPoint& pos ) const
{
	const QSize tileSize = mTiles->tileSize();
	const QPoint point( pos.x() /tileSize.width(), pos.y() /tileSize.height() );
	return QPoint( point.x() *tileSize.width(), point.y() *tileSize.height() );
}

AbstractTile* MapItem::mappedTile( uint id ) const
{
	const QString name = mMapping.value( id );
	return mTiles ? mTiles->tile( name ) : 0;
}

MapObjectItem* MapItem::nearestObject( const QPoint& strokePoint, Globals::PadStroke stroke, const QSet<MapObjectItem*>& objects ) const
{
	if ( objects.isEmpty() )
	{
		return 0;
	}
	else if ( objects.count() == 1 )
	{
		return *objects.begin();
	}
	
	QMap<int, MapObjectItem*> items;
	
	foreach ( MapObjectItem* object, objects )
	{
		const QPoint center = object->mapToScene( object->boundingRect().center() ).toPoint();
		int diff, min = 0, max = 0;
		
		switch ( stroke )
		{
			case Globals::PadStrokeUp:
			case Globals::PadStrokeDown:
				min = qMin( strokePoint.x(), center.x() );
				max = qMax( strokePoint.x(), center.x() );
				break;
			case Globals::PadStrokeLeft:
			case Globals::PadStrokeRight:
				min = qMin( strokePoint.y(), center.y() );
				max = qMax( strokePoint.y(), center.y() );
				break;
			default:
				Q_ASSERT( 0 );
				break;
		}
		
		diff = max -min;
		items[ diff ] = object;
	}
	
	return items.values().first();
}

void MapItem::updateMap()
{
	const QSize tileSize = mTiles->tileSize();
	
	foreach ( const int layer, mLayers.keys() )
	{
		foreach ( const QPoint& pos, mLayers[ layer ].keys() )
		{
			MapObjectItem* object = mLayers[ layer ][ pos ];
			
			if ( pos.x() >= mSize.width() || pos.y() >= mSize.height() )
			{
				delete object;
				mLayers[ layer ].remove( pos );
			}
			else
			{
				const QPoint itemPos( pos.x() *tileSize.width(), pos.y() *tileSize.height() );
				object->setPos( itemPos );
			}
		}
	}
	
	setRect( 0, 0, mSize.width() *tileSize.width(), mSize.height() *tileSize.height() );
}
