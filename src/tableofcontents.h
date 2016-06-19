/**
 * This is the main table of contents class.
 * It gets called from the plugin class after the options have been set in the dialog
 */
#ifndef TABLEOFCONTENTS_H
#define TABLEOFCONTENTS_H

#include <QObject>
#include <QVector>

class ScribusDoc;

/**
 * @brief The options for the Epub export, as defined in the Export dialog.
 */
struct TableOfContentsOptions
{
    // QString targetTextFrame;
    QVector<QString> headingStyles;
    QVector<QString> tocStyles;
    TableOfContentsOptions()
    {
        // targetTextFrame = "";
		headingStyles << "h1" << "h2" << "h3";
		tocStyles << "toc1" << "toc2" << "toc3";
    }
};

struct TableOfContentsItem
{
	QString title;
	int level;
	QString tocStyleName;
	QString pageNumber;
};


/**
 * @brief This is the main table of contents class. It gets called by `TableOfContentsPlugin` after the the export dialog.
 */
class TableOfContents : public QObject
{
    Q_OBJECT

public:
	TableOfContents();
	~TableOfContents();

	TableOfContentsOptions options;
    void setOptions(TableOfContentsOptions options) { this->options = options; }

    void setScribusDocument(ScribusDoc* scribusDoc) { this->scribusDoc = scribusDoc; }
	bool doAppend();

private:
    ScribusDoc* scribusDoc;

};

QDebug operator<<(QDebug dbg, const TableOfContentsOptions options);
QDebug operator<<(QDebug dbg, const TableOfContentsItem item);

#endif // TABLEOFCONTENTS_H
