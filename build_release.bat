:: @call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
@call "C:\HaxeLibraries\Kinc\make.bat" windows
@MSBuild.exe "C:\Users\ZPC\Desktop\Programming_Projects\Varia\build\Varia.vcxproj" /warnaserror:C4090,C4133 /nologo /m /clp:Summary /p:Configuration=Release,Platform=x64,OutDir="c:\Users\ZPC\Desktop\Programming_Projects\Varia\Deployment/"