# Changelog

## Version 0.22

* Picture support: can now browse picture, and start them on the server
* Fixed some bugs with playlist
* Reduced the number of queries done to the server
* Detect wi-fi state, and disable connection if not available (this may
  break some setups, so there is an option in the server settings to
  override this)
* Fixed seek no longer worked (and improved it)

## Version 0.20

* Internationalization improvements:
  * Now available in two new languages, Dutch (as well as Belgian) and Norwegian
  * Large improvements in the English version, which should now look on par with
    the French one
* New playlist page with a much cleaner look and a more native behaviour
* New "Random albums" option in Music section
* Fix a cover page bug with SFOS 4
* Lots of small ui fixes

## Version 0.19

* fixes display issues with Sailfish OS 4

## Version 0.18

* improved cover pages when nothing playing / connection failed
* add resume handling of movies / tv show episodes
* display already played items differently in movies and episodes list
* fixed a bug with the player refreshing far too often, causing glitches in
the interface
* added a lot of busy indicators

## Version 0.17

* Added recently added / recently played items in music
* Improved currently playing page

## Version 0.16

* Fixed several usability issues 
  * no longer uses a pulldown menu for volume / playing options, instead
    clicking on the album art goes to the currently playing page
  * shows the volume in the dock
  * reworked the currently playing page
* added a remorse on the stop button

## Version 0.15

* Added wake on lan support to wake up the server
* Changed several icons

## Version 0.14

* Fixed a lot of layout issues
* Added basic library management (cleaning and refreshing for audio/video)
* Added menu to shutdown kodi server

## Version 0.13

* New home page - no longer uses a grid (did not work in landscape mode)
* Added server discovery
* Improved experience on first use

## Version 0.12

* Added search function in lists
* Added an icon provider, to add custom icons to the app. Movies icon has
been replaced by a new one
* fixed issues related to some play buttons/actions not working

## Version 0.11

* new gui for currently playing item, feedback for volume
* integration of repeat one/all and shuffle mode
* improvements on volume management
* fixed wrong indication on touchpad
* add download options for albums, not only using file browsing but
also in artist view or in list view

## Version 0.10

* added german translation (thanks popanz)
* fix #6 (images not displayed if using http auth)
* added icon to identify file type (ref #1)
* may fix #18 (to be checked)
* small cleanup in some files

## Version 0.9

* Fixed an issue which could prevent connection if using only one server
* Fixed TV Shows pages which were broken
* Few internal improvements
* Honor the "ignorearticle" setting of the server
* fix playlist page was sometimes not displaying anything (or slow to display)

## Version 0.8

* added about page
* fixed centering of text input
* fixed albums list not alphabetically sorted
* added busy indicator on some lists

## Version 0.7

* lots of internal changes, fixes some app crashes
* correctly keep last connected server
* added a volume plugin for minidsp products
* download files to music / videos folder, respectively
* reworked the remote control page, which is much more usable now
* small ui/layout improvements

## Version 0.6

* fixes for landscape mode
* fixed some bugs related to genre for music
* reviewed settings page, use of a dialog page
* introduced preliminary support for volume plugins

## Version 0.5

* includes russian translation from dikonov
* added support for volume keys
* fixes for landscape mode

## Version 0.4

*Beware that starting for version 0.4, the name of the application changed.
Uninstall and reinstall of new version is required. Settings are ***not***
lost during the process.*

* mostly technical version, improvements of repository architecture
* moved the name from kontroller to harbour-kontroller (needed for jolla store
inclusion)
* use static build of libqjsonrpc - needed for jolla store inclusion
* added swedish translation, many thanks to eson57 for it

## Version 0.3

* new server settings interface
* removed the multiple zone options from interface (it is not working
currently)
* added the login / password fields in the server settings, implemented
basic auth handling

## Version 0.2

* initial public release
