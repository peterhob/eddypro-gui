/***************************************************************************
  advancedsettingspage.h
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

#ifndef ADVANCEDSETTINGSPAGE_H
#define ADVANCEDSETTINGSPAGE_H

#include <QWidget>

#include "configstate.h"

class QListWidget;

class AdvSettingsContainer;
class DlProject;
class EcProject;
class SmartFluxBar;

class AdvancedSettingsPage : public QWidget
{
    Q_OBJECT

public:
    AdvancedSettingsPage(QWidget* parent,
                         DlProject* dlProject,
                         EcProject* ecProject,
                         ConfigState* config);
    virtual ~AdvancedSettingsPage();

    AdvSettingsContainer* advancedSettingPages()
                            { return advancedSettingContainer; }
    void updateSmartfluxBar();

private slots:
    void changePage(int index);
    void resetButtonCLicked();

private:
    static const QString PAGE_TITLE_0;
    static const QString PAGE_TITLE_1;
    static const QString PAGE_TITLE_2;
    static const QString PAGE_TITLE_3;
    static const QString PAGE_TITLE_4;

    void createMenu();
    void createIcons();
    bool requestSettingsReset();

    DlProject* dlProject_;
    EcProject* ecProject_;
    ConfigState* configState_;

    SmartFluxBar* smartfluxBar {};
    QListWidget* menuWidget {};
    AdvSettingsContainer*  advancedSettingContainer {};
};

#endif // ADVANCEDSETTINGSPAGE_H