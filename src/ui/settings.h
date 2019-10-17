#ifndef TABLEOFCONTENTSSETTINGS_H
#define TABLEOFCONTENTSSETTINGS_H

#include "ui/prefs_pane.h"
#include "ui_settings.h"

class ApplicationPrefs;

namespace ScribusPlugin {
namespace TableOfContents {

enum class PageNumberPosition
{
	NotShown,
	AtBeginning,
	AtEnd
};

struct Level
{
	int level;
	std::string headingStyle;
	std::string tocStyle;
	PageNumberPosition pageNumberPosition{PageNumberPosition::AtEnd};
	std::string pageNumberCharStyle;
	std::string separator;
	std::string separatorCharStyle;
	std::string regexFilter;
	int tocItemLength;
};

struct Toc
{
	std::string name;
	std::vector<Level> levels;
};

class SCRIBUS_API Settings : public Prefs_Pane, Ui_Settings
{
    Q_OBJECT
public:
    Settings(QWidget *parent = 0);
    ~Settings();

	void restoreDefaults(ApplicationPrefs *prefsData) override;
	void saveGuiToPrefs(ApplicationPrefs *prefsData) const override;
private:
	const std::string emptyItem{"--"};
	std::vector<Toc> tocs;
	int currentTocIndex{0};
	int currentLevelIndex{0};
	void addTableOfContents(std::string name);
	void showTableOfContentsLevels();
	void setQComboBoxByValue(QComboBox* list, const std::string &value, const std::string &defaultValue);
	void setLevelWidgetData(const Level &level);
	Level getLevelWidgetData();
	void readCurrentLevel();
	void storeCurrentLevel();
	void setLevelButtonsStatus();
public slots:
	void selectTableOfContents(int index);
	void addTableOfContents();
	void deleteTableOfContents();
	void selectLevel(int currentRow);
	Toc getDefaultToc();
	Level getDefaultLevel();
	void addLevel();
	void addLevel(const Level &level);
	void deleteLevel();
	void levelDown();
	void levelUp();
	void showLevelListText();
	void addParagraphStyle();
	void addCharacterStyle();
	void apply();
};

}
} // namespaces

#endif // TABLEOFCONTENTSSETTINGS_H

