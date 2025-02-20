
### Rcmd1 – 7735R Initialization, Part 1 (Red or Green Tab)

**Total Commands:** 15
Note that DC is 1 when command sent, DC is 0 when arguments sent
| Step | Command Name           | Command Code | Argument Count   | Delay?             | Details & Comments                                                                             |
|------|------------------------|--------------|------------------|--------------------|-----------------------------------------------------------------------------------------------|
| 1    | **ST77XX_SWRESET**     | 0x01         | 0                | Yes (150 ms)       | Software reset; no arguments; delay of 150 ms after sending.                                  |
| 2    | **ST77XX_SLPOUT**      | 0x11         | 0                | Yes (255 ms ~500 ms)| Exit sleep mode; no arguments; delay of 500 ms.                                               |
| 3    | **ST7735_FRMCTR1**     | 0xB1         | 3                | No                 | Frame rate control (normal mode); Data: 0x01, 0x2C, 0x2D.                                    |
| 4    | **ST7735_FRMCTR2**     | 0xB2         | 3                | No                 | Frame rate control (idle mode); Data: 0x01, 0x2C, 0x2D.                                      |
| 5    | **ST7735_FRMCTR3**     | 0xB3         | 6                | No                 | Frame rate control (partial mode); Data: 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D.                   |
| 6    | **ST7735_INVCTR**      | 0xB4   | 1                | No                 | Display inversion control; Data: 0x07.                                                        |
| 7    | **ST7735_PWCTR1**      | 0xC0         | 3                | No                 | Power control 1; Data: 0xA2 (no delay), 0x02 (-4.6V), 0x84 (auto mode).                                                       |
| 8    | **ST7735_PWCTR2**      | 0xC1         | 1                | No                 | Power control 2; Data: 0xC5.                ( VGH25=2.4C VGSEL=-10 VGH=3 * AVDD)                                                    |
| 9    | **ST7735_PWCTR3**      | 0xC2         | 2                | No                 | Power control 3; Data: 0x0A (Opamp current small),0x00(Boost frequency).                                                             |
| 10   | **ST7735_PWCTR4**      | 0xC3         | 2                | No                 | Power control 4; Data: 0x8A (BCLK/2), 0x2A (opamp current small & medium low).                                                             |
| 11   | **ST7735_PWCTR5**      | 0xC4    | 2                | No                 | Power control 5; Data: 0x8A, 0xEE.                                                             |
| 12   | **ST7735_VMCTR1**      | 0xC5         | 1                | No                 | Power control; Data: 0x0E.                                                                      |
| 13   | **ST77XX_INVOFF**      | 0x20         | 0                | No                 | Inversion off command; no arguments.                                                          |
| 14   | **ST77XX_MADCTL**      | 0x36         | 1                | No                 | Memory access control; Data: 0xC8 (row/col addr, bottom-top refresh).       |
| 15   | **ST77XX_COLMOD**      | 0x3A         | 1                | No                 | Color mode set; Data: 0x05 (16‑bit color format).                                               |

---

### Rcmd2 – Part 2 Variants

These command sets adjust the column and row address ranges depending on the display “tab” variant.

#### Rcmd2green (Green Tab Only)

**Total Commands:** 2

| Step | Command Name      | Command Code | Argument Count | Delay? | Details & Comments                                                                              |
|------|-------------------|--------------|----------------|--------|-------------------------------------------------------------------------------------------------|
| 1    | **ST77XX_CASET**  | 0x2A         | 4              | No     | Column address set; Data: 0x00, 0x02, 0x00, (0x7F+0x02 = 0x81) → XSTART = 0, XEND = 127.         |
| 2    | **ST77XX_RASET**  | 0x2B         | 4              | No     | Row address set; Data: 0x00, 0x01, 0x00, (0x9F+0x01 = 0xA0) → YSTART = 0, YEND = 159.            |

#### Rcmd2red (Red Tab Only)

**Total Commands:** 2

