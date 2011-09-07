/*
 * help_dialog.cpp
 *
 *  Created on: 06.09.2011
 *      Author: Sascha Brandt
 */

#include <Gui/help_dialog.h>

HelpDialog::HelpDialog(QWidget *parent) : QDialog(parent) {
	ui_.setupUi(this);
}

HelpDialog::~HelpDialog() {
}


void HelpDialog::update_table(const QList<QAction*>& actions) {

	ui_.table_action_keys->clearContents();
	ui_.table_action_keys->setRowCount(actions.size());

	int r=0;
	QList<QAction*>::const_iterator it;
	for( it = actions.begin(); it != actions.end(); ++it ) {
		if( !(*it)->shortcut().isEmpty() ) {
			ui_.table_action_keys->setItem(r, 0, new QTableWidgetItem((*it)->text().remove(QChar('&'))));
			ui_.table_action_keys->setItem(r++, 1, new QTableWidgetItem((*it)->shortcut().toString()));
		}
	}
}
