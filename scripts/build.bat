@echo off

rem Copyright (C) 2015 Mathieu Champlon
rem
rem Distributed under the Boost Software License, Version 1.0.
rem (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

setlocal

rem Need to set BOOST_ROOT and run from the source root directory

set PROJECT_DIR=%cd%

cd %BOOST_ROOT%
b2.exe %PROJECT_DIR%\test -q %BUILD_ARGS% %*
if errorlevel 1 exit /b %ERRORLEVEL%

cd %PROJECT_DIR%
set BOOSTBOOK_DIR=%PROJECT_DIR%\bin\turtle\boostbook
xcopy /Y /S /Q /I %BOOST_ROOT%\tools\boostbook\xsl %BOOSTBOOK_DIR%\xsl
xcopy /Y /S /Q /I %BOOST_ROOT%\tools\boostbook\dtd %BOOSTBOOK_DIR%\dtd
xcopy /Y /S /Q /I doc\boostbook %BOOSTBOOK_DIR%
xcopy /Y /S /Q /I %BOOST_ROOT%\doc\src\boostbook.css doc\html
xcopy /Y /S /Q /I %BOOST_ROOT%\doc\src\images\*.png doc\html\images
xcopy /Y /S /Q /I %BOOST_ROOT%\doc\src\images\callouts\*.png doc\html\images\callouts
if errorlevel 1 exit /b %ERRORLEVEL%

cd %BOOST_ROOT%
b2.exe %PROJECT_DIR%\doc -q %*
if errorlevel 1 exit /b %ERRORLEVEL%
