/*
 * GeneratorWizard.h
 *
 *  Created on: 27.07.2011
 *      Author: Sascha Brandt
 */

#ifndef GENERATORWIZARD_H_
#define GENERATORWIZARD_H_

#include <QtGui/QWizard>
#include "ui_generator_wizard.h"

class GeneratorWizard : public QWizard {
	Q_OBJECT

public:
	GeneratorWizard(QWidget *parent = 0);
	~GeneratorWizard();

private:
	Ui::GeneratorWizard _ui;
};

#endif /* GENERATORWIZARD_H_ */
