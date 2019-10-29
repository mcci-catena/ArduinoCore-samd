# Adding MCCI Boards

In order to ensure consistency in menu entries, `boards.txt` is created by processing the template file `boards.template` using the bright script `generate_boards.bri` and the input file `boards.json`.

For information, please refer to the [STM32 version of MCCI-ADDING-BOARDS.md](https://github.com/mcci-catena/Arduino_Core_STM32/blob/master/MCCI-ADDING-BOARDS.md).

The following command is used to generate `boards.txt`.

```shell
bright ../../stm32/2.6.0/generate_boards.bri boards.template boards.json > boards.txt
```

Change `../../stm32/2.6.0/` to the path to the most current MCCI STM32 BSP on your system.
