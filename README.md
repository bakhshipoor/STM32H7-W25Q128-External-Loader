# STM32H7-W25Q128-External-Loader
## W25Q128 External Loader for STM32H7b0VBT6

## Pin Out:

![Pinout-2](https://github.com/bakhshipoor/STM32H7-W25Q128-External-Loader/assets/2270529/d1cc357a-50b5-40d9-8fa4-2ca9d6f9822a)

## OCTOSPI Configuration:
### 
### 
![0](https://github.com/bakhshipoor/STM32H7-W25Q128-External-Loader/assets/2270529/8050c96c-6ed4-4136-bfa5-3006840a31ff)
### 
![1](https://github.com/bakhshipoor/STM32H7-W25Q128-External-Loader/assets/2270529/802ea2a7-18bd-4b90-9ce0-3fa0a892e061)

## Schematic:
### 
### 
![2](https://github.com/bakhshipoor/STM32H7-W25Q128-External-Loader/assets/2270529/f2cebaa4-2a4b-461a-8db6-e9037db31386)

## Project Properties
### cmd.exe /C copy/Y "${BuildArtifactFileBaseName}.elf" "..\W25Q128_STM32H7B0VBT6.stldr"
###
![3](https://github.com/bakhshipoor/STM32H7-W25Q128-External-Loader/assets/2270529/558aa98c-c969-4c67-9065-412d4892bd8a)
###
### 
### 
### Change Linker Script file. Click Browse and choose "linker.ld"
### Unchek "Discard Unused Sections (-WL,--gc-sections)"
###
![4](https://github.com/bakhshipoor/STM32H7-W25Q128-External-Loader/assets/2270529/3cc9c5a8-c4b7-4ae5-b78f-5ebd7b74bf25)

