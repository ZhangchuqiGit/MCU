SET PATH=C:\Keil_v5\MDK\ARM\ARMCC\Bin;C:\Program Files (x86)\Common Files\Intel\Shared Libraries\redist\intel64\compiler;C:\Program Files (x86)\Intel\iCLS Client\;C:\Program Files\Intel\iCLS Client\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files\Intel\Intel(R) Management Engine Components\IPT;C:\Users\Administrator\AppData\Local\Microsoft\WindowsApps;
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=ADC-DEMO
SET CPU_CLOCK=0x00B71B00
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmAsm" --Via ".\startup_stm32f10x_hd._ia"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\main.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_it.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\usart1.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\led.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\time_test.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\exti.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\ultrasonic.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\timer3.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_gpio.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_rcc.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_usart.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_adc.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_dma.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_flash.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_rtc.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\misc.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_tim.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_exti.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_bkp.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\core_cm3.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\system_stm32f10x.__i"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\ArmLink" --Via ".\STM32-DEMO.lnp"
"C:\Keil_v5\MDK\ARM\ARMCC\Bin\fromelf.exe" ".\STM32-DEMO.axf" --i32combined --output ".\STM32-DEMO.hex"
