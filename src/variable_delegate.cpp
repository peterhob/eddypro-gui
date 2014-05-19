/***************************************************************************
  variable_delegate.cpp
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

#include <QDebug>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QAbstractItemView>
#include <QKeyEvent>
#include <QLabel>

#include "nonzerodoublespinbox.h"
#include "dbghelper.h"
#include "variable_model.h"
#include "alia.h"
#include "variable_delegate.h"

VariableDelegate::VariableDelegate(QObject *parent) :
    QItemDelegate(parent),
    varsBuffer_(new QStringList())
{
    installEventFilter(this);
    Alia::getCustomVariables(varsBuffer_.data());
}

VariableDelegate::~VariableDelegate()
{
    DEBUG_FUNC_NAME
}

QWidget *VariableDelegate::createEditor(QWidget* parent,
                                     const QStyleOptionViewItem& option,
                                     const QModelIndex& index) const
{
    QLabel *label;
    QComboBox *combo;
    QDoubleSpinBox *dspin;
    NonZeroDoubleSpinBox *nzdspin;

    int column = index.column();
    QString currentVar = index.model()->data(index.model()->index(VariableModel::VARIABLE, column)).toString();
    QString currentInputUnit = index.model()->data(index.model()->index(VariableModel::INPUTUNIT, column)).toString();

    // can only edit name on blank column
    if (column >= index.model()->columnCount()) return 0;

    // different kind of editor for each row
    switch (index.row())
    {
        case VariableModel::IGNORE:
            combo = new QComboBox(parent);
            combo->addItems(VariableDesc::yesNoStringList());
            combo->setMinimumWidth(130);
            connect(combo, SIGNAL(activated(int)),
                    this, SLOT(commitAndCloseEditor()));
            return combo;
        case VariableModel::NUMERIC:
            combo = new QComboBox(parent);
            combo->addItems(VariableDesc::yesNoStringList());
            combo->setMinimumWidth(130);
            connect(combo, SIGNAL(activated(int)),
                    this, SLOT(commitAndCloseEditor()));
            return combo;
        case VariableModel::VARIABLE:
            combo = new QComboBox(parent);
            combo->setEditable(true);
            combo->setMaxVisibleItems(40);
            combo->addItems(VariableDesc::variableStringList() + *varsBuffer_);
            combo->setMinimumWidth(130);
            combo->view()->setAlternatingRowColors(true);
            connect(combo, SIGNAL(activated(int)),
                    this, SLOT(commitAndCloseEditor()));
            return combo;
        case VariableModel::INSTRUMENT:
            combo = new QComboBox(parent);
            combo->setEditable(false);
            combo->addItems(((const VariableModel *)index.model())->instrModels());
            combo->setMinimumWidth(130);
            connect(combo, SIGNAL(activated(int)),
                    this, SLOT(commitAndCloseEditor()));
            return combo;
        case VariableModel::MEASURETYPE:
            if (!VariableDesc::isGasVariable(currentVar)
                && !VariableDesc::isCustomVariable(currentVar))
            {
                label = new QLabel(parent);
                return label;
            }
            else
            {
                combo = new QComboBox(parent);
                combo->setEditable(false);
                combo->addItems(VariableDesc::measureTypeStringList());
                combo->setMinimumWidth(130);
                connect(combo, SIGNAL(activated(int)),
                        this, SLOT(commitAndCloseEditor()));
                return combo;
            }
        case VariableModel::INPUTUNIT:
            combo = new QComboBox(parent);
            combo->setEditable(false);

            if (VariableDesc::isVelocityVar(currentVar))
            {
                combo->addItems(VariableDesc::velocityInputUnitStringList());
            }
            else if (VariableDesc::isAngleVar(currentVar))
            {
                combo->addItems(VariableDesc::angleInputUnitStringList());
            }
            else if (VariableDesc::isTemperatureVar(currentVar))
            {
                combo->addItems(VariableDesc::temperatureInputUnitStringList());
            }
            else if (VariableDesc::isPressureVar(currentVar))
            {
                combo->addItems(VariableDesc::pressureInputUnitStringList());
            }
            else if (VariableDesc::isGasVariable(currentVar))
            {
                combo->addItems(VariableDesc::gasInputUnitStringList());
            }
            else if (VariableDesc::isFlowRateVar(currentVar))
            {
                combo->addItems(VariableDesc::flowRateInputUnitStringList());
            }
            else if (VariableDesc::isDiagnosticVar(currentVar))
            {
                combo->addItem(VariableDesc::getVARIABLE_MEASURE_UNIT_STRING_17());
            }
            else if (VariableDesc::isCustomVariable(currentVar))
            {
                combo->addItems(VariableDesc::inputUnitStringList());
            }
            // empty
            else
            {
                combo->addItems(VariableDesc::inputUnitStringList());
            }

            combo->setMaxVisibleItems(40);
            combo->setMinimumWidth(130);
            connect(combo, SIGNAL(activated(int)),
                    this, SLOT(commitAndCloseEditor()));

            return combo;
        case VariableModel::CONVERSIONTYPE:
            if (!VariableDesc::isScalableVariable(currentInputUnit))
            {
                label = new QLabel(parent);
                return label;
            }
            else
            {
                combo = new QComboBox(parent);
                combo->setEditable(false);
                combo->setMaxVisibleItems(20);
                combo->addItems(VariableDesc::conversionTypeStringList());
                combo->setMinimumWidth(130);
                combo->view()->setAlternatingRowColors(true);
                connect(combo, SIGNAL(activated(int)),
                        this, SLOT(commitAndCloseEditor()));
                return combo;
            }
        case VariableModel::OUTPUTUNIT:
            if (!VariableDesc::isScalableVariable(currentInputUnit))
            {
                label = new QLabel(parent);
                return label;
            }
            else
            {
                combo = new QComboBox(parent);
                combo->setEditable(false);

                if (VariableDesc::isVelocityVar(currentVar))
                {
                    combo->addItems(VariableDesc::velocityOutputUnitStringList());
                }
                else if (VariableDesc::isAngleVar(currentVar))
                {
                    combo->addItems(VariableDesc::angleOutputUnitStringList());
                }
                else if (VariableDesc::isTemperatureVar(currentVar))
                {
                    combo->addItems(VariableDesc::temperatureOutputUnitStringList());
                }
                else if (VariableDesc::isPressureVar(currentVar))
                {
                    combo->addItems(VariableDesc::pressureOutputUnitStringList());
                }
                else if (VariableDesc::isGasVariable(currentVar))
                {
                    combo->addItems(VariableDesc::gasOutputUnitStringList());
                }
                else if (VariableDesc::isFlowRateVar(currentVar))
                {
                    combo->addItems(VariableDesc::flowRateOutputUnitStringList());
                }
                else if (VariableDesc::isDiagnosticVar(currentVar))
                {
                    combo->addItem(VariableDesc::getVARIABLE_MEASURE_UNIT_STRING_17());
                }
                else if (VariableDesc::isCustomVariable(currentVar))
                {
                    combo->addItems(VariableDesc::outputUnitStringList());
                }
                // currentVar is empty
                else
                {
                    combo->addItems(VariableDesc::outputUnitStringList());
                }

                combo->setMaxVisibleItems(40);
                combo->setMinimumWidth(130);
                connect(combo, SIGNAL(activated(int)),
                        this, SLOT(commitAndCloseEditor()));
                return combo;
            }
        case VariableModel::AVALUE:
        case VariableModel::BVALUE:
            if (!VariableDesc::isScalableVariable(currentInputUnit))
            {
                label = new QLabel(parent);
                return label;
            }
            else
            {
                nzdspin = new NonZeroDoubleSpinBox(parent);
                nzdspin->setDecimals(6);
                nzdspin->setRange(-999999.0, 999999.0);
                nzdspin->setSingleStep(1.0);
                nzdspin->setAccelerated(true);
                connect(nzdspin, SIGNAL(editingFinished()),
                        this, SLOT(commitAndCloseEditor()));
                return nzdspin;
            }
        case VariableModel::NOMTIMELAG:
        case VariableModel::MINTIMELAG:
        case VariableModel::MAXTIMELAG:
            dspin = new QDoubleSpinBox(parent);
            dspin->setDecimals(2);
            dspin->setRange(-60.0, 60.0);
            dspin->setSingleStep(1.0);
            dspin->setAccelerated(true);
            dspin->setSuffix(QStringLiteral(" [s]"));
            connect(dspin, SIGNAL(editingFinished()),
                    this, SLOT(commitAndCloseEditor()));
            return dspin;
        default:
            return QItemDelegate::createEditor(parent, option, index);
    }
}

void VariableDelegate::setEditorData(QWidget* editor,
                                  const QModelIndex& index) const
{
    QLabel *label;
    QComboBox *combo;
    QDoubleSpinBox *dspin;
    NonZeroDoubleSpinBox *nzdspin;

    int column = index.column();
    QString currentVar = index.model()->data(index.model()->index(VariableModel::VARIABLE, column)).toString();
    QString currentInputUnit = index.model()->data(index.model()->index(VariableModel::INPUTUNIT, column)).toString();

    QVariant value = index.model()->data(index, Qt::EditRole);
    QString stringValue = value.toString();

    // different kind of editor for each row
    switch (index.row())
    {
        case VariableModel::IGNORE:
        case VariableModel::NUMERIC:
        case VariableModel::INSTRUMENT:
        case VariableModel::INPUTUNIT:
            combo = static_cast<QComboBox*>(editor);
            if (!combo) return;
            combo->setCurrentIndex(combo->findText(stringValue));
            break;
        case VariableModel::VARIABLE:
            combo = static_cast<QComboBox*>(editor);
            if (!combo) return;

            // add the editable item which was selected but not present in the original combo
            if (combo->findText(stringValue) < 0)
            {
                combo->addItem(stringValue);
                varsBuffer_->append(stringValue);
                Alia::setCustomVariables(*varsBuffer_);
            }

            combo->setCurrentIndex(combo->findText(stringValue));
            break;
        case VariableModel::MEASURETYPE:
            if (!VariableDesc::isGasVariable(currentVar)
                && !VariableDesc::isCustomVariable(currentVar))
            {
                label = static_cast<QLabel*>(editor);
                if (!label) return;
            }
            else
            {
                combo = static_cast<QComboBox*>(editor);
                if (!combo) return;

                combo->setCurrentIndex(combo->findText(stringValue));
            }
            break;
        case VariableModel::CONVERSIONTYPE:
        case VariableModel::OUTPUTUNIT:
            if (!VariableDesc::isScalableVariable(currentInputUnit))
            {
                label = static_cast<QLabel*>(editor);
                if (!label) return;
            }
            else
            {
                combo = static_cast<QComboBox*>(editor);
                if (!combo) return;

                combo->setCurrentIndex(combo->findText(stringValue));
            }
            break;
        case VariableModel::AVALUE:
        case VariableModel::BVALUE:
            if (!VariableDesc::isScalableVariable(currentInputUnit))
            {
                label = static_cast<QLabel*>(editor);
                if (!label) return;
            }
            else
            {
                nzdspin = static_cast<NonZeroDoubleSpinBox*>(editor);
                if (!nzdspin) return;
                nzdspin->setValue(value.toReal());
            }
            break;
        case VariableModel::NOMTIMELAG:
        case VariableModel::MINTIMELAG:
        case VariableModel::MAXTIMELAG:
            dspin = static_cast<QDoubleSpinBox*>(editor);
            if (!dspin) return;
            dspin->setValue(value.toReal());
            break;
        default:
            QItemDelegate::setEditorData(editor, index);
            break;
    }
}

void VariableDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                const QModelIndex& index) const
{
    QComboBox *combo;
    QDoubleSpinBox *dspin;
    NonZeroDoubleSpinBox *nzdspin;
    QVariant value;
    QLabel *label;

    int row = index.row();
    int column = index.column();
    QString currentVar = index.model()->data(index.model()->index(VariableModel::VARIABLE, column)).toString();
    QString currentInputUnit = index.model()->data(index.model()->index(VariableModel::INPUTUNIT, column)).toString();

    // different kind of editor for each row
    switch (row)
    {
        case VariableModel::IGNORE:
        case VariableModel::NUMERIC:
        case VariableModel::VARIABLE:
        case VariableModel::INSTRUMENT:
        case VariableModel::INPUTUNIT:
            combo = static_cast<QComboBox*>(editor);
            if (!combo) return;
            value = combo->currentText();
            model->setData(index, value);
            break;
        case VariableModel::CONVERSIONTYPE:
        case VariableModel::OUTPUTUNIT:
            if (!VariableDesc::isScalableVariable(currentInputUnit))
            {
                label = static_cast<QLabel*>(editor);
                if (!label) return;
            }
            else
            {
                combo = static_cast<QComboBox*>(editor);
                if (!combo) return;
                value = combo->currentText();
                model->setData(index, value);
            }
            break;
        case VariableModel::MEASURETYPE:
            if (!VariableDesc::isGasVariable(currentVar)
                && !VariableDesc::isCustomVariable(currentVar))
            {
                label = static_cast<QLabel*>(editor);
                if (!label) return;
            }
            else
            {
                combo = static_cast<QComboBox*>(editor);
                if (!combo) return;
                value = combo->currentText();
                model->setData(index, value);
            }
            break;
        case VariableModel::AVALUE:
        case VariableModel::BVALUE:
            if (!VariableDesc::isScalableVariable(currentInputUnit))
            {
                label = static_cast<QLabel*>(editor);
                if (!label) return;
            }
            else
            {
                nzdspin = static_cast<NonZeroDoubleSpinBox*>(editor);
                if (!nzdspin) return;
                value = nzdspin->value();
                model->setData(index, value);
            }
            break;
        case VariableModel::NOMTIMELAG:
        case VariableModel::MINTIMELAG:
        case VariableModel::MAXTIMELAG:
            dspin = static_cast<QDoubleSpinBox*>(editor);
            if (!dspin) return;
            value = dspin->value();
            model->setData(index, value);
            break;
        default:
            QItemDelegate::setModelData(editor, model, index);
            break;
    }
}

void VariableDelegate::updateEditorGeometry(QWidget* editor,
                                        const QStyleOptionViewItem& option,
                                        const QModelIndex& index) const
{
    Q_UNUSED(index)

    if (editor) editor->setGeometry(option.rect);
}

void VariableDelegate::commitAndCloseEditor()
{
    DEBUG_FUNC_NAME
    QWidget* senderWidget = qobject_cast<QWidget *>(sender());

    emit commitData(senderWidget);
    emit closeEditor(senderWidget, QAbstractItemDelegate::NoHint);
}

void VariableDelegate::commitAndCloseEditor(QObject* editor)
{
    emit commitData(qobject_cast<QWidget *>(editor));
    emit closeEditor(qobject_cast<QWidget *>(editor), QAbstractItemDelegate::NoHint);
}

bool VariableDelegate::eventFilter(QObject* editor, QEvent* event)
{
    QComboBox* combo = qobject_cast<QComboBox *>(editor);
    QEvent::Type eventType = event->type();
    int eventKey = static_cast<const QKeyEvent*>(event)->key();
    if (combo
         && (eventType == QEvent::MouseButtonRelease
             || (eventType == QEvent::KeyPress && (eventKey == Qt::Key_Space
                                                || eventKey == Qt::Key_Enter
                                                || eventKey == Qt::Key_Return))))
    {
        if (combo)
            combo->showPopup();
        return true;
    }
    else if ((eventType == QEvent::ShortcutOverride && eventKey == Qt::Key_Escape)
             || eventType == QEvent::CloseSoftwareInputPanel)
    {
        commitAndCloseEditor(editor);
        return true;
    }
    else
    {
        return QObject::eventFilter(editor, event);
    }
}
