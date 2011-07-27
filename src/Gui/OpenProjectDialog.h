/*
 * OpenProjectDialog.h
 *
 *  Created on: 27.07.2011
 *      Author: Sascha Brandt
 */

#ifndef OPENPROJECTDIALOG_H_
#define OPENPROJECTDIALOG_H_

#include <QtGui/QDialog>
#include "ui_OpenProjectDialog.h"

class OpenProjectDialog : public QDialog {
	Q_OBJECT

public:
	OpenProjectDialog(QWidget *parent = 0);
	~OpenProjectDialog();

public slots:
	void openFile();
	void openPath();

private:
	Ui::OpenProjectDialog _ui;
};

#endif /* OPENPROJECTDIALOG_H_ */
