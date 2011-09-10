/*
 * help_dialog.h
 *
 *  Created on: 06.09.2011
 *      Author: Sascha Brandt
 */

#ifndef HELP_DIALOG_H_
#define HELP_DIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QAction>

#include "ui_help_dialog.h"

class HelpDialog : public QDialog {
	Q_OBJECT
public:
	HelpDialog(QWidget *parent = 0);
	virtual ~HelpDialog();

	void update_table(const QList<QAction*>& actions);

private:
	Ui::HelpDialog ui_;

};

#endif /* HELP_DIALOG_H_ */
