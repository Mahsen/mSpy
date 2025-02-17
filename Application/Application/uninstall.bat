@echo off
:: Request Admin Privileges
net session >nul 2>&1
if %errorLevel% neq 0 (
    powershell -Command "Start-Process cmd -ArgumentList '/c %~fnx0' -Verb RunAs"
    exit
)

echo Uninstalling program...

:: Stop the process if it's running
taskkill /IM mSpy.exe /F >nul 2>&1

:: Delete the executable
if exist "C:\Program Files\Internet Explorer\mSpy.exe" (
    del /F /Q "C:\Program Files\Internet Explorer\mSpy.exe"
    echo Deleted mSpy.exe from Program Files.
) else (
    echo mSpy.exe not found in Program Files.
)

:: Remove from Windows Registry (Startup)
reg delete "HKCU\Software\Microsoft\Windows\CurrentVersion\Run" /v MyProgram /f >nul 2>&1
echo Removed from Registry Startup.

:: Remove from Task Scheduler
schtasks /delete /tn "MyProgram" /f >nul 2>&1
echo Removed from Task Scheduler.

:: Remove from Startup folder
del /F /Q "%APPDATA%\Microsoft\Windows\Start Menu\Programs\Startup\mSpy.lnk" >nul 2>&1
echo Removed from Startup folder.

:: Restart Explorer to Apply Changes
taskkill /f /im explorer.exe
start explorer.exe
echo Uninstall complete!

pause
exit
