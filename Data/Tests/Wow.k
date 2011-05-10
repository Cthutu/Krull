#
# World of Warcraft test file
#

table EquipType
(
	string			name
	bool			b
	float			f
)

data EquipType: EquipType
(
	Weapon			( "Weapon" true)
	Relic			( "Relic" true)
	Ranged			( "Ranged" false)
	Wand			( "Wand" false)
)

table EquipCategory
(
	string			name
	EquipType		type
	int				hands
)

data EquipCategory: EquipCategory
(
	Axe				( "Two-handed axe"			Weapon		2 )
	Sword			( "Two-handed sword"		Weapon		2 )
	Mace			( "Two-handed mace"			Weapon		2 )
	PoleArm			( "Pole-arm"				Weapon		2 )
	Relic			( "Relic"					Relic		0 )
	Staff			( "Staff"					Weapon		2 )
	Dagger			( "Dagger"					Weapon		1 )
	Fist			( "Fist"					Weapon		1 )
	Bow				( "Bow"						Ranged		2 )
	Crossbow		( "Crossbow"				Ranged		2 )
	Gun				( "Gun"						Ranged		2 )
	Wand			( "Wand"					Wand		0 )
	Axe1			( "One-handed axe"			Weapon		1 )
	Sword1			( "One-handed sword"		Weapon		1 )
	Mace1			( "One-handed mace"			Weapon		1 )
	Thrown			( "Thrown"					Ranged		0 )
)

table Class
(
	string			name
	EquipCategory*	allowedEquipment
)

data PlayerClass: Class
(
	Warrior			( "Warrior"			( Axe Axe1 Sword Sword1 Mace Mace1 PoleArm Staff Dagger Fist Bow Crossbow Gun Thrown )			)
	Mage			( "Mage"			( Sword Sword1 Staff Dagger Wand )																)
	Rogue			( "Rogue"			( Axe1 Sword1 Mace1 Dagger Fist Bow Crossbow Gun Thrown )										)
	Priest			( "Priest"			( Mace1 Staff Dagger Wand )																		)
	Druid			( "Druid"			( Mace Mace1 PoleArm Staff Dagger Fist Relic )													)
	Hunter			( "Hunter"			( Axe Axe1 Sword Sword1 PoleArm Staff Dagger Fist Bow Crossbow Gun )							)
	Paladin			( "Paladin"			( Axe Axe1 Sword Sword1 Mace Mace1 PoleArm Relic )												)
	Shaman			( "Shaman"			( Axe Axe1 Mace Mace1 PoleArm Staff Dagger Fist Relic )											)
	Warlock			( "Warlock"			( Sword Sword1 Staff Dagger Wand )																)
	DeathKnight		( "DeathKnight"		( Axe Axe1 Sword Sword1 Mace Mace1 PoleArm Relic )												)
)

table Faction
(
	string			name
)

data Faction: Faction
(
	Alliance		( "Alliance" )
	Horde			( "Horde" )
)	

table Race
(
	string			name
	Faction			faction
	PlayerClass*	allowedClasses
	int				strength
	int				agility
	int				stamina
	int				intelligence
	int				spirit
)

data PlayerRaces: Race
(					# name			faction			allowed classes															str	agi	sta	int	spi
	Human			( "Human"		Alliance		(Priest Rogue Warrior Mage Hunter Warlock Paladin DeathKnight)			20	20	20	20	21		)
	Dwarf			( "Dwarf"		Alliance		(Priest Rogue Warrior Mage Hunter Warlock Shaman Paladin DeathKnight)	22	16	23	19	19		)
	NightElf		( "Night Elf"	Alliance		(Priest Rogue Warrior Mage Druid Hunter DeathKnight)					17	25	19	20	20		)
	Gnome			( "Gnome"		Alliance		(Priest Rogue Warrior Mage Warlock DeathKnight)							15	23	19	24	20		)
	Draenei			( "Draenei"		Alliance		(Priest Warrior Mage Hunter Shaman Paladin DeathKnight)					21	17	19	21	22		)
	Worgen			( "Worgen"		Alliance		(Priest Rogue Warrior Mage Druid Hunter Warlock DeathKnight)			23	22	20	16	19		)
	Orc				( "Orc"			Horde			(Rogue Warrior Mage Hunter Warlock Shaman DeathKnight)					23	17	22	17	23		)
	Undead			( "Undead"		Horde			(Priest Rogue Warrior Mage Hunter Warlock DeathKnight)					19	18	21	18	25		)
	Tauren			( "Tauren"		Horde			(Priest Warrior Druid Hunter Shaman Paladin DeathKnight)				25	15	22	15	22		)
	Troll			( "Troll"		Horde			(Priest Rogue Warrior Mage Druid Hunter Warlock Shaman DeathKnight)		21	22	21	16	21		)
	BloodElf		( "Blood Elf"	Horde			(Priest Rogue Warrior Mage Hunter Warlock Paladin DeathKnight)			17	22	18	24	19		)
	Goblin			( "Goblin"		Horde			(Priest Rogue Warrior Mage Hunter Warlock Shaman DeathKnight)			17	22	20	23	18		)
)

