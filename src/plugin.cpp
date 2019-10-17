#include "plugin.h"

#include "tableofcontents.h"

#include <QPixmap>
#include <QWidget>

#include "scribuscore.h"
#include "scribusdoc.h"
#include "iconmanager.h"
#include "ui/prefs_pane.h"

#include "api/document.h"

#include "ui/settings.h"

int tableofcontentsplugin_getPluginAPIVersion()
{
	return PLUGIN_API_VERSION;
}

ScPlugin* tableofcontentsplugin_getPlugin()
{
	auto plug = new ScribusPlugin::TableOfContents::Plugin();
	Q_CHECK_PTR(plug);
	return plug;
}

void tableofcontentsplugin_freePlugin(ScPlugin* plugin)
{
	auto plug = dynamic_cast<ScribusPlugin::TableOfContents::Plugin*>(plugin);
	Q_ASSERT(plug);
	delete plug;
}

namespace ScribusPlugin {
namespace TableOfContents {

Plugin::Plugin() : ScActionPlugin()
{
	// Set action info in languageChange, so we only have to do
	// it in one place.
	languageChange();
}

Plugin::~Plugin()
{
	// unregisterAll();
};

void Plugin::languageChange()
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

const QString Plugin::fullTrName() const
{
	return QObject::tr("Table of Contents");
}

const ScActionPlugin::AboutData* Plugin::getAboutData() const
{
	AboutData* about = new AboutData;
	about->authors = "Ale Rimoldi <a.l.e@ideale.ch>";
	about->shortDescription = tr("Create a Table of Contents");
	about->description = tr("Table of Contents");
	about->license = "GPL";
	Q_CHECK_PTR(about);
	return about;
}

void Plugin::deleteAboutData(const AboutData* about) const
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
bool Plugin::run(ScribusDoc* doc, const QString& target)
{

	Q_ASSERT(target.isNull());
	auto document = ::API::Document::getActive();

	if (document == nullptr)
		return false;

	ScribusPlugin::TableOfContents::TableOfContents toc{document};
	// return toc.doAppend();

	/*
	TableOfContents *tableofcontents = new TableOfContents();

	tableofcontents->setScribusDocument(currDoc);
	// tableofcontents->setOptions(options);
	bool success = tableofcontents->doAppend();
	delete tableofcontents;
	return success;
	 */
	return true;
}

bool Plugin::newPrefsPanelWidget(QWidget* parent, Prefs_Pane*& panel)
{
    auto document = ::API::Document::getActive();
	if (document == nullptr)
		return false;

	panel= new Settings(parent);
	Q_CHECK_PTR(panel);
	// connect(panel, SIGNAL(prefsChanged()), scripterCore, SLOT(updateSyntaxHighlighter()));
	return true;
}

} // namespaces
}
