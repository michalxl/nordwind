/*
 * IndexFile.cpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */
#include "resource/IndexFile.hpp"
#include <qfile.h>

using namespace resource;

IndexFile::IndexFile(QString _indexFile, QString _dataFile, QObject *_parent)
: QAbstractListModel(_parent) {
	Q_ASSERT_X( QFile::exists( _indexFile ) && QFile::exists(_dataFile),
				__PRETTY_FUNCTION__,
				QString("File(s) not available! Is the path correct? %1 %2").arg(_indexFile).arg(_dataFile).toAscii().constData());
	mIndexStream.setDevice( new QFile( _indexFile, this ) );
	mDataStream.setDevice(  new QFile( _dataFile, this ) );
	Q_ASSERT_X(mIndexStream.device()->open( QIODevice::ReadOnly ) &&
				 mDataStream.device()->open( QIODevice::ReadOnly ),
				__PRETTY_FUNCTION__,
				QString("Unable to open file(s) in read-mode! %1 %2").arg(_indexFile).arg(_dataFile).toAscii().constData() );
	mIndexStream.setByteOrder( QDataStream::LittleEndian );
	mDataStream.setByteOrder( QDataStream::LittleEndian );
	quint32 entry_count = mIndexStream.device()->size()/12;
	for( quint32 i = 0; i < entry_count; i++ ) {
		IndexFile::Entry entry;
		mIndexStream >> entry.mOffset >> entry.mSize >> entry.mExtra;
		if(entry.isValid()) {
			mEntries.insert( i, entry );
		}
	}
}

IndexFile::~IndexFile() {
}

QByteArray IndexFile::getData(ID _id ) {
	QByteArray result;
	if(mEntries.contains(_id)) {
		IndexFile::Entry entry = mEntries[_id];
		mDataStream.device()->seek( entry.mOffset );
		result.resize(entry.mSize);
		Q_ASSERT_X(mDataStream.readRawData(result.data(), entry.mSize),
				__PRETTY_FUNCTION__, "Insufficient bytes to read!");
	}
	return result;
}

QVariant IndexFile::data( const QModelIndex& _index, int _role ) const {
	Q_UNUSED(_role)
	return !_index.isValid() ? QVariant() : (QString)mEntries.value(_index.row());
}
