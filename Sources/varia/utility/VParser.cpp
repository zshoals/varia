#include "varia/utility/VParser.hpp"

#include "varia/utility/VStringUtil.hpp"
#include <string.h>


static inline Boolean v_parser_is_number(Parser * p, Integer_64 head)
{
    return v_string_utility_is_numeric(p->source_data[head]);
}

static inline Integer_64 v_parser_remaining_bytes(Parser const * p, Integer_64 head)
{
    return (p->length - head);
}

static inline Boolean v_parser_is_in_bounds(Parser const * p, Integer_64 head)
{
    return ( (head >= 0) && (head < p->length));
}

static inline Boolean v_parser_is_in_line(Parser const * p, Integer_64 head)
{
    Boolean in_bounds = v_parser_is_in_bounds(p, head);
    Boolean in_line = ( (head >= p->line_begin_head) && (head < p->line_end_head) );
    return (in_bounds & in_line);
}

static inline Integer_64 v_parser_remaining_line_bytes(Parser const * p, Integer_64 head)
{
    return (head >= p->line_begin_head && head < p->line_end_head);
}

static inline Integer_64 v_parser_sub_region_length(Parser const * p)
{
    if (p->end_head <= p->begin_head)
    {
        return 0;
    }
    else
    {
        return (p->end_head - p->begin_head);
    }
}

static inline void v_parser_mark_sub_region_begin(Parser * p)
{
    p->begin_head = p->working_head;
}

static inline void v_parser_mark_sub_region_end(Parser * p)
{
    p->end_head = p->working_head;
}

static inline void v_parser_mark_line_end(Parser * p)
{
    p->line_end_head = p->line_begin_head;
    Integer_64 remaining_bytes = v_parser_remaining_bytes(p, p->line_end_head);

    while (remaining_bytes > 0)
    {
        Boolean is_newline = v_string_utility_is_newline(p->source_data[p->line_end_head]);
        if (!is_newline)
        {
            p->line_end_head += 1;
            remaining_bytes -= 1;
        }
        else if (is_newline)
        {
            break;
        }
    }

    //Step back from the carriage return
    if ( (p->source_data[p->line_end_head] == '\r') && (p->line_end_head > 0) )
    {
        p->line_end_head -= 1;
    }
}

void v_parser_move_to_next_line(Parser * p)
{
    Integer_64 remaining_bytes = v_parser_remaining_bytes(p, p->working_head);

    while (remaining_bytes > 0)
    {
        Boolean is_newline = v_string_utility_is_newline(p->source_data[p->working_head]);

        if (!is_newline)
        {
            p->working_head += 1;
            remaining_bytes -= 1;
        }
        else if (is_newline)
        {
            break;
        }
    }

    //Advance past the newline character we found, or past the end by one (which is ok due to safety checks)
    p->working_head += 1;
    p->line_begin_head = p->working_head;

    v_parser_mark_line_end(p);
}

Boolean v_parser_line_starts_with(Parser * p, char const * search)
{
    if (search == nullptr)
    {
        return false;
    }

    Integer_64 search_string_length = strnlen(search, 10000);
    Integer_64 line_bytes = v_parser_remaining_line_bytes(p, p->line_begin_head);

    if (line_bytes >= search_string_length)
    {
        return (strncmp(address_of(p->source_data[p->line_begin_head]), search, search_string_length) == 0);
    }
    else
    {
        return false;
    }
}

void v_parser_advance_past(Parser * p, char const * string)
{
    if (string == nullptr)
    {
        return;
    }

    Integer_64 remaining_line_bytes = v_parser_remaining_line_bytes(p, p->working_head);
    Integer_64 string_length = strnlen(string, 13337);
    Integer_64 string_iterator = 0;

    while ( (remaining_line_bytes > 0) && (string_length > 0) && (p->source_data[p->working_head] == string[string_iterator]) )
    {
        p->working_head += 1;
        remaining_line_bytes -= 1;
        string_length -= 1;
        string_iterator += 1;
    }
}

void v_parser_move_to_next_integer(Parser * p)
{
    Integer_64 remaining_line_bytes = v_parser_remaining_line_bytes(p, p->working_head);

    //We're already on a number, move past it
    while ( (remaining_line_bytes > 0) && v_parser_is_number(p, p->working_head) )
    {
        p->working_head += 1;
        remaining_line_bytes -= 1;
    }

    //Advance past anything that isn't a number to get to the next number on the line
    while ( (remaining_line_bytes > 0) && !(v_parser_is_number(p, p->working_head)) )
    {
        p->working_head += 1;
        remaining_line_bytes -= 1;
    }
}

Integer_64 v_parser_read_integer(Parser * p)
{
    v_parser_mark_sub_region_begin(p);
    v_parser_mark_sub_region_end(p);

    Integer_64 remaining_line_bytes = v_parser_remaining_line_bytes(p, p->begin_head);

    while ( (remaining_line_bytes > 0) && v_parser_is_number(p, p->end_head) )
    {
        p->end_head += 1;
    }

    if (v_parser_is_in_bounds(p, p->begin_head))
    {
        char const * number_start = address_of(p->source_data[p->begin_head]);
        Integer_64 number_length = v_parser_sub_region_length(p);
        Integer_64 parsed_number = v_string_utility_string_as_int(number_start, number_length);

        return parsed_number;
    }
    else
    {
        return 0;
    }
}

Boolean v_parser_done_reading(Parser * p)
{
    return (p->working_head >= p->length);
}

void v_parser_initialize(Parser * p, char const * source_data, Integer_64 source_data_length)
{
    p->source_data = source_data;
    p->length = source_data_length;

    p->working_head = 0;
    p->line_begin_head = 0;
    p->line_end_head = 0;
    p->begin_head = 0;
    p->end_head = 0;

    v_parser_mark_line_end(p);
}
