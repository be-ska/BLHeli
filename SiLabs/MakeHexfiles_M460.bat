@ECHO off
@ECHO ***** Batch file for BlHeli (from 4712)  v.2           *****
@ECHO ***** All Messages will be saved to MakeHex_Result.txt *****
@ECHO ***** Start compile with any key  - CTRL-C to abort    *****
Break ON
@pause
DEL MakeHex_Result.txt /Q

rem ***** Adapt settings to your enviroment ****
DEL Output\Hex\*.* /Q
RMDIR Output\Hex
DEL Output\*.* /Q
RMDIR Output
MKDIR Output
MKDIR Output\Hex
SET Revision=REV14_9
SET SilabsPath=C:\SiLabs
SET RaisonancePath=C:\Raisonance

@ECHO Revision: %Revision% >> MakeHex_Result.txt
@ECHO Path for Silabs IDE: %SilabsPath% >> MakeHex_Result.txt
@ECHO Path for Raisonance IDE: %RaisonancePath% >> MakeHex_Result.txt
@ECHO Start compile ..... >> MakeHex_Result.txt

SET BESCNO=291
SET BESCNAME=ALIGN_M460
call:compile

goto :end


:compile
SET BESC="%BESCNAME%_MULTI"
call:compile2
goto :eof

:compile2
@ECHO compiling %BESC% %BESCNO%
@ECHO. >> MakeHex_Result.txt
@ECHO ********************************************************************  >> MakeHex_Result.txt
@ECHO %BESCNAME% >> MakeHex_Result.txt
@ECHO ********************************************************************  >> MakeHex_Result.txt
%RaisonancePath%\Ride\bin\ma51.exe "BLHeli.asm" SET(BESCNO=%BESCNO%) OBJECT(Output\%BESCNAME%_%Revision%.OBJ) DEBUG EP QUIET PIN(%SilabsPath%\MCU\Inc;%RaisonancePath%\Ride\inc;%RaisonancePath%\Ride\inc\51) >> MakeHex_Result.txt
%RaisonancePath%\Ride\bin\lx51.exe "Output\%BESCNAME%_%Revision%.OBJ"  TO(Output\%BESCNAME%_%Revision%.OMF) RS(256) PL(68) PW(78) OUTPUTSUMMARY LIBPATH(%RaisonancePath%\Ride\lib\51) >> MakeHex_Result.txt
%RaisonancePath%\Ride\bin\oh51.exe "Output\%BESCNAME%_%Revision%.OMF" >> MakeHex_Result.txt 
copy "Output\%BESCNAME%_%Revision%.HEX" "Output\Hex\%BESC%_%Revision%.HEX" > nul
del "Output\%BESCNAME%_%Revision%.HEX" > nul
@ECHO. >> MakeHex_Result.txt
goto :eof

:end

@pause
exit