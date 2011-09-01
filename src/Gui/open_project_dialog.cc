/*
 * OpenProjectDialog.cc
 *
 *  Created on: 27.07.2011
 *      Author: Sascha Brandt
 */
#include "open_project_dialog.h"

#include <QtGui/QFileDialog>
#include <QIntValidator>
#include <QSettings>

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
	QString file = QFileDialog::getOpenFileName(this,
			"Open project file", "",
			tr("Project Files (*.swarm)"));
	file = QDir::current().relativeFilePath(file);
	ui_.lineedit_file->setText(file);
}

void OpenProjectDialog::open_path() {
	QString dir = QFileDialog::getExistingDirectory();
	dir = QDir::current().relativeFilePath(dir);
	ui_.lineedit_out->setText(dir);
}

void OpenProjectDialog::accept() {
	data_.project_file = ui_.lineedit_file->text().toStdString();
	data_.output = ui_.lineedit_out->text().toStdString();
	data_.history_length = ui_.lineedit_history_length->text().toUInt();
	data_.dry = ui_.checkbox_dry->isChecked();
	data_.steps = ui_.lineedit_steps->text().toUInt();
	data_.luaseed = ui_.lineedit_luaseed->text().toUInt();
	data_.run_until_no_multiplicity = ui_.checkbox_no_multiplicity->isChecked();
	writeSettings();
	QDialog::accept();
}


void OpenProjectDialog::showEvent( QShowEvent * event ) {
	readSettings();
	ui_.lineedit_file->setText(QString::fromStdString(data_.project_file));
	ui_.lineedit_out->setText(QString::fromStdString(data_.output));
	ui_.lineedit_history_length->setText(QString("%1").arg(data_.history_length));
	ui_.checkbox_dry->setChecked(data_.dry);
	ui_.lineedit_steps->setText(QString("%1").arg(data_.steps));
	ui_.lineedit_luaseed->setText(QString("%1").arg(data_.luaseed));
	ui_.checkbox_no_multiplicity->setChecked(data_.run_until_no_multiplicity);

	QDialog::showEvent(event);
}

void OpenProjectDialog::writeSettings() {
	QSettings settings("Uni Paderborn", "RobotSwarmSimulator");

	settings.beginGroup("OpenProjectDialog");
	settings.setValue("project_file", QString::fromStdString(data_.project_file));
	settings.setValue("output", QString::fromStdString(data_.output));
	settings.setValue("history_length", data_.history_length);
	settings.setValue("dry", data_.dry);
	settings.setValue("steps", data_.steps);
	settings.setValue("luaseed", data_.luaseed);
	settings.setValue("run_until_no_multiplicity", data_.run_until_no_multiplicity);
	settings.endGroup();
}

void OpenProjectDialog::readSettings() {
	QSettings settings("Uni Paderborn", "RobotSwarmSimulator");

	settings.beginGroup("OpenProjectDialog");
	data_.project_file = settings.value("project_file").toString().toStdString();
	data_.output = settings.value("output").toString().toStdString();
	data_.history_length = settings.value("history_length", 25).toUInt();
	data_.dry = settings.value("dry").toBool();
	data_.steps = settings.value("steps").toUInt();
	data_.luaseed = settings.value("luaseed").toUInt();
	data_.run_until_no_multiplicity = settings.value("run_until_no_multiplicity").toBool();
	settings.endGroup();
}
