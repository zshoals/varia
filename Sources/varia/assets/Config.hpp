#pragma once

enum class varia_config_value_type_tag_e
{
	VARIA_CONFIG_VALUE_TYPE_TAG_INT,
	VARIA_CONFIG_VALUE_TYPE_TAG_FLOAT,
	VARIA_CONFIG_VALUE_TYPE_TAG_STRING,
};

union varia_config_value_u
{
	int as_int;
	float as_float;
	char const * as_string;

	varia_config_value_type_tag_e value_type;
};
