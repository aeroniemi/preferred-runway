#include "stdafx.h"
#include "PreferredRunwayPlugIn.h"

#define MY_PLUGIN_NAME      "Preferred Runway Plugin"
#define MY_PLUGIN_VERSION   "0.1.2"
#define MY_PLUGIN_DEVELOPER "Theodore Messinezis"
#define MY_PLUGIN_COPYRIGHT "MIT License (c) 2015"

PreferredRunwayPlugIn::PreferredRunwayPlugIn(void) : CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE,
	MY_PLUGIN_NAME,
	MY_PLUGIN_VERSION,
	MY_PLUGIN_DEVELOPER,
	MY_PLUGIN_COPYRIGHT){

	//Iterate through all runways from sector file and add them to the _RunwayData map
	EuroScopePlugIn::CSectorElement RunwayIterator;
	for (RunwayIterator = SectorFileElementSelectFirst(EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY); RunwayIterator.IsValid(); RunwayIterator = SectorFileElementSelectNext(RunwayIterator, EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY)) {
		string AirportICAOCode = RunwayIterator.GetAirportName();
		AirportICAOCode = AirportICAOCode.substr(0, 4);

		runway Runway;
		Runway.Heading = RunwayIterator.GetRunwayHeading(0);
		Runway.Ident = RunwayIterator.GetRunwayName(0);
		
		_RunwayData[AirportICAOCode].push_back(Runway);
	}
}


PreferredRunwayPlugIn::~PreferredRunwayPlugIn(){
}

bool PreferredRunwayPlugIn::OnCompileCommand(const char * sCommandLine) {

	// Only accept .rwy commands
	if (strncmp(sCommandLine, ".rwy", 4))
		return false;

	string strICAO(sCommandLine + 5, sCommandLine + 9); ///< Strip ICAO code from sCommandLine
	
	string header = "Preferred Runway for " + strICAO;

	if (_RunwayData[strICAO].empty()) {
		DisplayUserMessage("Preferred Runway", header.c_str(), "Undefined ICAO code.", true, true, false, true, false);
		return true;
	}

	if (_WindData.find(strICAO) == _WindData.end()) {
		string message = "No METAR data found for " + strICAO + " ICAO code. Please load METAR and try again...";
		DisplayUserMessage("Preferred Runway", header.c_str(), message.c_str(), true, true, false, true, false);
		return true;
	}

	DisplayUserMessage("Preferred Runway", header.c_str(), GetActiveRunway(strICAO).c_str(), true, true, false, true, false);

	return true;
}

string PreferredRunwayPlugIn::GetActiveRunway(string ICAO){
	string CompiledWind = to_string(_WindData[ICAO].direction) + " degrees, " + to_string(_WindData[ICAO].speed) + " knots.";
	return CompiledWind;
}

void PreferredRunwayPlugIn::OnNewMetarReceived(const char * sStation, const char * sFullMetar) {
	string ICAO = sStation;
	wind decodedWindData;

	_WindData[ICAO] = decodedWindData;
}