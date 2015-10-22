#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <vector>
#include <string>
#include "SimplePSD.h"
#include <qstandarditemmodel.h>
#include <map>
#include <QImage>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void setProgressBar(int n, int m);
    void setImage(PSDLayer *layer);

private slots:
    // add psd
    void on_pushButton_clicked();
    // click treeview checkbox
    void treeItemChanged (QStandardItem * item);
    // create psd
    void on_pushButton_2_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_treeView_clicked(const QModelIndex &index);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    void createPsdTreeView(std::string fileName, int id);
    void visitPsdTreeToTreeView(PSDLayer *root, QStandardItem* parent);
    void visitModelAddPath(QStandardItem *item, int id, QString subPath);

private:
    Ui::Dialog *ui;
    std::vector<std::string> m_psdName;
    std::vector<std::string> m_layerPath;
    std::vector<QStandardItemModel*> m_model;
    QStandardItemModel *m_listModel;
    //manage all psd tree
    SimplePSD m_simplePsd;
    std::multimap<QString, int> m_path;
    QImage *m_img;
    QImage *m_scaleImg;
    int m_id;
};

#endif // DIALOG_H
