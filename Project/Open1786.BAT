SET PATH=C:\Keil\ARM\ARMCC\bin\;D:\Profils\ProgramData\Oracle\Java\javapath;C:\Program Files (x86)\Intel\iCLS Client\;C:\Program Files\Intel\iCLS Client\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files\TortoiseSVN\bin;C:\Program Files\MiKTeX 2.9\miktex\bin\x64\;C:\Program Files (x86)\GTK2-Runtime\bin;C:\Program Files (x86)\PuTTY\;C:\Program Files (x86)\Intel\OpenCL SDK\2.0\bin\x86;C:\Program Files (x86)\Intel\OpenCL SDK\2.0\bin\x64;C:\Program Files (x86)\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files\Microsoft SQL Server\100\DTS\Binn\;C:\Program Files\Microsoft Windows Performance Toolkit\;C:\Program Files\MATLAB\R2012b\bin;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin;C:\Program Files (x86)\IVI Foundation\VISA\WinNT\Bin\;C:\Program Files\IVI Foundation\VISA\Win64\Bin\;C:\Program Files (x86)\IVI Foundation\IVI\bin;C:\Program Files\IVI Foundation\IVI\bin;C:\Program Files (x86)\Common Files\Pleora;C:\Program Files\Common Files\Pleora
SET CPU_TYPE=LPC1768
SET CPU_VENDOR=NXP (founded by Philips)
SET UV2_TARGET=Open1786
SET CPU_CLOCK=0x00B71B00
"C:\Keil\ARM\ARMCC\bin\ArmAsm" --Via ".\output\startup_lpc17xx._ia"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\system_lpc17xx.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\core_cm3.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_clkpwr.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_gpio.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_pinsel.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_timer.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_exti.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_nvic.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_ssp.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lpc17xx_i2c.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\main.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\ili_lcd_general.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\lcd_api.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\english_16x8.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\affichagelcd.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\touch_panel.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\fonctionsecran.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\fonctionsmemoire.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\fonctionsson.__i"
"C:\Keil\ARM\ARMCC\bin\ArmCC" --Via ".\output\pinconf.__i"
"C:\Keil\ARM\ARMCC\bin\ArmLink" --Via ".\Output\Project.lnp"
C:\Keil\ARM\ARMCC\bin\fromelf.exe ".\Output\Project.axf" --i32combined --output ".\Output\Project.hex"
