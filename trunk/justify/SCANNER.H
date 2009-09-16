#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef __ZCM_SCANNER_H__
#define __ZCM_SCANNER_H__

/**
 * An enumeration of possible match types. Set internally by the rules when matched.
 */
enum MATCH_TYPE {
	MATCH_TYPE_EOF,
	MATCH_TYPE_NONSPACEBLOCK,
	MATCH_TYPE_PARAGRAPH,
	NONMATCH_SPACE
};

/**
 * A structure containing a match result, such as the one returned from a call to
 * scanner_next(). Contains the matched text and some metadata about the match.
 */
struct MATCH_RESULT {
	enum MATCH_TYPE type; //!< The type of token matched.
	char *text; //!< The matched text.
	int len; //!< The length of the matched text.
};

typedef struct MATCH_RESULT matchresult;

/**
 * Instructs the scanner to begin parsing a new file or block device.
 * @param file the file pointer to begin parsing (which may be stdin)
 */
void scanner_scanfile(FILE *file);
/**
 * Instructs the scanner to begin parsing a new string from the beginning.
 * @param str the string to begin parsing
 */
void scanner_scanstring(const char *str);
/**
 * Scans the input for the next token and returns it as a (matchresult *)
 * object.
 * @return the next token matched from the input
 */
matchresult *scanner_next();
/**
 * Cleans the memory used by a matchresult instance. Do not use this
 * function to clean up for a matchresult whose text member was not allocated
 * dynamically. Doing so will trigger a runtime error.
 * @param match the matchresult object to destroy
 */
void destroy_matchresult(matchresult *match);

#endif
