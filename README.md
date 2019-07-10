# zEmu

ZX Spectrum Emulator by:

- restorer (ex Sinus) - emulator idea and most of code
- mastermind (mkoloberdin) - a lot of work to speedup emulation, migration to CMake, additional work on emulator
- boo_boo - initial cpu core (z80ex, not used anymore), extracting WD1793 and YM/AY emulation from Unreal by SMT, kempston joystick support
- breeze - graphics and icons, work on compilation under FreeBSD
- Doctor Max - mouse scroll support, initial work for UTF-8 support
- and others

Special thanks to SMT for open sourcing the Unreal spectrum emulator.

# Compilation under macOS and Linux

At first install dependencies: `cmake`, `ruby` and `sdl2`. Obviously, you should have a C++ compiler - `gcc` for Linux (probably other compilers should work also, but not tested), `XCode` for macOS (do not forget to do `xcode-select --install`). Than do:

```
cd <into emulator directory>
./build.sh
```

Binary will be in the `build` folder.

# Compilation under Windows

Sorry, that was too long ago. All I remember is that you should use MinGW.

# Emulator idea (in russian)

У каждого продукта в целом и у эмуляторов в частности обязательно есть какая-нибудь идея.

Эмулировать, например, побольше различного оборудования. Или сделать наиболее user-friendly интерфейс.
Или сделать рендерер, который рендерит максимально похоже на оригинальный спектрум.
У каждого подхода находится своя целевая аудитория (именно по-этому считаю абсолютно бессмысленными споры
на тему "эмулятор A круче эмулятора B потому что а эмуляторе A рендерер со сглаживанием, а в B - квадратиками").

Цель ZEmu - проэмулировать Pentagon.
Это всё потому, что в детстве у меня был компьютер Байт, а я очень мечтал о пентагоне.
Теперь я хоть и в эмуляторе, но реализовываю свою мечту.

А почему я так хотел пентагон? Да чтобы смотреть демки. Именно по-этому, все мало относящиеся к работе демок (сверхточная
эмуляция ВГ или поддержка разных форматов ленты) идёт лесом.

Спасибо за внимание.

# Product support

The product has already been completed, but some refactoring is doing right now (as of 2019), and who knows...

| Feature | Support status |
|---|---|
| New features | No |
| Non-critical bugfixes | Maybe |
| Critical bugfixes | Yes, but it depends... |
| Pull requests | Accepted (after review) |
| Issues | Monitored, but if you want to change something - submit a pull request |
| Suport of existing platforms (Windows, Linux, macOS) | At the moment emulator is tested only on macOS, however it should work on other platforms (I hope) |
| Support of new platforms | At the moment only via pull requests, but who knows... |
| Estimated end-of-life | Up to 2019 |
