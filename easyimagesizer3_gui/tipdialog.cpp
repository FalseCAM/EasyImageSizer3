/* (C) 2011 FalseCAM
 This file is part of EasyImageSizer-GUI.

 EasyImageSizer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 any later version.

 EasyImageSizer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with EasyImageSizer.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tipdialog.h"
#include "ui_tipdialog.h"
#include <QSettings>

TipDialog::TipDialog(QWidget *parent) :
	QDialog(parent), ui(new Ui::TipDialog) {
	this->tips = new QStringList();
	this->ui->setupUi(this);
	this->tipNumber = 0;
	loadConfig();
	loadTips();
	setTip();
}

TipDialog::~TipDialog() {
	this->nextTip();
	saveConfig();
	delete ui;
	delete this->tips;
}

void TipDialog::startUp() {
	if (ui->checkBox->isChecked())
		this->exec();
}

void TipDialog::setTip() {
	if (tipNumber > tips->size() - 1)
		tipNumber = 0;
	if (tipNumber < 0)
		tipNumber = tips->size() - 1;
	ui->textField->setText(tips->at(tipNumber));
}

void TipDialog::nextTip() {
	tipNumber++;
	setTip();
}

void TipDialog::previousTip() {
	tipNumber--;
	setTip();
}

/*
 Loads a list of tips to show them in the Dialog.
 */
void TipDialog::loadTips() {
	qDebug("[TipDialog] loading Tips");
	tips->append(
			QString("Functions of this program are organized in plugins.<br/>"
				"You have to config and activate plugins first, to use them."));
	tips->append(
			QString("Activated plugins can be configured through their widget."));
	qDebug("[TipDialog] Tips loaded");
}

/*
 Loads configuration
 */
void TipDialog::loadConfig() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	qDebug("[TipDialog] settings path: %s / %s",
			qPrintable(QCoreApplication::organizationName()),
			qPrintable(QCoreApplication::applicationName()));

	ui->checkBox->setChecked(settings.value("showTips", true).toBool());
	tipNumber = settings.value("tipNumber", QVariant(0)).toInt();
	qDebug("[TipDialog] loaded config for TipDialog");
}

/*
 Saves configuration
 Windows saves to regestry.
 Unix to config file in home folder
 */
void TipDialog::saveConfig() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
	qDebug("[TipDialog] settings path: %s / %s",
			qPrintable(QCoreApplication::organizationName()),
			qPrintable(QCoreApplication::applicationName()));
	settings.setValue("showTips", ui->checkBox->isChecked());
	settings.setValue("tipNumber", QVariant(tipNumber));
	qDebug("[TipDialog] saved config for TipDialog");
}

void TipDialog::on_previousButton_clicked() {
	this->previousTip();
}

void TipDialog::on_nextButton_clicked() {
	this->nextTip();
}
