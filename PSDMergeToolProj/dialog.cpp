#include "dialog.h"
#include "ui_dialog.h"
#include <qfiledialog.h>
#include <QPixmap>
#include <math.h>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    m_listModel = new QStandardItemModel(ui->listView);
    ui->listView->setModel(m_listModel);
    m_simplePsd.dialog = this;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::visitPsdTreeToTreeView(PSDLayer *layer, QStandardItem* item){
    if(layer){
        if(layer->m_pixels){
            item->setCheckable(true);
        }
        else{
            item->setCheckable(true);
            item->setTristate(true);
        }
        for(int i=layer->m_children.size()-1; i>=0; i--){
            QStandardItem* childItem = new QStandardItem(QString::fromLocal8Bit(layer->m_children[i]->m_name.c_str()));
            item->appendRow(childItem);
            visitPsdTreeToTreeView(layer->m_children[i], childItem);
        }
    }
}

// create model for treeview
void Dialog::createPsdTreeView(std::string fileName, int id){
    // create a psd tree(psdlayer)
    std::vector<std::string> str;
    str.push_back(fileName);
    m_simplePsd.createForest(str);

    // create treeview model
    m_model.push_back(new QStandardItemModel(ui->treeView));
    m_model[id]->setHorizontalHeaderLabels(QStringList(QString::fromLocal8Bit(fileName.c_str())));
    connect(m_model[id] , &QStandardItemModel::itemChanged , this, &Dialog::treeItemChanged );
    for(int i=0; i!=m_simplePsd.m_forest[id]->m_children.size(); i++){
        PSDLayer *layer = m_simplePsd.m_forest[id]->m_children[i];
        QStandardItem* item = new QStandardItem(QString::fromLocal8Bit(layer->m_name.c_str()));
        m_model[id]->appendRow(item);
        visitPsdTreeToTreeView(layer, item);
    }
}

void Dialog::setImage(PSDLayer *layer){
    m_img = new QImage((uchar *)(layer->m_pixels), layer->m_width, layer->m_height, QImage::Format_ARGB32);
    m_scaleImg = new QImage((uchar *)(layer->m_pixels), layer->m_width, layer->m_height, QImage::Format_ARGB32);
    if(m_img->width() > ui->label_3->width() || m_img->height() > ui->label_3->height()){
        *m_img = m_img->scaled(1.0f*m_img->width()*ui->label_3->width()/(m_img->width()+100),
                             1.0f*m_img->height()*ui->label_3->height()/(m_img->height()+100),  Qt::KeepAspectRatio);
        *m_scaleImg = *m_img;
    }

    ui->label_3->setPixmap(QPixmap::fromImage(*m_img));

    // image info
    QString name = QString::fromLocal8Bit(layer->m_name.c_str());
    QString imageInfo = QString("图片信息：\n名字：%1\n宽：%2\n高：%3\n").arg(name).arg(layer->m_width).arg(layer->m_height);
    ui->label_imageInfo->setText(imageInfo);
}

// add psd file
void Dialog::on_pushButton_clicked()
{
    // check if choose file to open
    QStringList fileList = QFileDialog::getOpenFileNames(this, "打开PSD文件", "", "PSD(*.psd)");
    if(fileList.empty()) return;

    //prevent repeat
    for(int i=0; i!=fileList.size(); i++){
        bool bOpened = false;
        for(int j=0; j!=m_psdName.size(); j++){
            if(fileList[i].toStdString() == m_psdName[j]){
                bOpened = true;
                QString text =  QString("%1 已经打开了！").arg(fileList[i]);
                QMessageBox::information(this, "打开PSD文件", text);
                break;
            }
        }

        if(!bOpened){
            // add to listmodel
            m_listModel->appendRow(new QStandardItem(fileList[i]));
            m_psdName.push_back(fileList[i].toStdString());
            createPsdTreeView(std::string(fileList[i].toLocal8Bit()), m_psdName.size()-1);
        }
    }


}

