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
	connect(ui_.button_file, SIGNAL(clicked()), this, SLOT(open_file()) );
	connect(ui_.button_out, SIGNAL(clicked()), this, SLOT(open_path()) );

	// create validators
	ui_.lineedit_history_length->setValidator(new QIntValidator(0, INT_MAX, ui_.lineedit_history_length));
	ui_.lineedit_luaseed->setValidator(new QIntValidator(0, INT_MAX, ui_.lineedit_luaseed));
	ui_.lineedit_steps->setValidator(new QIntValidator(0, INT_MAX, ui_.lineedit_steps));
}

OpenProjectDialog::~OpenProjectDialog() {
}


void OpenProjectDialog::open_file() {
	ui_.lineedit_file->setText(QFileDialog::getOpenFileName(this,
			"Open project file", "",
			tr("Project Files (*.swarm)")));
}

void OpenProjectDialog::open_path() {
	ui_.lineedit_out->setText(QFileDialog::getExistingDirectory());
}

void OpenProjectDialog::accept() {
	data_.project_file = ui_.lineedit_file->text().toStdString();
	data_.output = ui_.lineedit_out->text().toStdString();
	data_.history_length = ui_.lineedit_history_length->text().toUInt();
	data_.dry = ui_.checkbox_dry->isChecked();
	data_.steps = ui_.lineedit_steps->text().toUInt();
	data_.luaseed = ui_.lineedit_luaseed->text().toUInt();
	data_.run_until_no_multiplicity = ui_.checkbox_no_multiplicity->isChecked();
	QDialog::accept();
}
