/***************************************************************************
  advoutputoptions.cpp
  -------------------
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

#include "advoutputoptions.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QTimer>
#include <QUrl>

#include "clicklabel.h"
#include "configstate.h"
#include "dbghelper.h"
#include "ecproject.h"
#include "widget_utils.h"

AdvOutputOptions::AdvOutputOptions(QWidget* parent,
                                   EcProject* ecProject,
                                   ConfigState* config) :
    QWidget(parent),
    ecProject_(ecProject),
    configState_(config)
{
    DEBUG_FUNC_NAME

    QString tooltipStr;

    fullSelectionButton = new QPushButton;
    fullSelectionButton->setText(tr("Set Thorough"));
    fullSelectionButton->setProperty("mdButton", true);
    fullSelectionButton->setMaximumWidth(fullSelectionButton->sizeHint().width());
    tooltipStr =
        tr("<b>Set thorough:</b> Click this button to pre-select a thorough "
           "set of output files. While program execution increases (even "
           "dramatically), you are provided with full results and diagnostic "
           "information. Note that you can customize this pre-selection by "
           "adding or removing items.");
    fullSelectionButton->setToolTip(tooltipStr);

    fullSelectionDesc = new QLabel;
    fullSelectionDesc->setText(tr("Complete results and diagnostic information"));
    fullSelectionDesc->setStyleSheet(QStringLiteral("QLabel {margin-left: 30px;}"));
    fullSelectionDesc->setToolTip(fullSelectionButton->toolTip());

    auto fullSelectionLayout = new QHBoxLayout;
    fullSelectionLayout->addWidget(fullSelectionButton);
    fullSelectionLayout->addWidget(fullSelectionDesc);

    minSelectionButton = new QPushButton;
    minSelectionButton->setText(tr("Set Minimal"));
    minSelectionButton->setProperty("mdButton", true);
    minSelectionButton->setMaximumWidth(fullSelectionButton->sizeHint().width());
    tooltipStr =
        tr("<b>Set minimal:</b> Click this button to pre-select a minimal "
           "set of output files, providing you with the essential results "
           "while speeding up program execution. Suggested for processing "
           "long datasets without a need for an in-depth analysis and "
           "thorough validation of computations. Note that you can customize "
           "this pre-selection by adding or removing items.");
    minSelectionButton->setToolTip(tooltipStr);

    minSelectionDesc = new QLabel;
    minSelectionDesc->setText(tr("Maximize computational speed"));
    minSelectionDesc->setStyleSheet(QStringLiteral("QLabel {margin-left: 30px}"));
    minSelectionDesc->setToolTip(minSelectionButton->toolTip());

    auto minSelectionLayout = new QHBoxLayout;
    minSelectionLayout->addWidget(minSelectionButton);
    minSelectionLayout->addWidget(minSelectionDesc);

    typicalSelectionButton = new QPushButton;
    typicalSelectionButton->setText(tr("Set Typical"));
    typicalSelectionButton->setProperty("mdButton", true);
    typicalSelectionButton->setMaximumWidth(fullSelectionButton->sizeHint().width());
    tooltipStr =
        tr("<b>Set typical:</b> Click this button to pre-select a balanced "
           "set of output files, providing you with the essential results as "
           "well as diagnostic information. The computation time increases "
           "with respect to the minimal output configuration. Note that you "
           "can customize this pre-selection by adding or removing items.");
    typicalSelectionButton->setToolTip(tooltipStr);

    typicalSelectionDesc = new QLabel;
    typicalSelectionDesc->setText(tr("Typical output selection"));
    typicalSelectionDesc->setStyleSheet(QStringLiteral("QLabel {margin-left: 30px;}"));
    typicalSelectionDesc->setToolTip(typicalSelectionButton->toolTip());

    auto typicalSelectionLayout = new QHBoxLayout;
    typicalSelectionLayout->addWidget(typicalSelectionButton);
    typicalSelectionLayout->addWidget(typicalSelectionDesc);

    hrLabel_1 = new QLabel;
    hrLabel_1->setObjectName(QStringLiteral("hrLabel"));
    auto hrLabel_2 = new QLabel;
    hrLabel_2->setObjectName(QStringLiteral("hrLabel"));
    auto hrLabel_3 = new QLabel;
    hrLabel_3->setObjectName(QStringLiteral("hrLabel"));
    auto hrLabel_4 = new QLabel;
    hrLabel_4->setObjectName(QStringLiteral("hrLabel"));
    auto hrLabel_5 = new QLabel;
    hrLabel_5->setObjectName(QStringLiteral("hrLabel"));

    auto vrLabel_1 = new QLabel;
    vrLabel_1->setObjectName(QStringLiteral("vrLabel"));
    auto vrLabel_2 = new QLabel;
    vrLabel_2->setObjectName(QStringLiteral("vrLabel"));

    outBinSpectraCheckBox = new QCheckBox;
    outBinSpectraCheckBox->setText(tr("All binned spectra and cospectra"));
    tooltipStr =
        tr("<b>All binned spectra and cospectra:</b> Binned spectra and "
           "cospectra are derived from \"full\" ones, by aggregating "
           "frequencies into clusters (bins) of exponentially increasing "
           "spectral width. All frequency components falling in each bin "
           "are averaged together to provide a much shorter file in which "
           "only the main spectral slopes are evidenced. Select this option "
           "to output binned spectra and cospectra for all available "
           "variables for each flux averaging interval. Results files are "
           "stored in a separate sub-folder \"\\eddypro_binned_cospectra\" "
           "inside the selected output folder.");
    outBinSpectraCheckBox->setToolTip(tooltipStr);

    outBinOgivesCheckBox = new QCheckBox;
    outBinOgivesCheckBox->setText(tr("All binned ogives"));
    tooltipStr =
            tr("<b>All binned ogives:</b> Binned ogives are calculated by "
               "binning \"full\" ogives. Full ogives are calculated by partial "
               "integration of cospectra: the ogive at a given frequency is "
               "the integration of the corresponding (co)spectrum from the "
               "highest frequency to the given one. Ogives are normalized to "
               "attain the value of 1 at the lower frequency. Select this "
               "option to output binned ogives for all available variables, "
               "for each flux averaging interval. Results files are stored "
               "in a separate sub-folder \"\\eddypro_binned_ogives\" inside "
               "the selected output folder.");
    outBinOgivesCheckBox->setToolTip(tooltipStr);

    outMeanCospCheckBox = new QCheckBox;
    outMeanCospCheckBox->setText(tr("Ensemble averaged cospectra and models"));
    tooltipStr =
        tr("<b>Ensemble averaged cospectra and models:</b> Check this box to "
           "instruct EddyPro to calculate ensemble-averaged cospectra, fit "
           "model cospectra and present \"ideal\" cospectra along with the "
           "former, to analyzer the turbulence structure at your site. Note "
           "that a fair amount of cospectral data are needed (e.g., more than "
           "2-3 weeks) to obtain a careful assessment.");
    outMeanCospCheckBox->setToolTip(tooltipStr);

    auto fullSpectraDescription = new QLabel;
    fullSpectraDescription->setText(tr("Obtaining full spectra and cospectra "
                            "will significantly increase the processing time.<br />"
                            "We recommend limitting the number of these outputs "
                            "to no more than two."));
    fullSpectraDescription->setObjectName(QStringLiteral("citeLabel"));

    outFullSpectraCheckBoxU = new QCheckBox;
    fullSpectraULabel = new ClickLabel(tr("U (longitudinal wind component)"));
    outFullSpectraCheckBoxV = new QCheckBox;
    fullSpectraVLabel = new ClickLabel(tr("V (transversal wind component)"));
    outFullSpectraCheckBoxW = new QCheckBox;
    fullSpectraWLabel = new ClickLabel(tr("W (vertical wind component)"));
    outFullSpectraCheckBoxTs = new QCheckBox;
    fullSpectraTsLabel = new ClickLabel(tr("%1 (sonic or fast ambient temperature)"
                                           ).arg(Defs::TSON_STRING));
    outFullSpectraCheckBoxCo2 = new QCheckBox;
    fullSpectraCo2Label = new ClickLabel(tr("%1 (concentration or density)"
                                            ).arg(Defs::CO2_STRING));
    outFullSpectraCheckBoxH2o = new QCheckBox;
    fullSpectraH2oLabel = new ClickLabel(tr("%1 (concentration or density)"
                                            ).arg(Defs::H2O_STRING));
    outFullSpectraCheckBoxCh4 = new QCheckBox;
    fullSpectraCh4Label = new ClickLabel(tr("%1 (concentration or density)"
                                            ).arg(Defs::CH4_STRING));
    outFullSpectraCheckBoxN2o = new QCheckBox;
    fullSpectraN2oLabel = new ClickLabel(tr("%1 Gas (concentration or density)"
                                            ).arg(Defs::GAS4_STRING));

    outFullCospectraCheckBoxU = new QCheckBox;
    fullCospectraULabel = new ClickLabel(tr("W/U"));
    outFullCospectraCheckBoxV = new QCheckBox;
    fullCospectraVLabel = new ClickLabel(tr("W/V"));
    outFullCospectraCheckBoxTs = new QCheckBox;
    fullCospectraTsLabel = new ClickLabel(tr("W/%1").arg(Defs::TSON_STRING));
    outFullCospectraCheckBoxCo2 = new QCheckBox;
    fullCospectraCo2Label = new ClickLabel(tr("W/%1").arg(Defs::CO2_STRING));
    outFullCospectraCheckBoxH2o = new QCheckBox;
    fullCospectraH2oLabel = new ClickLabel(tr("W/%1").arg(Defs::H2O_STRING));
    outFullCospectraCheckBoxCh4 = new QCheckBox;
    fullCospectraCh4Label = new ClickLabel(tr("W/%1").arg(Defs::CH4_STRING));
    outFullCospectraCheckBoxN2o = new QCheckBox;
    fullCospectraN2oLabel = new ClickLabel(tr("W/%1 Gas").arg(Defs::GAS4_STRING));

    outGhgEuCheckBox = new QCheckBox;
    outGhgEuCheckBox->setText(tr("GHG-Europe results (fluxes and more, "
                                 "formatted for GHG-Europe database)"));
    tooltipStr =
        tr("<b>GHG-Europe results:</b> A selection of main results formatted "
           "so as to be easily completed with missing variables and submitted "
           "to the GHG-Europe database.");
    outGhgEuCheckBox->setToolTip(tooltipStr);

    outAmFluxCheckBox = new QCheckBox;
    outAmFluxCheckBox->setText(tr("AmeriFlux results (fluxes and more, "
                                  "formatted for AmeriFlux database)"));
    tooltipStr =
        tr("<b>AmeriFlux results:</b> A selection of main results formatted "
           "so as to be easily completed with missing variables and submitted "
           "to the AmeriFlux database.");
    outAmFluxCheckBox->setToolTip(tooltipStr);

    outFullCheckBox = new QCheckBox;
    outFullCheckBox->setText(tr("Full output (fluxes, quality flags, "
                                "turbulence, statistics...)"));
    tooltipStr =
        tr("<b>Full Output:</b> This is the main EddyPro results file. It "
           "contains fluxes, quality flags, micrometeorological variables, "
           "gas concentrations and densities, footprint estimations and "
           "diagnostic information along with ancillary variables such as "
           "uncorrected fluxes, main statistics, etc.");
    outFullCheckBox->setToolTip(tooltipStr);

    outDetailsCheckBox = new QCheckBox;
    outDetailsCheckBox->setText(tr("Details of steady state and developed "
                                   "turbulence tests (Foken et al. 2004)"));
    tooltipStr =
        tr("<b>Details of steady state and developed turbulence tests:</b> "
           "Partial results obtained from the steady state and the developed "
           "turbulence tests. It reports the percentage of deviation from "
           "expectations, as well as individual test flags.");
    outDetailsCheckBox->setToolTip(tooltipStr);

    outMdCheckBox = new QCheckBox;
    outMdCheckBox->setText(tr("Metadata"));
    tooltipStr =
        tr("<b>Metadata:</b> Summarizes metadata used for the processed "
           "datasets. If an <b><i>Alternative metadata file</i></b> is used, "
           "without any <b><i>Dynamic metadata file</i></b>, the content of "
           "this result file will be identical for all lines and can be "
           "avoided. If you are processing GHG files and/or are using "
           "a <b><i>Dynamic metadata file</i></b>, this result file will "
           "tell you which metadata was actually used during processing.");
    outMdCheckBox->setToolTip(tooltipStr);

    outBiometCheckBox = new QCheckBox;
    outBiometCheckBox->setText(tr("Biomet measurements (averaged over the "
                                  "flux averaging period)"));
    tooltipStr =
        tr("<b>Biomet measurements:</b> Average values of all available "
           "biomet measurements, calculated over the same time period "
           "selected for fluxes. Biomet measurements that are recognized "
           "by EddyPro (i.e., marked by recognized labels) are screened "
           "for physical plausibility before calculating the average value "
           "and they are converted to units that coincide with other "
           "EddyPro results. All other variables are solely averaged "
           "and provided on output.");
    outBiometCheckBox->setToolTip(tooltipStr);

    createDatasetCheckBox = new QCheckBox;
    createDatasetCheckBox->setText(tr("Build continuous dataset "
                                      "(This is not gap-filling!\n"
                                      "Missing flux averaging period "
                                      "are filled with error codes)"));

    fullOutformatLabel = new ClickLabel;
    fullOutformatLabel->setText(tr("Output format :"));

    variableVarsOutputRadio = new QRadioButton;
    variableVarsOutputRadio->setText(tr("Output only available results"));

    fixedVarsOutputRadio = new QRadioButton;
    fixedVarsOutputRadio->setText(tr("Use standard output format"));

    outputFormatRadioGroup = new QButtonGroup(this);
    outputFormatRadioGroup->addButton(variableVarsOutputRadio, 0);
    outputFormatRadioGroup->addButton(fixedVarsOutputRadio, 1);

    errorFormatLabel = new ClickLabel;
    errorFormatLabel->setText(tr("Error label :"));

    errorFormatCombo = new QComboBox;
    errorFormatCombo->addItem(QStringLiteral("-9999.0"));
    errorFormatCombo->addItem(QStringLiteral("-6999.0"));
    errorFormatCombo->addItem(QStringLiteral("NaN"));
    errorFormatCombo->addItem(QStringLiteral("Error"));
    errorFormatCombo->addItem(QStringLiteral("N/A"));
    errorFormatCombo->addItem(QStringLiteral("NOOP"));
    errorFormatCombo->setEditable(true);
    QLineEdit* errorLinedit = errorFormatCombo->lineEdit();
    errorLinedit->setMaxLength(32);

    auto statLabel = new QLabel;
    statLabel->setText(tr("Statistics"));
    statLabel->setProperty("blueLabel", true);
    tooltipStr =
        tr("<b>Statistics:</b> Main statistics (mean values, standard "
           "deviations, variances and covariances, skewness and kurtosis) "
           "for all variables contained in the raw files. Result files "
           "concerning variables selected for flux computation are stored "
           "in a separate sub-folder \"\\eddypro_stats\" inside the "
           "selected output folder. Result files concerning variables "
           "not selected for flux computation but available in the raw "
           "files are stored in a separate sub-folder \"\\eddypro_user_stats\" "
           "inside the selected output folder.");
    statLabel->setToolTip(tooltipStr);

    auto timeSeriesLabel = new QLabel;
    timeSeriesLabel->setText(tr("Time series"));
    timeSeriesLabel->setProperty("blueLabel", true);
    tooltipStr =
            tr("<b>Time series:</b> Actual time series for each variable "
               "selected in the list on the right. Result files are stored "
               "in a separate sub-folder \"\\eddypro_raw_datasets\" inside "
               "the selected output folder.");
    timeSeriesLabel->setToolTip(tooltipStr);

    auto varLabel = new QLabel;
    varLabel->setText(tr("Variables"));
    varLabel->setProperty("blueLabel", true);

    level1Label = new ClickLabel(tr("Level 1 (unprocessed) :"));
    level2Label = new ClickLabel(tr("Level 2 (after despiking) :"));
    level3Label = new ClickLabel(tr("Level 3 (after cross-wind correction) :"));
    level4Label = new ClickLabel(tr("Level 4 (after angle-of-attack correction) :"));
    level5Label = new ClickLabel(tr("Level 5 (after tilt correction) :"));
    level6Label = new ClickLabel(tr("Level 6 (after time lag compensation) :"));
    level7Label = new ClickLabel(tr("Level 7 (after detrending) :"));

    rawULabel = new ClickLabel(tr("U"));
    rawVLabel = new ClickLabel(tr("V"));
    rawWLabel = new ClickLabel(tr("W"));
    rawTsLabel = new ClickLabel(tr("%1").arg(Defs::TSON_STRING));
    rawCo2Label = new ClickLabel(tr("%1").arg(Defs::CO2_STRING));
    rawH2oLabel = new ClickLabel(tr("%1").arg(Defs::H2O_STRING));
    rawCh4Label = new ClickLabel(tr("%1").arg(Defs::CH4_STRING));
    rawGas4Label = new ClickLabel(tr("%1 trace gas").arg(Defs::GAS4_STRING));
    rawTairLabel = new ClickLabel(tr("T<sub>air</sub>"));
    rawPairLabel = new ClickLabel(tr("P<sub>air</sub>"));

    outSt1CheckBox = new QCheckBox;
    outSt2CheckBox = new QCheckBox;
    outSt3CheckBox = new QCheckBox;
    outSt4CheckBox = new QCheckBox;
    outSt5CheckBox = new QCheckBox;
    outSt6CheckBox = new QCheckBox;
    outSt7CheckBox = new QCheckBox;

    outRaw1CheckBox = new QCheckBox;
    outRaw2CheckBox = new QCheckBox;
    outRaw3CheckBox = new QCheckBox;
    outRaw4CheckBox = new QCheckBox;
    outRaw5CheckBox = new QCheckBox;
    outRaw6CheckBox = new QCheckBox;
    outRaw7CheckBox = new QCheckBox;

    outRawUCheckBox = new QCheckBox;
    outRawVCheckBox = new QCheckBox;
    outRawWCheckBox = new QCheckBox;
    outRawTsCheckBox = new QCheckBox;
    outRawCo2CheckBox = new QCheckBox;
    outRawH2oCheckBox = new QCheckBox;
    outRawCh4CheckBox = new QCheckBox;
    outRawGas4CheckBox = new QCheckBox;
    outRawTairCheckBox = new QCheckBox;
    outRawPairCheckBox = new QCheckBox;

    outVarsAllCheckBox = new QCheckBox;
    outVarsAllCheckBox->setText(tr("Select all variables"));
    outVarsAllCheckBox->setStyleSheet(QStringLiteral("QCheckBox {margin-left: 40px;}"));

    auto checkboxContainerLayout_1 = new QHBoxLayout;
    checkboxContainerLayout_1->addWidget(outFullSpectraCheckBoxU);
    checkboxContainerLayout_1->addWidget(fullSpectraULabel);
    checkboxContainerLayout_1->addStretch();
    checkboxContainerLayout_1->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_1->setSpacing(0);
    auto checkboxContainer_1 = new QWidget;
    checkboxContainer_1->setLayout(checkboxContainerLayout_1);
    auto checkboxContainerLayout_2 = new QHBoxLayout;
    checkboxContainerLayout_2->addWidget(outFullSpectraCheckBoxV);
    checkboxContainerLayout_2->addWidget(fullSpectraVLabel);
    checkboxContainerLayout_2->addStretch();
    checkboxContainerLayout_2->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_2->setSpacing(0);
    auto checkboxContainer_2 = new QWidget;
    checkboxContainer_2->setLayout(checkboxContainerLayout_2);
    auto checkboxContainerLayout_3 = new QHBoxLayout;
    checkboxContainerLayout_3->addWidget(outFullSpectraCheckBoxW);
    checkboxContainerLayout_3->addWidget(fullSpectraWLabel);
    checkboxContainerLayout_3->addStretch();
    checkboxContainerLayout_3->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_3->setSpacing(0);
    auto checkboxContainer_3 = new QWidget;
    checkboxContainer_3->setLayout(checkboxContainerLayout_3);
    auto checkboxContainerLayout_4 = new QHBoxLayout;
    checkboxContainerLayout_4->addWidget(outFullSpectraCheckBoxTs);
    checkboxContainerLayout_4->addWidget(fullSpectraTsLabel);
    checkboxContainerLayout_4->addStretch();
    checkboxContainerLayout_4->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_4->setSpacing(0);
    auto checkboxContainer_4 = new QWidget;
    checkboxContainer_4->setLayout(checkboxContainerLayout_4);
    auto checkboxContainerLayout_5 = new QHBoxLayout;
    checkboxContainerLayout_5->addWidget(outFullSpectraCheckBoxCo2);
    checkboxContainerLayout_5->addWidget(fullSpectraCo2Label);
    checkboxContainerLayout_5->addStretch();
    checkboxContainerLayout_5->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_5->setSpacing(0);
    auto checkboxContainer_5 = new QWidget;
    checkboxContainer_5->setLayout(checkboxContainerLayout_5);
    auto checkboxContainerLayout_6 = new QHBoxLayout;
    checkboxContainerLayout_6->addWidget(outFullSpectraCheckBoxH2o);
    checkboxContainerLayout_6->addWidget(fullSpectraH2oLabel);
    checkboxContainerLayout_6->addStretch();
    checkboxContainerLayout_6->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_6->setSpacing(0);
    auto checkboxContainer_6 = new QWidget;
    checkboxContainer_6->setLayout(checkboxContainerLayout_6);
    auto checkboxContainerLayout_7 = new QHBoxLayout;
    checkboxContainerLayout_7->addWidget(outFullSpectraCheckBoxCh4);
    checkboxContainerLayout_7->addWidget(fullSpectraCh4Label);
    checkboxContainerLayout_7->addStretch();
    checkboxContainerLayout_7->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_7->setSpacing(0);
    auto checkboxContainer_7 = new QWidget;
    checkboxContainer_7->setLayout(checkboxContainerLayout_7);
    auto checkboxContainerLayout_8 = new QHBoxLayout;
    checkboxContainerLayout_8->addWidget(outFullSpectraCheckBoxN2o);
    checkboxContainerLayout_8->addWidget(fullSpectraN2oLabel);
    checkboxContainerLayout_8->addStretch();
    checkboxContainerLayout_8->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_8->setSpacing(0);
    auto checkboxContainer_8 = new QWidget;
    checkboxContainer_8->setLayout(checkboxContainerLayout_8);

    auto checkboxContainerLayout_12 = new QHBoxLayout;
    checkboxContainerLayout_12->addWidget(outFullCospectraCheckBoxU);
    checkboxContainerLayout_12->addWidget(fullCospectraULabel);
    checkboxContainerLayout_12->addStretch();
    checkboxContainerLayout_12->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_12->setSpacing(0);
    auto checkboxContainer_12 = new QWidget;
    checkboxContainer_12->setLayout(checkboxContainerLayout_12);
    auto checkboxContainerLayout_22 = new QHBoxLayout;
    checkboxContainerLayout_22->addWidget(outFullCospectraCheckBoxV);
    checkboxContainerLayout_22->addWidget(fullCospectraVLabel);
    checkboxContainerLayout_22->addStretch();
    checkboxContainerLayout_22->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_22->setSpacing(0);
    auto checkboxContainer_22 = new QWidget;
    checkboxContainer_22->setLayout(checkboxContainerLayout_22);
    auto checkboxContainerLayout_32 = new QHBoxLayout;
    checkboxContainerLayout_32->addWidget(outFullCospectraCheckBoxTs);
    checkboxContainerLayout_32->addWidget(fullCospectraTsLabel);
    checkboxContainerLayout_32->addStretch();
    checkboxContainerLayout_32->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_32->setSpacing(0);
    auto checkboxContainer_32 = new QWidget;
    checkboxContainer_32->setLayout(checkboxContainerLayout_32);
    auto checkboxContainerLayout_42 = new QHBoxLayout;
    checkboxContainerLayout_42->addWidget(outFullCospectraCheckBoxCo2);
    checkboxContainerLayout_42->addWidget(fullCospectraCo2Label);
    checkboxContainerLayout_42->addStretch();
    checkboxContainerLayout_42->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_42->setSpacing(0);
    auto checkboxContainer_42 = new QWidget;
    checkboxContainer_42->setLayout(checkboxContainerLayout_42);
    auto checkboxContainerLayout_52 = new QHBoxLayout;
    checkboxContainerLayout_52->addWidget(outFullCospectraCheckBoxH2o);
    checkboxContainerLayout_52->addWidget(fullCospectraH2oLabel);
    checkboxContainerLayout_52->addStretch();
    checkboxContainerLayout_52->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_52->setSpacing(0);
    auto checkboxContainer_52 = new QWidget;
    checkboxContainer_52->setLayout(checkboxContainerLayout_52);
    auto checkboxContainerLayout_62 = new QHBoxLayout;
    checkboxContainerLayout_62->addWidget(outFullCospectraCheckBoxCh4);
    checkboxContainerLayout_62->addWidget(fullCospectraCh4Label);
    checkboxContainerLayout_62->addStretch();
    checkboxContainerLayout_62->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_62->setSpacing(0);
    auto checkboxContainer_62 = new QWidget;
    checkboxContainer_62->setLayout(checkboxContainerLayout_62);
    auto checkboxContainerLayout_72 = new QHBoxLayout;
    checkboxContainerLayout_72->addWidget(outFullCospectraCheckBoxN2o);
    checkboxContainerLayout_72->addWidget(fullCospectraN2oLabel);
    checkboxContainerLayout_72->addStretch();
    checkboxContainerLayout_72->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_72->setSpacing(0);
    auto checkboxContainer_72 = new QWidget;
    checkboxContainer_72->setLayout(checkboxContainerLayout_72);

    auto checkboxContainerLayout_13 = new QHBoxLayout;
    checkboxContainerLayout_13->addWidget(outRawUCheckBox);
    checkboxContainerLayout_13->addWidget(rawULabel);
    checkboxContainerLayout_13->addStretch();
    checkboxContainerLayout_13->setContentsMargins(50, 0, 11, 0);
    checkboxContainerLayout_13->setSpacing(0);
    auto checkboxContainer_13 = new QWidget;
    checkboxContainer_13->setLayout(checkboxContainerLayout_13);
    auto checkboxContainerLayout_23 = new QHBoxLayout;
    checkboxContainerLayout_23->addWidget(outRawVCheckBox);
    checkboxContainerLayout_23->addWidget(rawVLabel);
    checkboxContainerLayout_23->addStretch();
    checkboxContainerLayout_23->setContentsMargins(50, 0, 11, 0);
    checkboxContainerLayout_23->setSpacing(0);
    auto checkboxContainer_23 = new QWidget;
    checkboxContainer_23->setLayout(checkboxContainerLayout_23);
    auto checkboxContainerLayout_33 = new QHBoxLayout;
    checkboxContainerLayout_33->addWidget(outRawWCheckBox);
    checkboxContainerLayout_33->addWidget(rawWLabel);
    checkboxContainerLayout_33->addStretch();
    checkboxContainerLayout_33->setContentsMargins(50, 0, 11, 0);
    checkboxContainerLayout_33->setSpacing(0);
    auto checkboxContainer_33 = new QWidget;
    checkboxContainer_33->setLayout(checkboxContainerLayout_33);
    auto checkboxContainerLayout_43 = new QHBoxLayout;
    checkboxContainerLayout_43->addWidget(outRawTsCheckBox);
    checkboxContainerLayout_43->addWidget(rawTsLabel);
    checkboxContainerLayout_43->addStretch();
    checkboxContainerLayout_43->setContentsMargins(50, 0, 11, 0);
    checkboxContainerLayout_43->setSpacing(0);
    auto checkboxContainer_43 = new QWidget;
    checkboxContainer_43->setLayout(checkboxContainerLayout_43);
    auto checkboxContainerLayout_53 = new QHBoxLayout;
    checkboxContainerLayout_53->addWidget(outRawCo2CheckBox);
    checkboxContainerLayout_53->addWidget(rawCo2Label);
    checkboxContainerLayout_53->addStretch();
    checkboxContainerLayout_53->setContentsMargins(50, 0, 11, 0);
    checkboxContainerLayout_53->setSpacing(0);
    auto checkboxContainer_53 = new QWidget;
    checkboxContainer_53->setLayout(checkboxContainerLayout_53);
    auto checkboxContainerLayout_14 = new QHBoxLayout;
    checkboxContainerLayout_14->addWidget(outRawH2oCheckBox);
    checkboxContainerLayout_14->addWidget(rawH2oLabel);
    checkboxContainerLayout_14->addStretch();
    checkboxContainerLayout_14->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_14->setSpacing(0);
    auto checkboxContainer_14 = new QWidget;
    checkboxContainer_14->setLayout(checkboxContainerLayout_14);
    auto checkboxContainerLayout_24 = new QHBoxLayout;
    checkboxContainerLayout_24->addWidget(outRawCh4CheckBox);
    checkboxContainerLayout_24->addWidget(rawCh4Label);
    checkboxContainerLayout_24->addStretch();
    checkboxContainerLayout_24->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_24->setSpacing(0);
    auto checkboxContainer_24 = new QWidget;
    checkboxContainer_24->setLayout(checkboxContainerLayout_24);
    auto checkboxContainerLayout_34 = new QHBoxLayout;
    checkboxContainerLayout_34->addWidget(outRawGas4CheckBox);
    checkboxContainerLayout_34->addWidget(rawGas4Label);
    checkboxContainerLayout_34->addStretch();
    checkboxContainerLayout_34->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_34->setSpacing(0);
    auto checkboxContainer_34 = new QWidget;
    checkboxContainer_34->setLayout(checkboxContainerLayout_34);
    auto checkboxContainerLayout_44 = new QHBoxLayout;
    checkboxContainerLayout_44->addWidget(outRawTairCheckBox);
    checkboxContainerLayout_44->addWidget(rawTairLabel);
    checkboxContainerLayout_44->addStretch();
    checkboxContainerLayout_44->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_44->setSpacing(0);
    auto checkboxContainer_44 = new QWidget;
    checkboxContainer_44->setLayout(checkboxContainerLayout_44);
    auto checkboxContainerLayout_54 = new QHBoxLayout;
    checkboxContainerLayout_54->addWidget(outRawPairCheckBox);
    checkboxContainerLayout_54->addWidget(rawPairLabel);
    checkboxContainerLayout_54->addStretch();
    checkboxContainerLayout_54->setContentsMargins(0, 0, 11, 0);
    checkboxContainerLayout_54->setSpacing(0);
    auto checkboxContainer_54 = new QWidget;
    checkboxContainer_54->setLayout(checkboxContainerLayout_54);

    auto title_1 = new QLabel;
    title_1->setText(tr("Results files"));
    title_1->setProperty("groupLabel", true);

    auto title_6 = new QLabel;
    title_6->setText(tr("Spectral outputs"));
    title_6->setProperty("groupLabel", true);

    auto title_2 = new QLabel;
    title_2->setText(tr("Reduced spectra and ogives"));
    title_2->setProperty("blueLabel", true);

    auto title_3 = new QLabel;
    title_3->setText(tr("Full length spectra"));
    title_3->setProperty("blueLabel", true);
    tooltipStr =
        tr("<b>Full length spectra:</b> Spectra calculated for each variable, "
           "for each flux averaging interval. Results files are stored in "
           "a separate sub-folder \"\\eddypro_full_cospectra\" inside the "
           "selected output folder.");
    title_3->setToolTip(tooltipStr);

    auto title_4 = new QLabel(tr("Full length cospectra"));
    title_4->setProperty("blueLabel", true);
    tooltipStr =
        tr("<b>Full length cospectra:</b> Cospectra with the vertical wind "
           "component, calculated for each variable, for each flux averaging "
           "interval. Result files are stored in a separate sub-folder "
           "\"\\eddypro_full_cospectra\" inside the selected output folder.");
    title_4->setToolTip(tooltipStr);

    auto title_5 = new QLabel;
    title_5->setText(tr("Processed raw data"));
    title_5->setProperty("groupLabel", true);

    createQuestionMark();

    auto qBox_1 = new QHBoxLayout;
    qBox_1->addWidget(title_1);
    qBox_1->addWidget(questionMark_1);
    qBox_1->addStretch();

    auto qBox_2 = new QHBoxLayout;
    qBox_2->addStretch();
    qBox_2->addWidget(fullOutformatLabel);
    qBox_2->addWidget(questionMark_2);

    auto qBox_3 = new QHBoxLayout;
    qBox_3->addStretch();
    qBox_3->addWidget(errorFormatLabel);
    qBox_3->addSpacerItem(new QSpacerItem(18, 12));

    auto qBox_4 = new QHBoxLayout;
    qBox_4->addWidget(createDatasetCheckBox);
    qBox_4->addWidget(questionMark_4);
    qBox_4->addStretch();

    auto qBox_10 = new QHBoxLayout;
    qBox_10->addWidget(title_6);
    qBox_10->addWidget(questionMark_5);
    qBox_10->addStretch();

    auto qBox_5 = new QHBoxLayout;
    qBox_5->addWidget(title_2);
    qBox_5->addSpacerItem(new QSpacerItem(18, 12));
    qBox_5->addStretch();
    qBox_5->setContentsMargins(11, 0, 0, 0);

    auto qBox_6 = new QHBoxLayout;
    qBox_6->addWidget(title_3);
    qBox_5->addSpacerItem(new QSpacerItem(18, 12));
    qBox_6->addStretch();
    qBox_6->setContentsMargins(11, 0, 0, 0);

    auto qBox_7 = new QHBoxLayout;
    qBox_7->addWidget(title_4);
    qBox_5->addSpacerItem(new QSpacerItem(18, 12));
    qBox_7->addStretch();
    qBox_7->setContentsMargins(11, 0, 0, 0);

    auto qBox_8 = new QHBoxLayout;
    qBox_8->addWidget(statLabel);
    qBox_8->addWidget(questionMark_8);
    qBox_8->addStretch();

    auto qBox_9 = new QHBoxLayout;
    qBox_9->addWidget(timeSeriesLabel);
    qBox_9->addWidget(questionMark_9);
    qBox_9->addStretch();

    auto outputLayout = new QGridLayout;
    outputLayout->addLayout(minSelectionLayout, 0, 0, 1, -1);
    outputLayout->addLayout(typicalSelectionLayout, 1, 0, 1, -1);
    outputLayout->addLayout(fullSelectionLayout, 2, 0, 1, -1);
    outputLayout->addWidget(hrLabel_1, 3, 0, 1, -1);
    outputLayout->addLayout(qBox_1, 4, 0);
    outputLayout->addWidget(outFullCheckBox, 5, 0, 1, 4);
    outputLayout->addLayout(qBox_2, 5, 2, Qt::AlignRight);
    outputLayout->addWidget(variableVarsOutputRadio, 5, 3, 1, 2, Qt::AlignLeft);
    outputLayout->addWidget(fixedVarsOutputRadio, 6, 3, 1, 2, Qt::AlignLeft);
    outputLayout->addWidget(vrLabel_1, 5, 5, 6, 1);
    outputLayout->addLayout(qBox_4, 5, 6, 6, 2, Qt::AlignLeft);
    outputLayout->addLayout(qBox_3, 7, 2, Qt::AlignRight);
    outputLayout->addWidget(errorFormatCombo, 7, 3);
    outputLayout->addWidget(outAmFluxCheckBox, 8, 0, 1, 4);
    outputLayout->addWidget(outGhgEuCheckBox, 9, 0, 1, 4);
    outputLayout->addWidget(outBiometCheckBox, 10, 0, 1, 4);
    outputLayout->addWidget(outDetailsCheckBox, 11, 0, 1, 4);
    outputLayout->addWidget(outMdCheckBox, 12, 0, 1, 4);
    outputLayout->addWidget(hrLabel_2, 13, 0, 1, -1);
    outputLayout->addLayout(qBox_10, 14, 0);
    outputLayout->addLayout(qBox_5, 15, 0);
    outputLayout->addWidget(outBinSpectraCheckBox, 16, 0);
    outputLayout->addWidget(outBinOgivesCheckBox, 17, 0);
    outputLayout->addWidget(outMeanCospCheckBox, 18, 0);
    outputLayout->addLayout(qBox_6, 15, 1, 1, 2);
    outputLayout->addWidget(checkboxContainer_1, 16, 1, 1, 2);
    outputLayout->addWidget(checkboxContainer_2, 17, 1, 1, 2);
    outputLayout->addWidget(checkboxContainer_3, 18, 1, 1, 2);
    outputLayout->addWidget(checkboxContainer_4, 19, 1, 1, 2);
    outputLayout->addWidget(checkboxContainer_5, 20, 1, 1, 2);
    outputLayout->addWidget(checkboxContainer_6, 21, 1, 1, 2);
    outputLayout->addWidget(checkboxContainer_7, 22, 1, 1, 2);
    outputLayout->addWidget(checkboxContainer_8, 23, 1, 1, 2);
    outputLayout->addLayout(qBox_7, 15, 3, 1, 3);
    outputLayout->addWidget(checkboxContainer_12, 16, 3, 1, 3);
    outputLayout->addWidget(checkboxContainer_22, 17, 3, 1, 3);
    outputLayout->addWidget(checkboxContainer_32, 18, 3, 1, 3);
    outputLayout->addWidget(checkboxContainer_42, 19, 3, 1, 3);
    outputLayout->addWidget(checkboxContainer_52, 20, 3, 1, 3);
    outputLayout->addWidget(checkboxContainer_62, 21, 3, 1, 3);
    outputLayout->addWidget(checkboxContainer_72, 22, 3, 1, 3);
    outputLayout->addWidget(fullSpectraDescription, 24, 1, 1, 7);
    outputLayout->addWidget(hrLabel_3, 25, 0, 1, -1);
    outputLayout->addWidget(title_5, 26, 0);
    outputLayout->addLayout(qBox_8, 27, 1, Qt::AlignRight);
    outputLayout->addLayout(qBox_9, 27, 2, Qt::AlignRight);
    outputLayout->addWidget(varLabel, 27, 3, 1, 2, Qt::AlignCenter);
    outputLayout->addWidget(hrLabel_4, 28, 1, 1, 2);
    outputLayout->addWidget(hrLabel_5, 28, 3, 1, 2);
    outputLayout->addWidget(level1Label, 29, 0, Qt::AlignRight);
    outputLayout->addWidget(level2Label, 30, 0, Qt::AlignRight);
    outputLayout->addWidget(level3Label, 31, 0, Qt::AlignRight);
    outputLayout->addWidget(level4Label, 32, 0, Qt::AlignRight);
    outputLayout->addWidget(level5Label, 33, 0, Qt::AlignRight);
    outputLayout->addWidget(level6Label, 34, 0, Qt::AlignRight);
    outputLayout->addWidget(level7Label, 35, 0, Qt::AlignRight);
    outputLayout->addWidget(outSt1CheckBox, 29, 1, Qt::AlignCenter);
    outputLayout->addWidget(outSt2CheckBox, 30, 1, Qt::AlignCenter);
    outputLayout->addWidget(outSt3CheckBox, 31, 1, Qt::AlignCenter);
    outputLayout->addWidget(outSt4CheckBox, 32, 1, Qt::AlignCenter);
    outputLayout->addWidget(outSt5CheckBox, 33, 1, Qt::AlignCenter);
    outputLayout->addWidget(outSt6CheckBox, 34, 1, Qt::AlignCenter);
    outputLayout->addWidget(outSt7CheckBox, 35, 1, Qt::AlignCenter);
    outputLayout->addWidget(outRaw1CheckBox, 29, 2, Qt::AlignCenter);
    outputLayout->addWidget(outRaw2CheckBox, 30, 2, Qt::AlignCenter);
    outputLayout->addWidget(outRaw3CheckBox, 31, 2, Qt::AlignCenter);
    outputLayout->addWidget(outRaw4CheckBox, 32, 2, Qt::AlignCenter);
    outputLayout->addWidget(outRaw5CheckBox, 33, 2, Qt::AlignCenter);
    outputLayout->addWidget(outRaw6CheckBox, 34, 2, Qt::AlignCenter);
    outputLayout->addWidget(outRaw7CheckBox, 35, 2, Qt::AlignCenter);
    outputLayout->addWidget(vrLabel_2, 29, 3, 7, 1, Qt::AlignLeft);
    outputLayout->addWidget(checkboxContainer_13, 29, 3, 1, 2);
    outputLayout->addWidget(checkboxContainer_23, 30, 3, 1, 2);
    outputLayout->addWidget(checkboxContainer_33, 31, 3, 1, 2);
    outputLayout->addWidget(checkboxContainer_43, 32, 3, 1, 2);
    outputLayout->addWidget(checkboxContainer_53, 33, 3, 1, 2);
    outputLayout->addWidget(checkboxContainer_14, 29, 4, 1, 2);
    outputLayout->addWidget(checkboxContainer_24, 30, 4, 1, 2);
    outputLayout->addWidget(checkboxContainer_34, 31, 4, 1, 2);
    outputLayout->addWidget(checkboxContainer_44, 32, 4, 1, 2);
    outputLayout->addWidget(checkboxContainer_54, 33, 4, 1, 2);
    outputLayout->addWidget(outVarsAllCheckBox, 35, 3, 1, 3);
    outputLayout->setRowStretch(36, 1);
    outputLayout->setColumnStretch(8, 1);
    outputLayout->setColumnMinimumWidth(4, 60);

    auto outputFrame = new QWidget;
    outputFrame->setProperty("scrollContainerWidget", true);
    outputFrame->setLayout(outputLayout);

    auto scrollArea = new QScrollArea;
    scrollArea->setWidget(outputFrame);
    scrollArea->setWidgetResizable(true);

    auto settingsGroupLayout = new QHBoxLayout;
    settingsGroupLayout->addWidget(scrollArea);

    auto settingsGroupTitle = new QLabel;
    settingsGroupTitle->setText(tr("Output File Options"));
    settingsGroupTitle->setProperty("groupTitle", true);

    auto mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(settingsGroupTitle);
    mainLayout->addLayout(settingsGroupLayout);
    mainLayout->setContentsMargins(15, 15, 0, 0);
    setLayout(mainLayout);

    connect(outBinSpectraCheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutBinSpectra);
    connect(outBinOgivesCheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutBinOgives(checked); });
    connect(outMeanCospCheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setGeneralOutMeanCosp(checked); });
    connect(outMeanCospCheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateBinSpectra);
    connect(outGhgEuCheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setGeneralOutGhgEu(checked); });
    connect(outDetailsCheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenlOutDetails(checked); });
    connect(outMdCheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setGeneralOutMd(checked); });
    connect(outBiometCheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setGeneralOutBiomet(checked); });
    connect(createDatasetCheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setGeneralMakeDataset(checked); });
    connect(outAmFluxCheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setGeneralOutAmFluxOut(checked); });
    connect(outFullCheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setGeneralOutRich(checked); });

    // buttonClicked() is and overloaded signal...
    connect(outputFormatRadioGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(updateFixedOuputFormat(int)));

    connect(errorFormatLabel, &ClickLabel::clicked,
            this, &AdvOutputOptions::onClickerrorFormatLabel);

    // currentIndexChanged() is and overloaded signal...
    connect(errorFormatCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(updateErrorLabel(QString)));
    connect(errorFormatCombo, SIGNAL(editTextChanged(QString)),
            this, SLOT(updateErrorLabel(QString)));

    connect(outFullSpectraCheckBoxU, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullSpectraU(checked); });
    connect(outFullSpectraCheckBoxV, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullSpectraV(checked); });
    connect(outFullSpectraCheckBoxW, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullSpectraW(checked); });
    connect(outFullSpectraCheckBoxTs, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullSpectraTs(checked); });
    connect(outFullSpectraCheckBoxCo2, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullSpectraCo2(checked); });
    connect(outFullSpectraCheckBoxH2o, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullSpectraH2o(checked); });
    connect(outFullSpectraCheckBoxCh4, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullSpectraCh4(checked); });
    connect(outFullSpectraCheckBoxN2o, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullSpectraN2o(checked); });

    connect(fullSpectraULabel, &ClickLabel::clicked,
            outFullSpectraCheckBoxU, &QCheckBox::toggle);
    connect(fullSpectraVLabel, &ClickLabel::clicked,
            outFullSpectraCheckBoxV, &QCheckBox::toggle);
    connect(fullSpectraWLabel, &ClickLabel::clicked,
            outFullSpectraCheckBoxW, &QCheckBox::toggle);
    connect(fullSpectraTsLabel, &ClickLabel::clicked,
            outFullSpectraCheckBoxTs, &QCheckBox::toggle);
    connect(fullSpectraCo2Label, &ClickLabel::clicked,
            outFullSpectraCheckBoxCo2, &QCheckBox::toggle);
    connect(fullSpectraH2oLabel, &ClickLabel::clicked,
            outFullSpectraCheckBoxH2o, &QCheckBox::toggle);
    connect(fullSpectraCh4Label, &ClickLabel::clicked,
            outFullSpectraCheckBoxCh4, &QCheckBox::toggle);
    connect(fullSpectraN2oLabel, &ClickLabel::clicked,
            outFullSpectraCheckBoxN2o, &QCheckBox::toggle);

    connect(outFullCospectraCheckBoxU, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullCospectraU(checked); });
    connect(outFullCospectraCheckBoxV, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullCospectraV(checked); });
    connect(outFullCospectraCheckBoxTs, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullCospectraTs(checked); });
    connect(outFullCospectraCheckBoxCo2, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullCospectraCo2(checked); });
    connect(outFullCospectraCheckBoxH2o, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullCospectraH2o(checked); });
    connect(outFullCospectraCheckBoxCh4, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullCospectraCh4(checked); });
    connect(outFullCospectraCheckBoxN2o, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutFullCospectraN2o(checked); });

    connect(fullCospectraULabel, &ClickLabel::clicked,
            outFullCospectraCheckBoxU, &QCheckBox::toggle);
    connect(fullCospectraVLabel, &ClickLabel::clicked,
            outFullCospectraCheckBoxV, &QCheckBox::toggle);
    connect(fullCospectraTsLabel, &ClickLabel::clicked,
            outFullCospectraCheckBoxTs, &QCheckBox::toggle);
    connect(fullCospectraCo2Label, &ClickLabel::clicked,
            outFullCospectraCheckBoxCo2, &QCheckBox::toggle);
    connect(fullCospectraH2oLabel, &ClickLabel::clicked,
            outFullCospectraCheckBoxH2o, &QCheckBox::toggle);
    connect(fullCospectraCh4Label, &ClickLabel::clicked,
            outFullCospectraCheckBoxCh4, &QCheckBox::toggle);
    connect(fullCospectraN2oLabel, &ClickLabel::clicked,
            outFullCospectraCheckBoxN2o, &QCheckBox::toggle);

    connect(outSt1CheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutSt1(checked); });
    connect(outSt2CheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutSt2(checked); });
    connect(outSt3CheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutSt3(checked); });
    connect(outSt4CheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutSt4(checked); });
    connect(outSt5CheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutSt5(checked); });
    connect(outSt6CheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutSt6(checked); });
    connect(outSt7CheckBox, &QCheckBox::toggled, [=](bool checked)
            { ecProject_->setScreenOutSt7(checked); });

    connect(outRaw1CheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRaw1);
    connect(outRaw2CheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRaw2);
    connect(outRaw3CheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRaw3);
    connect(outRaw4CheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRaw4);
    connect(outRaw5CheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRaw5);
    connect(outRaw6CheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRaw6);
    connect(outRaw7CheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRaw7);

    connect(outRawUCheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRawU);
    connect(outRawVCheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRawV);
    connect(outRawWCheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRawW);
    connect(outRawTsCheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRawTs);
    connect(outRawCo2CheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRawCo2);
    connect(outRawH2oCheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRawH2o);
    connect(outRawCh4CheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRawCh4);
    connect(outRawGas4CheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRawGas4);
    connect(outRawTairCheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRawTair);
    connect(outRawPairCheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::updateOutputRawPair);
    connect(outVarsAllCheckBox, &QCheckBox::toggled,
            this, &AdvOutputOptions::checkVarsAll);

    connect(minSelectionButton, &QPushButton::clicked,
            this, &AdvOutputOptions::selectMin);
    connect(typicalSelectionButton, &QPushButton::clicked,
            this, &AdvOutputOptions::selectTypical);
    connect(fullSelectionButton, &QPushButton::clicked,
            this, &AdvOutputOptions::selectFull);

    connect(rawULabel,&ClickLabel::clicked,
            outRawUCheckBox, &QCheckBox::toggle);
    connect(rawVLabel, &ClickLabel::clicked,
            outRawVCheckBox, &QCheckBox::toggle);
    connect(rawWLabel, &ClickLabel::clicked,
            outRawWCheckBox, &QCheckBox::toggle);
    connect(rawTsLabel, &ClickLabel::clicked,
            outRawTsCheckBox, &QCheckBox::toggle);
    connect(rawCo2Label, &ClickLabel::clicked,
            outRawCo2CheckBox, &QCheckBox::toggle);
    connect(rawH2oLabel, &ClickLabel::clicked,
            outRawH2oCheckBox, &QCheckBox::toggle);
    connect(rawCh4Label, &ClickLabel::clicked,
            outRawCh4CheckBox, &QCheckBox::toggle);
    connect(rawGas4Label, &ClickLabel::clicked,
            outRawGas4CheckBox, &QCheckBox::toggle);
    connect(rawTairLabel, &ClickLabel::clicked,
            outRawTairCheckBox, &QCheckBox::toggle);
    connect(rawPairLabel, &ClickLabel::clicked,
            outRawPairCheckBox, &QCheckBox::toggle);

    connect(ecProject_, &EcProject::ecProjectNew,
            this, &AdvOutputOptions::reset);
    connect(ecProject_, &EcProject::ecProjectChanged,
            this, &AdvOutputOptions::refresh);

    // init
    QTimer::singleShot(0, this, SLOT(reset()));
}

void AdvOutputOptions::setSmartfluxUI()
{
    DEBUG_FUNC_NAME
    bool on = configState_->project.smartfluxMode;

    QList<QWidget *> enableableWidgets;
    enableableWidgets << outFullCheckBox
                      << fullOutformatLabel
                      << fixedVarsOutputRadio
                      << variableVarsOutputRadio
                      << errorFormatLabel
                      << errorFormatCombo
                      << createDatasetCheckBox
                      << outMeanCospCheckBox
                      << outAmFluxCheckBox
                      << outGhgEuCheckBox
                      << outBiometCheckBox
                      << outMdCheckBox
                      << outFullSpectraCheckBoxU
                      << outFullSpectraCheckBoxV
                      << outFullSpectraCheckBoxW
                      << outFullSpectraCheckBoxTs
                      << outFullSpectraCheckBoxCo2
                      << outFullSpectraCheckBoxH2o
                      << outFullSpectraCheckBoxCh4
                      << outFullSpectraCheckBoxN2o
                      << outFullCospectraCheckBoxU
                      << outFullCospectraCheckBoxV
                      << outFullCospectraCheckBoxTs
                      << outFullCospectraCheckBoxCo2
                      << outFullCospectraCheckBoxH2o
                      << outFullCospectraCheckBoxCh4
                      << outFullCospectraCheckBoxN2o
                      << outSt1CheckBox
                      << outSt2CheckBox
                      << outSt3CheckBox
                      << outSt4CheckBox
                      << outSt5CheckBox
                      << outSt6CheckBox
                      << outSt7CheckBox
                      << outRaw1CheckBox
                      << outRaw2CheckBox
                      << outRaw3CheckBox
                      << outRaw4CheckBox
                      << outRaw5CheckBox
                      << outRaw6CheckBox
                      << outRaw7CheckBox
                      << outRawUCheckBox
                      << outRawVCheckBox
                      << outRawWCheckBox
                      << outRawTsCheckBox
                      << outRawCo2CheckBox
                      << outRawH2oCheckBox
                      << outRawCh4CheckBox
                      << outRawGas4CheckBox
                      << outRawTairCheckBox
                      << outRawPairCheckBox
                      << outVarsAllCheckBox;

    foreach (QWidget* w, enableableWidgets)
    {
        if (on)
        {
            oldEnabled.push_back(w->isEnabled());
            w->setDisabled(on);
        }
        else
        {
            w->setEnabled(oldEnabled.at(enableableWidgets.indexOf(w)));
        }
    }

    QList<QWidget *> visibleWidgets;
    visibleWidgets << fullSelectionButton
            << fullSelectionDesc
            << minSelectionButton
            << minSelectionDesc
            << typicalSelectionButton
            << typicalSelectionDesc
            << hrLabel_1;

    foreach (QWidget* w, visibleWidgets)
    {
        if (on)
        {
            oldVisible.push_back(w->isEnabled());
            w->setHidden(on);
        }
        else
        {
            w->setVisible(oldVisible.at(visibleWidgets.indexOf(w)));
        }
    }

    // block project modified() signal
    bool oldmod = false;
    if (!on)
    {
        // save the modified flag to prevent side effects of setting widgets
        oldmod = ecProject_->modified();
        ecProject_->blockSignals(true);
    }

    QList<QAbstractButton *> checkableWidgets;
    checkableWidgets << outFullCheckBox
                     << outBiometCheckBox
                     << fixedVarsOutputRadio;
    foreach (QAbstractButton* w, checkableWidgets)
    {
        if (on)
        {
            w->setChecked(on);
        }
    }

    QList<QAbstractButton *> uncheckableWidgets;
    uncheckableWidgets << outAmFluxCheckBox
                       << outGhgEuCheckBox
                       << createDatasetCheckBox
                       << outMeanCospCheckBox
                       << outDetailsCheckBox
                       << outMdCheckBox
                       << outBinOgivesCheckBox
                       << outFullSpectraCheckBoxU
                       << outFullSpectraCheckBoxV
                       << outFullSpectraCheckBoxW
                       << outFullSpectraCheckBoxTs
                       << outFullSpectraCheckBoxCo2
                       << outFullSpectraCheckBoxH2o
                       << outFullSpectraCheckBoxCh4
                       << outFullSpectraCheckBoxN2o
                       << outFullCospectraCheckBoxU
                       << outFullCospectraCheckBoxV
                       << outFullCospectraCheckBoxTs
                       << outFullCospectraCheckBoxCo2
                       << outFullCospectraCheckBoxH2o
                       << outFullCospectraCheckBoxCh4
                       << outFullCospectraCheckBoxN2o
                       << outSt1CheckBox
                       << outSt2CheckBox
                       << outSt3CheckBox
                       << outSt4CheckBox
                       << outSt5CheckBox
                       << outSt6CheckBox
                       << outSt7CheckBox
                       << outRaw1CheckBox
                       << outRaw2CheckBox
                       << outRaw3CheckBox
                       << outRaw4CheckBox
                       << outRaw5CheckBox
                       << outRaw6CheckBox
                       << outRaw7CheckBox
                       << outRawUCheckBox
                       << outRawVCheckBox
                       << outRawWCheckBox
                       << outRawTsCheckBox
                       << outRawCo2CheckBox
                       << outRawH2oCheckBox
                       << outRawCh4CheckBox
                       << outRawGas4CheckBox
                       << outRawTairCheckBox
                       << outRawPairCheckBox
                       << outVarsAllCheckBox;
    foreach (QAbstractButton* w, uncheckableWidgets)
    {
        if (on)
        {
            w->setChecked(!on);
        }
    }

    if (on)
    {
        errorFormatCombo->setCurrentIndex(2);
    }

    // restore project modified() signal
    if (!on)
    {
        // restore modified flag
        ecProject_->setModified(oldmod);
        ecProject_->blockSignals(false);
    }
}

void AdvOutputOptions::reset()
{
    DEBUG_FUNC_NAME

    // save the modified flag to prevent side effects of setting widgets
    bool oldmod = ecProject_->modified();
    ecProject_->blockSignals(true);

    selectMin();
    setVarsAvailable(false);
    outVarsAllCheckBox->setEnabled(false);
    outVarsAllCheckBox->setChecked(false);
    variableVarsOutputRadio->setChecked(true);
    errorFormatCombo->setCurrentIndex(0);

    if (ecProject_->generalUseBiomet())
    {
        outBiometCheckBox->setChecked(true);
    }

    // restore modified flag
    ecProject_->setModified(oldmod);
    ecProject_->blockSignals(false);
}

void AdvOutputOptions::refresh()
{
    DEBUG_FUNC_NAME

    // save the modified flag to prevent side effects of setting widgets
    bool oldmod = ecProject_->modified();
    ecProject_->blockSignals(true);

    outBinSpectraCheckBox->setChecked(ecProject_->screenOutBinSpectra());
    outBinOgivesCheckBox->setChecked(ecProject_->screenOutBinOgives());
    outMeanCospCheckBox->setChecked(ecProject_->generalOutMeanCosp());

    outFullSpectraCheckBoxU->setChecked(ecProject_->screenOutFullSpectraU());
    outFullSpectraCheckBoxV->setChecked(ecProject_->screenOutFullSpectraV());
    outFullSpectraCheckBoxW->setChecked(ecProject_->screenOutFullSpectraW());
    outFullSpectraCheckBoxTs->setChecked(ecProject_->screenOutFullSpectraTs());
    outFullSpectraCheckBoxCo2->setChecked(ecProject_->screenOutFullSpectraCo2());
    outFullSpectraCheckBoxH2o->setChecked(ecProject_->screenOutFullSpectraH2o());
    outFullSpectraCheckBoxCh4->setChecked(ecProject_->screenOutFullSpectraCh4());
    outFullSpectraCheckBoxN2o->setChecked(ecProject_->screenOutFullSpectraN2o());

    outFullCospectraCheckBoxU->setChecked(ecProject_->screenOutFullCospectraU());
    outFullCospectraCheckBoxV->setChecked(ecProject_->screenOutFullCospectraV());
    outFullCospectraCheckBoxTs->setChecked(ecProject_->screenOutFullCospectraTs());
    outFullCospectraCheckBoxCo2->setChecked(ecProject_->screenOutFullCospectraCo2());
    outFullCospectraCheckBoxH2o->setChecked(ecProject_->screenOutFullCospectraH2o());
    outFullCospectraCheckBoxCh4->setChecked(ecProject_->screenOutFullCospectraCh4());
    outFullCospectraCheckBoxN2o->setChecked(ecProject_->screenOutFullCospectraN2o());

    outGhgEuCheckBox->setChecked(ecProject_->generalOutGhgEu());
    outAmFluxCheckBox->setChecked(ecProject_->generalOutAmFlux());
    outFullCheckBox->setChecked(ecProject_->generalOutRich());
    outDetailsCheckBox->setChecked(ecProject_->screenOutDetails());
    outMdCheckBox->setChecked(ecProject_->generalOutMd());
    outBiometCheckBox->setChecked(ecProject_->generalOutBiomet());
    createDatasetCheckBox->setChecked(ecProject_->generalMakeDataset());

    outputFormatRadioGroup->buttons().at(ecProject_->generalFixedOutFormat())->setChecked(true);

    QString s(ecProject_->generalErroLabel());
    if (!s.isEmpty())
    {
        int n = errorFormatCombo->findText(s);
        if (n < 0)
        {
            errorFormatCombo->addItem(s);
            errorFormatCombo->setCurrentIndex(errorFormatCombo->findText(s));
        }
        else
            errorFormatCombo->setCurrentIndex(n);
    }
    else
    {
        errorFormatCombo->setCurrentIndex(0);
    }

    outSt1CheckBox->setChecked(ecProject_->screenOutSt1());
    outSt2CheckBox->setChecked(ecProject_->screenOutSt2());
    outSt3CheckBox->setChecked(ecProject_->screenOutSt3());
    outSt4CheckBox->setChecked(ecProject_->screenOutSt4());
    outSt5CheckBox->setChecked(ecProject_->screenOutSt5());
    outSt6CheckBox->setChecked(ecProject_->screenOutSt6());
    outSt7CheckBox->setChecked(ecProject_->screenOutSt7());
    outRaw1CheckBox->setChecked(ecProject_->screenOutRaw1());
    outRaw2CheckBox->setChecked(ecProject_->screenOutRaw2());
    outRaw3CheckBox->setChecked(ecProject_->screenOutRaw3());
    outRaw4CheckBox->setChecked(ecProject_->screenOutRaw4());
    outRaw5CheckBox->setChecked(ecProject_->screenOutRaw5());
    outRaw6CheckBox->setChecked(ecProject_->screenOutRaw6());
    outRaw7CheckBox->setChecked(ecProject_->screenOutRaw7());
    outRawUCheckBox->setChecked(ecProject_->screenOutRawU());
    outRawVCheckBox->setChecked(ecProject_->screenOutRawV());
    outRawWCheckBox->setChecked(ecProject_->screenOutRawW());
    outRawTsCheckBox->setChecked(ecProject_->screenOutRawTs());
    outRawCo2CheckBox->setChecked(ecProject_->screenOutRawCo2());
    outRawH2oCheckBox->setChecked(ecProject_->screenOutRawH2o());
    outRawCh4CheckBox->setChecked(ecProject_->screenOutRawCh4());
    outRawGas4CheckBox->setChecked(ecProject_->screenOutRawGas4());
    outRawTairCheckBox->setChecked(ecProject_->screenOutRawTair());
    outRawPairCheckBox->setChecked(ecProject_->screenOutRawPair());
    setVarsAvailable(areTimeSeriesChecked());

    outVarsAllCheckBox->setChecked(areAllCheckedVars());
    outVarsAllCheckBox->setEnabled(areTimeSeriesChecked());

    // restore modified flag
    ecProject_->setModified(oldmod);
    ecProject_->blockSignals(false);
}

void AdvOutputOptions::updateOutBinSpectra(bool b)
{
    DEBUG_FUNC_NAME
    ecProject_->setScreenOutBinSpectra(b);
}

void AdvOutputOptions::updateBinSpectra(bool b)
{
    DEBUG_FUNC_NAME

    updateOutBinSpectra(b);
    outBinSpectraCheckBox->setDisabled(b);
}

void AdvOutputOptions::updateOutputRaw1(bool b)
{
    ecProject_->setScreenOutRaw1(b);
    updateVarsAvailable();
}

void AdvOutputOptions::updateOutputRaw2(bool b)
{
    ecProject_->setScreenOutRaw2(b);
    updateVarsAvailable();
}

void AdvOutputOptions::updateOutputRaw3(bool b)
{
    ecProject_->setScreenOutRaw3(b);
    updateVarsAvailable();
}

void AdvOutputOptions::updateOutputRaw4(bool b)
{
    ecProject_->setScreenOutRaw4(b);
    updateVarsAvailable();
}

void AdvOutputOptions::updateOutputRaw5(bool b)
{
    ecProject_->setScreenOutRaw5(b);
    updateVarsAvailable();
}

void AdvOutputOptions::updateOutputRaw6(bool b)
{
    ecProject_->setScreenOutRaw6(b);
    updateVarsAvailable();
}

void AdvOutputOptions::updateOutputRaw7(bool b)
{
    ecProject_->setScreenOutRaw7(b);
    updateVarsAvailable();
}
void AdvOutputOptions::updateOutputRawU(bool b)
{
    ecProject_->setScreenOutRawU(b);
    updateSelectAllCheckbox();
}

void AdvOutputOptions::updateOutputRawV(bool b)
{
    ecProject_->setScreenOutRawV(b);
    updateVarsAvailable();
}

void AdvOutputOptions::updateOutputRawW(bool b)
{
    ecProject_->setScreenOutRawW(b);
    updateVarsAvailable();
}

void AdvOutputOptions::updateOutputRawTs(bool b)
{
    ecProject_->setScreenOutRawTs(b);
    updateVarsAvailable();
}

void AdvOutputOptions::updateOutputRawCo2(bool b)
{
    ecProject_->setScreenOutRawCo2(b);
    updateVarsAvailable();
}

void AdvOutputOptions::updateOutputRawH2o(bool b)
{
    ecProject_->setScreenOutRawH2o(b);
    updateVarsAvailable();
}

void AdvOutputOptions::updateOutputRawCh4(bool b)
{
    ecProject_->setScreenOutRawCh4(b);
    updateVarsAvailable();
}

void AdvOutputOptions::updateOutputRawGas4(bool b)
{
    ecProject_->setScreenOutRawGas4(b);
    updateVarsAvailable();
}

void AdvOutputOptions::updateOutputRawTair(bool b)
{
    ecProject_->setScreenOutRawTair(b);
    updateVarsAvailable();
}

void AdvOutputOptions::updateOutputRawPair(bool b)
{
    ecProject_->setScreenOutRawPair(b);
    updateVarsAvailable();
}

void AdvOutputOptions::checkFullSpectraAll(bool b)
{
    outFullSpectraCheckBoxU->setChecked(b);
    outFullSpectraCheckBoxV->setChecked(b);
    outFullSpectraCheckBoxW->setChecked(b);
    outFullSpectraCheckBoxTs->setChecked(b);
    outFullSpectraCheckBoxCo2->setChecked(b);
    outFullSpectraCheckBoxH2o->setChecked(b);
    outFullSpectraCheckBoxCh4->setChecked(b);
    outFullSpectraCheckBoxN2o->setChecked(b);
}

bool AdvOutputOptions::areAllCheckedFullSpectra()
{
    return (outFullSpectraCheckBoxU->isChecked()
            && outFullSpectraCheckBoxV->isChecked()
            && outFullSpectraCheckBoxW->isChecked()
            && outFullSpectraCheckBoxTs->isChecked()
            && outFullSpectraCheckBoxCo2->isChecked()
            && outFullSpectraCheckBoxH2o->isChecked()
            && outFullSpectraCheckBoxCh4->isChecked()
            && outFullSpectraCheckBoxN2o->isChecked());
}

bool AdvOutputOptions::areAllCheckedVars()
{
    return (outRawUCheckBox->isChecked()
            && outRawVCheckBox->isChecked()
            && outRawWCheckBox->isChecked()
            && outRawTsCheckBox->isChecked()
            && outRawCo2CheckBox->isChecked()
            && outRawH2oCheckBox->isChecked()
            && outRawCh4CheckBox->isChecked()
            && outRawGas4CheckBox->isChecked()
            && outRawTairCheckBox->isChecked()
            && outRawPairCheckBox->isChecked());
}

void AdvOutputOptions::checkFullCospectraAll(bool b)
{
    outFullCospectraCheckBoxU->setChecked(b);
    outFullCospectraCheckBoxV->setChecked(b);
    outFullCospectraCheckBoxTs->setChecked(b);
    outFullCospectraCheckBoxCo2->setChecked(b);
    outFullCospectraCheckBoxH2o->setChecked(b);
    outFullCospectraCheckBoxCh4->setChecked(b);
    outFullCospectraCheckBoxN2o->setChecked(b);
}

void AdvOutputOptions::checkStAll(bool b)
{
    DEBUG_FUNC_NAME
    outSt1CheckBox->setChecked(b);
    outSt2CheckBox->setChecked(b);
    outSt3CheckBox->setChecked(b);
    outSt4CheckBox->setChecked(b);
    outSt5CheckBox->setChecked(b);
    outSt6CheckBox->setChecked(b);
    outSt7CheckBox->setChecked(b);
}

void AdvOutputOptions::checkTimeSeriesAll(bool b)
{
    DEBUG_FUNC_NAME
    outRaw1CheckBox->setChecked(b);
    outRaw2CheckBox->setChecked(b);
    outRaw3CheckBox->setChecked(b);
    outRaw4CheckBox->setChecked(b);
    outRaw5CheckBox->setChecked(b);
    outRaw6CheckBox->setChecked(b);
    outRaw7CheckBox->setChecked(b);
}

void AdvOutputOptions::checkVarsAll(bool b)
{
    DEBUG_FUNC_NAME
    outRawUCheckBox->setChecked(b);
    outRawVCheckBox->setChecked(b);
    outRawWCheckBox->setChecked(b);
    outRawTsCheckBox->setChecked(b);
    outRawCo2CheckBox->setChecked(b);
    outRawH2oCheckBox->setChecked(b);
    outRawCh4CheckBox->setChecked(b);
    outRawGas4CheckBox->setChecked(b);
    outRawTairCheckBox->setChecked(b);
    outRawPairCheckBox->setChecked(b);
}

void AdvOutputOptions::selectMin()
{
    outFullCheckBox->setChecked(true);
    outAmFluxCheckBox->setChecked(false);
    outGhgEuCheckBox->setChecked(false);
    outDetailsCheckBox->setChecked(false);
    outMdCheckBox->setChecked(true);
    outBiometCheckBox->setChecked(false);
    createDatasetCheckBox->setChecked(false);
    outBinSpectraCheckBox->setChecked(true);
    outBinOgivesCheckBox->setChecked(false);
    outMeanCospCheckBox->setChecked(false);
    checkFullSpectraAll(false);
    checkFullCospectraAll(false);
    outFullCospectraCheckBoxTs->setChecked(true);
    checkStAll(false);
    outSt1CheckBox->setChecked(true);
    checkTimeSeriesAll(false);
    checkVarsAll(false);
}

void AdvOutputOptions::selectTypical()
{
    outFullCheckBox->setChecked(true);
    outAmFluxCheckBox->setChecked(false);
    outGhgEuCheckBox->setChecked(false);
    outDetailsCheckBox->setChecked(false);
    outMdCheckBox->setChecked(true);
    outBiometCheckBox->setChecked(true);
    createDatasetCheckBox->setChecked(true);
    outBinSpectraCheckBox->setChecked(true);
    outBinOgivesCheckBox->setChecked(true);
    outMeanCospCheckBox->setChecked(true);
    checkFullSpectraAll(false);
    checkFullCospectraAll(false);
    outFullCospectraCheckBoxTs->setChecked(true);
    checkStAll(true);
    checkTimeSeriesAll(false);
    checkVarsAll(false);
}

void AdvOutputOptions::selectFull()
{
    outFullCheckBox->setChecked(true);
    outAmFluxCheckBox->setChecked(true);
    outGhgEuCheckBox->setChecked(true);
    outDetailsCheckBox->setChecked(true);
    outMdCheckBox->setChecked(true);
    outBiometCheckBox->setChecked(true);
    createDatasetCheckBox->setChecked(true);
    outBinSpectraCheckBox->setChecked(true);
    outBinOgivesCheckBox->setChecked(true);
    outMeanCospCheckBox->setChecked(true);
    checkFullSpectraAll(false);
    checkFullCospectraAll(false);
    outFullCospectraCheckBoxTs->setChecked(true);
    checkStAll(true);
    checkTimeSeriesAll(false);
    checkVarsAll(false);
}

void AdvOutputOptions::fullSpectraULabelClicked()
{
    outFullSpectraCheckBoxU->toggle();
}

void AdvOutputOptions::setVarsAvailable(bool ok)
{
    rawULabel->setEnabled(ok);
    rawVLabel->setEnabled(ok);
    rawWLabel->setEnabled(ok);
    rawTsLabel->setEnabled(ok);
    rawCo2Label->setEnabled(ok);
    rawH2oLabel->setEnabled(ok);
    rawCh4Label->setEnabled(ok);
    rawGas4Label->setEnabled(ok);
    rawTairLabel->setEnabled(ok);
    rawPairLabel->setEnabled(ok);

    outRawUCheckBox->setEnabled(ok);
    outRawVCheckBox->setEnabled(ok);
    outRawWCheckBox->setEnabled(ok);
    outRawTsCheckBox->setEnabled(ok);
    outRawCo2CheckBox->setEnabled(ok);
    outRawH2oCheckBox->setEnabled(ok);
    outRawCh4CheckBox->setEnabled(ok);
    outRawGas4CheckBox->setEnabled(ok);
    outRawTairCheckBox->setEnabled(ok);
    outRawPairCheckBox->setEnabled(ok);
    outVarsAllCheckBox->setEnabled(ok);
}

bool AdvOutputOptions::areTimeSeriesChecked()
{
    return (outRaw1CheckBox->isChecked()
            || outRaw2CheckBox->isChecked()
            || outRaw3CheckBox->isChecked()
            || outRaw4CheckBox->isChecked()
            || outRaw5CheckBox->isChecked()
            || outRaw6CheckBox->isChecked()
            || outRaw7CheckBox->isChecked());
}

void AdvOutputOptions::updateVarsAvailable()
{
    setVarsAvailable(areTimeSeriesChecked());
    outVarsAllCheckBox->setEnabled(areTimeSeriesChecked());
}

void AdvOutputOptions::updateOutputs(int n)
{
    DEBUG_FUNC_NAME

    qDebug() << n << "ecProject_->spectraMode()"
             << ecProject_->spectraMode();

    if (n == 1 || n == 2)
    {
        qDebug() << "ecProject_->generalBinSpectraAvail()"
                 << ecProject_->generalBinSpectraAvail();

        if (ecProject_->spectraMode())
        {
            if (ecProject_->generalBinSpectraAvail())
            {
                outBinSpectraCheckBox->setChecked(false);
                outBinSpectraCheckBox->setEnabled(true);
            }
            else
            {
                outBinSpectraCheckBox->setChecked(true);
                outBinSpectraCheckBox->setEnabled(false);
            }
        }
        else
        {
            outBinSpectraCheckBox->setEnabled(true);
        }
    }
    else if (n == 3)
    {
        qDebug() << "ecProject_->generalBinSpectraAvail()"
                 << ecProject_->generalBinSpectraAvail();

        if (ecProject_->spectraMode())
        {
            if (ecProject_->generalBinSpectraAvail())
            {
                outBinSpectraCheckBox->setChecked(false);
                outBinSpectraCheckBox->setEnabled(true);
            }
            else
            {
                outBinSpectraCheckBox->setChecked(true);
                outBinSpectraCheckBox->setEnabled(false);
            }
        }
        else
        {
            outBinSpectraCheckBox->setEnabled(true);
        }

        qDebug() << "ecProject_->generalFullSpectraAvail()"
                 << ecProject_->generalFullSpectraAvail();
        if (ecProject_->generalFullSpectraAvail())
        {
            outFullCospectraCheckBoxTs->setChecked(false);
            outFullCospectraCheckBoxTs->setEnabled(true);
            fullCospectraTsLabel->setEnabled(true);
        }
        else
        {
            outFullCospectraCheckBoxTs->setChecked(true);
            outFullCospectraCheckBoxTs->setEnabled(false);
            fullCospectraTsLabel->setEnabled(false);
        }
    }
    else
    {
        outBinSpectraCheckBox->setEnabled(true);
        outFullCospectraCheckBoxTs->setEnabled(true);
        fullCospectraTsLabel->setEnabled(true);
    }
}

void AdvOutputOptions::updateFixedOuputFormat(int n)
{
    ecProject_->setGeneralFixedOutFormat(n);
}

void AdvOutputOptions::updateErrorLabel(const QString& s)
{
    DEBUG_FUNC_NAME
    if (s.isEmpty() || s.toUpper() == QLatin1String("NONE"))
    {
        WidgetUtils::warning(nullptr,
                             tr("Error Label"),
                             tr("Enter a label other than "
                                "\"none\" (case insensitive)."));

        errorFormatCombo->setCurrentIndex(0);
        return;
    }
    ecProject_->setGeneralErrorLabel(s);
}

void AdvOutputOptions::createQuestionMark()
{
    questionMark_1 = new QPushButton;
    questionMark_1->setObjectName(QStringLiteral("questionMarkImg"));
    questionMark_2 = new QPushButton;
    questionMark_2->setObjectName(QStringLiteral("questionMarkImg"));
    questionMark_4 = new QPushButton;
    questionMark_4->setObjectName(QStringLiteral("questionMarkImg"));
    questionMark_5 = new QPushButton;
    questionMark_5->setObjectName(QStringLiteral("questionMarkImg"));
    questionMark_8 = new QPushButton;
    questionMark_8->setObjectName(QStringLiteral("questionMarkImg"));
    questionMark_9 = new QPushButton;
    questionMark_9->setObjectName(QStringLiteral("questionMarkImg"));

    connect(questionMark_1, &QPushButton::clicked,
            this, &AdvOutputOptions::onlineHelpTrigger_1);
    connect(questionMark_2, &QPushButton::clicked,
            this, &AdvOutputOptions::onlineHelpTrigger_2);
    connect(questionMark_4, &QPushButton::clicked,
            this, &AdvOutputOptions::onlineHelpTrigger_4);
    connect(questionMark_5, &QPushButton::clicked,
            this, &AdvOutputOptions::onlineHelpTrigger_5);
    connect(questionMark_8, &QPushButton::clicked,
            this, &AdvOutputOptions::onlineHelpTrigger_8);
    connect(questionMark_9, &QPushButton::clicked,
            this, &AdvOutputOptions::onlineHelpTrigger_9);
}

void AdvOutputOptions::onlineHelpTrigger_1()
{
    WidgetUtils::showHelp(QUrl(QStringLiteral("http://envsupport.licor.com/help/EddyPro5/index.htm#Output_Files_Overview.htm")));
}

void AdvOutputOptions::onlineHelpTrigger_2()
{
    WidgetUtils::showHelp(QUrl(QStringLiteral("http://envsupport.licor.com/help/EddyPro5/index.htm#Output_Files.htm")));
}

void AdvOutputOptions::onlineHelpTrigger_3()
{
    WidgetUtils::showHelp(QUrl(QStringLiteral("http://")));
}

void AdvOutputOptions::onlineHelpTrigger_4()
{
    WidgetUtils::showHelp(QUrl(QStringLiteral("http://envsupport.licor.com/help/EddyPro5/index.htm#Output_Files_Full_Output.htm#Build")));
}

void AdvOutputOptions::onlineHelpTrigger_5()
{
    WidgetUtils::showHelp(QUrl(QStringLiteral("http://envsupport.licor.com/help/EddyPro5/index.htm#Calculating_Spectra_Cospectra_and_Ogives.htm")));
}

void AdvOutputOptions::onlineHelpTrigger_6()
{
    WidgetUtils::showHelp(QUrl(QStringLiteral("http://")));
}

void AdvOutputOptions::onlineHelpTrigger_7()
{
    WidgetUtils::showHelp(QUrl(QStringLiteral("http://")));
}

void AdvOutputOptions::onlineHelpTrigger_8()
{
    WidgetUtils::showHelp(QUrl(QStringLiteral("http://envsupport.licor.com/help/EddyPro5/index.htm#Output_Files_The_Stats_Folder.htm")));
}

void AdvOutputOptions::onlineHelpTrigger_9()
{
    WidgetUtils::showHelp(QUrl(QStringLiteral("http://envsupport.licor.com/help/EddyPro5/index.htm#Output_Files.htm#Statistics")));
}

void AdvOutputOptions::onClickerrorFormatLabel()
{
    errorFormatCombo->setFocus();
    errorFormatCombo->showPopup();
}

void AdvOutputOptions::checkMetadataOutput()
{
    if (!outMdCheckBox->isChecked())
    {
        outMdCheckBox->setChecked(true);
    }
}

void AdvOutputOptions::updateSelectAllCheckbox()
{
    DEBUG_FUNC_NAME

    outVarsAllCheckBox->blockSignals(true);

    if (areAllCheckedVars())
        outVarsAllCheckBox->setChecked(true);
    else
        outVarsAllCheckBox->setChecked(false);

    outVarsAllCheckBox->blockSignals(false);
}

void AdvOutputOptions::setOutputBiomet()
{
    // save the modified flag to prevent side effects of setting widgets
    bool oldmod = ecProject_->modified();
    ecProject_->blockSignals(true);

    outBiometCheckBox->setChecked(true);

    // restore modified flag
    ecProject_->setModified(oldmod);
    ecProject_->blockSignals(false);
}