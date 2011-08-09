/*
 * OpenProjectDialog.h
 *
 *  Created on: 27.07.2011
 *      Author: Sascha Brandt
 */

#ifndef OPENPROJECTDIALOG_H_
#define OPENPROJECTDIALOG_H_

#include <string>

#include <QtGui/QDialog>

#include "ui_open_project_dialog.h"

struct ProjectData {
	std::string project_file;
	std::string output;
	unsigned int history_length;
	bool dry;
	unsigned int steps;
	unsigned int luaseed;
	bool run_until_no_multiplicity;
};

class OpenProjectDialog : public QDialog {
	Q_OBJECT

public:
	OpenProjectDialog(QWidget *parent = 0);
	~OpenProjectDialog();

	const ProjectData& projectData() const { return data_; };

public slots:
	void openFile();
	void openPath();
	void accept();

private:
	Ui::OpenProjectDialog ui_;
	ProjectData data_;
};

#endif /* OPENPROJECTDIALOG_H_ */
