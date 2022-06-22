SET PATH=C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Program Files (x86)\VMware\VMware Workstation\bin\;C:\Program Files (x86)\Common Files\Intel\Shared Libraries\redist\intel64\compiler;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;D:\Win10_Software\CLion_stm32\mingw64\bin;D:\Win10_Software\CLion_stm32\gcc-arm-none-eabi-10\10 2020-q4-major\bin;D:\Win10_Software\CLion_stm32\jre-8u291\bin;D:\Win10_Software\CLion_stm32\OpenOCD-20210407-0.10.0\bin;D:\Win10_Software\CLion_stm32\gcc-arm-none-eabi-10\10 2020-q4-major\bin;C:\Users\zcq\AppData\Local\Microsoft\WindowsApps;D:\Win10_Software\CLion_2020\CLion 2020.2.5\bin;
SET ARMCC5_ASMOPT=--diag_suppress=9931
SET ARMCC5_CCOPT=--diag_suppress=9931
SET ARMCC5_LINKOPT=--diag_suppress=9931
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=ADC-DEMO
SET CPU_CLOCK=0x007A1200
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmAsm" --Via ".\startup_stm32f10x_hd._ia"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\main.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_it.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\usart1.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\led.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\timer2.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\timer3.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_gpio.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_rcc.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_usart.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_adc.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_dma.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_flash.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_rtc.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\misc.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_tim.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\stm32f10x_exti.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\core_cm3.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmCC" --Via ".\system_stm32f10x.__i"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\ArmLink" --Via ".\STM32-DEMO.lnp"
"C:\Keil-MDK-C51\MDK\ARM\ARMCC\Bin\fromelf.exe" ".\STM32-DEMO.axf" --i32combined --output ".\STM32-DEMO.hex"
