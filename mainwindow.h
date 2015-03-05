#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QFileSystemModel>
#include <QProcess>
#include <QSettings>
#include "storemodel.h"
#include "dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

enum actionType { GPG, GIT, REFRESH, EDIT };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setPassExecutable(QString);
    void setGitExecutable(QString);
    void setGpgExecutable(QString);
    void checkConfig();

private slots:
    void on_updateButton_clicked();
    void on_treeView_clicked(const QModelIndex &index);
    void on_configButton_clicked();
    void readyRead();
    void processFinished(int, QProcess::ExitStatus);
    void processError(QProcess::ProcessError);
    void clearClipboard();
    void on_lineEdit_textChanged(const QString &arg1);
    void on_lineEdit_returnPressed();
    void on_clearButton_clicked();
    void on_editButton_clicked();
    void on_saveButton_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel model;
    StoreModel proxyModel;
    QItemSelectionModel *selectionModel;
    QProcess *process;
    Dialog* d;
    bool usePass;
    bool useClipboard;
    bool useAutoclear;
    int autoclearSeconds;
    QString passStore;
    QString passExecutable;
    QString gitExecutable;
    QString gpgExecutable;   
    QString currentFile;
    actionType currentAction;
    void updateText();
    void executePass(QString);
    void executeWrapper(QString, QString);
    void config();
    void enableUiElements(bool);
    void selectFirstFile();
    void openFile(QString);
    QModelIndex firstFile(QModelIndex parentIndex);
};

#endif // MAINWINDOW_H
