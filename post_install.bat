@echo off
set ARGS=/SE /SW /SA
if "%PROCESSOR_ARCHITECTURE%" == "AMD64" (
  drivers\MCCI-CATENA-SAMD-DPINST-x64.exe %ARGS%
) ELSE IF "%PROCESSOR_ARCHITEW6432%" == "AMD64" (
  drivers\MCCI-CATENA-SAMD-DPINST-x64.exe %ARGS%
) ELSE (
  drivers\MCCI-CATENA-SAMD-DPINST-x86.exe %ARGS%
)

@echo off

exit /b 0