void treeItem_checkAllChild(QStandardItem * item, bool check)
{
    if(item == nullptr)
        return;
    if(item->isCheckable())
        item->setCheckState(check ? Qt::Checked : Qt::Unchecked);

    int rowCount = item->rowCount();
    for(int i=0;i<rowCount;++i)
    {
        QStandardItem* childItem = item->child(i);
        treeItem_checkAllChild(childItem,check);
    }
}

Qt::CheckState checkSibling(QStandardItem * item)
{
    //先通过父节点获取兄弟节点
    QStandardItem * parent = item->parent();
    if(nullptr == parent)
        return item->checkState();
    int brotherCount = parent->rowCount();
    int checkedCount(0),unCheckedCount(0);
    Qt::CheckState state;
    for(int i=0;i<brotherCount;++i)
    {
        QStandardItem* siblingItem = parent->child(i);
        state = siblingItem->checkState();
        if(Qt::PartiallyChecked == state)
            return Qt::PartiallyChecked;
        else if(Qt::Unchecked == state)
            ++unCheckedCount;
        else
            ++checkedCount;
        if(checkedCount>0 && unCheckedCount>0)
            return Qt::PartiallyChecked;
    }
    if(unCheckedCount>0)
        return Qt::Unchecked;
    return Qt::Checked;
}

void treeItem_CheckChildChanged(QStandardItem * item)
{
    if(nullptr == item)
        return;
    Qt::CheckState siblingState = checkSibling(item);
    QStandardItem * parentItem = item->parent();
    if(nullptr == parentItem)
        return;
    if(parentItem->isCheckable()){
         parentItem->setCheckState(siblingState);
    }

    treeItem_CheckChildChanged(parentItem);
}


//首先要判断条目的状态，如果条目是有复选框的话，那么就进行操作。通过函数isCheckable（）可以判断条目是否有复选框
//在确认条目有复选框后，需要获取当前条目的选中状态，使用checkState ()函数可以判断当前条目的选中状态;
//现在分两种情况：
//1.如果条目是三态的，说明要判断它的子节点。条目选中时，所有子节点都将选中，条目不选中时，所有子节点都不选中
//2.如果条目是两态的，说明可能会影响它的三态的父节点，当两态节点选中且其所有的兄弟节点都选中，三态父节点选中，若两态子节点和其兄弟节点都没选中，那么其三态父节点将不选中，若果兄弟节点有选中有不选中，三态父节点将是处于不完全选中状态
//tree item changed slot
 void Dialog::treeItemChanged(QStandardItem * item){
     if ( item == nullptr )
        return ;
        if(item->isCheckable())
        {
            //如果条目是存在复选框的，那么就进行下面的操作
            Qt::CheckState state = item->checkState(); //获取当前的选择状态
            if (item->isTristate())
            {
                 //如果条目是三态的，说明可以对子目录进行全选和全不选的设置
                if (state != Qt::PartiallyChecked )
                {
                    //当前是选中状态，需要对其子项目进行全选
                    treeItem_checkAllChild(item, state == Qt::Checked ? true : false );
                }
            }
            else
            {
                //说明是两态的，两态会对父级的三态有影响
                //判断兄弟节点的情况
                treeItem_CheckChildChanged(item);
            }
        }

 }

 void Dialog::visitModelAddPath(QStandardItem *item, int id, QString path){
     // if item is checked then add to m_path
    if(item->checkState() == Qt::Checked){
        path += "/" + item->index().data().toString();
        m_path.insert(std::make_pair(path, id));
    }
    else if (item->checkState() == Qt::PartiallyChecked) {
        int childCount = item->rowCount();
        path += "/" + item->index().data().toString();
        for(int i=0; i!=childCount; i++){
             QStandardItem *childItem = item->child(i);
             visitModelAddPath(childItem, id, path);
        }
    }
 }

