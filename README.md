# Preferred Runway Plugin [![Latest Unstable Version](https://img.shields.io/badge/unstable-0.1.x--dev-orange.svg?style=flat-square)](https://github.com/theomessin/preferred-runway) [![License](https://img.shields.io/badge/license-MIT-4A8F80.svg?style=flat-square)](https://opensource.org/licenses/MIT)
This plugin for Euroscope will quickly determine the active runway of an airport. It is especially useful when controlling an entire FIR by yourself. 

## Using the Plugin (In a nutshell)
Run the following dot command in the chat input box of Euroscope:
```
.rwy LGAV
```
Of course you can replace LGAV with any valid ICAO airport code. Please note that this airport __must__ be defined in the currently used Sector file.
Depending on the current wind, the plugin will respond with the active runway for that airport, as defined by the preferred runways JSON file.
