/** @file justify.h
 * This is the header file for the justification routines provided by the Flytools. This
 * file will implicitly include the dllist abstract data type. To use the functions provided
 * herein, simply include this header file in your source code.
 *
 * This source file is a part of the Flytools and is copyright (c) 2008-2009 Zachary Murray,
 * all rights reserved. This source file may not be redistributed, in whole or in part. By
 * viewing and/or using this file, you agree not to redistribute the source files without the
 * express written permission of the author(s). Doing so is a violation of international
 * copyright laws.
 *
 * @author Zachary Murray (dremelofdeath@gmail.com)
 */

#include <stdio.h>
#include <stdlib.h>
// This takes care of a problem with realtive includes on some legacy
// compilers.
#include "../llist/llnodes.h"
#include "../llist/dllist.h"

#ifndef __ZCM_TTY_JUSTIFY_CS_350__
#define __ZCM_TTY_JUSTIFY_CS_350__

/**
 * Parses the next paragraph in file. If file is NULL, the scanner continues
 * scanning the current file.
 * @param file the open file to parse (may be NULL)
 * @return a doubly linked list of type (matchresult *) containing the words and end-of-paragraph marker
 */
dllist *parse_paragraph(FILE *file);
/**
 * Parses all paragraphs in the file. The file parameter may not be NULL.
 * Continues parsing until the scanner reaches an end-of-file character.
 * @param file the open file to parse (may not be NULL)
 * @return a doubly linked list of type (dllist *) containing each of the paragraphs parsed
 */
dllist *parse_all_paragraphs(FILE *file);
/**
 * Parses the next paragraph in the string. If the string is NULL, the scanner
 * continues scanning the current buffer.
 * @param str the string to scan (may be NULL)
 * @return a doubly linked list of type (matchresult *) containing the words and end-of-paragraph marker
 */
dllist *parse_paragraph_string(const char *str);
/**
 * Parses all paragraphs in the string. The str parameter may not be NULL.
 * Continues parsing until the scanner reaches a NUL character.
 * @param str the string to scan (which may not be NULL)
 * @return a doubly linked list of type (dllist *) containing each of the paragraphs parsed
 */
dllist *parse_all_paragraphs_string(const char *str);
/**
 * Justifies the next line in the provided paragraph for the given screen
 * width. This function will return an empty string when provided with an
 * empty paragraph list. Also, the string returned will never be any longer
 * than the provided width. To avoid memory leaks, free the returned string
 * when it is done being used, as it is dynamically allocated.
 * @param paragraph the doubly linked list of (matchresult *) objects that contain the text to justify
 * @param width the width of the screen to justify for
 * @return a dynamically allocated string containing the justified text
 */
char *justify_next_line(dllist *paragraph, const int width);
/**
 * Justifies the specified text using the functions defined in this file
 * and prints them to a string in a standard process.
 * @param texttoprint the text to justify and print to the screen
 * @param width the virtual width of the target screen on which we're printing
 */
void print_justified_text(const char *texttoprint, const int width);

#endif
