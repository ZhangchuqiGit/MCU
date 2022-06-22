SET PATH=C:\KEIL\MDK\ARM\ARMCC\Bin;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Users\ZCQ\AppData\Local\Microsoft\WindowsApps;
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=ADC-DEMO
SET CPU_CLOCK=0x007A1200
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmAsm" --Via ".\startup_stm32f10x_md._ia"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\main.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\led.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\usart1.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\delay.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\dht11.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\sys.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_it.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_gpio.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_rcc.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_usart.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_adc.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_dma.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_flash.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_rtc.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\misc.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_tim.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_exti.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\core_cm3.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\system_stm32f10x.__i"
"C:\KEIL\MDK\ARM\ARMCC\Bin\ArmLink" --Via ".\STM32-DEMO.lnp"
"C:\KEIL\MDK\ARM\ARMCC\Bin\fromelf.exe" ".\STM32-DEMO.axf" --i32combined --output ".\STM32-DEMO.hex"
