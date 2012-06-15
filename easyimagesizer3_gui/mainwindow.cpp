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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "easyimagesizer3/easyimagesizer3plugin.h"
#include "easyimagesizer3/easyimagesizer3.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QList>
#include <QIODevice>
#include <QImageReader>
#include <QSettings>
#include <QUrl>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QSplashScreen>
#include <QElapsedTimer>
#include "tipdialog.h"

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent), ui(new Ui::MainWindow) {
	/*
	 Splash screen on starting.
	 */
	QSplashScreen *splash = new QSplashScreen();
	splash->setPixmap(QPixmap(":/image/eis-large.png"));
	splash->show();
	splash->showMessage("Starting EasyImageSizer ...",
			Qt::AlignTop | Qt::AlignRight, Qt::white);
	easyImageSizer = new EasyImageSizer3();
        splash->showMessage("Loading gui ...", Qt::AlignTop | Qt::AlignRight,
                        Qt::white);
	form = new Form();
	ui->setupUi(this);
	QApplication::setWindowIcon(easyImageSizer->applicationIcon());
	initSignalSlots();
	splash->showMessage("Loading plugins ...", Qt::AlignTop | Qt::AlignRight,
			Qt::white);
	loadPluginWidgets();

	this->setWindowTitle(
			QCoreApplication::applicationName() + " "
					+ QCoreApplication::applicationVersion());

	splash->showMessage("Loading config ...", Qt::AlignTop | Qt::AlignRight,
			Qt::white);
	loadConfig();
	TipDialog(this).startUp();
	splash->finish(this);
	delete splash;
}
/*
 This class connects actions/signals to functions/slots
 */
void MainWindow::initSignalSlots() {

	connect(this, SIGNAL(image_list_Changed(int)), this,
			SLOT(imageListChanged(int)));
	connect(ui->ShowIconCheckBox, SIGNAL(toggled(bool)), this,
			SLOT(changeShowImages()));
	connect(ui->actionMore_functions, SIGNAL(triggered()), this,
			SLOT(configPlugins()));

	QList<EasyImageSizer3Plugin*> plugins = easyImageSizer->getPlugins();
	for (int i = 0; i < plugins.count(); i++) {
        connect(easyImageSizer->getPlugins().at(i)->getObject(), SIGNAL(progress(int)),
				ui->taskProgressBar, SLOT(setValue(int)));
	}
}

MainWindow::~MainWindow() {
	saveConfig();
	delete easyImageSizer;
	delete form;
    delete ui;
    qDebug("[MainWindow] destructor finished");
}

void MainWindow::loadPluginWidgets() {
	// Adds in PluginLoader loaded plugins to tabWidget
	qDebug("[MainWindow] loads plugin Widgets");

	ui->tabWidget->clear();

	ui->tabWidget->addTab(form, form->getTitle());

	foreach (EasyImageSizer3Plugin *plugin, easyImageSizer->getPlugins())
	{
		if (plugin)
            ui->tabWidget->addTab(plugin->createWidget(), plugin->getTitle());
	}
}

/*
 Opens a dialog that shows tips for using this program.
 */
void MainWindow::showTips() {
	TipDialog(this).exec();
}

/*
 Loads config usings Qts settings class
 */
void MainWindow::loadConfig() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
}

/*
 Saves config using Qts settings class.
 */
void MainWindow::saveConfig() {
	QSettings settings(QCoreApplication::organizationName(),
			QCoreApplication::applicationName());
}

void MainWindow::addImage() {
	// Last open dir is last file dir
	if (imagesList.count() > 0)
		this->lastOpenFileDir = QDir(imagesList.last()).absolutePath();
	// supported image formats
	QString supportedFormats;
	foreach(QByteArray format, QImageReader::supportedImageFormats())
	{
		supportedFormats.append(QString(" *.").append(format));
	}
	// Create File Dialog and open selected Files
	QStringList files = QFileDialog::getOpenFileNames(this,
			tr("Please select a file"), this->lastOpenFileDir,
			"All Supported Image Files (" + supportedFormats + ");;"
					"Joint Photographic Experts Group File (*.jpg *.jpeg);;"
					"All Files (*.*)");
	if (files.isEmpty())
		return;
	// checks for existing dir and saves last dialog opened dir
	if (QDir(QFileInfo(files.at(0)).absoluteDir()).exists())
		this->lastOpenFileDir =
				QFileInfo(files.at(0)).absoluteDir().absolutePath();

	addImages(files);
}

