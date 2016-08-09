/**
 * @file text/color.h
 * @version 6.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#ifndef __TEXT_COLOR_H__
#define __TEXT_COLOR_H__

/* Reset; clears all colors and styles (to white on black). */
#define COLOR_RESET               "0"

/* Bold on. */
#define COLOR_BOLD_ON             "1"

/* Italics on. */
#define COLOR_ITALICS_ON          "3"

/* Underline on. */
#define COLOR_UNDERLINE_ON        "4"

/* Inverse on; reverses foreground & background colors. */
#define COLOR_INVERSE_ON          "7"

/* Strikethrough on. */
#define COLOR_STRIKETHROUGH_ON    "9"

/* Bold off. */
#define COLOR_BOLD_OFF            "22"

/* Italics off. */
#define COLOR_ITALICS_OFF         "23"

/* Underline off. */
#define COLOR_UNDERLINE_OFF       "24"

/* Inverse off. */
#define COLOR_INVERSE_OFF         "27"

/* Strikethrough off. */
#define COLOR_STRIKETHROUGH_OFF   "29"

/* Set foreground color to black. */
#define COLOR_FOREGROUND_BLACK    "30"

/* Set foreground color to red. */
#define COLOR_FOREGROUND_RED      "31"

/* Set foreground color to green. */
#define COLOR_FOREGROUND_GREEN    "32"

/* Set foreground color to yellow. */
#define COLOR_FOREGROUND_YELLOW   "33"

/* Set foreground color to blue. */
#define COLOR_FOREGROUND_BLUE     "34"

/* Set foreground color to magenta (purple). */
#define COLOR_FOREGROUND_MAGENTA  "35"

/* Set foreground color to cyan. */
#define COLOR_FOREGROUND_CYAN     "36"

/* Set foreground color to white. */
#define COLOR_FOREGROUND_WHITE    "37"

/* Set foreground color to default (white). */
#define COLOR_FOREGROUND_DEFAULT  "39"

/* Set background color to black. */
#define COLOR_BACKGROUND_BLACK    "40"

/* Set background color to red. */
#define COLOR_BACKGROUND_RED      "41"

/* Set background color to green. */
#define COLOR_BACKGROUND_GREEN    "42"

/* Set background color to yellow. */
#define COLOR_BACKGROUND_YELLOW   "43"

/* Set background color to blue. */
#define COLOR_BACKGROUND_BLUE     "44"

/* Set background color to magenta (purple). */
#define COLOR_BACKGROUND_MAGENTA  "45"

/* Set background color to cyan. */
#define COLOR_BACKGROUND_CYAN     "46"

/* Set background color to white. */
#define COLOR_BACKGROUND_WHITE    "47"

/* Set background color to default (black). */
#define COLOR_BACKGROUND_DEFAULT  "49"

#define COLOR(...) "\x1b[" __VA_ARGS__ "m"

#endif
