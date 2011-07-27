/*
 * RSSMainWindow.h
 *
 *  Created on: 26.07.2011
 *      Author: Sascha Brandt
 */

#ifndef RSSMAINWINDOW_H_
#define RSSMAINWINDOW_H_

#include <QtGui/QMainWindow>
#include "ui_RSSMainWindow.h"

class OpenProjectDialog;
class GeneratorWizard;

class RSSMainWindow : public QMainWindow {
	Q_OBJECT

public:
	RSSMainWindow(QWidget *parent = 0);
	~RSSMainWindow();

	void init();

private:
	Ui::RSSMainWindow _ui;
	OpenProjectDialog *_open_dialog;
	GeneratorWizard *_generator_wizard;
};

#endif /* RSSMAINWINDOW_H_ */
