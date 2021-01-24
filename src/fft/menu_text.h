/*
 * menu_text.h
 *
 *  Created on: Jan 21, 2021
 *      Author: kaanari
 */

#ifndef MENU_TEXT_H_
#define MENU_TEXT_H_

const char TUNER_TEXT[] = {

      0x00, 0x00, 0x00, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, // 0x10 - ������� ������� ������

      0x00, 0x00, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0x03, 0x03, 0x03, 0x00, 0x00, // 0x54 - T

      0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, // 0x55 - U

      0x00, 0xFF, 0xFF, 0x0E, 0x38, 0xF0, 0xC0, 0x00, 0x00, 0xFF, 0xFF, 0x00, // 0x4E - N

      0x00, 0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x03, 0x03, 0x00, // 0x45 - E

      0x00, 0xFF, 0xFF, 0x83, 0x83, 0x83, 0x83, 0x83, 0xC7, 0xFE, 0x7C, 0x00, // 0x52 - R

      0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0x00, 0x00, 0x00, // 0x11 - ������� ������� �����

      0x00, 0x00, 0x00, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, // 0x10 ALT
      0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, // 0x54 - T ALT
      0x00, 0x07, 0x1F, 0x38, 0x30, 0x30, 0x30, 0x30, 0x38, 0x1F, 0x07, 0x00,
      0x00, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x03, 0x07, 0x1C, 0x3F, 0x3F, 0x00,
      0x00, 0x3F, 0x3F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00,
      0x00, 0x3F, 0x3F, 0x01, 0x01, 0x03, 0x07, 0x0F, 0x1D, 0x38, 0x30, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x00, 0x00, 0x00,

};



const char SPECTRUM_TEXT[] = {

      0x00, 0x00, 0x00, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, // 0x10 - ������� ������� ������

      0x00, 0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xC3, 0xC3, 0xC7, 0x8E, 0x0C, 0x00, // 0x53 - S

      0x00, 0xFF, 0xFF, 0x83, 0x83, 0x83, 0x83, 0x83, 0xC7, 0xFE, 0x7C, 0x00, // 0x50 - P

      0x00, 0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x03, 0x03, 0x00, // 0x45 - E

      0x00, 0xF0, 0xFC, 0x0E, 0x07, 0x03, 0x03, 0x03, 0x07, 0x0E, 0x0C, 0x00, // 0x43 - C

      0x00, 0x00, 0x03, 0x03, 0x03, 0xFF, 0xFF, 0x03, 0x03, 0x03, 0x00, 0x00, // 0x54 - T

      0x00, 0xFF, 0xFF, 0x83, 0x83, 0x83, 0x83, 0x83, 0xC7, 0xFE, 0x7C, 0x00, // 0x52 - R

      0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, // 0x55 - U

      0x00, 0xFF, 0xFF, 0x1E, 0x78, 0xE0, 0xE0, 0x78, 0x1E, 0xFF, 0xFF, 0x00, // 0x4D - M

      0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0x00, 0x00, 0x00, // 0x11 - ������� ������� �����

      0x00, 0x00, 0x00, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x0C, 0x1C, 0x38, 0x30, 0x30, 0x30, 0x30, 0x39, 0x1F, 0x0F, 0x00,
      0x00, 0x3F, 0x3F, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
      0x00, 0x3F, 0x3F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00,
      0x00, 0x03, 0x0F, 0x1C, 0x38, 0x30, 0x30, 0x30, 0x38, 0x1C, 0x0C, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x3F, 0x3F, 0x01, 0x01, 0x03, 0x07, 0x0F, 0x1D, 0x38, 0x30, 0x00,
      0x00, 0x07, 0x1F, 0x38, 0x30, 0x30, 0x30, 0x30, 0x38, 0x1F, 0x07, 0x00,
      0x00, 0x3F, 0x3F, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x3F, 0x3F, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x00, 0x00, 0x00,
};



#endif /* MENU_TEXT_H_ */