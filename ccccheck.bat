rem Replace ccc paths with your ones.
rem Replace mmp file names with your ones.
rem Replace the path -Ic:\symbianfiles\mobo\Repeat\gui\Repeat_Data\REPEAT_WINSCW_UDEB\ObjectCode with
rem the path where your project's rsg file is outputted by the tool chain.
rem Replace output file names with your ones, if you wish so.
rem Place this file above your project's engine and gui directories.
rem Set the epocroot, since ccc uses it, not the devices tool info.
rem Also reset the epocroot in the end.

set epocroot=\Symbian\8.0a\S60_2nd_FP2_CW\
cls
cd engine
c:\ccc\ccc.exe -w -pplayereng.mmp > ..\engine.txt
cd ..\gui
c:\ccc\ccc.exe -w -pplayer.mmp > ..\gui.txt
cd ..
set epocroot=
notepad engine.txt
notepad gui.txt