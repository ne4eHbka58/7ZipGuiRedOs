#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QListWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QStringList>
#include <QLineEdit>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui{class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget*parent = nullptr);
    ~MainWindow();

private slots:
    void GetSelectedItem(const QModelIndex& index);
    void AddFiles();
    void CreateArchive();
    void ExecuteArchiveCommand(QStringList command);
    void dragEnterEvent(QDragEnterEvent *event); // Новый слот
    void dragMoveEvent(QDragMoveEvent *event);  // Новый слот
    void dropEvent(QDropEvent *event);       // Новый слот

private:
    Ui::MainWindow*ui;
    QFileSystemModel *FileSystemModel;
    QString PATH;
    QStringList archiveFiles;


    QString archivePassword;
    QString archiveFormat;
    QString archivePath;
    QString archiveCompression;
    QString archiveName;
    QStringList command;

};
#endif // MAINWINDOW_H
