// paranoia
//
// This game was published in Volume 1, Issue 94 of the "comp.sources.games"
// Usenet newsgroup on 1987-07-16.  It was translated to Go in 2022-02 as part
// of the "comp.sources.games" Github project.
//
// Original description follows:
//
//     This is a solo paranoia game taken from the Jan/Feb issue (No. 77) of
//     "SpaceGamer/FantasyGamer" magazine.
//
//     Article by Sam Shirley.
//     Implemented in C on Vax 11/780 under UNIX by Tim Lister.
//
//     This is a public domain adventure and may not be sold for profit.
package main

import (
	"bufio"
	"flag"
	"fmt"
	"html/template"
	"log"
	"os"
)

var debug = flag.Bool("debug", false, "Turn on debugging mode.")

const (
	MaxLives = 6 // Maximum number of player clones.
	MaxKill  = 7 // Maximum number of Ultraviolets you can kill.
)

// input provides all of our player input.
var input = bufio.NewReader(os.Stdin)

// Abilities wraps the subset of player ability scores that are actually
// referenced by the game.  These ability score are generally tested against
// a 2d10 die roll.
type Abilities struct {
	Moxie, Agility int
}

// NewAbilities creates a new Abilities struct.
func NewAbilities() *Abilities {
	return &Abilities{
		Moxie:   13,
		Agility: 15,
	}
}

// Flags wraps the collection of flags associated with the player's progress
// through the game.
type Flags struct {
	// ActionDoll indicates possession of the action doll.
	ActionDoll bool

	// BlastDoor indicates whether the player has opened the blast door.
	BlastDoor bool

	// ComputerRequest indicates whether the player has complied with the
	// computer request to "stop and chat".
	ComputerRequest bool

	// ReadLetter indicates whether the player has read the sealed letter.
	ReadLetter bool

	// Ultraviolet indicates whether the player is disguised as an Ultraviolet.
	Ultraviolet bool
}

// State describes the current state of the game.
type State struct {
	// Clone holds the number of the player's current clone.
	Clone int

	// Page holds the current "page number" within the story.
	Page int

	// HitPoints holds the current number of hit points.
	HitPoints int

	// PlatoClone holds the number of the Plato clone.
	PlatoClone int

	// KillerCount holds the number of player kills.
	KillerCount int

	// Abilities holds the player's ability scores.
	Abilities *Abilities

	// Flags holds the collection of status flags.
	Flags *Flags
}

// NewState initializes a fresh, new game state.
func NewState() *State {
	return &State{
		Clone:      1,
		Page:       1,
		HitPoints:  10,
		PlatoClone: 3,
		Abilities:  NewAbilities(),
		Flags:      &Flags{},
	}
}

// Character prints out the character sheet.
func (s *State) Character() {
	t := template.Must(template.New("character").Parse(`
===============================================================================
The Character : Philo-R-DMD {{.Clone}}
Primary Attributes                      Secondary Attributes
===============================================================================
Strength ..................... 13       Carrying Capacity ................. 30
Endurance .................... 13       Damage Bonus ....................... 0
Agility ...................... {{printf "%2d" .Abilities.Agility}}       Macho Bonus ....................... -1
Manual Dexterity ............. 15       Melee Bonus ...................... +5%
Moxie ........................ {{printf "%2d" .Abilities.Moxie}}       Aimed Weapon Bonus .............. +10%
Chutzpah ...................... 8       Comprehension Bonus .............. +4%
Mechanical Aptitude .......... 14       Believability Bonus .............. +5%
Power Index .................. 10       Repair Bonus ..................... +5%
===============================================================================
Credits: 160        Secret Society: Illuminati        Secret Society Rank: 1
Service Group: Power Services               Mutant Power: Precognition
Weapon: laser pistol; to hit, 40%; type, L; Range, 50m; Reload, 6r; Malfnt, 00
Skills: Basics 1(20%), Aimed Weapon Combat 2(35%), Laser 3(40%),
        Personal Development 1(20%), Communications 2(29%), Intimidation 3(34%)
Equipment: Red Reflec Armour, Laser Pistol, Laser Barrel (red),
           Notebook & Stylus, Knife, Com Unit 1, Jump suit,
           Secret Illuminati Eye-In-The-Pyramid(tm) Decoder ring,
           Utility Belt & Pouches
===============================================================================
`))
	t.Execute(os.Stdout, s)
}

// Choose offers the player an A/B choice.  The return value is the page number
// associated with that choice.
func (s *State) Choose(aPage int, a string, bPage int, b string) int {
	fmt.Println("\nSelect 'a' or 'b' :")
	fmt.Printf("  a - %s.\n", a)
	fmt.Printf("  b - %s.\n", b)

	for ch := GetChar(); ; ch = GetChar() {
		switch ch {
		case 'a':
			return aPage
		case 'b':
			return bPage
		}
	}
}

// Exec executes the function for the current page and returns the index of
// the next page for the player to encounter.
func (s *State) Exec() int {
	fmt.Printf("PAGE %d\n", s.Page)
	next := Pages[s.Page](s)
	if next != 0 {
		s.More()
	}
	return next
}

// Instructions presents the instruction page to the player.
func (s *State) Instructions() {
	fmt.Print(`
Welcome to Paranoia!
	
HOW TO PLAY:

  Just press <RETURN> until you are asked to make a choice.
  Select 'a' or 'b' or whatever for your choice, then press <RETURN>.

  You may select 'p' at any time to get a display of your statistics.
  Always choose the least dangerous option.  Continue doing this until you win.
  At times you will use a skill or engage in combat and and will be informed of
  the outcome.  These sections will be self explanatory.

HOW TO DIE:

  As Philo-R-DMD you will die at times during the adventure.
  When this happens you will be given an new clone at a particular location.
  The new Philo-R will usually have to retrace some of the old Philo-R's path;
  hopefully he won't make the same mistake as his predecessor.

HOW TO WIN:

  Simply complete the mission before you expend all six clones.
  If you make it, congratulations.
  If not, you can try again later.
`)
}

