#include "MyModel.h"
#include <QColor>
#include <QFile>
#include <QDomDocument>
#include <QMessageBox>

MyModel::MyModel(QObject *parent) : QAbstractTableModel(parent) {}

int MyModel::rowCount(const QModelIndex &index) const
{
    return _person.size();
}

int MyModel::columnCount(const QModelIndex &index) const{ return cCount; }

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case cId:
        {
            return _person[row].id;
        }
        case cDescription:
            return _person[row].description;
        }
    }

    if(role == Qt::ToolTipRole)
    {

        switch (index.column())
        {
        case cId:
            return QString("Person :") + _person[index.row()].description;
        }

    }

    if(role == Qt::BackgroundRole)
    {
        if(index.row() % 2 == 0)
        {
            return QColor("#FFFFFF");
        } else
            return QColor("#c9c9c9");
    }
    return QVariant();
}

QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        if(role == Qt::DisplayRole) {
            switch (section) {
            case cId:
                return "ID";
            case cDescription:
                return "TEXT";
            }
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

bool MyModel::setData(const QModelIndex &index, const QVariant &value, int role) //Обновление Элементов
{
    if(index.column() == cId)
    {
        _person[index.row()].id = value.toInt();
    }

    else if(index.column() == cDescription)
    {
        _person[index.row()].description = value.toString();
    }

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags MyModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

void MyModel::addPerson(int id, const QString &description)
{

    beginInsertRows(QModelIndex(), _person.size(), _person.size());
    int i;

    if(_person.isEmpty())
        i = 0;
    else
        i = _person.lastKey()+1;

    Person s;
    s.id = id;
    s.description = description;
    _person.insert(i,s);
    endInsertRows();
}

void MyModel::removePerson(int row)
{

    beginRemoveRows(QModelIndex(), row, row);

    for(int i = row+1;i < _person.size();i++)
    {
        _person[i-1] = _person[i];
    }

    for(auto it = _person.begin(); it!=_person.end();)
        if(it.key() == _person.lastKey())
            it = _person.erase(it);
        else
            ++it;

    endRemoveRows();
}

bool MyModel::checkBeforeRemove(int row)
{

    if(row<0)
        return false;
    else
        return true;
}

QString MyModel::checkBeforeAdd(QString ID, QString TEXT)
{

    bool ok;
    ID.toInt(&ok, 10);

    if(ID == "")
    {
        return "EmptyID";
    }

    if(TEXT == "")
    {
        return "EmptyTEXT";
    }

    else if(ok == false)
    {
        return "IDNotInt";
    }

    return "OK";
}


QString MyModel::serializeXML() {
  QDomDocument document;

  QDomElement root = document.createElement("PersonDataBase");
  document.appendChild(root);

  for (Person s : _person) {
    QDomElement studentNode = document.createElement("Person");
    studentNode.setAttribute("id", s.id);

    QDomElement descriptionNode = document.createElement("Description");
    descriptionNode.appendChild(document.createTextNode(s.description));
    studentNode.appendChild(descriptionNode);

    root.appendChild(studentNode);
  }

  return document.toString();
}

void MyModel::deserializeXML(const QString &xml)
{
    beginResetModel();
    int i = 0;
    _person.clear();

    QDomDocument document;
    if (!document.setContent(xml))
    return;

    QDomElement root = document.firstChildElement("PersonDataBase");

    QDomElement personNode = root.firstChildElement("Person");
    while (!personNode.isNull()) {
    Person subg;

    subg.id = personNode.attributeNode("id").value().toInt();
    QDomElement fioNode = personNode.firstChildElement("Description");
    subg.description = fioNode.firstChild().nodeValue();

    _person.insert(i,subg);
    i++;
    personNode = personNode.nextSiblingElement("Person");
}

    endResetModel();
}
