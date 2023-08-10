:: @call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

:: @call "C:\HaxeLibraries\Kinc\make.bat" windows --noshaders --debug
@MSBuild.exe "C:\Users\ZPC\Desktop\Programming_Projects\Varia\build\Varia.vcxproj" /warnaserror:C4090,C4133,C4296 /nologo /m /clp:Summary /p:Configuration=Debug,Platform=x64,OutDir="c:\Users\ZPC\Desktop\Programming_Projects\Varia\Deployment/"

:: pushd "c:\Users\ZPC\Desktop\Programming_Projects\Varia\Deployment\"
:: c:\Users\ZPC\Desktop\Programming_Projects\Varia\Deployment\Varia.exe
:: popd