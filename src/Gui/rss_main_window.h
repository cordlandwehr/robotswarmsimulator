/*
 * RSSMainWindow.h
 *
 *  Created on: 26.07.2011
 *      Author: Sascha Brandt
 */

#ifndef RSS_MAIN_WINDOW_H_
#define RSS_MAIN_WINDOW_H_

#include <boost/smart_ptr.hpp>

#include <QtGui/QMainWindow>
#include <QtCore/QSignalMapper>
#include "ui_rss_main_window.h"

class OpenProjectDialog;
class GeneratorWizard;
class RSSGLWidget;
class Identifier;
class WorldObject;

class RSSMainWindow : public QMainWindow {
	Q_OBJECT

public:
	RSSMainWindow(QWidget *parent = 0);
	~RSSMainWindow();

	void init();

	RSSGLWidget * rss_gl_widget() { return rss_gl_widget_; }

protected:

	virtual void timerEvent(QTimerEvent * event);

private slots:
	void toggle_simulation();
	void update_simulation_speed(int op);
	void step_simulation();
	void update_simulation();
	void generate_simulation();
	void set_camera_mode(int mode);
	void selected_object_changed(boost::shared_ptr<Identifier> id);
	void tree_selection_changed(QTreeWidgetItem* item, int column);

private:

	void update_selected_object();

	enum {
		INCREASE_SPEED,
		DECREASE_SPEED,
		HALF_SPEED,
		DOUBLE_SPEED
	};
	enum {
		CAM_FREE=0,
		CAM_FOLLOW,
		CAM_COG
	};

	Ui::RSSMainWindow ui_;
	OpenProjectDialog *open_dialog_;
	GeneratorWizard *generator_wizard_;
	RSSGLWidget *rss_gl_widget_;
	QSignalMapper *speed_signal_mapper_;
	QSignalMapper *cam_signal_mapper_;
	boost::shared_ptr<Identifier> selection_id_;
};

#endif /* RSS_MAIN_WINDOW_H_ */
