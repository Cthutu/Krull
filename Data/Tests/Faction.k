# Faction data

uses Race

table Faction
(
	string			name				# Human-readable string
	PlayerRaces*	allowedRaces
)

data Faction: Faction
(
	# Player factions
	Free		("Free"		(Human)			)
	Foul		("Foul"		(Orc)			)
)
