#pragma once

#include "varia/VShared.hpp"

#include "varia/utility/VStringBuffer.hpp"

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
Boolean v_parser_done_reading(Parser * p);

void v_parser_initialize(Parser * p, char const * source_data, Integer_64 source_data_length);
Integer_64 v_parser_remaining_line_bytes(Parser const * p, Integer_64 head);

template <Integer_64 SIZE>
String_Buffer_Reference v_parser_read_line(Parser * p, String_Buffer<SIZE> * sb)
{
    char const * line = address_of(p->source_data[p->line_begin_head]);
    Integer_64 line_length = v_parser_remaining_line_bytes(p, p->line_begin_head);

    return v_string_buffer_emplace_string(sb, line, line_length);
}