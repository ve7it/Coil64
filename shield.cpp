/* shield.cpp - source text to Coil64 - Radio frequency inductor and choke calculator
Copyright (C) 2019 Kustarev V.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses
*/

#include "shield.h"
#include "ui_shield.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Shield::Shield(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Shield)
{
    ui->setupUi(this);
    fOpt = new _OptionStruct;
    dv = new QDoubleValidator;
    ui->lineEdit_1->setValidator(dv);
    ui->lineEdit_2->setValidator(dv);
    ui->lineEdit_3->setValidator(dv);
    ui->lineEdit_4->setValidator(dv);
    ui->lineEdit_N->setValidator(dv);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Shield::~Shield()
{
    double L = loc.toDouble(ui->lineEdit_N->text());
    double D = loc.toDouble(ui->lineEdit_1->text())*fOpt->dwLengthMultiplier;
    double l = loc.toDouble(ui->lineEdit_2->text())*fOpt->dwLengthMultiplier;
    double Ds = loc.toDouble(ui->lineEdit_3->text())*fOpt->dwLengthMultiplier;
    double Hs = loc.toDouble(ui->lineEdit_4->text())*fOpt->dwLengthMultiplier;
    QSettings *settings;
    defineAppSettings(settings);
    settings->beginGroup( "Shield" );
    settings->setValue("pos", this->pos());
    settings->setValue("L", L);
    settings->setValue("D", D);
    settings->setValue("l", l);
    settings->setValue("Ds", Ds);
    settings->setValue("Hs", Hs);
    settings->setValue("isShieldRectangular", ui->radioButton_2->isChecked());
    settings->endGroup();
    delete settings;
    delete fOpt;
    delete dv;
    delete ui;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Shield::getCurrentLocale(QLocale locale){
    this->loc = locale;
    this->setLocale(loc);
    dv->setLocale(loc);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Shield::getOpt(_OptionStruct gOpt){
    *fOpt = gOpt;
    QString tmp_txt = tr("Inductance") + " L:";
    ui->label_N->setText(tmp_txt);
    tmp_txt = tr("Winding diameter") + " D:";
    ui->label_1->setText(tmp_txt);
    tmp_txt = tr("Winding length") + " l:";
    ui->label_2->setText(tmp_txt);
    tmp_txt = tr("Diameter of the coil shield") + " Ds:";
    ui->label_3->setText(tmp_txt);
    tmp_txt = tr("Height of the coil shield") + " Hs:";
    ui->label_4->setText(tmp_txt);
    ui->label_N_m->setText(qApp->translate("Context", fOpt->ssInductanceMeasureUnit.toUtf8()));
    ui->label_01->setText(qApp->translate("Context", fOpt->ssLengthMeasureUnit.toUtf8()));
    ui->label_02->setText(qApp->translate("Context", fOpt->ssLengthMeasureUnit.toUtf8()));
    ui->label_03->setText(qApp->translate("Context", fOpt->ssLengthMeasureUnit.toUtf8()));
    ui->label_04->setText(qApp->translate("Context", fOpt->ssLengthMeasureUnit.toUtf8()));
    QFont f1 = this->font();
    f1.setFamily(fOpt->mainFontFamily);
    f1.setPixelSize(fOpt->mainFontSize);
    this->setFont(f1);
    QSettings *settings;
    defineAppSettings(settings);
    settings->beginGroup( "Shield" );
    double L = settings->value("L", 0).toDouble();
    double D = settings->value("D", 0).toDouble();
    double l = settings->value("l", 0).toDouble();
    double Ds = settings->value("Ds", 0).toDouble();
    double Hs = settings->value("Hs", 0).toDouble();
    bool isShieldRectangular = settings->value("isShieldRectangular",false).toBool();
    QRect screenGeometry = qApp->primaryScreen()->availableGeometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2;
    QPoint pos = settings->value("pos", QPoint(x, y)).toPoint();
    settings->endGroup();
    if (isShieldRectangular){
        ui->radioButton_2->setChecked(true);
        this->on_radioButton_2_clicked();
    } else {
        ui->radioButton_2->setChecked(false);
        this->on_radioButton_clicked();
    }
    ui->lineEdit_N->setText(loc.toString(L));
    ui->lineEdit_1->setText(loc.toString(D / fOpt->dwLengthMultiplier));
    ui->lineEdit_2->setText(loc.toString(l / fOpt->dwLengthMultiplier));
    ui->lineEdit_3->setText(loc.toString(Ds / fOpt->dwLengthMultiplier));
    ui->lineEdit_4->setText(loc.toString(Hs / fOpt->dwLengthMultiplier));
    move(pos);
    delete settings;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Shield::on_pushButton_2_clicked()
{
    this->close();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Shield::on_pushButton_3_clicked()
{
    QDesktopServices::openUrl(QUrl("https://coil32.net/shield-inductance.html"));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Shield::on_radioButton_2_clicked()
{
    ui->pict->setPixmap(QPixmap(":/images/res/shield-rect.png"));
    ui->lineEdit_N->setFocus();
    ui->lineEdit_N->selectAll();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Shield::on_radioButton_clicked()
{
    ui->pict->setPixmap(QPixmap(":/images/res/shield-round.png"));
    ui->lineEdit_N->setFocus();
    ui->lineEdit_N->selectAll();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Shield::on_pushButton_clicked()
{
    if ((ui->lineEdit_N->text().isEmpty())||(ui->lineEdit_1->text().isEmpty())||(ui->lineEdit_2->text().isEmpty())||(ui->lineEdit_3->text().isEmpty())
            ||(ui->lineEdit_4->text().isEmpty())){
        showWarning(tr("Warning"), tr("One or more inputs are empty!"));
        return;
    }
    bool ok1,ok2, ok3, ok4, ok5;
    double L = loc.toDouble(ui->lineEdit_N->text(), &ok1);
    double D = loc.toDouble(ui->lineEdit_1->text(), &ok2)*fOpt->dwLengthMultiplier;
    double l = loc.toDouble(ui->lineEdit_2->text(), &ok3)*fOpt->dwLengthMultiplier;
    double Ds = loc.toDouble(ui->lineEdit_3->text(), &ok4)*fOpt->dwLengthMultiplier;
    double Hs = loc.toDouble(ui->lineEdit_4->text(), &ok5)*fOpt->dwLengthMultiplier;
    QString sShieldForm = ui->radioButton->text();
    if (ui->radioButton_2->isChecked()){
        Ds = Ds * ((1 + sqrt(2)) / 2);
        sShieldForm = ui->radioButton_2->text();
    }
    if((!ok1)||(!ok2)||(!ok3)||(!ok4)||(!ok5)){
        showWarning(tr("Warning"), tr("One or more inputs have an illegal format!"));
        return;
    }
    if ((L == 0)||(D == 0)||(l == 0)||(Ds == 0) || (Hs == 0)){
        showWarning(tr("Warning"), tr("One or more inputs are equal to null!"));
        return;
    }
    if (D >= Ds){
        showWarning(tr("Warning"), "D >= Ds");
        return;
    }
    if (l >= Hs){
        showWarning(tr("Warning"), "l >= Hs");
        return;
    }
    double L_shilded = findSheildedInductance(L, D, Ds, l, Hs);
    QString sResult = "<hr><h2>" +QCoreApplication::applicationName() + " " + QCoreApplication::applicationVersion() + " - " + windowTitle() + "</h2><br/>";
    if (fOpt->isInsertImage){
        sResult += "<img src=\":/images/res/shield.png\">";
    }
    sResult += "<p><u>" + tr("Input data") + ":</u><br/>";
    sResult += ui->label_N->text() + " " + ui->lineEdit_N->text() + " " + ui->label_N_m->text() + "<br/>";
    sResult += "<u>" + tr("Dimensions") + ":</u><br/>";
    sResult += ui->label_1->text() + " " + ui->lineEdit_1->text() + " " + ui->label_01->text() + "<br/>";
    sResult += ui->label_2->text() + " " + ui->lineEdit_2->text() + " " + ui->label_02->text() + "<br/>";
    sResult += ui->label_3->text() + " " + ui->lineEdit_3->text() + " " + ui->label_03->text() + "<br/>";
    sResult += ui->label_4->text() + " " + ui->lineEdit_4->text() + " " + ui->label_04->text() + "<br/>";
    sResult += ui->groupBox_3->title() + ": " + sShieldForm + "</p>";
    sResult += "<hr>";
    sResult += "<p><u>" + tr("Result") + ":</u><br/>";
    sResult += tr("Inductance of the shielded coil") + " Ls = " + loc.toString(L_shilded, 'f', fOpt->dwAccuracy) + " "
            + qApp->translate("Context", fOpt->ssInductanceMeasureUnit.toUtf8()) + "<br/>";
    sResult += tr("Relative reducing of the inductance") + ": " + loc.toString(round(100 - L_shilded * 100 / L)) + "%" + "<br/>";
    sResult += "</p><hr>";
    emit sendResult(sResult);
}