// More pauses output for a user response, optionally giving them a chance to
// review their character info through the "p" command.
func (s *State) More() {
	fmt.Print("\n---------- More ----------")
	if *debug {
		fmt.Printf("(page %d)", s.Page)
	}

	if GetChar() == 'p' {
		s.Character()
		fmt.Print("\n---------- More ----------")
		GetChar()
	}
}

// NewClone kills the player's current clone and attempts to create a new one
// starting at the given page.  The return value is the new page number, which
// is 0 for game over.
func (s *State) NewClone(page int) int {
	fmt.Printf("\nClone %d just died.\n", s.Clone)
	s.Clone++

	if s.Clone > MaxLives {
		fmt.Print(`
*** You Lose ***

All your clones are dead.  Your name has been stricken from the records.

			THE END
`)
		return 0
	}

	fmt.Printf("Clone %d now activated.\n", s.Clone)
	s.Flags.Ultraviolet = false
	s.Flags.ActionDoll = false
	s.HitPoints = 10
	s.KillerCount = 0
	return page
}

//-----------------------------------------------------------------------------

// A PageFunc implements a single "page" in the game and returns the index of
// the next page for the player.
type PageFunc func(*State) int

// Pages is the global map of page functions.
var Pages = map[int]PageFunc{
	1: func(*State) int {
		fmt.Print(`
You wake up face down on the red and pink checked E-Z-Kleen linoleum floor.

You recognise the pattern, it's the type preferred in the internal security
briefing cells.  When you finally look around you, you see that you are alone
in a large mission briefing room.
`)
		return 57
	},

	2: func(s *State) int {
		fmt.Print(`
"Greetings," says the kindly Internal Security self incrimination expert who
meets you at the door, "How are we doing today?"  He offers you a doughnut
and coffee and asks what brings you here.  This doesn't seem so bad, so you
tell him that you have come to confess some possible security lapses.  He
smiles knowingly, deftly catching your coffee as you slump to the floor.
"Nothing to be alarmed about; it's just the truth serum," he says,
dragging you back into a discussion room.

The next five hours are a dim haze, but you can recall snatches of conversation
about your secret society, your mutant power, and your somewhat paranoid
distrust of The Computer.  This should explain why you are hogtied and moving
slowly down the conveyer belt towards the meat processing unit in Food
Services.
`)
		if s.Flags.ComputerRequest {
			return s.NewClone(45)
		}
		return s.NewClone(32)
	},

	3: func(s *State) int {
		fmt.Print(`
You walk to the nearest Computer terminal and request more information about
Christmas.  The Computer says, "That is an A-1 ULTRAVIOLET ONLY IMMEDIATE
TERMINATION classified topic.  What is your clearance please, Troubleshooter?"
`)
		return s.Choose(
			4, "You give your correct clearance",
			5, "You lie and claim Ultraviolet clearance")
	},

	4: func(*State) int {
		fmt.Print(`
"That is classified information, Troubleshooter, thank you for your inquiry.
 Please report to an Internal Security self incrimination station as soon as
 possible."
`)
		return 9
	},

	5: func(s *State) int {
		fmt.Print(`
The computer says, "Troubleshooter, you are not wearing the correct colour
uniform.  You must put on an Ultraviolet uniform immediately.  I have seen to
your needs and ordered one already; it will be here shortly.  Please wait with
your back to the wall until it arrives."  In less than a minute an infrared
arrives carrying a white bundle.  He asks you to sign for it, then hands it to
you and stands back, well outside of a fragmentation grenade's blast radius.
`)
		return s.Choose(
			6, "You open the package and put on the uniform",
			7, "You finally come to your senses and run for it")
	},

	6: func(s *State) int {
		fmt.Print(`
The uniform definitely makes you look snappy and pert.  It really looks
impressive, and even has the new lopsided lapel fashion that you admire so
much.  What's more, citizens of all ranks come to obsequious attention as you
walk past.  This isn't so bad being an Ultraviolet.  You could probably come
to like it, given time.

The beeping computer terminal interrupts your musings.
`)
		s.Flags.Ultraviolet = true
		return 8
	},

	7: func(s *State) int {
		fmt.Print(`
The corridor lights dim and are replaced by red battle lamps as the Security
Breach alarms howl all around you.  You run headlong down the corridor and
desperately windmill around a corner, only to collide with a squad of 12 Blue
clearance Vulture squadron soldiers.  "Stop, Slime Face," shouts the
commander, "or there won't be enough of you left for a tissue sample."

"All right, soldiers, stuff the greasy traitor into the uniform," he orders,
waving the business end of his blue laser scant inches from your nose.
With his other hand he shakes open a white bundle to reveal a pristine new
Ultraviolet citizen's uniform.

One of the Vulture squadron Troubleshooters grabs you by the neck in the
exotic and very painful Vulture Clamp(tm) death grip (you saw a special about
it on the Teela O'Malley show), while the rest tear off your clothes and
force you into the Ultraviolet uniform.  The moment you are dressed they step
clear and stand at attention.
`)
		s.More()
		fmt.Print(`
"Thank you for your cooperation, sir," says the steely eyed leader of the
Vulture Squad.  "We will be going about our business now."  With perfect
timing the Vultures wheel smartly and goosestep down the corridor.

Special Note: don't make the mistake of assuming that your skills have
improved any because of the uniform; you're only a Red Troubleshooter
traitorously posing as an Ultraviolet, and don't you forget it!

Suddenly, a computer terminal comes to life beside you.
`)
		s.Flags.Ultraviolet = true
		return 8
	},

	11: func(s *State) int {
		fmt.Print(`
The printing on the folder says "Experimental Self Briefing."

You open it and begin to read the following:

Step 1: Compel the briefing subject to attend the briefing.
        Note: See Experimental Briefing Sub Form Indigo-WY-2,
        'Experimental Self Briefing Subject Acquisition Through The Use Of
         Neurotoxin Room Foggers.'

Step 2: Inform the briefing subject that the briefing has begun.
        ATTENTION: THE BRIEFING HAS BEGUN.

Step 3: Present the briefing material to the briefing subject.
        GREETINGS TROUBLESHOOTER.
        YOU HAVE BEEN SPECIALLY SELECTED TO SINGLEHANDEDLY
        WIPE OUT A DEN OF TRAITOROUS CHRISTMAS ACTIVITY.  YOUR MISSION IS TO
        GO TO GOODS DISTRIBUTION HALL 7-BETA AND ASSESS ANY CHRISTMAS ACTIVITY
        YOU FIND THERE.  YOU ARE TO INFILTRATE THESE CHRISTMAS CELEBRANTS,
        LOCATE THEIR RINGLEADER, AN UNKNOWN MASTER RETAILER, AND BRING HIM
        BACK FOR EXECUTION AND TRIAL.  THANK YOU.  THE COMPUTER IS YOUR FRIEND.
`)
		s.More()
		fmt.Print(`
Step 4: Sign the briefing subject's briefing release form to indicate that
        the briefing subject has completed the briefing.
        ATTENTION: PLEASE SIGN YOUR BRIEFING RELEASE FORM.

Step 5: Terminate the briefing
        ATTENTION: THE BRIEFING IS TERMINATED.

You walk to the door and hold your signed briefing release form up to the
plexiglass window.  A guard scrutinises it for a moment and then slides back
the megabolts holding the door shut.  You are now free to continue the
mission.
`)
		return s.Choose(
			3, "You wish to ask The Computer for more information about Christmas",
			10, "You have decided to go directly to Goods Distribution Hall 7-beta")
	},

	57: func(s *State) int {
		fmt.Print(`
In the centre of the room is a table and a single chair.  There is an Orange
folder on the table top, but you can\'t make out the lettering on it.
`)
		return s.Choose(
			11, "You sit down and read the folder",
			12, "You leave the room")
	},
}

