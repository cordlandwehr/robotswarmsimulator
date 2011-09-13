/*
 * rss_gl_widget.cc
 *
 *  Created on: 09.08.2011
 *      Author: Sascha Brandt
 */

#include <QKeyEvent>

#include <Utilities/console_output.h>
#include "../Model/identifier.h"
#include "../Visualisation/orthogonal_camera.h"

#include "rss_gl_widget.h"

const float kMouseZoomSpeed = 0.005;

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
	if(simulation_control_.get()) {
		simulation_control_->terminate_simulation();
	}

	simulation_control_ = simulation_control;
	simulation_control_->set_visualizer(simulation_renderer_);

	Vector3d cam_pos = string_to_vec( simulation_control_->camera_position() );
	Vector3d cam_dir = string_to_vec( simulation_control_->camera_direction() );
	std::string cam_type = simulation_control_->camera_type();

	if( cam_type.compare("FREE") == 0 ){
		simulation_renderer_->set_free_cam_para( cam_pos, cam_dir );
		simulation_renderer_->set_active_cam(CAM_FREE);
	} else if( cam_type.compare("COG") == 0) {
		simulation_renderer_->set_active_cam(CAM_COG);
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

void RSSGLWidget::move_camera( int direction ) {
	switch(direction) {
	case UP:
		simulation_renderer_->camera()->move_up();
		break;
	case DOWN:
		simulation_renderer_->camera()->move_down();
		break;
	case LEFT:
		simulation_renderer_->camera()->strafe_left();
		break;
	case RIGHT:
		simulation_renderer_->camera()->strafe_right();
		break;
	case FORWARD:
		simulation_renderer_->camera()->move_forward();
		break;
	case BACKWARD:
		simulation_renderer_->camera()->move_backward();
		break;
	}
}

void RSSGLWidget::toggle_camera_mode() {
	set_camera_mode(simulation_renderer_->active_cam_index()+1);
}

void RSSGLWidget::set_camera_mode(int mode) {
	simulation_renderer_->set_projection_type( mode == CAM_ORTHO ? SimulationRenderer::PROJ_ORTHO : SimulationRenderer::PROJ_PERSP);
	simulation_renderer_->set_active_cam(mode);
}

void RSSGLWidget::set_camera_speed(int type) {
	switch(type) {
	case DOUBLE_SPEED:
		simulation_renderer_->camera()->set_speed(simulation_renderer_->camera()->speed()*2);
		break;
	case HALF_SPEED:
		simulation_renderer_->camera()->set_speed(simulation_renderer_->camera()->speed()*0.5);
		break;
	}
}

void RSSGLWidget::set_orthogonal_axis(int axis) {

	boost::shared_ptr<OrthogonalCamera> cam = boost::dynamic_pointer_cast<OrthogonalCamera>(simulation_renderer_->camera());
	if(!cam.get())
		return;
	cam->set_ortho_axis(axis);
}


void RSSGLWidget::toggle_view_cog(bool enabled) {
	simulation_renderer_->set_render_cog(enabled);
}

void RSSGLWidget::toggle_view_local_cs(bool enabled) {
	simulation_renderer_->set_render_local_coord_system(enabled);
}

void RSSGLWidget::toggle_view_global_cs(bool enabled) {
	simulation_renderer_->set_render_coord_system(enabled);
}


void RSSGLWidget::mouseMoveEvent( QMouseEvent * event ) {
	if(!simulation_control_.get()) {
		event->ignore();
		return;
	}
	simulation_renderer_->mouse_motion_func(event->x(), event->y());
}

void RSSGLWidget::mousePressEvent( QMouseEvent * event ) {
	if(!simulation_control_.get()) {
		event->ignore();
		return;
	}
	simulation_renderer_->mouse_func(event->button(), event->type(), event->x(), event->y());

	boost::shared_ptr<Identifier> id = simulation_renderer_->pick_object(event->x(), event->y());
	emit selected_object_changed(id);
}

void RSSGLWidget::mouseReleaseEvent( QMouseEvent * event ) {
	if(!simulation_control_.get()) {
		event->ignore();
		return;
	}
	simulation_renderer_->mouse_func(event->button(), event->type(), event->x(), event->y());
}


void RSSGLWidget::wheelEvent( QWheelEvent * event ) {
	if(!simulation_control_.get()) {
		event->ignore();
		return;
	}
	if(simulation_renderer_->active_cam_index() == CAM_ORTHO) {
		simulation_renderer_->camera()->move_camera_up_down(-(float)event->delta()*kMouseZoomSpeed);
	} else {
		set_camera_speed( event->delta() > 0 ? DOUBLE_SPEED : HALF_SPEED );
	}
	event->accept();
}
