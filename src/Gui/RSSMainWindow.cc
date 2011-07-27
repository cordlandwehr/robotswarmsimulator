/*
 * RSSMainWindow.cc
 *
 *  Created on: 26.07.2011
 *      Author: Sascha Brandt
 */

#include "RSSMainWindow.h"
#include "OpenProjectDialog.h"
#include "GeneratorWizard.h"

#include <Utilities/console_output.h>

#include <QCommonStyle>
#include <QBitmap>
#include <QResource>

// creates a togglable icon
QIcon standardToggleIcon(const QStyle& style, QStyle::StandardPixmap iconOn, QStyle::StandardPixmap iconOff) {
	QIcon * icon = new QIcon();
	icon->addPixmap(style.standardPixmap(iconOn), QIcon::Normal, QIcon::On);
	icon->addPixmap(style.standardPixmap(iconOff), QIcon::Normal, QIcon::Off);
	return *icon;
}

RSSMainWindow::RSSMainWindow(QWidget *parent) : QMainWindow(parent) {
	_ui.setupUi(this);
	_open_dialog = new OpenProjectDialog(this);
	_generator_wizard = new GeneratorWizard(this);

	// connect signals/slots
	connect(_ui.actionOpen, SIGNAL(activated()), _open_dialog, SLOT(show()));
	connect(_ui.action_New, SIGNAL(activated()), _generator_wizard, SLOT(show()));

	// setup icons
	QCommonStyle style;
	_ui.action_New->setIcon(style.standardIcon(QStyle::SP_FileIcon));
	_ui.actionOpen->setIcon(style.standardIcon(QStyle::SP_DirOpenIcon));
	_ui.action_Quit->setIcon(style.standardIcon(QStyle::SP_DialogCloseButton));

}

RSSMainWindow::~RSSMainWindow() {
	delete _open_dialog;
}

void RSSMainWindow::init() {

}
