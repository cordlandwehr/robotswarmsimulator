/*
 * GeneratorWizard.cc
 *
 *  Created on: 27.07.2011
 *      Author: Sascha Brandt
 */

#include <QButtonGroup>

#include "generator_wizard.h"

GeneratorWizard::GeneratorWizard(QWidget *parent) : QWizard(parent) {
	ui_.setupUi(this);

	QButtonGroup* button_group = new QButtonGroup(this);

	button_group->addButton(ui_.radiobutton_no_distr);
	button_group->addButton(ui_.radiobutton_circle);
	button_group->addButton(ui_.radiobutton_cube);
	button_group->addButton(ui_.radiobutton_sphere);
	button_group->addButton(ui_.radiobutton_gauss);
	button_group->addButton(ui_.radiobutton_poisson);
	button_group->addButton(ui_.radiobutton_cluster);
	button_group->addButton(ui_.radiobutton_rnd_walk);

	// create validators
	ui_.lineedit_angle_circ->setValidator(new QDoubleValidator(ui_.lineedit_angle_circ));
	ui_.lineedit_cluster_max_dist->setValidator(new QDoubleValidator(ui_.lineedit_cluster_max_dist));
	ui_.lineedit_cluster_min_dist->setValidator(new QDoubleValidator(ui_.lineedit_cluster_min_dist));
	ui_.lineedit_cluster_sigma->setValidator(new QDoubleValidator(ui_.lineedit_cluster_sigma));
	ui_.lineedit_cluster_num->setValidator(new QIntValidator(0, INT_MAX, ui_.lineedit_cluster_num));
	ui_.lineedit_diam_cube->setValidator(new QDoubleValidator(ui_.lineedit_diam_cube));
	ui_.lineedit_diam_gauss->setValidator(new QDoubleValidator(ui_.lineedit_diam_gauss));
	ui_.lineedit_max_acc->setValidator(new QDoubleValidator(ui_.lineedit_max_acc));
	ui_.lineedit_max_vel->setValidator(new QDoubleValidator(ui_.lineedit_max_vel));
	ui_.lineedit_mean_acc->setValidator(new QDoubleValidator(ui_.lineedit_mean_acc));
	ui_.lineedit_mean_pos->setValidator(new QDoubleValidator(ui_.lineedit_mean_pos));
	ui_.lineedit_mean_vel->setValidator(new QDoubleValidator(ui_.lineedit_mean_vel));
	ui_.lineedit_min_acc->setValidator(new QDoubleValidator(ui_.lineedit_min_acc));
	ui_.lineedit_min_vel->setValidator(new QDoubleValidator(ui_.lineedit_min_vel));
	ui_.lineedit_poisson_diam->setValidator(new QDoubleValidator(ui_.lineedit_poisson_diam));
	ui_.lineedit_poisson_spread->setValidator(new QDoubleValidator(ui_.lineedit_poisson_spread));
	ui_.lineedit_radius_circ->setValidator(new QDoubleValidator(ui_.lineedit_radius_circ));
	ui_.lineedit_radius_sphere->setValidator(new QDoubleValidator(ui_.lineedit_radius_sphere));
	ui_.lineedit_rnd_max_dist->setValidator(new QDoubleValidator(ui_.lineedit_rnd_max_dist));
	ui_.lineedit_rnd_min_dist->setValidator(new QDoubleValidator(ui_.lineedit_rnd_min_dist));
	ui_.lineedit_rnd_steps->setValidator(new QIntValidator(0, INT_MAX, ui_.lineedit_rnd_steps));
	ui_.lineedit_robots->setValidator(new QIntValidator(0, INT_MAX, ui_.lineedit_robots));
	ui_.lineedit_seed->setValidator(new QIntValidator(0, INT_MAX, ui_.lineedit_seed));
	ui_.lineedit_sigma_acc->setValidator(new QDoubleValidator(ui_.lineedit_sigma_acc));
	ui_.lineedit_sigma_pos->setValidator(new QDoubleValidator(ui_.lineedit_sigma_pos));
	ui_.lineedit_sigma_vel->setValidator(new QDoubleValidator(ui_.lineedit_sigma_vel));
}

GeneratorWizard::~GeneratorWizard() {
}

