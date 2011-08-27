/*
 * rss_gl_widget.cc
 *
 *  Created on: 09.08.2011
 *      Author: Sascha Brandt
 */

#include <QKeyEvent>

#include "../Model/robot_data.h"

#include "rss_gl_widget.h"


RSSGLWidget::RSSGLWidget(QWidget *parent) : QGLWidget(parent),
		simulation_control_(), simulation_renderer_() {
	simulation_renderer_.reset(new SimulationRenderer());
	simulation_renderer_->init();
	setFocusPolicy(Qt::StrongFocus);

	startTimer(50);
}


RSSGLWidget::~RSSGLWidget() {

}

void RSSGLWidget::set_simulation_control(boost::shared_ptr<SimulationControl> simulation_control) {
	if(!simulation_control.get())
		return;

	simulation_control_ = simulation_control;
	simulation_control_->set_visualizer(simulation_renderer_);

	Vector3d cam_pos = string_to_vec( simulation_control_->camera_position() );
	Vector3d cam_dir = string_to_vec( simulation_control_->camera_direction() );
	std::string cam_type = simulation_control_->camera_type();

	if( cam_type.compare("FREE") == 0 ){
		simulation_renderer_->set_free_cam_para( cam_pos, cam_dir );
		simulation_renderer_->set_active_cam(1);
	} else if( cam_type.compare("COG") == 0) {
		simulation_renderer_->set_active_cam(2);
		simulation_renderer_->set_cog_cam_pos( cam_pos );
	}

	simulation_control_->start_simulation();
	simulation_control_->pause_processing_time(true);
}

void RSSGLWidget::initializeGL() {
	simulation_renderer_->init();
}

void RSSGLWidget::resizeGL( int width, int height ) {
	simulation_renderer_->resize(width, height);
}

void RSSGLWidget::paintGL() {
	if(simulation_control_.get())
		simulation_control_->process_simulation();
}

void RSSGLWidget::timerEvent(QTimerEvent * event) {
	updateGL();
}

void RSSGLWidget::keyPressEvent( QKeyEvent *event ) {
	if(!simulation_control_.get())
		return;

	switch (event->key()) {
		case Qt::Key_Left:
		case Qt::Key_A:
			simulation_renderer_->camera()->strafe_left();
			break;
		case Qt::Key_Right:
		case Qt::Key_D:
			simulation_renderer_->camera()->strafe_right();
			break;
		case Qt::Key_R:
			simulation_renderer_->camera()->move_up();
			break;
		case Qt::Key_F:
			simulation_renderer_->camera()->move_down();
			break;
		case Qt::Key_Up:
		case Qt::Key_W:
			simulation_renderer_->camera()->move_forward();
			break;
		case Qt::Key_Down:
		case Qt::Key_S:
			simulation_renderer_->camera()->move_backward();
			break;
		case Qt::Key_Space: // pause simulation
			simulation_control_->pause_processing_time(!simulation_control_->is_processing_time_paused());
			break;
		case Qt::Key_Enter:
			// simulate one more step
			if(simulation_control_->is_single_step_mode()) {
				simulation_control_->do_single_step();
			} else {
				simulation_control_->enter_single_step_mode();
			}
			break;
		case Qt::Key_Plus:
			simulation_control_->increase_processing_time_linearly();
			break;
		case Qt::Key_Minus:
			simulation_control_->decrease_processing_time_linearly();
			break;
		case Qt::Key_multiply:
			simulation_control_->increase_processing_time_exp();
			break;
		case Qt::Key_division:
			simulation_control_->decrease_processing_time_exp();
			break;
		default:
			QGLWidget::keyPressEvent(event);
	}
}

void RSSGLWidget::mouseMoveEvent( QMouseEvent * event ) {
	simulation_renderer_->mouse_motion_func(event->x(), event->y());
}

void RSSGLWidget::mousePressEvent( QMouseEvent * event ) {
	simulation_renderer_->mouse_func(event->button(), event->type(), event->x(), event->y());

	boost::shared_ptr<RobotData> data = simulation_renderer_->pick_robot(event->x(), event->y());
	emit selected_robot_changed(data);
}

void RSSGLWidget::mouseReleaseEvent( QMouseEvent * event ) {
	simulation_renderer_->mouse_func(event->button(), event->type(), event->x(), event->y());
}
