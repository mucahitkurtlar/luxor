# luxor

LUKS2 brute force cracker.

## Table of Contents

- [Installation](#installation)
  - [Build from source](#build-from-source)
  - [Arch Linux](#arch-linux)
- [Usage](#usage)
- [License](#license)

## Installation

### Build from source

You can build the project from source.

#### Dependencies

- libcryptsetup
- CMake
- make
- gcc

#### Build

```bash
git clone https://github.com/mucahitkurtlar/luxor.git
cd luxor
cmake -B build -S .
cmake --build build
```

### Arch Linux

Alternatively, you can install the package using makepkg.

```bash
git clone https://github.com/mucahitkurtlar/luxor.git
cd luxor/packaging/arch
makepkg -si
```

## Usage

```bash
luxor <device> <wordlist>
```

Example:

```bash
luxor /dev/sda1 /usr/share/wordlists/rockyou.txt
```

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.
