#include "resource/Facets.hpp"
#include <qfile.h>

using namespace resource;

Facets::Entry::Entry(QString _name, QString _mapFile, QSize _dimension, QString _indexFile, QString _dataFile, QObject* _parent, QSize _blockSize )
: IndexFile(_indexFile, _dataFile, _parent),
  mName(_name),
  mSize(_dimension),
  mBlockSize(_blockSize) {
	mMapStream.setDevice( new QFile(_mapFile, this) );
	Q_ASSERT_X(mMapStream.device()->open(QIODevice::ReadOnly),__PRETTY_FUNCTION__,_mapFile.toAscii().constData());
	mMapStream.setByteOrder(QDataStream::LittleEndian);
}

Facets::Entry::~Entry() {
}

Facets::Block Facets::Entry::getBlock( const QPoint& _blockPos ) {
	ID id = convertBlockPosToID(_blockPos);

	if(!mCache.contains(id)) {
		Facets::Block block(QPoint(_blockPos.x()*mBlockSize.width(),_blockPos.y()*mBlockSize.height()),
													decodeMap( getMapData(id) ),
													decodeStatic( getData(id) ) );
		mCache.insert(id, new Block(block));
		return block;
	}
	return Block(*(mCache[id]));
}

QByteArray Facets::Entry::getMapData( ID _id ) {
	Q_ASSERT(mMapStream.device()->seek( _id * 196 ));
	QByteArray result( 196, 0x00 );
	mMapStream.readRawData(result.data(), 196);
	return result;
}

Facets::MapTiles Facets::Entry::decodeMap( QByteArray _data ) {
	MapTiles result;
	if(!_data.isEmpty()) {
		QDataStream stream(_data);
		stream.setByteOrder(QDataStream::LittleEndian);
		stream.skipRawData(4); //skip header
		for( quint8 i = 0; i < 64; i ++ ) {
			MapTile mapTile;
			stream >> mapTile.mID >> mapTile.mZ;
			result.push_back(mapTile);
		}
	}
	return result;
}

Facets::StaticTiles Facets::Entry::decodeStatic( QByteArray _data ) {
	StaticTiles result;
	if(!_data.isEmpty()) {
		QDataStream stream(_data);
		stream.setByteOrder(QDataStream::LittleEndian);
		while(!stream.atEnd()) {
			StaticTile staticTile;
			stream >> staticTile.mID
			>> staticTile.mXOffset
			>> staticTile.mYOffset
			>> staticTile.mZ
			>> staticTile.mHueID;
			result.push_back(staticTile);
		}
	}
	return result;
}