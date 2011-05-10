# Test file

table names
(
	string name
)

data names: names
(
	Matt	( "Matt" )
	James	( "James" )
	Davies	( "Davies" )
	Kieron	( "Kieron" )
	John	( "John" )
	Anthony	( "Anthony" )
)

table test
(
	names* names
)

data test: test
(
	Matt ( (Matt James Davies) )
	Kieron ( (Kieron John Anthony Davies) )
)

