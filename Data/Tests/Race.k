# Race data

uses Class

table Race
(
	string			race
	PlayerClasses*	allowedClasses
)

data PlayerRaces: Race
(
	Human	( "Human"		(Warrior Mage)		)
	Orc		( "Orc"			(Warrior Mage)		)
)

