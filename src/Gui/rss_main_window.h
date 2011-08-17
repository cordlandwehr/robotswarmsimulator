/*
 * RSSMainWindow.h
 *
 *  Created on: 26.07.2011
 *      Author: Sascha Brandt
 */

#ifndef RSS_MAIN_WINDOW_H_
#define RSS_MAIN_WINDOW_H_

#include <QtGui/QMainWindow>
#include <QtCore/QSignalMapper>
#include "ui_rss_main_window.h"

class OpenProjectDialog;
class GeneratorWizard;
class RSSGLWidget;

class RSSMainWindow : public QMainWindow {
	Q_OBJECT

public:
	RSSMainWindow(QWidget *parent = 0);
	~RSSMainWindow();

	void init();

private slots:
	void toggleSimulation();
	void updateSimulationSpeed(int op);
	void stepSimulation();
	void updateSimulation();

private:
	enum {
		INCREASE_SPEED,
		DECREASE_SPEED,
		HALF_SPEED,
		DOUBLE_SPEED
	};

	Ui::RSSMainWindow ui_;
	OpenProjectDialog *open_dialog_;
	GeneratorWizard *generator_wizard_;
	RSSGLWidget *rss_gl_widget_;
	QSignalMapper *signal_mapper_;
};

#endif /* RSS_MAIN_WINDOW_H_ */
