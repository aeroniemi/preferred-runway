#pragma once
#include <string>
#include <EuroScopePlugIn.h>
#include "preferred-runway.h"

using namespace std;

class PreferredRunwayPlugIn :
	public EuroScopePlugIn::CPlugIn
{
public:
	PreferredRunwayPlugIn();
	virtual ~PreferredRunwayPlugIn();

	inline virtual void OnAsrContentLoaded(bool Loaded);

	inline virtual bool OnCompileCommand(const char * sCommandLine);

	inline virtual void OnAsrContentToBeClosed(void) {
		delete this;
	};

	inline virtual string getActiveRunway(string ICAO);
};