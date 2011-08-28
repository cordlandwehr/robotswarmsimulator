/*
 * rss_gl_widget.h
 *
 *  Created on: 09.08.2011
 *      Author: Sascha Brandt
 */

#ifndef RSS_GL_WIDGET_H_
#define RSS_GL_WIDGET_H_

#include <boost/shared_ptr.hpp>

#include <QtOpenGL/QGLWidget>
#include <QtCore/QTimerEvent>

#include <SimulationControl/simulation_control.h>
#include <Visualisation/simulation_renderer.h>

class RobotData;

class RSSGLWidget : public QGLWidget {
	Q_OBJECT

public:
	RSSGLWidget(QWidget *parent = 0);
	~RSSGLWidget();

	void set_simulation_control(boost::shared_ptr<SimulationControl> simulation_control);
	boost::shared_ptr<SimulationControl> simulation_control() {
		return simulation_control_;
	}
	boost::shared_ptr<SimulationRenderer> simulation_renderer() {
		return simulation_renderer_;
	}

signals:
	void selected_object_changed(boost::shared_ptr<Identifier> id);

protected:
	virtual void initializeGL();
	virtual void resizeGL( int width, int height );
	virtual void paintGL();

	virtual void timerEvent(QTimerEvent * event);

	virtual void keyPressEvent( QKeyEvent *event );
	virtual void mouseMoveEvent( QMouseEvent * event );
	virtual void mousePressEvent( QMouseEvent * event );
	virtual void mouseReleaseEvent( QMouseEvent * event );

private:

	boost::shared_ptr<SimulationControl> simulation_control_;
	boost::shared_ptr<SimulationRenderer> simulation_renderer_;
};

#endif /* RSS_GL_WIDGET_H_ */
