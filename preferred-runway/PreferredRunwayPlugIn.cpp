#include "stdafx.h"
#include "PreferredRunwayPlugIn.h"

#define MY_PLUGIN_NAME      "Preferred Runway Plugin"
#define MY_PLUGIN_VERSION   "0.1"
#define MY_PLUGIN_DEVELOPER "Theodore Messinezis"
#define MY_PLUGIN_COPYRIGHT "MIT License (c) 2015"

PreferredRunwayPlugIn::PreferredRunwayPlugIn(void) : CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE,
	MY_PLUGIN_NAME,
	MY_PLUGIN_VERSION,
	MY_PLUGIN_DEVELOPER,
	MY_PLUGIN_COPYRIGHT)
{

}


PreferredRunwayPlugIn::~PreferredRunwayPlugIn(){

}

void PreferredRunwayPlugIn::OnAsrContentLoaded(bool Loaded) {

}

bool PreferredRunwayPlugIn::OnCompileCommand(const char * sCommandLine) {

	// Only accept .rwy commands
	if (strncmp(sCommandLine, ".rwy", 4))
		return false;

	string strICAO(sCommandLine + 5, sCommandLine + 9); ///< Strip ICAO code from sCommandLine

	DisplayUserMessage("Message", "Preferred Runway", getActiveRunway(strICAO).c_str(), true, true, false, true, false);

	return true;
}

string PreferredRunwayPlugIn::getActiveRunway(string ICAO) {
	return ICAO;
}