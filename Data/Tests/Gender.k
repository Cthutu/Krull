# Gender information

# General object definition:
#
#	<object_type> <object_name> [ ':' <object_qualifier> ] '(' <object_details> ')'
#


# Table definition
#
#	<field_type> <field_name>
#
# Supported field types:
#
#	string
#	int
#	float
#	bool
#

table Gender
(
	string		name
)

# Data definition
#
#	Qualifier is the name of the table
#
#	Each row has the format:
#
#		[ <optional_id> ] '(' <values> ... ')'
#

data Gender: Gender
(
	Male ("Male")
	Female ("Female")
)


