#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QProcess>
#include <QMimeData>
#include <QUrl>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    FileSystemModel = new QFileSystemModel(this);
    FileSystemModel -> setRootPath(QDir::homePath());
    ui -> FileView -> setModel(FileSystemModel);
    ui -> FileView -> setRootIndex(FileSystemModel -> index(QDir::homePath()));
    connect(ui->FileView, &QTreeView::clicked, this, &MainWindow::GetSelectedItem);
    connect(ui->AddButton, &QPushButton::clicked, this, &MainWindow::AddFiles);
    connect(ui->ArchivateButton, &QPushButton::clicked, this, &MainWindow::CreateArchive);
    ui->ArchievView->setAcceptDrops(true);
    PATH = "";
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::GetSelectedItem(const QModelIndex& index){
    QString FilePath = FileSystemModel -> filePath(index);
    PATH = FilePath;
}

void MainWindow::AddFiles(){
    if(!archiveFiles.contains(PATH)){
        archiveFiles.append(PATH);
        ui->ArchievView->addItem(PATH);
    }
    PATH = "";
}

void MainWindow::CreateArchive(){
    archivePassword = ui->PasswordEdit->text();
    archivePath = QFileDialog::getSaveFileName(this, "Сохранить архив", "", "");
    if (archivePath.isEmpty()){
        return;
    }
    archiveFormat = ui->FormatType->currentText();
    archiveName = archivePath + "." + archiveFormat;
    archiveCompression = ui->CompressType->currentText();

    QStringList command;
    command << "7z" << "a";

    if (!archivePassword.isEmpty()){
        command << "-p" + archivePassword;
    }

    command << "-" + archiveCompression << archiveName;

    command.append(archiveFiles);


    ExecuteArchiveCommand(command);
    archiveFiles.clear();
    ui->ArchievView->clear();
}

void MainWindow::ExecuteArchiveCommand(QStringList command){
    QProcess *process = new QProcess(this);
    process->start(command.at(0), command.mid(1));
    command.clear();
}


void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        QList<QUrl> urls = mimeData->urls();
        for (const QUrl &url : urls)
        {
            QString file_path = url.toLocalFile();
            if (!file_path.isEmpty())
            {
                if(!archiveFiles.contains(file_path))
                {
                    archiveFiles.append(file_path);
                    ui->ArchievView->addItem(file_path);
                }
            }
        }
    }
}

