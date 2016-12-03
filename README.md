# RFIDDoorEntrySW
Work in progress.

* esp8266 serves local config page. syncs with online DB (downloads json file).
* esp8266 writes Card details to i2c eeprom it shares with atmega8
* Door system run my an atmega8 talking to an off the shelf card reader (wiegand protocol)
* Atmega checks scanned cards against EEPROM and runs independant of node (more reliable)

EEPROM Format [binformat](binformat)
Schema for user info (make your management app spit this out and the reader will update itself) [sample schema](webui/cards.json)
