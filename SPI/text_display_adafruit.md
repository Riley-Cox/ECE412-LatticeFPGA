# ST7735R Text Display

## 1. SPI Command Sequence for Character Display

### 1.1 Basic Process Flow

1. Set display window for character
2. Send character bitmap data
3. Move cursor position

### 1.2 Detailed Command Sequence

```
For each character:

1. Set Column Address Window:
   DC=0: Send 0x2A (CASET command)
   DC=1: Send 4 bytes:
        - Start column high byte
        - Start column low byte
        - End column high byte
        - End column low byte

2. Set Row Address Window:
   DC=0: Send 0x2B (RASET command)
   DC=1: Send 4 bytes:
        - Start row high byte
        - Start row low byte
        - End row high byte
        - End row low byte

3. Begin Memory Write:
   DC=0: Send 0x2C (RAMWR command)
   DC=1: Send pixel data (16-bit color for each pixel)
```

### 1.3 Character Data Transmission

Adafruit is using their own font table and provided in the library, but lack of comments and hard a phrase, here's a standard 5x7 bitmap for ASCII characters for reference:
<https://andygock.github.io/glcd-documentation/font5x7_8h_source.html>

```text
Example: Drawing character 'A'
- Window size: 5×7 pixels
- Font data for A : 0x7E, 0x11, 0x11, 0x11, 0x7E
- Each byte represents one vertical column
- For each '1' bit: Send text color (16-bit)
- For each '0' bit: Send background color (16-bit)
```

## 2. Required Files and Dependencies

### 2.1 Core Files

1. glcdfont.c - Contains font bitmap data or any font file in Fonts folder
   - 5 bytes per character
   - ASCII characters 0x20 to 0x7E
   - Total size: ~480 bytes

2. Adafruit_GFX.cpp
   - Character drawing functions
   - Coordinate handling
   - Text attributes (size, color)

### 2.2 Process Flow

```text
Character Print Request
       ↓
Get font data from glcdfont.c(font file)
       ↓
Calculate window position
       ↓
Send CASET/RASET commands
       ↓
Convert font bits to pixels
       ↓
Send pixel data via SPI
       ↓
Update cursor position
```