void MainWindow::addDir() {
	// Last open dir is last file dir
	if (imagesList.count() > 0)
		this->lastOpenFileDir = QDir(imagesList.last()).absolutePath();
	// Create dir dialog to choose a directory
	QString dirString = QFileDialog::getExistingDirectory(this,
			tr("Please select a directory"), this->lastOpenFileDir);
	// return if no Dir is chosen or if Dir is non existent
	if (dirString.isEmpty())
		return;
	if (!QDir(dirString).exists())
		return;
	// save chosen dir to show the same dir next File Dialog
	this->lastOpenFileDir = dirString;
	addDir(dirString);
}
// Adds a dir and all its subdirs
void MainWindow::addRDir() {
	// Last open dir is last file dir
	if (imagesList.count() > 0)
		this->lastOpenFileDir = QDir(imagesList.last()).absolutePath();
	// Create dir dialog to choose a directory
	QString dirString = QFileDialog::getExistingDirectory(this,
			tr("Please select a directory"), this->lastOpenFileDir);
	// return if no Dir is chosen or if Dir is non existent
	if (dirString.isEmpty())
		return;
	if (!QDir(dirString).exists())
		return;
	// save chosen dir to show the same dir next File Dialog
	this->lastOpenFileDir = dirString;
	addRDir(dirString);
}

// Add Images to Image List
void MainWindow::addImages(QStringList files) {
	// Check if a file exists
	if (!files.isEmpty()) {
		for (int i = 0; i < files.count(); i++) {
			//foreach(QString file, files){
			ui->overallProgressBar->setValue(
					int((i + 1) * 100 / files.count()));
			if (QFile().exists(files.at(i)) && !fileAlreadyAdded(files.at(i))) {
				imagesList.append(files.at(i));
				emit image_list_Changed(this->imagesList.count() - 1);
			}
		}
		ui->overallProgressBar->setValue(0);
	}
}

void MainWindow::addDir(QString dirString) {
	if (QDir().exists(dirString)) {
		QDir dir = QDir(dirString);
		// supported image formats
		QStringList supportedFormats;
		foreach(QByteArray format, QImageReader::supportedImageFormats())
		{
			supportedFormats << QString("*.").append(format);
		}

		// Set Filter to only load Image Files
		dir.setNameFilters(supportedFormats);
		QFileInfoList finfo = dir.entryInfoList(QDir::Files);
		QStringList files;
		// generate file list
		for (int i = 0; i < finfo.count(); i++) {
			files.append(finfo.at(i).absoluteFilePath());
		}
		// adds files to Image List
		this->addImages(files);
	}
}

void MainWindow::addRDir(QString dirString) {
	if (QDir().exists(dirString)) {
		QDir dir = QDir(dirString);
		QFileInfoList finfo = dir.entryInfoList(
				QDir::NoDotAndDotDot | QDir::Dirs);
		for (int i = 0; i < finfo.count(); i++) {
			addRDir(finfo.at(i).absoluteFilePath());
		}
		this->addDir(dirString);
	}
}

/*
 Checks if file is already added to list.
 */
bool MainWindow::fileAlreadyAdded(QString file) {
	if (this->imagesList.contains(file))
		return true;
	return false;
}

// loads preview of an Image in Image List
void MainWindow::imageListChanged(int index) {
	QStringList files = imagesList;
	if (index < 0 || index > files.count())
		return;
	// Process Bar
	ui->taskProgressBar->setValue(33);
	// check if file exists
	if (files.at(index) == "")
		return;
	if (!QFile(files.at(index)).exists())
		return;
	QListWidgetItem *item = new QListWidgetItem(files.at(index));
	if (this->ui->ShowIconCheckBox->isChecked()) {
		QIcon icon;
		QPixmap pixmap;
		// make copy of scaled image
		pixmap = QPixmap(files.at(index)).scaledToHeight(48);
		ui->taskProgressBar->setValue(66);
		// open Image to use it as Icon
		icon = QIcon(pixmap);
		// Process Bar
		ui->overallProgressBar->setValue(100);
		// create List Item
		item->setIcon(icon);
	}
	// add Item to List
	ui->FilesList->addItem(item);
	// Process Bar
	ui->taskProgressBar->setValue(0);
	// Process Bar
	ui->taskProgressBar->setValue(0);
}

// Clears List of images
void MainWindow::clearList() {
	this->imagesList.clear();
	ui->FilesList->clear();
}

// remove Image
void MainWindow::removeImage() {
	this->imagesList.removeAt(ui->FilesList->currentRow());
	ui->FilesList->takeItem(ui->FilesList->currentRow());
}

// Drag and Drop
void MainWindow::dragEnterEvent(QDragEnterEvent* event) {
	// Drag part
	const QMimeData* md = event->mimeData();
	if (event && (md->hasUrls()))
		event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent* event) {
	// Drop part
	if (event && event->mimeData()) {
		const QMimeData *mData = event->mimeData();
		// Drop Images from FileManager into ImageList
		if (mData->hasUrls()) {
			QList<QUrl> urls = mData->urls();
			QStringList files;
			for (int x = 0; x < urls.count(); ++x) {
				if (QFileInfo(urls.at(x).toLocalFile()).isDir())
					this->addRDir(urls.at(x).toLocalFile());
				else
					this->addImages(QStringList(urls.at(x).toLocalFile()));
			}
		}
	}
}

