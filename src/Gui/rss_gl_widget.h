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
	enum { UP, DOWN, LEFT, RIGHT, FORWARD, BACKWARD };
	enum { DOUBLE_SPEED, HALF_SPEED };
	enum { CAM_FREE=0, CAM_FOLLOW, CAM_COG, CAM_ORTHO };
	enum { ORTHO_AXIS_X, ORTHO_AXIS_Y, ORTHO_AXIS_Z };

	RSSGLWidget(QWidget *parent = 0);
	~RSSGLWidget();

	void set_simulation_control(boost::shared_ptr<SimulationControl> simulation_control);
	boost::shared_ptr<SimulationControl> simulation_control() {
		return simulation_control_;
	}
	boost::shared_ptr<SimulationRenderer> simulation_renderer() {
		return simulation_renderer_;
	}

public slots:
	void move_camera(int direction);
	void toggle_camera_mode();
	void set_camera_mode(int mode);
	void set_camera_speed(int type);
	void set_orthogonal_axis(int axis);
	void toggle_view_cog(bool enabled);
	void toggle_view_local_cs(bool enabled);
	void toggle_view_global_cs(bool enabled);

signals:
	void selected_object_changed(boost::shared_ptr<Identifier> id);

protected:
	virtual void initializeGL();
	virtual void resizeGL( int width, int height );
	virtual void paintGL();

	virtual void timerEvent(QTimerEvent * event);

	virtual void mouseMoveEvent( QMouseEvent * event );
	virtual void mousePressEvent( QMouseEvent * event );
	virtual void mouseReleaseEvent( QMouseEvent * event );
	virtual void wheelEvent( QWheelEvent * event );

private:

	boost::shared_ptr<SimulationControl> simulation_control_;
	boost::shared_ptr<SimulationRenderer> simulation_renderer_;
};

#endif /* RSS_GL_WIDGET_H_ */
