﻿#include "converter.h"

ConverterModel::ConverterModel(QObject *parent): QAbstractTableModel(parent)
{

}

int ConverterModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return rows.count();
}

int ConverterModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    if (rows.count() == 0)
            return 0;

    return rows.first().count();
}

QVariant ConverterModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (!index.isValid())
            return QVariant();

        return rows.at(index.row()).at(index.column());
    }

    return QVariant();
}

QVariant ConverterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section < header.size()) return header[section];
            else return "";
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void ConverterModel::clearTable()
{
    if (columnCount(QModelIndex()) == 0)
        return;

    rows.clear();
    header.clear();
}
