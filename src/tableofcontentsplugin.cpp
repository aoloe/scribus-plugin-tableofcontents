#include "tableofcontents.h"
#include "tableofcontentsplugin.h"

#include "scribuscore.h"
#include "scribusdoc.h"

int tableofcontentsplugin_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* tableofcontentsplugin_getPlugin()
{
	TableOfContentsPlugin* plug = new TableOfContentsPlugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void tableofcontentsplugin_freePlugin(ScPlugin* plugin)
{
	TableOfContentsPlugin* plug = dynamic_cast<TableOfContentsPlugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

TableOfContentsPlugin::TableOfContentsPlugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

TableOfContentsPlugin::~TableOfContentsPlugin()
{
	// unregisterAll();
};

void TableOfContentsPlugin::languageChange()
{
	// Note that we leave the unused members unset. They'll be initialised
	// with their default ctors during construction.
	// Action name
	m_actionInfo.name = "AppendTableOfContents";
	// Action text for menu, including accel
	m_actionInfo.text = tr("Append &Table of Contents");
	// Menu
	m_actionInfo.menu = "Insert";
	m_actionInfo.enabledOnStartup = false;
	m_actionInfo.needsNumObjects = -1;
}

const QString TableOfContentsPlugin::fullTrName() const
{
	return QObject::tr("Table of Contents");
}

const ScActionPlugin::AboutData* TableOfContentsPlugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Ale Rimoldi <a.l.e@ideale.ch>";
	about->shortDescription = tr("Create a Table of Contents");
	about->description = tr("Table of Contents");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void TableOfContentsPlugin::deleteAboutData(const AboutData* about) const
{
	Q_ASSERT(about);
	delete about;
}

/**
 * This method is automatically called by Scribus when the plugin action is activated.
 *
 * If a document is open, show the export dialog and call `TableOfContents::doExport()`.
 *
 * @todo:
 * - do not show the dialog if Scribus runs in "server" mode.
 */
bool TableOfContentsPlugin::run(ScribusDoc* doc, QString target)
{

	Q_ASSERT(target.isNull());
	ScribusDoc* currDoc=doc;
	if (currDoc == 0)
		currDoc = ScCore->primaryMainWindow()->doc;
	if (currDoc == 0)
		return false;

	TableOfContents *tableofcontents = new TableOfContents();

	tableofcontents->setScribusDocument(currDoc);
	// tableofcontents->setOptions(options);
	bool success = tableofcontents->doAppend();
	delete tableofcontents;
	return success;
}
