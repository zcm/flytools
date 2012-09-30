/** @file justify.c
 * This file contains the text justification routines that are provided by the
 * Flytools. This file depends on the scanning routines in the scanner module.
 *
 * This source file is a part of the Flytools and is copyright (c) 2008-2009
 * Zachary Murray, all rights reserved. This source file may not be
 * redistributed, in whole or in part. By viewing and/or using this file, you
 * agree not to redistribute the source files without the express written
 * permission of the author(s). Doing so is a violation of international
 * copyright laws.
 *
 * @author Zachary Murray (dremelofdeath@gmail.com)
 */

// alloca.h is not included with Turbo C or Visual C++, so we should fix it
#if defined(__TURBOC__) || defined(_MSC_VER)
#define __JUSTIFY_FIX_ALLOCA__
#endif

#include <assert.h>
#ifndef __JUSTIFY_FIX_ALLOCA__
#include <alloca.h>
#endif
#include "justify.h"
#include "scanner.h"

//#include "internal.h"

static const int longest_english_word = 28;

list *__internal_parse_paragraph() {
	list *ret = list_create();
	matchresult *match;
	int stop = 0;
	// keep scanning until we hit a paragraph (which we record in the list) or the end of the
	// file (which we don't record in the list)
	while(!stop && (match = scanner_next())->type) {
		if(match->type == MATCH_TYPE_PARAGRAPH) {
			free(match->text);
			match->text = strdup("\n");
		}
		list_insert_right(ret, match);
		stop = match->type == MATCH_TYPE_PARAGRAPH;
	}
	return ret;
}

list *parse_paragraph(FILE *file) {
	// we don't have to provide a file if we're just scanning the next paragraph in the
	// currently scanned file
	if(file) {
		scanner_scanfile(file);
	}
	return __internal_parse_paragraph();
}

list *__internal_parse_all_paragraphs() {
	list *ret = list_create();
	// keep scanning paragraphs until there is no more paragraphs to scan
	// i.e., the last element of the last paragraph scanned is not of type MATCH_TYPE_PARAGRAPH
	do {
		list_insert_right(ret, parse_paragraph(NULL));
	} while(((matchresult *)((list *)ret->head->prev->data)->head->prev->data)->type == MATCH_TYPE_PARAGRAPH);
	return ret;
}

list *parse_all_paragraphs(FILE *file) {
	scanner_scanfile(file);
	return __internal_parse_all_paragraphs();
}

list *parse_paragraph_string(const char *str) {
	if(str) {
		scanner_scanstring(str);
	}
	return __internal_parse_paragraph();
}

list *parse_all_paragraphs_string(const char *str) {
	scanner_scanstring(str);
	return __internal_parse_all_paragraphs();
}

#if defined(__TURBOC__) || defined(_MSC_VER)
#define __min(a, b) \
	(((a) < (b)) ? (a) : (b))
#else
static inline int __min(int a, int b) {
	return a < b ? a : b;
}
#endif

char *justify_next_line(list *paragraph, const int width) {
	register int len = 0;
	register int nwords = 0;
	int i = 0, j = 0, ranout = 0, extraspacesperword = 0, extraspaces = 0;
	int extraspacestartindex = 0;
#ifdef __JUSTIFY_FIX_ALLOCA__
	char *wordbuf = (char *)malloc(sizeof(char)*longest_english_word + 1);
#else
	char *wordbuf = (char *)alloca(sizeof(char)*longest_english_word + 1);
#endif
	char *ret = (char *)calloc(width + 1, sizeof(char));
	listnode *current = paragraph->head;
	matchresult *match;
	// calculate how many words will fit on one line
	while(!(ranout = (current = current->next) == paragraph->head) &&
	      len + __min(longest_english_word, ((matchresult *)current->data)->len) < width) {
		len += __min(longest_english_word, ((matchresult *)current->data)->len) + 1;
		nwords++;
	}
	len--;
	// the last line is not justified, so if we ran out of words, we have 1 space per word
	if(!ranout) {
		extraspacesperword = nwords > 0 ? (width - len) / (nwords - 1) : 0;
		extraspaces = nwords > 0 ? (width - len) % (nwords - 1) : 0;
		extraspacestartindex = (nwords - 1) / 2 - extraspaces / 2;
	}
	// concatenate the words and spaces to the return value
	while(i < nwords - 1) {
		matchresult *match = (matchresult *)list_remove_leftmost(paragraph);
		// fill the buffer
		strncpy(wordbuf, match->text, longest_english_word + 1);
		wordbuf[longest_english_word] = '\0';
		// concatenate the return value with the buffer
		strcat(ret, wordbuf);
		j = 0;
		// add the spaces
		strcat(ret, " ");
		while(j++ < extraspacesperword) {
			strcat(ret, " ");
		}
		// add a single extra space, if we have one to give
		if(extraspaces > 0 && i >= extraspacestartindex) {
			strcat(ret, " ");
			extraspaces--;
		}
		// clean up
		destroy_matchresult(match);
		i++;
	}
	i = 0;
	match = (matchresult *)list_remove_leftmost(paragraph);
	strcat(ret, match->text);
	destroy_matchresult(match);
#ifdef __JUSTIFY_FIX_ALLOCA__
	free(wordbuf);
#endif
	assert(ranout ? strlen(ret) < width : strlen(ret) == width);
	return ret;
}

void print_justified_text(const char *texttoprint, const int width) {
	list *all_paragraphs = parse_all_paragraphs_string(texttoprint);
	while(all_paragraphs->size > 0) {
		list *current_paragraph = list_remove_leftmost(all_paragraphs);
		// justify each line
		while(current_paragraph->size > 0) {
			char *curline = justify_next_line(current_paragraph, width);
			// print each line
			printf("%s\n", curline);
			free(curline);
		}
		list_destroy(current_paragraph);
	}
	list_destroy(all_paragraphs);
}

#ifdef __JUSTIFY_FIX_ALLOCA__
#undef __JUSTIFY_FIX_ALLOCA__
#endif