| Step | Command Name      | Command Code | Argument Count | Delay? | Details & Comments                                                         |
|------|-------------------|--------------|----------------|--------|-----------------------------------------------------------------------------|
| 1    | **ST77XX_CASET**  | 0x2A         | 4              | No     | Column address set; Data: 0x00, 0x00, 0x00, 0x7F → XSTART = 0, XEND = 127. |
| 2    | **ST77XX_RASET**  | 0x2B         | 4              | No     | Row address set; Data: 0x00, 0x00, 0x00, 0x9F → YSTART = 0, YEND = 159.    |

*Note:* For the red tab, no offset is applied; the display area starts at (0,0).

#### Rcmd2green144 (1.44" Green Tab)

**Total Commands:** 2

| Step | Command Name      | Command Code | Argument Count | Delay? | Details & Comments                                              |
|------|-------------------|--------------|----------------|--------|------------------------------------------------------------------|
| 1    | **ST77XX_CASET**  | 0x2A         | 4              | No     | Column address set; Data: 0x00, 0x00, 0x00, 0x7F (X: 0–127).     |
| 2    | **ST77XX_RASET**  | 0x2B         | 4              | No     | Row address set; Data: 0x00, 0x00, 0x00, 0x7F (Y: 0–127).         |

#### Rcmd2green160x80 (Mini 160x80)

**Total Commands:** 2

| Step | Command Name      | Command Code | Argument Count | Delay? | Details & Comments                                                                     |
|------|-------------------|--------------|----------------|--------|-----------------------------------------------------------------------------------------|
| 1    | **ST77XX_CASET**  | 0x2A         | 4              | No     | Column address set; Data: 0x00, 0x00, 0x00, 0x4F → X: 0–79 (for 80-pixel width).         |
| 2    | **ST77XX_RASET**  | 0x2B         | 4              | No     | Row address set; Data: 0x00, 0x00, 0x00, 0x9F → Y: 0–159.                                |

#### Rcmd2green160x80plugin (Mini 160x80 with Plugin FPC)

**Total Commands:** 3

| Step | Command Name      | Command Code     | Argument Count | Delay? | Details & Comments                                                                       |
|------|-------------------|------------------|----------------|--------|-------------------------------------------------------------------------------------------|
| 1    | **ST77XX_INVON**  | 0x21          | 0           | No     | Inversion on command (specific for the plugin version).                                  |
| 2    | **ST77XX_CASET**  | 0x2A             | 4              | No     | Column address set; Data: 0x00, 0x00, 0x00, 0x4F.                                         |
| 3    | **ST77XX_RASET**  | 0x2B             | 4              | No     | Row address set; Data: 0x00, 0x00, 0x00, 0x9F.                                             |

---

### Rcmd3 – 7735R Initialization, Part 3 (Gamma Correction and Display On)

**Total Commands:** 4

| Step | Command Name           | Command Code | Argument Count | Delay?           | Details & Comments                                                                               |
|------|------------------------|--------------|----------------|------------------|--------------------------------------------------------------------------------------------------|
| 1    | **ST7735_GMCTRP1**     | 0xE0         | 16             | (Delay implied)* | Positive Gamma Correction. Data: 0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10. |
| 2    | **ST7735_GMCTRN1**     | 0xE1         | 16             | (Delay implied)* | Negative Gamma Correction. Data: 0x03, 0x1D, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10. |
| 3    | **ST77XX_NORON**       | 0x13    | 0              | Yes (10 ms)      | Normal display on; no data; delay of 10 ms to ensure proper processing.                        |
| 4    | **ST77XX_DISPON**      | 0x29         | 0              | Yes (100 ms)     | Display on; no data; a final delay of 100 ms to allow the display circuitry to stabilize.       |

---

 **For Each Command:**
   - **Set DC Low:** This indicates that the upcoming byte is a command.
   - **Send Command Code:** Transmit the command code.
   - **Set DC High:** Switch to data mode.
   - **Send Data Bytes:** If the command requires parameters (the argument count > 0), send them in order.
   - **Implement Delay (if required):**  
     If the command’s argument count includes the ST_CMD_DELAY flag in the original code, then the byte following the parameters is not data but a delay value (in ms). Wait for that duration before continuing.
     
---