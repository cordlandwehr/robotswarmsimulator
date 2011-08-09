/*
 * rss_gl_widget.cc
 *
 *  Created on: 09.08.2011
 *      Author: Sascha Brandt
 */

#include "rss_gl_widget.h"


RSSGLWidget::RSSGLWidget(QWidget *parent) : QGLWidget(parent),
		simulation_control_(), simulation_renderer_(), has_simulation_control_(false) {
	simulation_renderer_.reset(new SimulationRenderer());
	simulation_renderer_->init();
	simulation_renderer_->set_swap_buffers(false);
	startTimer(50);
}


RSSGLWidget::~RSSGLWidget() {

}

void RSSGLWidget::set_simulation_control(boost::shared_ptr<SimulationControl> simulation_control) {
	simulation_control_ = simulation_control;
	has_simulation_control_ = true;
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
}

void RSSGLWidget::initializeGL() {
	simulation_renderer_->init();
}

void RSSGLWidget::resizeGL( int width, int height ) {
	simulation_renderer_->resize(width, height);
}

void RSSGLWidget::paintGL() {
	if(has_simulation_control_)
		simulation_control_->process_simulation();
}

void RSSGLWidget::timerEvent(QTimerEvent * event) {
	updateGL();
}

void RSSGLWidget::keyPressEvent( QKeyEvent *e ) {

}
