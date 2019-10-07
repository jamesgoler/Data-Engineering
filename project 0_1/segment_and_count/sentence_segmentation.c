#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sentence_segmentation.h"

// Match delimiters which indicate the end of sentence and return the position of it.
// Return NULL if there is no words in the sentence.
uchar *locate_sentence(uchar **start) {
    move_until_not_whitespace(start);

    // Remove redundant texts.
    if (is_img_ref(*start)) {
        move_across_right_parenthese(start);
    } else if (is_ad(*start)) {
        move_to_next_sentence(start);
    }

    // Remove redundant symbols.
    move_until_not_stop_word(start);

    uchar *end = *start;
    move_to_next_sentence(&end);

    return (end == *start) ? NULL : end;
}

void remove_control_chars(uchar *ptr) {
    uchar *in, *out;
    in = out = ptr;
    while(*in != '\0') {
        if (!is_control_char(*in)) {
            *out = *in;
            out++;
        }
        in++;
    }
    *out = '\0';
}

void move_to_next_sentence(uchar **ptr) {
    while(**ptr != '\0') {
        // find first occurring delimiter.
        if (is_sentence_delimiter(*ptr)) {
            *ptr += get_char_length(**ptr);

            // Sentences may end with multiple delimiters, e.g., xxx?!!.
            while(is_sentence_delimiter(*ptr) || match(*ptr, "」", 3) || match(*ptr, "｣", 3) || match(*ptr, "』", 3) || **ptr == '"') {
                if (is_direction_symbol(*ptr)) { // Special delimiters which should not be included in sentence.
                    break;
                }
                *ptr += get_char_length(**ptr);
            }
            break;
        } else {
            *ptr += get_char_length(**ptr);
        }
    }
    return;
}

void move_across_right_parenthese(uchar **ptr) {
    uchar *backup = *ptr;
    while (**ptr != '\0') {
        if (match(*ptr, "）", 3)) {
            *ptr += 3;
            return;
        } else {
            *ptr += 1;
        }
    }
    *ptr = backup;
}

void move_until_not_whitespace(uchar **ptr) {
    while (**ptr == ' ') {
        *ptr += 1;
    }
}

// Stop words includes {' ', "▲", "►", "▼", "◄"}.
void move_until_not_stop_word(uchar **ptr) {
    while (**ptr != '\0') {
        if (is_direction_symbol(*ptr)) {
            *ptr += 3;
        } else if (**ptr == ' '){
            *ptr += 1;
        } else {
            break;
        }
    }
}

inline int get_char_length(uchar c) {
    return ((c & 0xE0) == 0xE0) ? 3 : 1;
}

// Check whether this sentence is img ref.
// Patterns include: {"（圖／"}.
inline int is_img_ref(uchar *input) {
    return match(input, "（圖／", 9);
}

// Check whether this sentence is ad.
// Patterns include: {"►►►", ">>>", "★"}.
inline int is_ad(uchar *input) {
    return (match(input, "►►►", 9) || match(input, ">>>", 3) || match(input, "★", 3));
}

inline int is_direction_symbol(uchar *input) {
    return (match(input, "▲", 3) || match(input, "►", 3) || match(input, "▼", 3) || match(input, "◄", 3));
}

inline int is_control_char(uchar input) {
    return (input <= 31 || input == 127);
}

// Patterns include: {"。", "！", "？", "…", "!", "?"}.
inline int is_sentence_delimiter(uchar *input) {
    if(match(input, "。", 3) || match(input, "！", 3) || match(input, "？", 3) || match(input, "…", 3) || match(input, "；", 3)) {
        return 3;
    } else if(*input == '!' || *input == '?') {
        return 1;
    }
    return 0;
}

// Check if the sentence start with "@body:".
inline int is_body_block(const uchar *input) {
    return match(input, "@body:", 6);
}

inline int still_in_body_block(const uchar *input) {
    return (input[0] != '@' && input[1] != '\n');
}

// Shorthand for strncmp.
inline int match(const uchar *a, const uchar *b, size_t len) {
    return (strncmp(a, b, len) == 0);
}