SET PATH=C:\KEIL\MDK\ARM\ARMCC\Bin;C:\windows\system32;C:\windows;C:\windows\System32\Wbem;C:\windows\System32\WindowsPowerShell\v1.0\;C:\windows\System32\OpenSSH\;C:\Users\Administrator\AppData\Local\Microsoft\WindowsApps;
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=moban
SET CPU_CLOCK=0x007A1200
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\output\main.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\output\sys.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\output\delay.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\output\usart1.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\output\usart2.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\output\usart3.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\output\spi1.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\output\spi2.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\output\core_cm3.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_it.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\output\system_stm32f10x.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmAsm" --Via ".\output\startup_stm32f10x_hd._ia"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmLink" --Via ".\output\moban.lnp"
