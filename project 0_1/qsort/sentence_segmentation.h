#ifndef SENTENCE_SEGMENTATION_H_
#define SENTENCE_SEGMENTATION_H_

#include "sentence_count.h"

// Match delimiters which indicate the end of sentence and return the position of it.
uchar *locate_sentence(uchar **start);

void remove_control_chars(uchar *ptr);

void move_to_next_sentence(uchar **ptr);

void move_across_right_parenthese(uchar **ptr);

void move_until_not_whitespace(uchar **ptr);

// Stop words includes {' ', '\n', "▲", "►", "▼", "◄"}.
void move_until_not_stop_word(uchar **ptr);

int get_char_length(uchar c);

// Check whether this sentence is img ref.
// Patterns include: {"（圖／"}.
int is_img_ref(uchar *input);

// Check whether this sentence is ad.
// Patterns include: {"►►►", ">>>", "★"}.
int is_ad(uchar *input);

int is_direction_symbol(uchar *input);

int is_control_char(uchar input);

// Patterns include {"。", "！", "？", "…", "!", "?"}.
int is_sentence_delimiter(uchar *input);

int is_body_block(const uchar *input);

int still_in_body_block(const uchar *input);

int match(const uchar *a, const uchar *b, size_t len);

uchar *copy_and_return_end(uchar *dest, uchar *start, uchar *end);

#endif