/*
dice_roll(number,faces)
int number, faces;
{
	int i,total=0;
	for(i=number;i>0;i--)	total+= rand()%faces+1;
	return total;
}

page8()
{
	printf("\"Now, about your question, citizen.  Christmas was an old world marketing ploy\n");
	printf("to induce lower clearance citizens to purchase vast quantities of goods, thus\n");
	printf("accumulation a large amount of credit under the control of a single class of\n");
	printf("citizen known as Retailers.  The strategy used is to imply that all good\n");
	printf("citizens give gifts during Christmas, thus if one wishes to be a valuable\n");
	printf("member of society one must also give gifts during Christmas.  More valuable\n");
	printf("gifts make one a more valuable member, and thus did the Retailers come to\n");
	printf("control a disproportionate amount of the currency.  In this way Christmas\n");
	printf("eventually caused the collapse of the old world.  Understandably, Christmas\n");
	printf("has been declared a treasonable practice in Alpha Complex.\n");
	printf("Thank you for your inquiry.\"\n");
	printf("You continue on your way to GDH7-beta.\n");
	return 10;
}

page9()
{
	int choice;
	printf("As you walk toward the tubecar that will take you to GDH7-beta, you pass one\n");
	printf("of the bright blue and orange Internal Security self incrimination stations.\n");
	printf("Inside, you can see an IS agent cheerfully greet an infrared citizen and then\n");
	printf("lead him at gunpoint into one of the rubber lined discussion rooms.\n");
	choice=choose(2,"You decide to stop here and chat, as ordered by The Computer",10,"You just continue blithely on past");
	if (choice==2) computer_request = 1;
	else	       computer_request = 0;
	return choice;
}

page10()
{
	int choice;
	printf("You stroll briskly down the corridor, up a ladder, across an unrailed catwalk,\n");
	printf("under a perilously swinging blast door in urgent need of repair, and into\n");
	printf("tubecar grand central.  This is the bustling hub of Alpha Complex tubecar\n");
	printf("transportation.  Before you spreads a spaghetti maze of magnalift tube tracks\n");
	printf("and linear accelerators.  You bravely study the specially enhanced 3-D tube\n");
	printf("route map; you wouldn\'t be the first Troubleshooter to take a fast tube ride\n");
	printf("to nowhere.\n");
	if (ultra_violet==0)
	{
		choice=choose(3,"You decide to ask The Computer about Christmas using a nearby terminal",10,"You think you have the route worked out, so you\'ll board a tube train");
		if (choice==3) return choice;
	};
	printf("You nervously select a tubecar and step aboard.\n");
	if (dice_roll(2,10)<MOXIE)
	{
		printf("You just caught a purple line tubecar.\n");
		return 13;
	}
	else
	{
		printf("You just caught a brown line tubecar.\n");
		return 48;
	}
}

page12()
{
	printf("You walk up to the door and push the button labelled \"push to exit.\"\n");
	printf("Within seconds a surly looking guard shoves his face into the small plexiglass\n");
	printf("window.  You can see his mouth forming words but you can\'t hear any of them.\n");
	printf("You just stare at him blankly  for a few moments until he points down to a\n");
	printf("speaker on your side of the door.  When you put your ear to it you can barely\n");
	printf("hear him say, \"Let\'s see your briefing release form, bud.  You aren\'t\n");
	printf("getting out of here without it.\"\n");
	return choose(11,"You sit down at the table and read the Orange packet",57,"You stare around the room some more");
}

page13()
{
	printf("You step into the shiny plasteel tubecar, wondering why the shape has always\n");
	printf("reminded you of bullets.  The car shoots forward the instant your feet touch\n");
	printf("the slippery gray floor, pinning you immobile against the back wall as the\n");
	printf("tubecar careens toward GDH7-beta.  Your only solace is the knowledge that it\n");
	printf("could be worse, much worse.\n");
	printf("Before too long the car comes to a stop.  You can see signs for GDH7-beta\n");
	printf("through the window.  With a little practice you discover that you can crawl\n");
	printf("to the door and pull open the latch.\n");
	return 14;
}

page14()
{
	printf("You manage to pull yourself out of the tubecar and look around.  Before you is\n");
	printf("one of the most confusing things you have ever seen, a hallway that is\n");
	printf("simultaneously both red and green clearance.  If this is the result of\n");
	printf("Christmas then it\'s easy to see the evils inherent in its practice.\n");
	printf("You are in the heart of a large goods distribution centre.  You can see all\n");
	printf("about you evidence of traitorous secret society Christmas celebration; rubber\n");
	printf("faced robots whiz back and forth selling toys to holiday shoppers, simul-plast\n");
	printf("wreaths hang from every light fixture, while ahead in the shadows is a citizen\n");
	printf("wearing a huge red synthetic flower.\n");
	return 22;
}

page15()
{
	printf("You are set upon by a runty robot with a queer looking face and two pointy\n");
	printf("rubber ears poking from beneath a tattered cap.  \"Hey mister,\" it says,\n");
	printf("\"you done all your last minute Christmas shopping?  I got some real neat junk\n");
	printf("here.  You don\'t wanna miss the big day tommorrow, if you know what I mean.\"\n");
	printf("The robot opens its bag to show you a pile of shoddy Troubleshooter dolls.  It\n");
	printf("reaches in and pulls out one of them.  \"Look, these Action Troubleshooter(tm)\n");
	printf("dolls are the neatest thing.  This one\'s got moveable arms and when you\n");
	printf("squeeze him, his little rifle squirts realistic looking napalm.  It\'s only\n");
	printf("50 credits.  Oh yeah, Merry Christmas.\"\n");
	printf("\nSelect \'a\', \'b\' or \'c\' :\n");
	printf(" a - You decide to buy the doll.\n");
	printf(" b - You shoot the robot.\n");
	printf(" c - You ignore the robot and keep searching the hall.\n");
	switch(get_char())
	{
		case 'a' :	return 16;
		case 'b' :	return 17;
		case 'c' :
		default  :	return 22;
	}
}

page16()
{
	printf("The doll is a good buy for fifty credits; it will make a fine Christmas present\n");
	printf("for one of your friends.  After the sale the robot rolls away.  You can use\n");
	printf("the doll later in combat.  It works just like a cone rifle firing napalm,\n");
	printf("except that occasionally it will explode and blow the user to smithereens.\n");
	printf("But don\'t let that stop you.\n");
	action_doll=1;
	return 22;
}

page17()
{
	int i, robot_hp=15;
	printf("You whip out your laser and shoot the robot, but not before it squeezes the\n");
	printf("toy at you.  The squeeze toy has the same effect as a cone rifle firing napalm,\n");
	printf("and the elfbot\'s armour has no effect against your laser.\n");
	for(i=0;i<2;i++)
	{
		if(dice_roll(1,100)<=25)
		{
			printf("You have been hit!\n");
			hit_points-= dice_roll(1,10);
			if (hit_points<=0)	return new_clone(45);
		}
		else	printf("It missed you, but not by much!\n");
		if(dice_roll(1,100)<=40)
		{
			printf("You zapped the little bastard!\n");
			robot_hp-= dice_roll(2,10);
			if (robot_hp<=0)
			{
				printf("You wasted it! Good shooting!\n");
				printf("You will need more evidence, so you search GDH7-beta further\n");
				if (hit_points<10) printf("after the GDH medbot has patched you up.\n");
				hit_points=10;
				return 22;
			}
		}
		else	printf("Damn! You missed!\n");
	};
	printf("It tried to fire again, but the toy exploded and demolished it.\n");
	printf("You will need more evidence, so you search GDH7-beta further\n");
	if (hit_points<10) printf("after the GDH medbot has patched you up.\n");
	hit_points=10;
	return 22;
}

page18()
{
	printf("You walk to the centre of the hall, ogling like an infrared fresh from the\n");
	printf("clone vats.  Towering before you is the most unearthly thing you have ever\n");
	printf("seen, a green multi armed mutant horror hulking 15 feet above your head.\n");
	printf("Its skeletal body is draped with hundreds of metallic strips (probably to\n");
	printf("negate the effects of some insidious mutant power), and the entire hideous\n");
	printf("creature is wrapped in a thousand blinking hazard lights.  It\'s times like\n");
	printf("this when you wish you\'d had some training for this job.  Luckily the\n");
	printf("creature doesn\'t take notice of you but stands unmoving, as though waiting for\n");
	printf("a summons from its dark lord, the Master Retailer.\n");
	printf("WHAM, suddenly you are struck from behind.\n");
	if (dice_roll(2,10)<AGILITY)	return 19;
	else				return 20;
}

page19()
{
	printf("Quickly you regain your balance, whirl and fire your laser into the Ultraviolet\n");
	printf("citizen behind you.  For a moment your heart leaps to your throat, then you\n");
	printf("realise that he is indeed dead and you will be the only one filing a report on\n");
	printf("this incident.  Besides, he was participating in this traitorous Christmas\n");
	printf("shopping, as is evident from the rain of shoddy toys falling all around you.\n");
	printf("Another valorous deed done in the service of The Computer!\n");
	if (++killer_count>(MAXKILL-clone))	return 21;
	if (read_letter==1)	return 22;
	return choose(34,"You search the body, keeping an eye open for Internal Security",22,"You run away like the cowardly dog you are");
}

page20()
{
	printf("Oh no! you can\'t keep your balance.  You\'re falling, falling head first into\n");
	printf("the Christmas beast\'s gaping maw.  It\'s a valiant struggle; you think you are\n");
	printf("gone when its poisonous needles dig into your flesh, but with a heroic effort\n");
	printf("you jerk a string of lights free and jam the live wires into the creature\'s\n");
	printf("spine.  The Christmas beast topples to the ground and begins to burn, filling\n");
	printf("the area with a thick acrid smoke.  It takes only a moment to compose yourself,\n");
	printf("and then you are ready to continue your search for the Master Retailer.\n");
	return 22;
}

page21()
{
	printf("You have been wasting the leading citizens of Alpha Complex at a prodigious\n");
	printf("rate.  This has not gone unnoticed by the Internal Security squad at GDH7-beta.\n");
	printf("Suddenly, a net of laser beams spear out of the gloomy corners of the hall,\n");
	printf("chopping you into teeny, weeny bite size pieces.\n");
	return new_clone(45);
}

page22()
{
	printf("You are searching Goods Distribution Hall 7-beta.\n");
	switch(dice_roll(1,4))
	{
		case 1:	return 18;
		case 2: return 15;
		case 3: return 18;
		case 4: return 29;
	}
}

page23()
{
	printf("You go to the nearest computer terminal and declare yourself a mutant.\n");
	printf("\"A mutant, he\'s a mutant,\" yells a previously unnoticed infrared who had\n");
	printf("been looking over your shoulder.  You easily gun him down, but not before a\n");
	printf("dozen more citizens take notice and aim their weapons at you.\n");
	return choose(28,"You tell them that it was really only a bad joke",24,"You want to fight it out, one against twelve");
}

page24()
{
	printf("Golly, I never expected someone to pick this.  I haven\'t even designed\n");
	printf("the 12 citizens who are going to make a sponge out of you.  Tell you what,\n");
	printf("I\'ll give you a second chance.\n");
	return choose(28,"You change your mind and say it was only a bad joke",25,"You REALLY want to shoot it out");
}

page25()
{
	printf("Boy, you really can\'t take a hint!\n");
	printf("They\'re closing in.  Their trigger fingers are twitching, they\'re about to\n");
	printf("shoot.  This is your last chance.\n");
	return choose(28,"You tell them it was all just a bad joke",26,"You are going to shoot");
}

page26()
{
	printf("You can read the cold, sober hatred in their eyes (They really didn\'t think\n");
	printf("it was funny), as they tighten the circle around you.  One of them shoves a\n");
	printf("blaster up your nose, but that doesn\'t hurt as much as the multi-gigawatt\n");
	printf("carbonium tipped food drill in the small of your back.\n");
	printf("You spend the remaining micro-seconds of your life wondering what you did wrong\n");
	return new_clone(32);
}

page27()
{
	// doesn't exist.  Can't happen with computer version.
	//   designed to catch dice cheats
}

page28()
{
	printf("They don\'t think it\'s funny.\n");
	return 26;
}

page29()
{
	printf("\"Psst, hey citizen, come here.  Pssfft,\" you hear.  When you peer around\n");
	printf("you can see someone\'s dim outline in the shadows.  \"I got some information\n");
	printf("on the Master Retailer.  It\'ll only cost you 30 psst credits.\"\n");
	printf("\nSelect \'a\', \'b\' or \'c\' :\n");
	printf(" a - You pay the 30 credits for the info.\n");
	printf(" b - You would rather threaten him for the information.\n");
	printf(" c - You ignore him and walk away.\n");
	switch(get_char())
	{
		case 'a' : return 30;
		case 'b' : return 31;
		case 'c' :
		default  : return 22;
	}
}

page30()
{
	printf("You step into the shadows and offer the man a thirty credit bill.  \"Just drop\n");
	printf("it on the floor,\" he says.  \"So you\'re looking for the Master Retailer, pssfft?\n");
	printf("I\'ve seen him, he\'s a fat man in a fuzzy red and white jump suit.  They say\n");
	printf("he\'s a high programmer with no respect for proper security.  If you want to\n");
	printf("find him then pssfft step behind me and go through the door.\"\n");
	printf("Behind the man is a reinforced plasteel blast door.  The centre of it has been\n");
	printf("buckled toward you in a manner you only saw once before when you were field\n");
	printf("testing the rocket assist plasma slingshot (you found it easily portable but\n");
	printf("prone to misfire).  Luckily it isn\'t buckled too far for you to make out the\n");
	printf("warning sign.  WARNING!! Don\'t open this door or the same thing will happen to\n");
	printf("you.  Opening this door is a capital offense.  Do not do it.  Not at all. This\n");
	printf("is not a joke.\n");
	printf("\nSelect \'a\', \'b\' or \'c\' :\n");
	printf(" a - You use your Precognition mutant power on opening the door.\n");
	printf(" b - You just go through the door anyway.\n");
	printf(" c - You decide it\'s too dangerous and walk away.\n");
	switch(get_char())
	{
		case 'a' : return 56;
		case 'b' : return 33;
		case 'c' :
		default  : return 22;
	}
}

page31()
{
	printf("Like any good troubleshooter you make the least expensive decision and threaten\n");
	printf("him for information.  With lightning like reflexes you whip out your laser and\n");
	printf("stick it up his nose.  \"Talk, you traitorous Christmas celebrator, or who nose\n");
	printf("what will happen to you, yuk yuk,\" you pun menacingly, and then you notice\n");
	printf("something is very wrong.  He doesn\'t have a nose.  As a matter of fact he\'s\n");
	printf("made of one eighth inch cardboard and your laser is sticking through the other\n");
	printf("side of his head.  \"Are you going to pay?\" says his mouth speaker,\n");
	printf("\"or are you going to pssfft go away stupid?\"\n");
	return choose(30,"You pay the 30 credits",22,"You pssfft go away stupid");
}

page32()
{
	printf("Finally it\'s your big chance to prove that you\'re as good a troubleshooter\n");
	printf("as your previous clone.  You walk briskly to mission briefing and pick up your\n");
	printf("previous clone\'s personal effects and notepad.  After reviewing the notes you\n");
	printf("know what has to be done.  You catch the purple line to Goods Distribution Hall\n");
	printf("7-beta and begin to search for the blast door.\n");
	return 22;
}

page33()
{
	blast_door=1;
	printf("You release the megabolts on the blast door, then strain against it with your\n");
	printf("awesome strength.  Slowly the door creaks open.  You bravely leap through the\n");
	printf("opening and smack your head into the barrel of a 300 mm \'ultra shock\' class\n");
	printf("plasma cannon.  It\'s dark in the barrel now, but just before your head got\n");
	printf("stuck you can remember seeing a group of technicians anxiously watch you leap\n");
	printf("into the room.\n");
	if (ultra_violet==1)	return 35;
	else			return 36;
}

page34()
{
	printf("You have found a sealed envelope on the body.  You open it and read:\n");
	printf("\"WARNING: Ultraviolet Clearance ONLY.  DO NOT READ.\n");
	printf("Memo from Chico-U-MRX4 to Harpo-U-MRX5.\n");
	printf("The planned takeover of the Troubleshooter Training Course goes well, Comrade.\n");
	printf("Once we have trained the unwitting bourgeois troubleshooters to work as\n");
	printf("communist dupes, the overthrow of Alpha Complex will be unstoppable.  My survey\n");
	printf("of the complex has convinced me that no one suspects a thing; soon it will be\n");
	printf("too late for them to oppose the revolution.  The only thing that could possibly\n");
	printf("impede the people\'s revolution would be someone alerting The Computer to our\n");
	printf("plans (for instance, some enterprising Troubleshooter could tell The Computer\n");
	printf("that the communists have liberated the Troubleshooter Training Course and plan\n");
	printf("to use it as a jumping off point from which to undermine the stability of all\n");
	printf("Alpha Complex), but as we both know, the capitalistic Troubleshooters would\n");
	printf("never serve the interests of the proletariat above their own bourgeois desires.\n");
	printf("P.S. I\'m doing some Christmas shopping later today.  Would you like me to pick\n");
	printf("you up something?\"\n");
	more();
	printf("When you put down the memo you are overcome by that strange deja\'vu again.\n");
	printf("You see yourself talking privately with The Computer.  You are telling it all\n");
	printf("about the communists\' plan, and then the scene shifts and you see yourself\n");
	printf("showered with awards for foiling the insidious communist plot to take over the\n");
	printf("complex.\n");
	read_letter=1;
	return choose(46,"You rush off to the nearest computer terminal to expose the commies",22,"You wander off to look for more evidence");
}

page35()
{
	printf("\"Oh master,\" you hear through the gun barrel, \"where have you been? It is\n");
	printf("time for the great Christmas gifting ceremony.  You had better hurry and get\n");
	printf("the costume on or the trainee may begin to suspect.\"  For the second time\n");
	printf("today you are forced to wear attire not of your own choosing.  They zip the\n");
	printf("suit to your chin just as you hear gunfire erupt behind you.\n");
	printf("\"Oh no! Who left the door open?  The commies will get in.  Quick, fire the\n");
	printf("laser cannon or we\'re all doomed.\"\n");
	printf("\"Too late you capitalist swine, the people\'s revolutionary strike force claims\n");
	printf("this cannon for the proletariat\'s valiant struggle against oppression.  Take\n");
	printf("that, you running dog imperialist lackey.  ZAP, KAPOW\"\n");
	printf("Just when you think that things couldn\'t get worse, \"Aha, look what we have\n");
	printf("here, the Master Retailer himself with his head caught in his own cannon.  His\n");
	printf("death will serve as a symbol of freedom for all Alpha Complex.\n");
	printf("Fire the cannon.\"\n");
	return new_clone(32);
}

page36()
{
	printf("\"Congratulations, troubleshooter, you have successfully found the lair of the\n");
	printf("Master Retailer and completed the Troubleshooter Training Course test mission,\"\n");
	printf("a muffled voice tells you through the barrel.  \"Once we dislodge your head\n");
	printf("from the barrel of the \'Ultra Shock\' plasma cannon you can begin with the\n");
	printf("training seminars, the first of which will concern the 100%% accurate\n");
	printf("identification and elimination of unregistered mutants.  If you have any\n");
	printf("objections please voice them now.\"\n");
	printf("\nSelect \'a\', \'b\' or \'c\' :\n");
	printf(" a - You appreciate his courtesy and voice an objection.\n");
	printf(" b - After your head is removed from the cannon, you register as a mutant.\n");
	printf(" c - After your head is removed from the cannon, you go to the unregistered\n");
	printf("     mutant identification and elimination seminar.\n");
	switch(get_char())
	{
		case 'a' : return new_clone(32);
		case 'b' : return 23;
		case 'c' :
		default  : return 37;
	}
}

page37()
{
	printf("\"Come with me please, Troubleshooter,\" says the Green clearance technician\n");
	printf("after he has dislodged your head from the cannon.  \"You have been participating\n");
	printf("in the Troubleshooter Training Course since you got off the tube car in\n");
	printf("GDH7-beta,\" he explains as he leads you down a corridor.  \"The entire\n");
	printf("Christmas assignment was a test mission to assess your current level of\n");
	printf("training.  You didn\'t do so well.  We\'re going to start at the beginning with\n");
	printf("the other student.  Ah, here we are, the mutant identification and elimination\n");
	printf("lecture.\"  He shows you into a vast lecture hall filled with empty seats.\n");
	printf("There is only one other student here, a Troubleshooter near the front row\n");
	printf("playing with his Action Troubleshooter(tm) figure.  \"Find a seat and I will\n");
	printf("begin,\" says the instructor.\n");
	return 38;
}

page38()
{
	printf("\"I am Plato-B-PHI%d, head of mutant propaganda here at the training course.\n",plato_clone);
	printf("If you have any questions about mutants please come to me.  Today I will be\n");
	printf("talking about mutant detection.  Detecting mutants is very easy.  One simply\n");
	printf("watches for certain tell tale signs, such as the green scaly skin, the third\n");
	printf("arm growing from the forehead, or other similar disfigurements so common with\n");
	printf("their kind.  There are, however, a few rare specimens that show no outward sign\n");
	printf("of their treason.  This has been a significant problem, so our researchers have\n");
	printf("been working on a solution.  I would like a volunteer to test this device,\"\n");
	printf("he says, holding up a ray gun looking thing.  \"It is a mutant detection ray.\n");
	printf("This little button detects for mutants, and this big button stuns them once\n");
	printf("they are discovered.  Who would like to volunteer for a test?\"\n");
	printf("The Troubleshooter down the front squirms deeper into his chair.\n");
	return choose(39,"You volunteer for the test",40,"You duck behind a chair and hope the instructor doesn\'t notice you");
}

page39()
{
	printf("You bravely volunteer to test the mutant detection gun.  You stand up and walk\n");
	printf("down the steps to the podium, passing a very relieved Troubleshooter along the\n");
	printf("way.  When you reach the podium Plato-B-PHI hands you the mutant detection gun\n");
	printf("and says, \"Here, aim the gun at that Troubleshooter and push the small button.\n");
	printf("If you see a purple light, stun him.\"  Grasping the opportunity to prove your\n");
	printf("worth to The Computer, you fire the mutant detection ray at the Troubleshooter.\n");
	printf("A brilliant purple nimbus instantly surrounds his body.  You slip your finger\n");
	printf("to the large stun button and he falls writhing to the floor.\n");
	printf("\"Good shot,\" says the instructor as you hand him the mutant detection gun,\n");
	printf("\"I\'ll see that you get a commendation for this.  It seems you have the hang\n");
	printf("of mutant detection and elimination.  You can go on to the secret society\n");
	printf("infiltration class.  I\'ll see that the little mutie gets packaged for\n");
	printf("tomorrow\'s mutant dissection class.\"\n");
	return 41;
}

page40()
{
	printf("You breathe a sigh of relief as Plato-B-PHI picks on the other Troubleshooter.\n");
	printf("\"You down here in the front,\" says the instructor pointing at the other\n");
	printf("Troubleshooter, \"you\'ll make a good volunteer.  Please step forward.\"\n");
	printf("The Troubleshooter looks around with a \`who me?\' expression on his face, but\n");
	printf("since he is the only one visible in the audience he figures his number is up.\n");
	printf("He walks down to the podium clutching his Action Troubleshooter(tm) doll before\n");
	printf("him like a weapon.  \"Here,\" says Plato-B-PHI, \"take the mutant detection ray\n");
	printf("and point it at the audience.  If there are any mutants out there we\'ll know\n");
	printf("soon enough.\"  Suddenly your skin prickles with static electricity as a bright\n");
	printf("purple nimbus surrounds your body.  \"Ha Ha, got one,\" says the instructor.\n");
	printf("\"Stun him before he gets away.\"\n");
	more();
	while(1)
	{
		if (dice_roll(1,100)<=30)
		{
			printf("His shot hits you.  You feel numb all over.\n");
			return 49;
		}
		else	printf("His shot just missed.\n");

		if (dice_roll(1,100)<=40)
		{
			printf("You just blew his head off.  His lifeless hand drops the mutant detector ray.\n");
			return 50;
		}
		else	printf("You burnt a hole in the podium.  He sights the mutant detector ray on you.\n");
	}
}

page41()
{
	printf("You stumble down the hallway of the Troubleshooter Training Course looking for\n");
	printf("your next class.  Up ahead you see one of the instructors waving to you.  When\n");
	printf("you get there he shakes your hand and says, \"I am Jung-I-PSY.  Welcome to the\n");
	printf("secret society infiltration seminar.  I hope you ...\"  You don\'t catch the\n");
	printf("rest of his greeting because you\'re paying too much attention to his handshake;\n");
	printf("it is the strangest thing that has ever been done to your hand, sort of how it\n");
	printf("would feel if you put a neuro whip in a high energy palm massage unit.\n");
	printf("It doesn\'t take you long to learn what he is up to; you feel him briefly shake\n");
	printf("your hand with the secret Illuminati handshake.\n");
	return choose(42,"You respond with the proper Illuminati code phrase, \"Ewige Blumenkraft\"",43,"You ignore this secret society contact");
}

page42()
{
	printf("\"Aha, so you are a member of the elitist Illuminati secret society,\" he says\n");
	printf("loudly, \"that is most interesting.\"  He turns to the large class already\n");
	printf("seated in the auditorium and says, \"You see, class, by simply using the correct\n");
	printf("hand shake you can identify the member of any secret society.  Please keep your\n");
	printf("weapons trained on him while I call a guard.\n");
	return choose(51,"You run for it",52,"You wait for the guard");
}

page43()
{
	printf("You sit through a long lecture on how to recognise and infiltrate secret\n");
	printf("societies, with an emphasis on mimicking secret handshakes.  The basic theory,\n");
	printf("which you realise to be sound from your Iluminati training, is that with the\n");
	printf("proper handshake you can pass unnoticed in any secret society gathering.\n");
	printf("What\'s more, the proper handshake will open doors faster than an \'ultra shock\'\n");
	printf("plasma cannon.  You are certain that with the information you learn here you\n");
	printf("will easily be promoted to the next level of your Illuminati secret society.\n");
	printf("The lecture continues for three hours, during which you have the opportunity\n");
	printf("to practice many different handshakes.  Afterwards everyone is directed to\n");
	printf("attend the graduation ceremony.  Before you must go you have a little time to\n");
	printf("talk to The Computer about, you know, certain topics.\n");
	return choose(44,"You go looking for a computer terminal",55,"You go to the graduation ceremony immediately");
}

page44()
{
	printf("You walk down to a semi-secluded part of the training course complex and\n");
	printf("activate a computer terminal.  \"AT YOUR SERVICE\" reads the computer screen.\n");
	if (read_letter==0)	return choose(23,"You register yourself as a mutant",55,"You change your mind and go to the graduation ceremony");
	printf("\nSelect \'a\', \'b\' or \'c\' :\n");
	printf(" a - You register yourself as a mutant.\n");
	printf(" b - You want to chat about the commies.\n");
	printf(" c - You change your mind and go to the graduation ceremony.\n");
	switch(get_char())
	{
		case 'a' : return 23;
		case 'b' : return 46;
		case 'c' :
		default  : return 55;
	}
}

page45()
{
	printf("\"Hrank Hrank,\" snorts the alarm in your living quarters.  Something is up.\n");
	printf("You look at the monitor above the bathroom mirror and see the message you have\n");
	printf("been waiting for all these years.  \"ATTENTION TROUBLESHOOTER, YOU ARE BEING\n");
	printf("ACTIVATED. PLEASE REPORT IMMEDIATELY TO MISSION ASSIGNMENT ROOM A17/GAMMA/LB22.\n");
	printf("THANK YOU. THE COMPUTER IS YOUR FRIEND.\"  When you arrive at mission\n");
	printf("assignment room A17-gamma/LB22 you are given your previous clone\'s\n");
	printf("remaining possessions and notebook.  You puzzle through your predecessor\'s\n");
	printf("cryptic notes, managing to decipher enough to lead you to the tube station and\n");
	printf("the tube car to GDH7-beta.\n");
	return 10;
}

page46()
{
	printf("\"Why do you ask about the communists, Troubleshooter?  It is not in the\n");
	printf("interest of your continued survival to be asking about such topics,\" says\n");
	printf("The Computer.\n");
	return choose(53,"You insist on talking about the communists",54,"You change the subject");
}

page47()
{
	printf("The Computer orders the entire Vulture squadron to terminate the Troubleshooter\n");
	printf("Training Course.  Unfortunately you too are terminated for possessing\n");
	printf("classified information.\n\n");
	printf("Don\'t act so innocent, we both know that you are an Illuminatus which is in\n");
	printf("itself an act of treason.\n\n");
	printf("Don\'t look to me for sympathy.\n\n");
	printf("			THE END\n");
	return 0;
}

page48()
{
	printf("The tubecar shoots forward as you enter, slamming you back into a pile of\n");
	printf("garbage.  The front end rotates upward and you, the garbage and the garbage\n");
	printf("disposal car shoot straight up out of Alpha Complex.  One of the last things\n");
	printf("you see is a small blue sphere slowly dwindling behind you.  After you fail to\n");
	printf("report in, you will be assumed dead.\n");
	return new_clone(45);
}

page49()
{
	printf("The instructor drags your inert body into a specimen detainment cage.\n");
	printf("\"He\'ll make a good subject for tomorrow\'s mutant dissection class,\" you hear.\n");
	return new_clone(32);
}

page50()
{
	printf("You put down the other Troubleshooter, and then wisely decide to drill a few\n");
	printf("holes in the instructor as well; the only good witness is a dead witness.\n");
	printf("You continue with the training course.\n");
	plato_clone++;
	return 41;
}

page51()
{
	printf("You run for it, but you don\'t run far.  Three hundred strange and exotic\n");
	printf("weapons turn you into a freeze dried cloud of soot.\n");
	return new_clone(32);
}

page52()
{
	printf("You wisely wait until the instructor returns with a Blue Internal Security\n");
	printf("guard.  The guard leads you to an Internal Security self incrimination station.\n");
	return 2;
}

page53()
{
	printf("You tell The Computer about:\n");
	return choose(47,"The commies who have infiltrated the Troubleshooter Training Course\n     and the impending People\'s Revolution",54,"Something less dangerous");
}

page54()
{
	printf("\"Do not try to change the subject, Troubleshooter,\" says The Computer.\n");
	printf("\"It is a serious crime to ask about the communists.  You will be terminated\n");
	printf("immediately.  Thank you for your inquiry.  The Computer is your friend.\"\n");
	printf("Steel bars drop to your left and right, trapping you here in the hallway.\n");
	printf("A spotlight beams from the computer console to brilliantly iiluminate you while\n");
	printf("the speaker above your head rapidly repeats \"Traitor, Traitor, Traitor.\"\n");
	printf("It doesn\'t take long for a few guards to notice your predicament and come to\n");
	printf("finish you off.\n");
	if (blast_door==0) return new_clone(45);
	else		   return new_clone(32);
}

page55()
{
	printf("You and 300 other excited graduates are marched  from the lecture hall and into\n");
	printf("a large auditorium for the graduation exercise.  The auditorium is\n");
	printf("extravagantly decorated in the colours of the graduating class.  Great red and\n");
	printf("green plasti-paper ribbons drape from the walls, while a huge sign reading\n");
	printf("\"Congratulations class of GDH7-beta-203.44/A\" hangs from the raised stage down\n");
	printf("front.  Once everyone finds a seat the ceremony begins.  Jung-I-PSY is the\n");
	printf("first to speak, \"Congratulations students, you have successfully survived the\n");
	printf("Troubleshooter Training Course.  It always brings me great pride to address\n");
	printf("the graduating class, for I know, as I am sure you do too, that you are now\n");
	printf("qualified for the most perilous missions The Computer may select for you.  The\n");
	printf("thanks is not owed to us of the teaching staff, but to all of you, who have\n");
	printf("persevered and graduated.  Good luck and die trying.\"  Then the instructor\n");
	printf("begins reading the names of the students who one by one walk to the front of\n");
	printf("the auditorium and receive their diplomas.  Soon it is your turn,\n");
	printf("\"Philo-R-DMD, graduating a master of mutant identification and secret society\n");
	printf("infiltration.\"  You walk up and receive your diploma from Plato-B-PHI%d, then\n",plato_clone);
	printf("return to your seat.  There is another speech after the diplomas are handed\n");
	printf("out, but it is cut short by by rapid fire laser bursts from the high spirited\n");
	printf("graduating class.  You are free to return to your barracks to wait, trained\n");
	printf("and fully qualified, for your next mission.  You also get that cherished\n");
	printf("promotion from the Illuminati secret society.  In a week you receive a\n");
	printf("detailed Training Course bill totalling 1,523 credits.\n");
	printf("			THE END\n");
	return 0;
}

page56()
{
	printf("That familiar strange feeling of deja\'vu envelops you again.  It is hard to\n");
	printf("say, but whatever is on the other side of the door does not seem to be intended\n");
	printf("for you.\n");
	return choose(33,"You open the door and step through",22,"You go looking for more information");
}

*/

// GetChar reads a line of input and returns the first byte from that input.
// This is plain ASCII handling, and UTF-8 encoded input is not supported.
// It returns '\n' for lines of empty input.
func GetChar() byte {
	text, err := input.ReadString('\n')
	if err != nil {
		log.Fatalf("input failure: %v", err)
	}

	if len(text) == 0 || text[0] == '\r' {
		return '\n'
	}
	return text[0]
}

func main() {
	s := NewState()
	s.Instructions()
	s.More()
	s.Character()
	s.More()

	for s.Page = s.Exec(); s.Page != 0; s.Page = s.Exec() {
	}
}