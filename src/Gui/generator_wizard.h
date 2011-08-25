/*
 * GeneratorWizard.h
 *
 *  Created on: 27.07.2011
 *      Author: Sascha Brandt
 */

#ifndef GENERATORWIZARD_H_
#define GENERATORWIZARD_H_

#include <boost/program_options.hpp>

#include <QtGui/QWizard>

#include "ui_generator_wizard.h"

class GeneratorWizard : public QWizard {
	Q_OBJECT

public:
	GeneratorWizard(QWidget *parent = 0);
	~GeneratorWizard();

	const boost::program_options::variables_map& generator_data() const { return data_; };

public slots:
	void accept();

private:
	Ui::GeneratorWizard ui_;
	boost::program_options::variables_map data_;
};

#endif /* GENERATORWIZARD_H_ */
