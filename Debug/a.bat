set #1=&set #2=&cls&@echo off&setlocal enabledelayedexpansion&rem !#2!
set /a #1=1&set /a #2=2&set /a #3=#1+#2&set #4=!#3!&set /a #5=6&set /a #6=1&(for /l %%z in (1,1,!#5!) do set /a #6*=#4)&echo !#6!
set /a #1=1&set /a #2=2&set /a #3=#1+#2&set #4=!#3!&set /a #5=6&set /a #6=1&(for /l %%z in (1,1,!#5!) do set /a #6*=#4)&echo !#6!
pause
