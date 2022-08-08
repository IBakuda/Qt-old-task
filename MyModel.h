#ifndef MYMODEL_H
#define MYMODEL_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>


struct Person{
    int id;
    QString description;
};

class MyModel : public QAbstractTableModel {
    Q_OBJECT
public:
    enum Column{ cId, cDescription, cCount };

    MyModel(QObject * parent = nullptr);

    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;

    bool setData(const QModelIndex &index, const QVariant &value,int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    void addPerson(int id, const QString & description);
    void removePerson(int row);
    bool checkBeforeRemove(int row);
    QString checkBeforeAdd(QString ID, QString TEXT);
    QString serializeXML();
    void deserializeXML(const QString &xml);

private:
    QMap<int,Person> _person;
};

#endif // MYMODEL_H
