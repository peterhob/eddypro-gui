/***************************************************************************
  customcombomodel.cpp
  -------------------
  Reimplement QStandardItemModel with disabled item feature
  -------------------
  Copyright (C) 2007-2011, Eco2s team, Antonio Forgione
  Copyright (C) 2011-2014, LI-COR Biosciences
  Author: Antonio Forgione

  This file is part of EddyPro (R).

  EddyPro (R) is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  EddyPro (R) is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with EddyPro (R). If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

#include "customcombomodel.h"

CustomComboModel::CustomComboModel(QObject *parent) : QStandardItemModel(parent)
{
}

Qt::ItemFlags CustomComboModel::flags(const QModelIndex & index) const
{
    Qt::ItemFlags flags = QStandardItemModel::flags(index);

    if (!index.isValid())
        return flags;

    QString value = index.data(Qt::UserRole).toString();
    if (value == QLatin1String("disabled"))
    {
        flags &= ~Qt::ItemIsSelectable;
        flags &= ~Qt::ItemIsEnabled;
    }

    return flags;
 }
