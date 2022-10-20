# pragmatic

## 開發容器
### TL;DR
1. 安裝 Docker, Make（Windows：安裝WSL, Docker Desktop, Ubuntu即可）
2. 打開本專案
3. 執行 `make` 或 `make keymap=${keymapName}`（Windows：`wsl -e make` or `wsl.exe -e make` or `wsl -e make keymap=${keymapName}` or `wsl.exe -e make keymap=${keymapName}`)
4. 第一次會跑比較久，完成後，可以在 `targets` 資料夾找到 hex 檔

![pragmatic](imgur.com image replace me!)

*A short description of the keyboard/project*

* Keyboard Maintainer: [James Sa](https://github.com/yourusername)
* Hardware Supported: *The PCBs, controllers supported*
* Hardware Availability: *Links to where you can find this hardware*

Make example for this keyboard (after setting up your build environment):

    make pragmatic:default

Flashing example for this keyboard:

    make pragmatic:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: Briefly press the button on the back of the PCB - some may have pads you must short instead
* **Keycode in layout**: Press the key mapped to `RESET` if it is available
