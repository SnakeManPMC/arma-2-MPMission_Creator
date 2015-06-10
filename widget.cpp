#include "widget.h"
#include "ui_widget.h"

#include <QDir>
#include <QTextStream>

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);
}

Widget::~Widget()
{
	delete ui;
}

void Widget::on_createMission_clicked()
{
	// grab the campaign name from the lineEdit box
	QString MPMissionDir = ui->missionName->text();
	MPMissionDir.append("." + ui->terrain->currentText());

	// remove all the garbage characters
	//MPMissionDir.replace(QRegExp("[()@!\\/]"), "_");

	// replace spaces with underscore
	MPMissionDir.replace(" ", "_");

	// lower case for linux dedicated server enjoyment
	MPMissionDir = MPMissionDir.toLower();

	// create the directory
	QDir file;
	file.mkdir (MPMissionDir);
	file.cd (MPMissionDir);

	// create scripts sub dir
	file.mkdir("PMC");

	// create description.ext file to the mission dir root
	QString descriptionExt = file.absolutePath() + "/description.ext";
	QFile des(descriptionExt);
	des.open(QIODevice::WriteOnly | QIODevice::Text);

	// this is stupid but dont know how to return the bool from these guys?
	QString disabledAI = "true";
	QString showGPS = "true";
	QString onLoadIntroTime = "false";
	QString onLoadMissionTime = "false";

	// fill up description.ext contents
	if (ui->disableAI->isChecked())
	{
		disabledAI = "true";
	}
	else
	{
		disabledAI = "false";
	}

	if (ui->showGPS->isChecked())
	{
		showGPS = "true";
	}
	else
	{
		showGPS = "false";
	}

	if (ui->onLoadIntroTime->isChecked())
	{
		onLoadIntroTime = "true";
	}
	else
	{
		onLoadIntroTime = "false";
	}

	if (ui->onLoadMissionTime->isChecked())
	{
		onLoadMissionTime = "true";
	}
	else
	{
		onLoadMissionTime = "false";
	}

	QString str1 = "/*\n\n\t" + ui->missionName->text() + "\n\n*/\n\n";
	str1.append("respawn = \"" + ui->respawn->currentText() + "\";\nrespawndelay = " + ui->respawnDelay->text() + ";\nonLoadMission = \"" + ui->onLoadMission->text() + "\";\n");
	str1.append("OnLoadIntro = \"" + ui->onLoadIntro->text() + "\";\nOnLoadIntroTime = " + onLoadIntroTime + ";\nOnLoadMissionTime = " + onLoadMissionTime + ";\n");
	str1.append("disabledAI = " + disabledAI + ";\nShowGPS = " + showGPS + ";\n\n");

	// settings class header thing
	str1.append("class Header\n{\n\tgameType = " + ui->type->currentText() + ";\n\tminPlayers = 1;\n\tmaxPlayers = " + ui->numberOfPlayers->text() + ";\n\tplayerCountMultipleOf = 1;\n};");

	QTextStream kala1(&des);
	kala1 << str1;
	des.close();

	QString initSQF = file.absolutePath() + "/init.sqf";
	QFile isqf(initSQF);
	isqf.open(QIODevice::WriteOnly | QIODevice::Text);

	str1.clear();
	str1.append("\n// if on server\nif (isServer) then\n{\n\t//[] execVM \"PMC\\PMC_init.sqf\";\n}\nelse\n{\n\t// set our weather using multiplayer parameter array\n");
	str1.append("\t//[] execVM \"PMC\\PMC_weather_with_mp_parameter.sqf\";\n\tplayer setVariable [\"BIS_noCoreConversations\", true];\n};\n");

	QTextStream kala2(&isqf);
	kala2 << str1;
	isqf.close();
}
