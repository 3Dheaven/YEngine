GET WXWIDGET (To add in dependencies/ in the future)
http://stackoverflow.com/questions/37995066/how-to-set-up-wxwidgets-3-1-0-with-visual-studio-2015

- Grab the sources : https://github.com/wxWidgets/wxWidgets
- Open \build\msw\wx-vc14.sln (adjust as necessary.)
- Go to "Build->Batch Build...", click "Select All", "Build".
- Go drink some cococola or watch TV eating a pizza.
- After the build finishes, open wxWidgets/samples/minimal/minimal_vc9.sln.
- Let MSVC convert the solution to become an appropriate format.
- Build and run the sample to make sure it works

-----------------------------------------------------------------------------------------------

0. Add an environment variable named WXWIN (with the path of wxwidget on your computer)
1. Make sure to have a directory named "build" next to this README file.
2. Open Cmake 
3. Specify the path of CMakeLists.txt in "Where is the source code"
4. Specify the path of the build directory in "Where to build the binaries"
5. -> Configurate and choose platform / IDE
6. -> Generate
7. In visual studio : 
-> Solution Properties -> General -> Character Set -> Choose "Use Unicode Character Set"
8. In visual studio : 
-> Solution Properties -> Linker -> System -> SubSystem -> Choose "Windows(/SUBSYSTEM:WINDOWS)"