// show images in ImageListWidget
void MainWindow::changeShowImages() {
	for (int i = 0; i < this->ui->FilesList->count(); ++i) {

		if (this->ui->ShowIconCheckBox->isChecked()) {
			// Process Bar
			ui->overallProgressBar->setValue(33);
			ui->taskProgressBar->setValue(
					int((i + 1) * 100 / this->ui->FilesList->count()));
			QIcon icon;
			QPixmap pixmap;
			// make copy of scaled image
			pixmap =
					QPixmap(this->ui->FilesList->item(i)->text()).scaledToHeight(
							48);
			ui->overallProgressBar->setValue(66);
			// open Image to use it as Icon
			icon = QIcon(pixmap);
			// Process Bar
			ui->overallProgressBar->setValue(100);
			// add Icon to Item
			QListWidgetItem *item = new QListWidgetItem(
					this->ui->FilesList->item(i)->text());
			item->setIcon(icon);
			// remove old item and insert new item
			ui->FilesList->takeItem(i);
			ui->FilesList->insertItem(i, item);
			// Process Bar
			ui->overallProgressBar->setValue(0);
		} else {
			// create new item without icon
			QListWidgetItem *item = new QListWidgetItem(
					this->ui->FilesList->item(i)->text());
			// remove old item and insert new item
			ui->FilesList->takeItem(i);
			ui->FilesList->insertItem(i, item);
		}

	}
	// Process Bar
	ui->overallProgressBar->setValue(0);
	ui->taskProgressBar->setValue(0);
}

/*
 Shows a dialog where plugins can be activated or deactivated.
 */
void MainWindow::configPlugins() {
	easyImageSizer->configPlugins(this);
	loadPluginWidgets();
}

/*
 Starts retranslation of UI
 */
void MainWindow::retranslateUi() {
	this->ui->retranslateUi(this);
}

/*
 Shows a dialog where plugins can be activated or deactivated.
 */
void MainWindow::on_actionConfig_triggered() {
	configPlugins();
}

void MainWindow::on_actionTips_triggered() {
	this->showTips();
}

/*
 About boxes that show information for PhotosDD-GUI,
 PhotosDD library and QT library.
 */
void MainWindow::on_actionAbout_EasyImageSizer_GUI_triggered() {
	QMessageBox aboutBox;
	aboutBox.about(
			0,
			tr("About ") + QCoreApplication::applicationName(),
			("<p align=\"right\"><h1>") + QCoreApplication::applicationName()
					+ ("</h1></p>")
					+ ("<p align=\"left\"><font size=\"2\" color=\"grey\">")
					+ tr("Version ") + QCoreApplication::applicationVersion()
                    + ("</font>") + ("<p align=\"left\">(C) 2012 <a href=\"")
					+ QCoreApplication::organizationDomain() + ("\">")
					+ QCoreApplication::organizationName() + ("</a></p>")
					+ ("<p><a href=\"http://easyimagesizer.sourceforge.net/\">")
					+ tr("Homepage") + ("</a></p>") + ("<br/><br/><p>")
					+ tr("English translation: FalseCAM") + ("</p>"));
}

void MainWindow::on_actionAbout_EasyImageSizer_triggered() {
	EasyImageSizer3::about(0);
}

void MainWindow::on_actionAbout_QT_triggered() {
	QMessageBox::aboutQt(0, tr("About QT"));
}

void MainWindow::on_AddImageButton_clicked() {
	this->addImage();
}

void MainWindow::on_AddDirButton_clicked() {
	this->addDir();
}

void MainWindow::on_ClearImageListButton_clicked() {
	this->clearList();
}

void MainWindow::on_RemoveImageButton_clicked() {
	this->removeImage();
}

void MainWindow::on_commandLinkButton_clicked() {
    connect(this->easyImageSizer, SIGNAL(progress(int)), this->ui->overallProgressBar, SLOT(setValue(int)));
	ui->taskProgressBar->setValue(0);
	ui->overallProgressBar->setValue(0);
        QElapsedTimer timer;
        timer.start();
	if (form->getSaveTo()) {
		this->easyImageSizer->convert(this->imagesList, form->getSaveFolder(),
				this->form->getImageFormat(), this->form->getImageQuality(),
				this->form->getCopyMetaData());
	} else {
		this->easyImageSizer->convert(this->imagesList, "",
				this->form->getImageFormat(), this->form->getImageQuality(),
				this->form->getCopyMetaData());
	}
    this->easyImageSizer->start();
    // hold gui responsive
    while(this->easyImageSizer->isRunning())
        QCoreApplication::processEvents();

    // Shows information MessageBox: Finished in X milliseconds
    QMessageBox::information(this, "Finished", tr("EasyImageSizer converted %1 files in %2 milliseconds.").arg(this->imagesList.size()).arg(timer.elapsed()));
}
