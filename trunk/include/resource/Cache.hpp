/*
 * ResourceCache.hpp
 *
 *  Created on: 13.12.2009
 *      Author: idstein
 */

#ifndef CACHE_HPP_
#define CACHE_HPP_

#include <qsharedpointer.h>
#include <qbytearray.h>
#include <qcache.h>
#include <qhash.h>
#include "../util/Singleton.hpp"
#include "Object.hpp"

namespace resource {
	class Cache : public Singleton<Cache> {
		friend class Singleton<Cache>;
		public:
			template<typename T>
			QSharedPointer<T> lookup( const QByteArray& _key );
			template<typename T>
			QSharedPointer<T> manage( Object* object );
			void moveToCache( const QByteArray& _key, Object* _object );
		protected:
			QSharedPointer<Object> getPointer( const QByteArray& _key );
		private:
			QHash< QByteArray, QWeakPointer<Object> > mObjects;
			QCache< QByteArray, Object > mCache;
	};

	template <typename T>
	inline QSharedPointer<T> Cache::lookup( const QByteArray& _key ) {
		return getPointer(_key).dynamicCast<T>();
	}

	template <typename T>
	inline QSharedPointer<T> Cache::manage( Object* _object ) {
		// check if T is derivative of Object
		QSharedPointer<Object> result( _object, &Object::cache );
		if(!result.isNull()) {
			mObjects[result->getKey()] = result.toWeakRef();
		}
		return result.staticCast<T>();
	}
}

#endif /* RESOURCECACHE_HPP_ */