// merge psd and out psd
void Dialog::on_pushButton_2_clicked()
{
    // no psdtree model exist
    if(m_model.empty()) return;

    // not choose file to save
    QString fileSavePath = QFileDialog::getSaveFileName(this, QString("保存PSD文件"), tr("未命名"), "PSD(*.psd)");
    if(fileSavePath == "") return;

    // clear m_path
    m_path.clear();

    // add choosen path to m_path, m_path used to create psd
    for(int i=0; i!=m_model.size(); i++){
        for(int j=0; j!=m_model[i]->rowCount(); j++){
            QModelIndex index = m_model[i]->index(j, 0);
            QStandardItem *item = m_model[i]->itemFromIndex(index);
            visitModelAddPath(item, i, QString("/images"));
        }
    }

    if(m_path.empty()) return;

    // create psd
    std::multimap<QString, int>::iterator iter;
    for(iter = m_path.begin(); iter!=m_path.end(); iter++){
        std::string path = iter->first.toStdString();
        int id = iter->second;
        m_simplePsd.markForest(path, id);

        // for test
        QString text = ui->label->text();
        text += QString("\n选择路径:%1 PSD文件ID：%2").arg(iter->first.toStdString().c_str()).arg(iter->second);
        ui->label->setText(text);
    }

    m_simplePsd.createNewTree();
    m_simplePsd.writePsdByNewTree(std::string(fileSavePath.toLocal8Bit()));

    QMessageBox::information(this, "合并保存PSD", "合并保存PSD成功");
    QString text = ui->label->text();
    text += QString("\n合并保存PSD成功");
    ui->label->setText(text);
}

// click listview
void Dialog::on_listView_clicked(const QModelIndex &index)
{
    // set treeview model
    m_id = index.row();
    ui->treeView->setModel(m_model[m_id]);

    // expand treeview all row
    for(int i=0; i!=m_model[m_id]->rowCount(); i++){
        QModelIndex modelIndex = m_model[m_id]->index(i, 0);
        ui->treeView->expand(modelIndex);
    }
}

void Dialog::setProgressBar(int n, int m){
    ui->progressBar->setRange(0, m);
    ui->progressBar->setValue(n);
}

// click treeview
void Dialog::on_treeView_clicked(const QModelIndex &index)
{
    QString path("");
    QModelIndex modelIndex = index;
    while (modelIndex.isValid()) {
        path  = QString("/%1%2").arg(modelIndex.data().toString()).arg(path);
        modelIndex = modelIndex.parent();
    }
    if(path != QString("")){
        path  = QString("/images%1").arg(path);
        PSDLayer *layer = m_simplePsd.findPsdLayer(path.toStdString(), m_id);
        setImage(layer);
    }
}

void Dialog::on_pushButton_3_clicked()
{
    if(!m_scaleImg->isNull() && !m_img->isNull()){
       *m_scaleImg = m_img->scaled(m_scaleImg->width() + ceil(m_img->width()/10.0f),
                                    m_scaleImg->height() + ceil(m_img->height()/10.0f),  Qt::KeepAspectRatio);
        ui->label_3->setPixmap(QPixmap::fromImage(*m_scaleImg));
    }
}

void Dialog::on_pushButton_4_clicked()
{
    if(!m_scaleImg->isNull() && !m_img->isNull()){
       *m_scaleImg = m_img->scaled(m_scaleImg->width() - m_img->width()/10.0f,
                                   m_scaleImg->height() - m_img->height()/10.0f,  Qt::KeepAspectRatio);
       ui->label_3->setPixmap(QPixmap::fromImage(*m_scaleImg));
    }
}

void Dialog::on_pushButton_5_clicked()
{
    if(!m_scaleImg->isNull() && !m_img->isNull()){
        *m_scaleImg = *m_img;
        ui->label_3->setPixmap(QPixmap::fromImage(*m_img));
    }
}

void Dialog::on_pushButton_6_clicked()
{
    if(!m_scaleImg->isNull() && !m_img->isNull()){
        QString saveFilePath = QFileDialog::getSaveFileName(this, "保存图片", "未命名", "PNG(*.png);;JPG(*.jpg);;BMP(*.bmp);;GIF(*.gif)");
        if(saveFilePath != ""){
            m_img->save(saveFilePath);
            QMessageBox::information(this, "保存图片", "保存成功");
        }
    }
}
