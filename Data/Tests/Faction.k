# Faction data

uses Class

table Faction
(
	string			name				# Human-readable string
	PlayerClasses*	allowedClasses
)

data Faction: Faction
(
	# Player factions
	Free		("Free"		(Warrior Mage)			)
	Foul		("Foul"		(Warrior Mage)			)
)
