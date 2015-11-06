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

	//Retrieve Configuration from JSON file
	GetModuleFileNameA((HINSTANCE)&__ImageBase, _DLLFilePath, sizeof(_DLLFilePath));
	_DLLFullPath = _DLLFilePath;
	_DLLFullPath.resize(_DLLFullPath.size() - strlen("preferred-runway.dll"));

	_JSONConfigurationFilePath = _DLLFullPath + "\\runways.json";

	stringstream ss;
	ifstream ifs;
	ifs.open(_JSONConfigurationFilePath, ios::binary);
	ss << ifs.rdbuf();
	ifs.close();

	if (_JSONConfiguration.Parse<0>(ss.str().c_str()).HasParseError())
		throw invalid_argument("Error parsing JSON Configuration file.");
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
	vector<runway>::iterator iterator = _RunwayData[ICAO].begin();
	runway tmpRunway;
	runway foundRunway = *iterator;
	int foundRunwayDiff = abs(tmpRunway.Heading - _WindData[ICAO].direction);

	for (iterator++; iterator != _RunwayData[ICAO].end(); iterator++) {
		tmpRunway = *iterator;
		int headingDifference = abs(tmpRunway.Heading - _WindData[ICAO].direction);
		if (headingDifference < foundRunwayDiff) {
			foundRunway = *iterator;
			foundRunwayDiff = headingDifference;	
		}
	}

	return foundRunway.Ident;
}

void PreferredRunwayPlugIn::OnNewMetarReceived(const char * sStation, const char * sFullMetar) {
	string ICAO = sStation;
	wind decodedWindData;
	string encodedMETAR = sFullMetar;
	 
	istringstream iss(encodedMETAR);
	
	vector<string> vec;
	copy(istream_iterator<string>(iss),
		istream_iterator<string>(),
		back_inserter(vec));
	vector<string>::iterator it = vec.begin();
	it++;
	it++;
	
	string encodedWind = *it;
	if (encodedWind == "AUTO") it++;
	encodedWind = *it;
	
	if (encodedWind.find('/') != string::npos) {
		if (_WindData.find(ICAO) != _WindData.end())
			_WindData.erase(ICAO);
		return;
	}

	string encodedWindDirect = encodedWind.substr(0,3);
	if (encodedWindDirect == "VRB")
		decodedWindData.direction = -1;
	else
		decodedWindData.direction = stoi(encodedWindDirect);

	string encodedWindSpeed;
	if (encodedWind.find('G') != string::npos)
		encodedWindSpeed = encodedWind.substr(6, 2);
	else
		encodedWindSpeed = encodedWind.substr(3, 2);

	decodedWindData.speed = stoi(encodedWindSpeed);

	_WindData[ICAO] = decodedWindData;
}