# Screen Color Configuration

## 1. Setting the Drawing Window

### 1.1 Column Address Configuration (CASET)
**Command**: `0x2A` ST77XX_CASET

#### Steps:
1. Set the DC pin low and send the command `0x2A`
2. Set the DC pin high and send four data bytes defining the column window

#### Example Data (128-column display):
```plaintext
Data: 0x00, 0x00, 0x00, 0x7F
```
*(Start at column 0 and end at column 127)*

**Note**: Set the horizontal drawing area by sending the CASET command (0x2A). First set DC low to send the command code, then set DC high to send four data bytes (e.g., 0x00, 0x00, 0x00, 0x7F represents column range 0-127).

### 1.2 Row Address Configuration (RASET)
**Command**: `0x2B` ST77XX_RASET

#### Steps:
1. Set DC low and send the command `0x2B`
2. Set DC high and send four data bytes defining the row window

#### Example Data (160-row display):
```plaintext
Data: 0x00, 0x00, 0x00, 0x9F
```
*(Start at row 0 and end at row 159)*

**Note**: Similarly, set the vertical drawing area by sending the RASET command (0x2B). The data bytes define the start and end positions of rows (e.g., 0-159).

## 2. Writing Pixel Data

### 2.1 Memory Write Operation (RAMWR)
**Command**: `0x2C` ST77XX_RAMWR

#### Steps:
1. Set DC low and send the command `0x2C`
2. This indicates that subsequent bytes will be pixel data

**Note**: Send the RAMWR command (0x2C) to inform the display that the following data will be written to the graphics memory. 

### 2.2 Pixel Color Data Configuration

#### Process:
1. Set DC high (indicating data mode)
2. For each pixel in the drawing window, send 2 bytes representing the 16-bit RGB565 color value

#### Color Conversion:
Use the conversion from the Adafruit_GFX library (color565 function):

```c
uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
```

#### Example:
To display red:
```plaintext
Red (255,0,0) converts to 0xF800 → Send high byte 0xF8, low byte 0x00
```

#### Implementation:
Loop through and repeat sending the 2-byte color data for every pixel in the region (e.g., 128 × 160 = 20480 pixels for a full-screen fill), totally cost 40960 bytes for a screen fill. Better use a for loop to realize this. The order of filling should be Form left to right, fill in next row after the first row is filled.(Not verified)
