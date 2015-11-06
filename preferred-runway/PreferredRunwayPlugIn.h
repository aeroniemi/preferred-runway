#pragma once
#include <vector>
#include <string>
#include <map>
#include <EuroScopePlugIn.h>
#include "preferred-runway.h"

using namespace std;

struct wind {
	int direction, speed;
	wind(){
		direction = 0;
		speed = 0;
	};
};

struct runway {
	string Ident;
	int Heading;
};


class PreferredRunwayPlugIn :
	public EuroScopePlugIn::CPlugIn
{
public:
	PreferredRunwayPlugIn();
	virtual ~PreferredRunwayPlugIn();

	inline virtual bool OnCompileCommand(const char * sCommandLine);

	inline  virtual void OnNewMetarReceived(const char * sStation, const char * sFullMetar);

	inline virtual void OnAsrContentToBeClosed(void) {
		delete this;
	};

	inline virtual string GetActiveRunway(string ICAO);

private:
	map<string, wind> _WindData;
	map<string, vector<runway> > _RunwayData;
};