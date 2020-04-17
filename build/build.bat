@echo off

rem Copyright (C) 2015 Mathieu Champlon
rem
rem Distributed under the Boost Software License, Version 1.0.
rem (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

setlocal

rem error if BOOST_ROOT not set
set BOOST=%BOOST_ROOT%

pushd ..\test
%BOOST%\b2.exe -q %BUILD_ARGS% %*
popd
if errorlevel 1 exit /b %ERRORLEVEL%

set BOOSTBOOK_DIR=..\bin\turtle\boostbook
xcopy /Y /S /Q /I %BOOST%\tools\boostbook\xsl %BOOSTBOOK_DIR%\xsl
xcopy /Y /S /Q /I %BOOST%\tools\boostbook\dtd %BOOSTBOOK_DIR%\dtd
xcopy /Y /S /Q /I boostbook %BOOSTBOOK_DIR%
xcopy /Y /S /Q /I %BOOST%\doc\src\boostbook.css ..\doc\html
xcopy /Y /S /Q /I %BOOST%\doc\src\images\*.png ..\doc\html\images
xcopy /Y /S /Q /I %BOOST%\doc\src\images\callouts\*.png ..\doc\html\images\callouts
if errorlevel 1 exit /b %ERRORLEVEL%
pushd ..\doc
%BOOST%\b2.exe -q %*
popd
if errorlevel 1 exit /b %ERRORLEVEL%
