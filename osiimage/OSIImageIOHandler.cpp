/*
 * OSIImageIOHandler.cpp
 *
 *  Created on: Sep 7, 2010
 *      Author: idstein
 */

#include "OSIImageIOHandler.hpp"
#include "OSIImageIOHandlerPrivate.hpp"
#include "AnimationIOHandler.hpp"
#include "ArtIOHandler.hpp"
#include "GumpIOHandler.hpp"
#include "TextureIOHandler.hpp"
#include <qvariant.h>
#include <qrect.h>

bool OSIImageIOHandler::canRead(QIODevice* device, const QByteArray& format) {
	return OSIImageIOHandler(device, format).canRead();
}

OSIImageIOHandler::OSIImageIOHandler(QIODevice* device,
		const QByteArray& format) :
	QImageIOHandler() {
	setDevice(device);
	setFormat(format);
}

OSIImageIOHandler::~OSIImageIOHandler() {
}

void OSIImageIOHandler::setFormat(const QByteArray& format) {
	if(format=="art")
		d_ptr.reset(new ArtIOHandler(this));
	else if(format=="animation")
		d_ptr.reset(new AnimationIOHandler(this));
	else if(format=="gump")
		d_ptr.reset(new GumpIOHandler(this));
	else if(format=="texture")
		d_ptr.reset(new TextureIOHandler(this));
	else
		d_ptr.reset(new OSIImageIOHandlerPrivate(this));
}

bool OSIImageIOHandler::canRead() const {
	Q_D(const OSIImageIOHandler);
	return d->canRead();
}

bool OSIImageIOHandler::read(QImage* image) {
	Q_D( OSIImageIOHandler);
	return d->read(image);
}

int OSIImageIOHandler::loopCount() const {
	Q_D(const OSIImageIOHandler);
	return d->loopCount();
}

void OSIImageIOHandler::setOption(ImageOption option, const QVariant & value) {
	Q_D( OSIImageIOHandler);
	d->setOption(option, value);
}

int OSIImageIOHandler::currentImageNumber() const {
	Q_D(const OSIImageIOHandler);
	return d->currentImageNumber();
}

bool OSIImageIOHandler::write(const QImage & image) {
	Q_D( OSIImageIOHandler);
	return d->write(image);
}

bool OSIImageIOHandler::jumpToNextImage() {
	Q_D( OSIImageIOHandler);
	return d->jumpToNextImage();
}

bool OSIImageIOHandler::supportsOption(ImageOption option) const {
	Q_D(const OSIImageIOHandler);
	return d->supportsOption(option);
}

QVariant OSIImageIOHandler::option(ImageOption option) const {
	Q_D(const OSIImageIOHandler);
	return d->option(option);
}

int OSIImageIOHandler::imageCount() const {
	Q_D(const OSIImageIOHandler);
	return d->imageCount();
}

QRect OSIImageIOHandler::currentImageRect() const {
	Q_D(const OSIImageIOHandler);
	return d->currentImageRect();
}

bool OSIImageIOHandler::jumpToImage(int imageNumber) {
	Q_D( OSIImageIOHandler);
	return d->jumpToImage(imageNumber);
}

int OSIImageIOHandler::nextImageDelay() const {
	Q_D(const OSIImageIOHandler);
	return d->nextImageDelay();
}