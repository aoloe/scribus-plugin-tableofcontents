/**
 * This is the main table of contents class.
 * It gets called from the plugin class after the options have been set in the dialog
 */
#ifndef TABLEOFCONTENTS_H
#define TABLEOFCONTENTS_H

#include "scplugin.h"
#include "pluginapi.h"

#include <string>
#include <vector>
#include <memory>

#include "api/document.h"

class QString;

namespace ScribusPlugin {
namespace TableOfContents {

/**
 * @brief The options for the Epub export, as defined in the Export dialog.
 */
struct Options
{
    // QString targetTextFrame;
    std::vector<std::string> headingStyles;
    std::vector<std::string> tocStyles;

    Options()
    {
        // targetTextFrame = "";
		// headingStyles << "h1" << "h2" << "h3";
		// tocStyles << "toc1" << "toc2" << "toc3";
    }
};

struct Item
{
	std::string title;
	int level;
	std::string tocStyleName;
	std::string pageNumber;
};


/**
 * @brief This is the main table of contents class. It gets called by `TableOfContentsPlugin` after the the export dialog.
 */
class TableOfContents : public QObject
{
	Q_OBJECT
    public:
        TableOfContents(std::shared_ptr<API::Document> document)
        : document{document}
        {}
        ~TableOfContents() {}

        Options options;
        void setOptions(Options options) { this->options = options; }

        bool doAppend();

    private:
        std::shared_ptr<API::Document> document;

};

} // ScribusPlugin::TableOfContents
}

/*
QDebug operator<<(QDebug dbg, const TableOfContentsOptions options);
QDebug operator<<(QDebug dbg, const TableOfContentsItem item);
*/

#endif // TABLEOFCONTENTS_H
