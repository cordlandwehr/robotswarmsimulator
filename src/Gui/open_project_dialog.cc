/*
 * OpenProjectDialog.cc
 *
 *  Created on: 27.07.2011
 *      Author: Sascha Brandt
 */
#include "open_project_dialog.h"

#include <QtGui/QFileDialog>
#include <QIntValidator>

OpenProjectDialog::OpenProjectDialog(QWidget *parent) : QDialog(parent), data_() {
	ui_.setupUi(this);

	// connect signals/slots
	connect(ui_.ProjectFileButton, SIGNAL(clicked()), this, SLOT(openFile()) );
	connect(ui_.OutputDirButton, SIGNAL(clicked()), this, SLOT(openPath()) );

	ui_.lineEdit_3->setValidator(new QIntValidator(0, INT_MAX, ui_.lineEdit_3));
	ui_.lineEdit_4->setValidator(new QIntValidator(0, INT_MAX, ui_.lineEdit_4));
	ui_.lineEdit_5->setValidator(new QIntValidator(0, INT_MAX, ui_.lineEdit_5));
}

OpenProjectDialog::~OpenProjectDialog() {
}


void OpenProjectDialog::openFile() {
	ui_.ProjectFileText->setText(QFileDialog::getOpenFileName(this,
			"Open project file", "",
			tr("Project Files (*.swarm)")));
}

void OpenProjectDialog::openPath() {
	ui_.OutputDirText->setText(QFileDialog::getExistingDirectory());
}

void OpenProjectDialog::accept() {
	data_.project_file = ui_.ProjectFileText->text().toStdString();
	data_.output = ui_.OutputDirText->text().toStdString();
	data_.history_length = ui_.lineEdit_4->text().toUInt();
	data_.dry = ui_.checkBox->isChecked();
	data_.steps = ui_.lineEdit_3->text().toUInt();
	data_.luaseed = ui_.lineEdit_5->text().toUInt();
	data_.run_until_no_multiplicity = ui_.checkBox_2->isChecked();
	QDialog::accept();
}