void GeneratorWizard::accept() {
	namespace po = boost::program_options;

	data_.clear();

	// Files
	data_.insert(std::make_pair("algorithm", po::variable_value(ui_.lineedit_algorithm->text().toStdString(), false)));
	data_.insert(std::make_pair("swarmfile", po::variable_value(ui_.lineedit_swarm_file->text().toStdString(), false)));
	data_.insert(std::make_pair("robotfile", po::variable_value(ui_.lineedit_robot_file->text().toStdString(), false)));
	data_.insert(std::make_pair("obstaclefile", po::variable_value(ui_.lineedit_obstacle_file->text().toStdString(), false)));

	// Init
	data_.insert(std::make_pair("robots", po::variable_value(ui_.lineedit_robots->text().toUInt(), false)));
	data_.insert(std::make_pair("seed", po::variable_value(ui_.lineedit_seed->text().toUInt(), false)));

	// Request handler
	if(ui_.checkbox_acc_rh->isChecked())
		data_.insert(std::make_pair("add-acc-handler", po::variable_value(true, false)));
	if(ui_.checkbox_pos_rh->isChecked())
		data_.insert(std::make_pair("add-pos-handler", po::variable_value(true, false)));
	if(ui_.checkbox_vel_rh->isChecked())
		data_.insert(std::make_pair("add-vel-handler", po::variable_value(true, false)));

	// Coordinate System
	if(ui_.checkbox_cs_rot_x->isChecked())
		data_.insert(std::make_pair("cosys-rotate-x", po::variable_value(true, false)));
	if(ui_.checkbox_cs_rot_y->isChecked())
		data_.insert(std::make_pair("cosys-rotate-y", po::variable_value(true, false)));
	if(ui_.checkbox_cs_rot_z->isChecked())
		data_.insert(std::make_pair("cosys-rotate-z", po::variable_value(true, false)));
	if(ui_.checkbox_cs_scale_x->isChecked())
		data_.insert(std::make_pair("cosys-scale-x", po::variable_value(true, false)));
	if(ui_.checkbox_cs_scale_y->isChecked())
		data_.insert(std::make_pair("cosys-scale-y", po::variable_value(true, false)));
	if(ui_.checkbox_cs_scale_z->isChecked())
		data_.insert(std::make_pair("cosys-scale-z", po::variable_value(true, false)));

	// Acceleration/Velocity
	data_.insert(std::make_pair("max-acc", po::variable_value(ui_.lineedit_max_acc->text().toDouble(), false)));
	data_.insert(std::make_pair("min-acc", po::variable_value(ui_.lineedit_min_acc->text().toDouble(), false)));
	data_.insert(std::make_pair("max-vel", po::variable_value(ui_.lineedit_max_vel->text().toDouble(), false)));
	data_.insert(std::make_pair("min-vel", po::variable_value(ui_.lineedit_min_vel->text().toDouble(), false)));

	// Formations
	if(ui_.radiobutton_cube->isChecked()) {
		data_.insert(std::make_pair("distr-pos", po::variable_value(ui_.lineedit_diam_cube->text().toDouble(), false)));
	} else {
		data_.insert(std::make_pair("distr-pos", po::variable_value(0.0, true)));
	}

	if(ui_.radiobutton_circle->isChecked()) {
		data_.insert(std::make_pair("distr-pos-circle", po::variable_value(ui_.lineedit_radius_circ->text().toDouble(), false)));
		data_.insert(std::make_pair("distr-pos-circle-angle", po::variable_value(ui_.lineedit_angle_circ->text().toDouble(), false)));
	} else if(ui_.radiobutton_sphere->isChecked()) {
		data_.insert(std::make_pair("distr-uniform-on-sphere", po::variable_value(ui_.lineedit_radius_sphere->text().toDouble(), false)));
	} else if(ui_.radiobutton_gauss->isChecked()) {
		data_.insert(std::make_pair("distr-gauss_pos", po::variable_value(ui_.lineedit_diam_gauss->text().toDouble(), false)));
		data_.insert(std::make_pair("mean-acc", po::variable_value(ui_.lineedit_mean_acc->text().toDouble(), false)));
		data_.insert(std::make_pair("mean-pos", po::variable_value(ui_.lineedit_mean_pos->text().toDouble(), false)));
		data_.insert(std::make_pair("mean-vel", po::variable_value(ui_.lineedit_mean_vel->text().toDouble(), false)));
		data_.insert(std::make_pair("sigma-acc", po::variable_value(ui_.lineedit_sigma_acc->text().toDouble(), false)));
		data_.insert(std::make_pair("sigma-pos", po::variable_value(ui_.lineedit_sigma_pos->text().toDouble(), false)));
		data_.insert(std::make_pair("sigma-vel", po::variable_value(ui_.lineedit_sigma_vel->text().toDouble(), false)));
	} else if(ui_.radiobutton_rnd_walk->isChecked()) {
		data_.insert(std::make_pair("random-walk", po::variable_value(ui_.radiobutton_rnd_walk->isChecked(), false)));
		data_.insert(std::make_pair("rnd-max-distance", po::variable_value(ui_.lineedit_rnd_max_dist->text().toDouble(), false)));
		data_.insert(std::make_pair("rnd-min-distance", po::variable_value(ui_.lineedit_rnd_min_dist->text().toDouble(), false)));
		data_.insert(std::make_pair("rnd-walk-steps", po::variable_value(ui_.lineedit_rnd_steps->text().toUInt(), false)));
	} else if(ui_.radiobutton_cluster->isChecked()) {
		data_.insert(std::make_pair("cluster", po::variable_value(ui_.radiobutton_cluster->isChecked(), false)));
		data_.insert(std::make_pair("clt-clusters", po::variable_value(ui_.lineedit_cluster_num->text().toUInt(), false)));
		data_.insert(std::make_pair("clt-max-distance", po::variable_value(ui_.lineedit_cluster_max_dist->text().toDouble(), false)));
		data_.insert(std::make_pair("clt-min-distance", po::variable_value(ui_.lineedit_cluster_min_dist->text().toDouble(), false)));
		data_.insert(std::make_pair("clt-sigma", po::variable_value(ui_.lineedit_cluster_sigma->text().toDouble(), false)));
	} else if(ui_.radiobutton_poisson->isChecked()) {
		data_.insert(std::make_pair("poisson-disc", po::variable_value(ui_.radiobutton_poisson->isChecked(), false)));
		data_.insert(std::make_pair("pd-diameter", po::variable_value(ui_.lineedit_poisson_diam->text().toDouble(), false)));
		data_.insert(std::make_pair("pd-spread", po::variable_value(ui_.lineedit_poisson_spread->text().toDouble(), false)));
	}

	po::notify(data_);

	QWizard::accept();
}
