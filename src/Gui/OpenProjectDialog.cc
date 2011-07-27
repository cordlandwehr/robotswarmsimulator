/*
 * OpenProjectDialog.cc
 *
 *  Created on: 27.07.2011
 *      Author: Sascha Brandt
 */
#include "OpenProjectDialog.h"

#include <QtGui/QFileDialog>
#include <QIntValidator>

OpenProjectDialog::OpenProjectDialog(QWidget *parent) : QDialog(parent) {
	_ui.setupUi(this);

	// connect signals/slots
	connect(_ui.ProjectFileButton, SIGNAL(clicked()), this, SLOT(openFile()) );
	connect(_ui.OutputDirButton, SIGNAL(clicked()), this, SLOT(openPath()) );

	_ui.lineEdit_3->setValidator(new QIntValidator(0, INT_MAX, _ui.lineEdit_3));
	_ui.lineEdit_4->setValidator(new QIntValidator(0, INT_MAX, _ui.lineEdit_4));
	_ui.lineEdit_5->setValidator(new QIntValidator(0, INT_MAX, _ui.lineEdit_5));
}

OpenProjectDialog::~OpenProjectDialog() {
}


void OpenProjectDialog::openFile() {
	_ui.ProjectFileText->setText(QFileDialog::getOpenFileName(this,
			"Open project file", "",
			tr("Project Files (*.swarm)")));
}

void OpenProjectDialog::openPath() {
	_ui.OutputDirText->setText(QFileDialog::getExistingDirectory());
}
