#include "tableofcontents.h"

#include "scribusdoc.h"

#include "plugins/scribusAPI/document.h"
#include "plugins/scribusAPI/textFrame.h"
#include "plugins/scribusAPI/textFormatting.h"
/*
#include "plugins/scribusAPI/scribusAPI.h"
#include "plugins/scribusAPI/scribusAPIDocument.h"
#include "plugins/scribusAPI/scribusAPIDocumentItem.h"
#include "plugins/scribusAPI/scribusAPIDocumentItemText.h"
#include "plugins/scribusAPI/scribusAPIDocumentItemTextFormatting.h"
*/

namespace ScribusPlugin {
namespace TableOfContents {


/*
TableOfContents::TableOfContents()
{
    // progressDialog = 0;
    // itemNumber = 0;

    // qDebug() << "marksList" << this->doc->get()->marksList();
    // qDebug() << "notesList" << this->doc->get()->notesList();
}
*/

/*
TableOfContents::~TableOfContents()
{
}
*/

/**
 * @brief append the table of contents to the seleted text frame.

 * If the current selection is a text frame parse all text frames in the document and
 * find the pages where h1 .. h3 are.
 */
bool TableOfContents::doAppend()
{
	auto documentItem = document.getActiveItem();

    // documentItem is optional
    if (!documentItem.has_value()) {
        return false;
    }

    if (!documentItem->isTextFrame()) {
        return false;
    }

    auto tocFrame = documentItem->getTextFrame();

    std::vector<Item> toc{};

    for (int page = 0; page < document.getPageCount(); page++) {
		auto pageNumber = document.getPageNumber(page);
		for (auto& item: document.getPageItems(page)) {
			if (!item.isTextFrame()) {
				continue;
            }
			// TODO: skip items that are not on the page
			// TODO: skip items that are not printable (layer too!)
			ScribusAPI::TextFormatting formatting{item};
			for (auto run: item.getTextFrame().getRuns())
			{
            }
        }
    }

    /*
    ScribusAPIDocument* scribusDocument = new ScribusAPIDocument(this->scribusDoc);

	ScribusAPIDocumentItem* tocFrame = scribusDocument->getCurrentItem();

	if (!tocFrame || !tocFrame->isTextFrame())
		return false;

	QVector<TableOfContentsItem> toc;
	
    // scribusDocument->setPageRange(this->options.pageRange);
    scribusDocument->readItems();

	qDebug() << "getPageCount" << scribusDocument->getPageCount();
	
	for (int page = 0; page < scribusDocument->getPageCount(); page++)
	{
		QString pageNumber = scribusDocument->getPageNumberByIndex(page);
		qDebug() << "pageNumber" << pageNumber;

		foreach (ScribusAPIDocumentItem* item, scribusDocument->getPageItems(page))
		{
			if (!item->isTextFrame())
				continue;
			// TODO: skip items that are not on the page
			// TODO: skip items that are not printable (layer too!)

			ScribusAPIDocumentItemTextFormatting formatting(item);

			foreach (ScribusAPIDocumentItemTextRuns run, item->getText()->getRuns())
			{
				qDebug() << "run" << run;
				if (!run.isParagraph())
					continue;

				formatting.readAtPosition(run.position);
				QString paragraphStyleName = formatting.getParagraphStyleName();
				qDebug() << "paragraphStyleName" << paragraphStyleName;

				int level = options.headingStyles.indexOf(paragraphStyleName);
				if (level < 0)
					continue;

				QString tocStyleName = options.tocStyles.at(level);
				qDebug() << "tocStyleName" << tocStyleName;

				TableOfContentsItem tocItem;

				foreach (QVector<QString> content, run.content)
				{
					QVectorIterator<QString> list(content);
					while (list.hasNext()) {
						tocItem.title.append(list.next());
						if (list.hasNext())
							tocItem.title.append(SpecialChars::PARSEP);
					}
				}

				tocItem.level = level;
				tocItem.tocStyleName = tocStyleName;
				tocItem.pageNumber = pageNumber;

				toc << tocItem;

				// TODO: here is where i left!
				// it correctly detects the paragraph style name
				// now, it's time to check in the source vector and get the level
			}
		}

	}
	qDebug() << "toc" << toc;

    // ScribusAPIDocumentItem* frame = scribusDocument->getCurrentItem();
	// if (frame &&  frame->isTextFrame())
    // {
    //     if (style.type == "paragraph")
    //         frame->getText()->applyParagraphStyle(style.name);
    //     else if (style.type == "character")
    //         frame->getText()->applyCharacterStyle(style.name);
    // }




	ScribusAPIDocumentItemText* tocFrameText = tocFrame->getText();
	foreach (TableOfContentsItem item, toc)
	{
		// @todo: make it CTL compatible
		// @todo: add the special chars to the API
		QString tocLine = item.title + SpecialChars::TAB + item.pageNumber + " ("+item.tocStyleName+")";
		tocFrameText->appendParagraph(tocLine , item.tocStyleName);

		// ParagraphStyle pstyle;
		// pstyle.setParent(levelSetup.textStyle);
		// tocFrame->itemText.applyStyle(pos, pstyle);
	}
	tocFrame->render();
	scribusDocument->render();
	// m_doc->regionsChanged()->render(QRect());
    */

	return true;

}

/*
QDebug operator<<(QDebug dbg, const TableOfContentsOptions options)
{
    // dbg.nospace() << "(targetFilename:" << options.targetFilename;
    QStringList output;
    foreach (QString style , options.headingStyles) {
        output << style;
    }
    dbg.nospace() << ", headingStyles: (" << output.join(", ") << ")";
    foreach (QString style , options.tocStyles) {
        output << style;
    }
    dbg.nospace() << ", tocStyles: (" << output.join(", ") << ")";
    return dbg.space();
}

QDebug operator<<(QDebug dbg, const TableOfContentsItem item)
{
    dbg.nospace() << "title:" << item.title;
    dbg.nospace() << ", level:" << item.level;
    dbg.nospace() << ", tocStyleName:" << item.tocStyleName;
    dbg.nospace() << ", pageNumber:" << item.pageNumber;
    return dbg.space();
}
*/

} // ScribusPlugin::TableOfContents
}
