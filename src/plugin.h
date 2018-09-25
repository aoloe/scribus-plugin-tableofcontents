 /**
 * This is the entry point for the plugin.
 * All the methods defined here are called by Scribus' plugin execution code.
 */

#ifndef TABLEOFCONTENTSPLUGIN_H
#define TABLEOFCONTENTSPLUGIN_H

#include "pluginapi.h"
#include "scplugin.h"

class QString;
class ScribusDoc;
class ScribusMainWindow;

namespace ScribusPlugin {
namespace TableOfContents {

/**
 * This is the standard entry point for the plugin and is automatically loaded by Scribus.
 * @brief Standard entry point for the plugin; Attaches itself to the menu, shows the "Settings" dialog and calls `TableOfContents::doInsert()`
 */
class PLUGIN_API Plugin : public ScActionPlugin
{
	Q_OBJECT
public:

	// Standard plugin implementation
	Plugin();
	virtual ~Plugin();
	/*!
	\author Ale Rimoldi
	\brief Run the Table of Contents
	\param filename a file to export to
	\retval bool true
	*/
	virtual bool run(ScribusDoc* doc, const QString& filename = QString::null) override;
	virtual const QString fullTrName() const override;
	virtual const AboutData* getAboutData() const override;
	virtual void deleteAboutData(const AboutData* about) const override;
	virtual void languageChange() override;
	virtual void addToMainWindowMenu(ScribusMainWindow *) override {};
};

} // ScribusPlugin::TableOfContents
}

extern "C" PLUGIN_API int tableofcontentsplugin_getPluginAPIVersion();
extern "C" PLUGIN_API ScPlugin* tableofcontentsplugin_getPlugin();
extern "C" PLUGIN_API void tableofcontentsplugin_freePlugin(ScPlugin* plugin);

#endif // TABLEOFCONTENTSPLUGIN_H
