# Test file

table category
(
	string name
)

data category: category
(
	FirstBorn	(	"First born"	)
	SecBorn		(	"Second born"	)
)

table names
(
	string		name
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
	names*		names
	category	cat
)

data test: test
(
	Matt ( (Matt James Davies) FirstBorn )
	Kieron ( (Kieron John Anthony Davies) SecBorn )
)

