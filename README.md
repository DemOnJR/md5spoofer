# MD5Spoofer

Plucore plugin, which grants ability to make files hidden from server.

## Commands & CVars

<b>consistency</b> - add consistency to file.

Syntax:
consistency < FileName > < Checksum >

Usage example:<br>
consistency "..\demoplayer.dll" "2130032001"<br>
consistency "..\plucore.dll" "0"<br>
consistency "..\plucore.asi" "0"<br>
consistency "..\msvcr60.dll" "0"<br>
consistency "..\msvcr70.dll" "0"<br>
consistency "..\msvcr71.dll" "0"<br>
consistency "..\msvcr80.dll" "0"<br>
consistency "..\msvcr90.dll" "0"<br>
consistency "..\msvcr100.dll" "0"<br>
consistency "..\msvcr110.dll" "0"<br>
consistency "..\msvcr120.dll" "0"<br>

If parameter "Checksum" is set to "0", server will think that this file doesn't exist. If you need to set custom value, you have to calculate md5 with md5 command.

<b>md5</b> - calculates md5 for consistency command.

Syntax:
<b>md5</b> < FileName >

Usage example:<br>
md5 "demoplayer.dll"<br>
md5 "cstrike\sprites\smokepuff.spr"<br>

consistency_dump < 1/0 > - cvar, which permits to view list of files, which are checked by detectors. Files list is brought to console whilst connecting to server.