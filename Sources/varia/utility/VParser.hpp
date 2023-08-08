#pragma once

#include "varia/VShared.hpp"

struct Parser
{
    char const * source_data;
    Integer_64 length;

    Integer_64 working_head;
    Integer_64 line_begin_head;
    Integer_64 line_end_head;
    Integer_64 begin_head;
    Integer_64 end_head;
};

void v_parser_move_to_next_line(Parser * p);
void v_parser_move_to_next_integer(Parser * p);
void v_parser_advance_past(Parser * p, char const * string);

Integer_64 v_parser_read_integer(Parser * p);
Boolean v_parser_line_starts_with(Parser * p, char const * search);
Boolean v_parser_has_lines(Parser * p);

void v_parser_initialize(Parser * p, char const * source_data, Integer_64 source_data_length);
Integer_64 v_parser_remaining_line_bytes(Parser const * p, Integer_64 head);

Integer_64 v_parser_get_line_length(Parser const * p);
char const * v_parser_read_line(Parser * p);