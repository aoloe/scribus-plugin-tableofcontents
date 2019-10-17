#include "settings.h"

#include "prefsstructs.h"
#include "iconmanager.h"
#include "api/document.h"

#include <QInputDialog>
#include <QComboBox>

#include <algorithm>

namespace ScribusPlugin {
namespace TableOfContents {

Settings::Settings(QWidget *parent) :
    Prefs_Pane(parent)
{
    setupUi(static_cast<QWidget*>(this));
	// languageChange();
	
	m_caption = tr("Table of Contents");
	m_icon = "tabtocindex_16.png";

	levelDownButton->setIcon(IconManager::instance().loadPixmap("16/go-down.png"));
	levelUpButton->setIcon(IconManager::instance().loadPixmap("16/go-up.png"));
	
	pageNumberPositionList->addItem(tr("Not shown"));
	pageNumberPositionList->addItem(tr("At beginning"));
	pageNumberPositionList->addItem(tr("At end"));

	auto document = API::Document::getActive();
	for (auto name: document->getParagraphStyleNames())
	{
		styleHeadingList->addItem(QString::fromStdString(name));
		styleTocList->addItem(QString::fromStdString(name));
	}
	pageNumberStyleList->addItem(QString::fromStdString(emptyItem));
	separatorStyleList->addItem(QString::fromStdString(emptyItem));
	for (auto name: document->getCharacterStyleNames())
	{
		pageNumberStyleList->addItem(QString::fromStdString(name));
		separatorStyleList->addItem(QString::fromStdString(name));
	}

	
	// NOTE: i could not get he drag and drop mode to correctly work
	// for reordening and i've added up and down buttons.
	// if you have a solution for this, it would be amazing to get it
	// to work! (ale)
	// levelList->setDragDropMode(QAbstractItemView::InternalMove);

	// TODO: read from the document settings

	// 	toc.name = tr("Default Table of Contents").toStdString();

	if (tocs.empty())
	{
		tableOfContentsList->setEnabled(false);
		deleteTableOfContentsButton->setEnabled(false);
		levelGroup->setEnabled(false);
		addLevelButton->setEnabled(false);
		deleteLevelButton->setEnabled(false);
	}
	else
	{
		for (auto &toc: tocs)
			tableOfContentsList->addItem(QString::fromStdString(toc.name));
		tableOfContentsList->setEnabled(true);
		deleteTableOfContentsButton->setEnabled(true);
		currentTocIndex = 0;
		tableOfContentsList->setCurrentIndex(currentTocIndex);

		levelGroup->setEnabled(true);
		showTableOfContentsLevels();
	}

	connect(tableOfContentsList, QOverload<int>::of(&QComboBox::activated), this, &Settings::selectTableOfContents);

	connect(addTableOfContentsButton, &QPushButton::clicked, this, QOverload<>::of(&Settings::addTableOfContents));
	connect(deleteTableOfContentsButton, &QPushButton::clicked, this, &Settings::deleteTableOfContents);
	connect(levelList, &QListWidget::currentRowChanged, this, &Settings::selectLevel);
	connect(addLevelButton, &QPushButton::clicked, this, QOverload<>::of(&Settings::addLevel));
	connect(deleteLevelButton, &QPushButton::clicked, this, &Settings::deleteLevel);
	connect(levelDownButton, &QPushButton::clicked, this, &Settings::levelDown);
	connect(levelUpButton, &QPushButton::clicked, this, &Settings::levelUp);

	connect(styleHeadingList, QOverload<int>::of(&QComboBox::activated), this, &Settings::showLevelListText);
	connect(styleTocList, QOverload<int>::of(&QComboBox::activated), this, &Settings::showLevelListText);
	connect(createStyleHeadingButton, &QPushButton::clicked, this, &Settings::addParagraphStyle);
	connect(createStyleTocButton, &QPushButton::clicked, this, &Settings::addParagraphStyle);
	connect(createStylePageNumberButton, &QPushButton::clicked, this, &Settings::addCharacterStyle);
	connect(createStyleSeparatorButton, &QPushButton::clicked, this, &Settings::addCharacterStyle);
}

Settings::~Settings()
{
    // delete ui;
}

void Settings::restoreDefaults(struct ApplicationPrefs *prefsData)
{
	// TODO: does probably not make any sense here
}

void Settings::saveGuiToPrefs(struct ApplicationPrefs *prefsData) const
{
	// TODO: when OK was pressed, first store the current level
}

/// @brief Ask for a TOC name and trigger the creation of a new table of contents
void Settings::addTableOfContents()
{
	bool okPressed  = false;
	QString defaultName = tocs.empty() ? tr("Default Table of Contents") : "";
	auto name = QInputDialog::getText(this,
		tr("New Table of Contents"), tr("Table of Contents Name"),
		QLineEdit::Normal, defaultName, &okPressed).toStdString();
	if (okPressed && !name.empty())
	{
		for (const auto &toc: tocs)
			if (toc.name == name)
				return;

		if (!tocs.empty())
			storeCurrentLevel();

		addTableOfContents(name);
	}
}

void Settings::addTableOfContents(std::string name)
{
	Toc toc;
	toc.name = name;
	toc.levels.push_back(getDefaultLevel());
	tocs.push_back(toc);

	tableOfContentsList->blockSignals(true);
	tableOfContentsList->addItem(QString::fromStdString(name));
	tableOfContentsList->setCurrentIndex(tableOfContentsList->count() - 1);
	tableOfContentsList->blockSignals(false);

	currentTocIndex = tableOfContentsList->currentIndex();

	levelGroup->setEnabled(true);

	showTableOfContentsLevels();

	tableOfContentsList->setEnabled(true);
	deleteTableOfContentsButton->setEnabled(true);

	addLevelButton->setEnabled(true);
}

void Settings::deleteTableOfContents()
{
	if (tocs.empty())
		return;

	tocs.erase(tocs.begin() + currentTocIndex);

	tableOfContentsList->blockSignals(true);
	tableOfContentsList->removeItem(currentTocIndex);
	tableOfContentsList->blockSignals(false);


	if (tocs.empty())
	{
		levelList->blockSignals(true);
		levelList->clear();
		levelList->blockSignals(false);

		tableOfContentsList->setEnabled(false);
		deleteTableOfContentsButton->setEnabled(false);

		levelGroup->setEnabled(false);

		setLevelButtonsStatus();
	}
	else
	{
		currentTocIndex = tableOfContentsList->currentIndex();
		showTableOfContentsLevels();
	}
}

void Settings::selectTableOfContents(int index)
{
	if (tableOfContentsList->currentIndex() == currentTocIndex)
		return;

	storeCurrentLevel();
	currentTocIndex = tableOfContentsList->currentIndex();

	showTableOfContentsLevels();

	setLevelButtonsStatus();
}

void Settings::showTableOfContentsLevels()
{
	levelList->blockSignals(true);
	levelList->clear();
	levelList->blockSignals(false);

	const auto& levels = tocs.at(currentTocIndex).levels;
	for (const auto &level: levels)
		addLevel(level);

	currentLevelIndex = 0;
	levelList->blockSignals(true);
	levelList->setCurrentRow(currentLevelIndex);
	levelList->blockSignals(false);

	readCurrentLevel();

	setLevelButtonsStatus();
}

void Settings::selectLevel(int currentRow)
{
	storeCurrentLevel();
	currentLevelIndex = levelList->currentRow();
	readCurrentLevel();
}

void Settings::showLevelListText()
{
	auto selected = levelList->selectedItems().at(0);
	selected->setText(styleHeadingList->currentText() + " / " + styleTocList->currentText());
}

Level Settings::getLevelWidgetData()
{
	Level level{};
	level.level = levelList->currentRow();
	level.headingStyle = styleHeadingList->currentText().toStdString();
	level.tocStyle = styleTocList->currentText().toStdString();
	level.pageNumberPosition = static_cast<PageNumberPosition>(pageNumberPositionList->currentIndex());
	if (pageNumberStyleList->currentIndex() > 0)
		level.pageNumberCharStyle = pageNumberStyleList->currentText().toUtf8().constData();
	if (!separatorInput->text().isEmpty())
		level.separator = separatorInput->text().toStdString();
	if (separatorStyleList->currentIndex() > 0)
		level.separatorCharStyle = separatorStyleList->currentText().toUtf8().constData();
	level.regexFilter = regexFilterInput->text().toStdString();
	// TODO: should probably a spinbox
	level.tocItemLength = tocItemLengthInput->text().toInt();
	return level;
}

void Settings::setQComboBoxByValue(QComboBox* list, const std::string &value, const std::string &defaultValue)
{
	int i = list->findText(QString::fromStdString(value));
	if (i < 0)
		i = list->findText(QString::fromStdString(defaultValue));
	list->setCurrentIndex(i);
}

void Settings::readCurrentLevel()
{
	const auto &level = tocs.at(currentTocIndex).levels.at(currentLevelIndex);
	setLevelWidgetData(level);
}

void Settings::setLevelWidgetData(const Level &level)
{
	const auto defaultParagraphStyle = API::Document::getActive()->getDefaultParagraphStyleName();
	const auto defaultCharStyle = API::Document::getActive()->getDefaultCharacterStyleName();

	styleHeadingList->blockSignals(true);
	setQComboBoxByValue(styleHeadingList, level.headingStyle, defaultParagraphStyle);
	styleHeadingList->blockSignals(false);
	styleTocList->blockSignals(true);
	setQComboBoxByValue(styleTocList, level.tocStyle, defaultParagraphStyle);
	styleTocList->blockSignals(false);
	pageNumberPositionList->setCurrentIndex(static_cast<int>(level.pageNumberPosition));
	setQComboBoxByValue(pageNumberStyleList, level.pageNumberCharStyle, emptyItem);

	separatorInput->setText(QString::fromStdString(level.separator));
	setQComboBoxByValue(separatorStyleList, level.separatorCharStyle, emptyItem);

	regexFilterInput->setText(QString::fromStdString(level.regexFilter));
	// TODO: should probably a spinbox
	tocItemLengthInput->setText(QString::number(level.tocItemLength));
}

void Settings::storeCurrentLevel()
{
	tocs.at(currentTocIndex).levels.at(currentLevelIndex) = getLevelWidgetData();
}

void Settings::setLevelButtonsStatus()
{
	{
		bool enabled = levelList->count() > 0;
		levelGroup->setEnabled(enabled);
		addLevelButton->setEnabled(enabled);
	}
	{
		bool enabled = levelList->count() > 1;
		deleteLevelButton->setEnabled(enabled);
		levelDownButton->setEnabled(enabled);
		levelUpButton->setEnabled(enabled);
	}
}

Toc Settings::getDefaultToc()
{
	Toc toc;
	toc.name = tr("Default Table of Contents").toStdString();
	toc.levels.push_back(getDefaultLevel());
	return toc;
}

Level Settings::getDefaultLevel()
{
	Level level{};
	const auto name = API::Document::getActive()->getDefaultParagraphStyleName();
	level.headingStyle = name;
	level.tocStyle = name;
	return level;
}

void Settings::addLevel()
{
	storeCurrentLevel();
	auto level = getDefaultLevel();
	tocs.at(currentTocIndex).levels.push_back(level);
	addLevel(level);

	levelList->blockSignals(true);
	levelList->setCurrentRow(levelList->count() - 1);
	levelList->blockSignals(false);

	currentLevelIndex = levelList->currentRow();
	setLevelWidgetData(level);

	setLevelButtonsStatus();
}

/// @brief add the level to the QList
void Settings::addLevel(const Level &level)
{
	levelList->blockSignals(true);
	levelList->addItem(QString::fromStdString(level.headingStyle + " / " + level.tocStyle));
	levelList->blockSignals(false);
}

void Settings::deleteLevel()
{
	if (levelList->count() <= 1)
		return;

	auto &levels = tocs.at(currentTocIndex).levels;
	levels.erase(levels.begin() + currentLevelIndex);

	levelList->blockSignals(true);
	levelList->takeItem(currentLevelIndex);
	levelList->blockSignals(false);
	currentLevelIndex = levelList->currentRow();
	readCurrentLevel();
	showLevelListText();

	setLevelButtonsStatus();
}

void Settings::levelDown()
{
	if (currentLevelIndex == levelList->count() - 1)
		return;

	auto it = tocs.at(currentTocIndex).levels.begin() + currentLevelIndex;
	std::iter_swap(it, it + 1);

	levelList->blockSignals(true);
	const auto item = levelList->takeItem(currentLevelIndex);
	++currentLevelIndex;
	if (currentLevelIndex == levelList->count())
		levelList->addItem(item);
	else
		levelList->insertItem(currentLevelIndex, item);
	levelList->blockSignals(false);
	levelList->setCurrentRow(currentLevelIndex);
}

void Settings::levelUp()
{
	if (currentLevelIndex == 0)
		return;

	auto it = tocs.at(currentTocIndex).levels.begin() + currentLevelIndex;
	std::iter_swap(it, it - 1);

	levelList->blockSignals(true);
	const auto item = levelList->takeItem(currentLevelIndex - 1);
	levelList->insertItem(currentLevelIndex, item);
	levelList->blockSignals(false);
	--currentLevelIndex;
	levelList->setCurrentRow(currentLevelIndex);
}

void Settings::addParagraphStyle()
{
	bool okPressed  = false;
	auto name = QInputDialog::getText(this,
		tr("New Paragraph Style"), tr("Paragraph Style Name"),
		QLineEdit::Normal, "", &okPressed);
	if (okPressed && !name.isEmpty())
	{
		auto document = API::Document::getActive();
		document->createParagraphStyle(name.toStdString());
	}
}

void Settings::addCharacterStyle()
{
	bool okPressed  = false;
	auto name = QInputDialog::getText(this,
		tr("New Paragraph Style"), tr("Paragraph Style Name"),
		QLineEdit::Normal, "", &okPressed);
	if (okPressed && !name.isEmpty())
	{
		auto document = API::Document::getActive();
		document->createCharacterStyle(name.toStdString());
	}
}

void Settings::apply()
{
/*
	PrefsContext* prefs = PrefsManager::instance().prefsFile->getPluginContext("tableOfContents");

	if (prefs)
	{
		// TODO: we should rather be writing it into the document (settings)
		// prefs->set("syntaxerror", errorColor.name());
		auto tocNames = prefs->getTable("tableOfContents");
		auto levelNames = prefs->getTable("levelNames");
		// levels->set()
	}
*/

}


}
} // namespaces
