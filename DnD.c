/* Thomas Anchor
 * COP 3223H MWF 9:30-10:20
 * Final Project - using user input to generate a D&D character
 */

 //included libraries
 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #include <math.h>
 #include <string.h>

 //constants
 #define STR 0  //Defining a constant for each ability for readability when using arrays.
 #define DEX 1  //These abilities will always be in the same order, so STR will always be in [0] of an array
 #define CON 2
 #define INT 3
 #define WIS 4
 #define CHA 5

 //struct definitions
 struct charClass{ //This will hold the information for the playable classes
     char className[20];
     int classNum;

     int numProf; //The number of skills a player can gain proficiency in

     char hitDice[5];
     int hpBase; //First level hp will be calculated with this and the con modifier

     char armorTypes[4][20]; //= {"light armor", "medium armor", "heavy armor", "shields"};
     int armorProf[4]; //Armor proficiencies.  Corresponds with armor types above

     char weaponTypes[2][20];// = {"simple weapons", "martial weapons"}; //for simplicity's sake I am keeping it to just these two types of proficiencies
     int weaponProf[2];                                                  //rather than individual weapons

     int spellsKnown;
     int cantrips;
     int spellSlots;
 };

 struct race{ //This will hold the information for the playable races
     char raceName[20];
     int bonuses[6];
     char languages[3][20];
     int numLang;
     char size[10];
     int speed;
 };

 struct character{  //This will hold the information for the player
     char name[21];

     struct charClass playerClass;

     char abilityName[6][4];
     int abilities[6];
     int abilitiesPlaced[6];
     int abilityModifiers[6];

     char skillNames[18][20];
     int skillProf[18];
     int skillScore[18]; //both skillProf and skillScore correspond with the skills in the order listed above
                         //The player will be able to pick any skill for any class, again for simplicity's sake

     struct race playerRace;

     int level;
     int profBonus;
     int hp;
 };

 //function prototypes
 void initArrays(struct character *player);
 void chooseClass(struct character *player);
 void chooseRace(struct character *player);
 int rollDice();
 void placeStats(int roll, struct character *player);
 void raceBonuses(struct character *player);
 void calcModifiers(struct character *player);
 void waitFor(int sec);
 void pickSkills(int num, struct character *player);
 void calcSkills(struct character *player);
 void printToFile(struct character player);
 void printToScreen(struct character player);

 //main
 int main(){
    struct character player;  //create player structure and initialize several beginning values
    initArrays(&player);
    player.level = 1;
    player.profBonus = 2;

    //variables
    int i, rolls[6];

    //get player name
    printf("What would you like your character's name to be? (1 word, max 20 characters)\n");
    scanf("%s", player.name);

    printf("\nYour character's name is %s.\n\n", player.name);

    //get player race
    chooseRace(&player);
    printf("\nYour character will be %s.\n", player.playerRace.raceName);

    //get player class
    chooseClass(&player);
    printf("\nYou will be playing as a %s.\n\n", player.playerClass.className);


    //roll and assign player abilities
    printf("\nIt's time to roll and assign abilities.\n  Your rolls are:\n");
    for (i = 0; i < 6; i++){
        rolls[i] = rollDice();
        printf("%d\n", rolls[i]);
        waitFor(1);
    }

    for (i = 0; i < 6; i++){ //initialize abilities placed array to 0 (no abilities placed)
        player.abilitiesPlaced[i] = 0;
    }

    for (i = 0; i < 6; i++){
        placeStats(rolls[i], &player);
    }

    //Apply race bonuses
    raceBonuses(&player);

    printf("\n\nYour abilities (including race bonuses) are, in order of STR, DEX, CON, INT, WIS, CHA: \n");
    for (i = 0; i < 5; i++){
        printf("%d, ", player.abilities[i]);
    }
    printf("and %d.\n", player.abilities[5]);

    //Calculate ability modifiers
    calcModifiers(&player);

    printf("\n\nYour modifiers (including race bonuses) are, in order of STR, DEX, CON, INT, WIS, CHA: \n");
    for (i = 0; i < 5; i++){
        printf("%d, ", player.abilityModifiers[i]);
    }
    printf("and %d.\n", player.abilityModifiers[5]);

    //Calculate HP
    player.hp = player.playerClass.hpBase + player.abilityModifiers[CON];

    //Skill proficiencies
    printf("Because of your chosen class, you can pick %d skills to gain proficiency in.\n", player.playerClass.numProf);
    pickSkills(player.playerClass.numProf, &player);

    printf("\nYou are proficient in: \n");
    for (i = 0; i < 18; i++){
        if (player.skillProf[i] == 1){
            printf("%s\n", player.skillNames[i]);
        }
    }

    calcSkills(&player);

    //print character info to a file and the screen
    printToFile(player);
    printf("\nYour character:\n\n");
    printToScreen(player);
    printf("\n\nAll of your character's info has also been saved to a file called %s.txt", player.name);

    return 0;
 }

 //initializes the various arrays used in structures
 //pre-conditions: player must be initialized in main
 //post-conditions: arrays in structs will be set to initial values
 void initArrays(struct character *player){
    char abilityName[6][4] = {"STR", "DEX", "CON", "INT", "WIS", "CHA"};
    char skillNames[18][20] = {"athletics", "acrobatics", "sleight of hand", "stealth", "arcana", "history", "investigation", "nature", "religion",
                                "animal handling", "insight", "medicine", "perception", "survival", "deception", "intimidation",
                                "performance", "persuasion"};
    int i;

    for (i = 0; i < 6; i++){
        strcpy(player->abilityName[i], abilityName[i]);
    }

    for (i = 0; i < 18; i++){
        strcpy(player->skillNames[i], skillNames[i]);
        player->skillProf[i] = 0;
    }

 }

 //Allows the user to choose a class
 //Pre-conditions: must have a character struct passed by reference
 //Post-conditions: will assign a class, and all the relevant features of that class, to the player
 void chooseClass(struct character *player){
     //This function still needs to be finished; however, the final function will be very similar to chooseRace()

    int choice, valid = 0;
    //classes

    //fighter
    struct charClass fighter;
        strcpy(fighter.className, "fighter");
        strcpy(fighter.hitDice, "1d10");
        fighter.hpBase = 10;
        fighter.classNum = 1;
        fighter.weaponProf[0] = 1; //simple weapons
        fighter.weaponProf[1] = 1; //martial weapons
        fighter.armorProf[0] = 1; //light armor
        fighter.armorProf[1] = 1; //medium armor
        fighter.armorProf[2] = 1; //heavy armor
        fighter.armorProf[3] = 1; //shields
        fighter.numProf = 2;
        fighter.spellsKnown = 0;
        fighter.spellSlots = 0;
        fighter.cantrips = 0;

    //barbarian
    struct charClass barbarian;
        strcpy(barbarian.className, "barbarian");
        strcpy(barbarian.hitDice, "1d12");
        barbarian.hpBase = 12;
        barbarian.classNum = 2;
        barbarian.weaponProf[0] = 1; //simple weapons
        barbarian.weaponProf[1] = 1; //martial weapons
        barbarian.armorProf[0] = 1; //light armor
        barbarian.armorProf[1] = 1; //medium armor
        barbarian.armorProf[2] = 0; //heavy armor
        barbarian.armorProf[3] = 1; //shields
        barbarian.numProf = 2;
        barbarian.spellsKnown = 0;
        barbarian.spellSlots = 0;
        barbarian.cantrips = 0;

    //rogue
    struct charClass rogue;
        strcpy(rogue.className, "rogue");
        strcpy(rogue.hitDice, "1d8");
        rogue.hpBase = 8;
        rogue.classNum = 3;
        rogue.weaponProf[0] = 1; //simple weapons
        rogue.weaponProf[1] = 0; //martial weapons
        rogue.armorProf[0] = 1; //light armor
        rogue.armorProf[1] = 0; //medium armor
        rogue.armorProf[2] = 0; //heavy armor
        rogue.armorProf[3] = 0; //shields
        rogue.numProf = 4;
        rogue.spellsKnown = 0;
        rogue.spellSlots = 0;
        rogue.cantrips = 0;

    //ranger
    struct charClass ranger;
        strcpy(ranger.className, "ranger");
        strcpy(ranger.hitDice, "1d10");
        ranger.hpBase = 10;
        ranger.classNum = 4;
        ranger.weaponProf[0] = 1; //simple weapons
        ranger.weaponProf[1] = 1; //martial weapons
        ranger.armorProf[0] = 1; //light armor
        ranger.armorProf[1] = 1; //medium armor
        ranger.armorProf[2] = 0; //heavy armor
        ranger.armorProf[3] = 1; //shields
        ranger.numProf = 3;
        ranger.spellsKnown = 0;
        ranger.spellSlots = 0;
        ranger.cantrips = 0;

    //paladin
    struct charClass paladin;
        strcpy(paladin.className, "paladin");
        strcpy(paladin.hitDice, "1d10");
        paladin.hpBase = 10;
        paladin.classNum = 5;
        paladin.weaponProf[0] = 1; //simple weapons
        paladin.weaponProf[1] = 1; //martial weapons
        paladin.armorProf[0] = 1; //light armor
        paladin.armorProf[1] = 1; //medium armor
        paladin.armorProf[2] = 1; //heavy armor
        paladin.armorProf[3] = 1; //shields
        paladin.numProf = 2;
        paladin.spellsKnown = 0;
        paladin.spellSlots = 0;
        paladin.cantrips = 0;

    //monk
    struct charClass monk;
        strcpy(monk.className, "monk");
        strcpy(monk.hitDice, "1d8");
        monk.hpBase = 8;
        monk.classNum = 6;
        monk.weaponProf[0] = 1; //simple weapons
        monk.weaponProf[1] = 0; //martial weapons
        monk.armorProf[0] = 0; //light armor
        monk.armorProf[1] = 0; //medium armor
        monk.armorProf[2] = 0; //heavy armor
        monk.armorProf[3] = 0; //shields
        monk.numProf = 2;
        monk.spellsKnown = 0;
        monk.spellSlots = 0;
        monk.cantrips = 0;

    //cleric
    struct charClass cleric;
        strcpy(cleric.className, "cleric");
        strcpy(cleric.hitDice, "1d8");
        cleric.hpBase = 8;
        cleric.classNum = 7;
        cleric.weaponProf[0] = 1; //simple weapons
        cleric.weaponProf[1] = 0; //martial weapons
        cleric.armorProf[0] = 1; //light armor
        cleric.armorProf[1] = 1; //medium armor
        cleric.armorProf[2] = 0; //heavy armor
        cleric.armorProf[3] = 1; //shields
        cleric.numProf = 2;
        cleric.spellsKnown = 0;
        cleric.spellSlots = 2;
        cleric.cantrips = 3;

    //bard
    struct charClass bard;
        strcpy(bard.className, "bard");
        strcpy(bard.hitDice, "1d8");
        bard.hpBase = 8;
        bard.classNum = 8;
        bard.weaponProf[0] = 1; //simple weapons
        bard.weaponProf[1] = 0; //martial weapons
        bard.armorProf[0] = 1; //light armor
        bard.armorProf[1] = 0; //medium armor
        bard.armorProf[2] = 0; //heavy armor
        bard.armorProf[3] = 0; //shields
        bard.numProf = 3;
        bard.spellsKnown = 4;
        bard.spellSlots = 2;
        bard.cantrips = 2;

    //druid
    struct charClass druid;
        strcpy(druid.className, "druid");
        strcpy(druid.hitDice, "1d8");
        druid.hpBase = 8;
        druid.classNum = 9;
        druid.weaponProf[0] = 1; //simple weapons
        druid.weaponProf[1] = 1; //martial weapons
        druid.armorProf[0] = 1; //light armor
        druid.armorProf[1] = 1; //medium armor
        druid.armorProf[2] = 0; //heavy armor
        druid.armorProf[3] = 1; //shields
        druid.numProf = 2;
        druid.spellsKnown = 0;
        druid.spellSlots = 2;
        druid.cantrips = 2;

    //wizard
    struct charClass wizard;
        strcpy(wizard.className, "wizard");
        strcpy(wizard.hitDice, "1d6");
        wizard.hpBase = 6;
        wizard.classNum = 10;
        wizard.weaponProf[0] = 1; //simple weapons
        wizard.weaponProf[1] = 0; //martial weapons
        wizard.armorProf[0] = 0; //light armor
        wizard.armorProf[1] = 0; //medium armor
        wizard.armorProf[2] = 0; //heavy armor
        wizard.armorProf[3] = 0; //shields
        wizard.numProf = 2;
        wizard.spellsKnown = 0;
        wizard.spellSlots = 2;
        wizard.cantrips = 3;

    //sorcerer
    struct charClass sorcerer;
        strcpy(sorcerer.className, "sorcerer");
        strcpy(sorcerer.hitDice, "1d6");
        sorcerer.hpBase = 6;
        sorcerer.classNum = 11;
        sorcerer.weaponProf[0] = 1; //simple weapons
        sorcerer.weaponProf[1] = 0; //martial weapons
        sorcerer.armorProf[0] = 0; //light armor
        sorcerer.armorProf[1] = 0; //medium armor
        sorcerer.armorProf[2] = 0; //heavy armor
        sorcerer.armorProf[3] = 0; //shields
        sorcerer.numProf = 2;
        sorcerer.spellsKnown = 2;
        sorcerer.spellSlots = 2;
        sorcerer.cantrips = 4;

    //warlock
    struct charClass warlock;
        strcpy(warlock.className, "warlock");
        strcpy(warlock.hitDice, "1d8");
        warlock.hpBase = 8;
        warlock.classNum = 12;
        warlock.weaponProf[0] = 1; //simple weapons
        warlock.weaponProf[1] = 0; //martial weapons
        warlock.armorProf[0] = 1; //light armor
        warlock.armorProf[1] = 0; //medium armor
        warlock.armorProf[2] = 0; //heavy armor
        warlock.armorProf[3] = 0; //shields
        warlock.numProf = 2;
        warlock.spellsKnown = 2;
        warlock.spellSlots = 1;
        warlock.cantrips = 2;

    //Menu for class choice
    while (valid == 0){
        printf("Choose a class:\n");
        printf("\t1. Fighter\n");
        printf("\t2. Barbarian\n");
        printf("\t3. Rogue\n");
        printf("\t4. Ranger\n");
        printf("\t5. Paladin\n");
        printf("\t6. Monk\n");
        printf("\t7. Cleric\n");
        printf("\t8. Bard\n");
        printf("\t9. Druid\n");
        printf("\t10. Wizard\n");
        printf("\t11. Sorcerer\n");
        printf("\t12. Warlock\n");
        scanf("%d", &choice);

        switch(choice){
        case 1:
            player->playerClass = fighter;
            valid = 1;
            break;
        case 2:
            player->playerClass = barbarian;
            valid = 1;
            break;
        case 3:
            player->playerClass = rogue;
            valid = 1;
            break;
        case 4:
            player->playerClass = ranger;
            valid = 1;
            break;
        case 5:
            player->playerClass = paladin;
            valid = 1;
            break;
        case 6:
            player->playerClass = monk;
            valid = 1;
            break;
        case 7:
            player->playerClass = cleric;
            valid = 1;
            break;
        case 8:
            player->playerClass = bard;
            valid = 1;
            break;
        case 9:
            player->playerClass = druid;
            valid = 1;
            break;
        case 10:
            player->playerClass = wizard;
            valid = 1;
            break;
        case 11:
            player->playerClass = sorcerer;
            valid = 1;
            break;
        case 12:
            player->playerClass = warlock;
            valid = 1;
            break;
        }

    }

    //Name armor and weapon types, used for output later
    strcpy(player->playerClass.armorTypes[0], "light armor");
    strcpy(player->playerClass.armorTypes[1], "medium armor");
    strcpy(player->playerClass.armorTypes[2], "heavy armor");
    strcpy(player->playerClass.armorTypes[3], "shields");
    strcpy(player->playerClass.weaponTypes[0], "simple weapons");
    strcpy(player->playerClass.weaponTypes[1], "martial weapons");

    return;
 }

 //Allows the user to choose a race
 //Pre-conditions: must have a character struct passed by reference
 //Post-conditions: will assign a race and all relevant features to the player
 void chooseRace(struct character *player){
    int i, choice, valid = 0, validStat = 0, abilityChoice1, abilityChoice2;

    //races

    //human
    struct race human;
        strcpy(human.raceName, "human");
        human.bonuses[STR] = 1;
        human.bonuses[DEX] = 1;
        human.bonuses[CON] = 1;
        human.bonuses[INT] = 1;
        human.bonuses[WIS] = 1;
        human.bonuses[CHA] = 1;
        human.speed = 30;
        strcpy(human.size, "medium");
        strcpy(human.languages[0], "common");
        strcpy(human.languages[1], "1 extra");
        human.numLang = 2;

    //dragonborn
    struct race dragonborn;
        strcpy(dragonborn.raceName, "dragonborn");
        dragonborn.bonuses[STR] = 2;
        dragonborn.bonuses[DEX] = 0;
        dragonborn.bonuses[CON] = 0;
        dragonborn.bonuses[INT] = 0;
        dragonborn.bonuses[WIS] = 0;
        dragonborn.bonuses[CHA] = 1;
        dragonborn.speed = 30;
        strcpy(dragonborn.size, "medium");
        strcpy(dragonborn.languages[0], "common");
        strcpy(dragonborn.languages[1], "draconic");
        dragonborn.numLang = 2;


    //tiefling
    struct race tiefling;
        strcpy(tiefling.raceName, "tiefling");
        tiefling.bonuses[STR] = 0;
        tiefling.bonuses[DEX] = 0;
        tiefling.bonuses[CON] = 0;
        tiefling.bonuses[INT] = 1;
        tiefling.bonuses[WIS] = 0;
        tiefling.bonuses[CHA] = 2;
        tiefling.speed = 30;
        strcpy(tiefling.size, "medium");
        strcpy(tiefling.languages[0], "common");
        strcpy(tiefling.languages[1], "infernal");
        tiefling.numLang = 2;


    //halfling
    struct race halfling;
        strcpy(halfling.raceName, "halfling");
        halfling.bonuses[STR] = 0;
        halfling.bonuses[DEX] = 2;
        halfling.bonuses[CON] = 0;
        halfling.bonuses[INT] = 0;
        halfling.bonuses[WIS] = 0;
        halfling.bonuses[CHA] = 0;
        halfling.speed = 25;
        strcpy(halfling.size, "small");
        strcpy(halfling.languages[0], "common");
        strcpy(halfling.languages[1], "halfling");
        halfling.numLang = 2;


    //half-orc
    struct race halfOrc;
        strcpy(halfOrc.raceName, "half-orc");
        halfOrc.bonuses[STR] = 2;
        halfOrc.bonuses[DEX] = 0;
        halfOrc.bonuses[CON] = 1;
        halfOrc.bonuses[INT] = 0;
        halfOrc.bonuses[WIS] = 0;
        halfOrc.bonuses[CHA] = 0;
        halfOrc.speed = 30;
        strcpy(halfOrc.size, "medium");
        strcpy(halfOrc.languages[0], "common");
        strcpy(halfOrc.languages[1], "orc");
        halfOrc.numLang = 2;


    //gnome
    struct race gnome;
        strcpy(gnome.raceName, "gnome");
        gnome.bonuses[STR] = 0;
        gnome.bonuses[DEX] = 0;
        gnome.bonuses[CON] = 0;
        gnome.bonuses[INT] = 2;
        gnome.bonuses[WIS] = 0;
        gnome.bonuses[CHA] = 0;
        gnome.speed = 25;
        strcpy(gnome.size, "small");
        strcpy(gnome.languages[0], "common");
        strcpy(gnome.languages[1], "gnomish");
        gnome.numLang = 2;


    //elf
    struct race elf;
        strcpy(elf.raceName, "elf");
        elf.bonuses[STR] = 0;
        elf.bonuses[DEX] = 2;
        elf.bonuses[CON] = 0;
        elf.bonuses[INT] = 0;
        elf.bonuses[WIS] = 0;
        elf.bonuses[CHA] = 0;
        elf.speed = 30;
        strcpy(elf.size, "medium");
        strcpy(elf.languages[0], "common");
        strcpy(elf.languages[1], "elvish");
        elf.numLang = 2;


    //dwarf
    struct race dwarf;
        strcpy(dwarf.raceName, "dwarf");
        dwarf.bonuses[STR] = 0;
        dwarf.bonuses[DEX] = 0;
        dwarf.bonuses[CON] = 2;
        dwarf.bonuses[INT] = 0;
        dwarf.bonuses[WIS] = 0;
        dwarf.bonuses[CHA] = 0;
        dwarf.speed = 25;
        strcpy(dwarf.size, "medium");
        strcpy(dwarf.languages[0], "common");
        strcpy(dwarf.languages[1], "gnomish");
        dwarf.numLang = 2;


    //half-elf
    struct race halfElf;
        strcpy(halfElf.raceName, "half-elf");
        halfElf.bonuses[STR] = 0;
        halfElf.bonuses[DEX] = 0;
        halfElf.bonuses[CON] = 0;
        halfElf.bonuses[INT] = 0;
        halfElf.bonuses[WIS] = 0;
        halfElf.bonuses[CHA] = 2;
        //half-elves also choose 2 abilities to add 1 to
        halfElf.speed = 30;
        strcpy(halfElf.size, "medium");
        strcpy(halfElf.languages[0], "common");
        strcpy(halfElf.languages[1], "elvish");
        strcpy(halfElf.languages[2], "1 extra");
        halfElf.numLang = 3;


    //Menu for race choice
    while (valid == 0){
        printf("Choose a race:\n");
        printf("\t1. Human\n");
        printf("\t2. Dragonborn\n");
        printf("\t3. Tiefling\n");
        printf("\t4. Halfling\n");
        printf("\t5. Half-Orc\n");
        printf("\t6. Gnome\n");
        printf("\t7. Elf\n");
        printf("\t8. Dwarf\n");
        printf("\t9. Half-Elf\n");
        scanf("%d", &choice);

        switch(choice){
        case 1:
            player->playerRace = human;
            valid = 1;
            break;
        case 2:
            player->playerRace = dragonborn;
            valid = 1;
            break;
        case 3:
            player->playerRace = tiefling;
            valid = 1;
            break;
        case 4:
            player->playerRace = halfling;
            valid = 1;
            break;
        case 5:
            player->playerRace = halfOrc;
            valid = 1;
            player->skillProf[15] = 1; //Proficiency in intimidation
            break;
        case 6:
            player->playerRace = gnome;
            valid = 1;
            break;
        case 7:
            player->playerRace = elf;
            player->skillProf[12] = 1; //Proficiency in perception
            valid = 1;
            break;
        case 8:
            player->playerRace = dwarf;
            valid = 1;
            break;
        case 9:
            player->playerRace = halfElf;
            valid = 1;

            //let the player pick the two abilities to increase
            printf("As a racial bonus, half-elf players choose two extra abilities to increase by 1.\n");
            while (validStat == 0){
                printf("Choose the first:\n");
                for (i = 0; i < 6; i++){
                    printf("\t%d - %s\n", i+1, player->abilityName[i]);
                }
                scanf("%d", &abilityChoice1);
                if (abilityChoice1 >= 1 && abilityChoice1 <= 6){
                    player->playerRace.bonuses[abilityChoice1-1] += 1;
                    validStat = 1;
                }
            }
            validStat = 0;
            while (validStat == 0){
                printf("Choose the second:\n");
                for (i = 0; i < 6; i++){
                    printf("\t%d - %s\n", i+1, player->abilityName[i]);
                }
                scanf("%d", &abilityChoice2);
                if (abilityChoice2 >= 1 && abilityChoice2 <= 6 && abilityChoice2 != abilityChoice1){
                    player->playerRace.bonuses[abilityChoice2-1] += 1;
                    validStat = 1;
                }
            }
            printf("You can also choose two skills to gain proficiency in.\n");
            pickSkills(2, player);
            break;
        }
    }

    return;
 }

 //Rolls 4 dice, drops the lowest, returns the sum of the remaining three
 //Pre-conditions: none
 //Post-conditions: returns one int between 0 and 18
 int rollDice(){
    int rolls[4], i, min, total = 0;

    srand(time(0));

    //generates 4 psuedo-random numbers
    for (i = 0; i < 4; i++){
        rolls[i] = rand() % 6 + 1;
    }

    //finds the smallest of the numbers
    min = rolls[0];
    for (i = 1; i < 4; i++){
        if (rolls[i] < min){
            min = rolls[i];
        }
    }

    //adds all the numbers
    for (i = 0; i < 4; i++){
        total += rolls[i];
    }

    //subtracts the smallest
    total -= min;

    return total;
 }
 void placeStats(int roll, struct character *player){
    int i, choice, valid = 0;

    printf("\nThe current roll is %d.  In which ability would you like to place it?\n", roll);

    //print menu consisting of remaining ability choices
    while (valid == 0){
        for (i = 0; i < 6; i++){
            if (player->abilitiesPlaced[i] == 0){
                printf("\t%d - %s\n", i+1, player->abilityName[i]);
            }
        }
        scanf("%d", &choice);

        //only allow the user to place abilities that have not already had a value assigned
        if (player->abilitiesPlaced[choice - 1] == 0){
            player->abilities[choice-1] = roll;
            player->abilitiesPlaced[choice - 1] = 1; //mark that ability as assigned
            valid = 1;
        }
    }
    printf("%d has been assigned to %s", roll, player->abilityName[choice - 1]);
 }

 //applies race bonuses to the player's abilities
 //pre-conditions: abilities have been rolled and assigned, and a race has been chosen
 //post-condition: the player's abilities will be modified to reflect race bonuseshttp://www.merriam-webster.com/dictionary/gruntle
 void raceBonuses(struct character *player){
    int i;

    for (i = 0; i < 6; i++){
        player->abilities[i] += player->playerRace.bonuses[i];
    }
 }


 //calculates the player's ability modifiers
 //pre-condition: abilities have been rolled and racial bonuses applied
 //post-condition: ability modifier array will be filled with the proper values
 void calcModifiers(struct character *player){
    int i;

    for (i = 0; i < 6; i++){
        if (player->abilities[i] > 10){ //if the ability is higher than 10, subtract ten and integer divide the remaining amount by 2
            player->abilityModifiers[i] = ((player->abilities[i] - 10)/2);
        }
        else if (player->abilities[i] < 10){ //if it is less than 10, find the negative ability modifier
            if (player->abilities[i] % 2 == 0){
                player->abilityModifiers[i] = (player->abilities[i] - 10)/2;
            }
            else {
                player->abilityModifiers[i] = (player->abilities[i] - 11)/2;
            }
        }
        else{ //if the ability is 10, set the modifier to 0
            player->abilityModifiers[i] = 0;
        }
    }

    return;
 }

 //Allows the user to pick a specified number of skill to get proficiency in.
 //Pre-conditions: the player struct has been initialized and the number of skills to pick is between 0 and 18
 //Post-condition: the skillProf array will hold a 1 in the position for all skills the player chooses
 void pickSkills(int num, struct character *player){
    int i, j, choice, valid;

    printf("The following skills are available: \n");
    for (i = 0; i < num; i++){
        valid = 0;
        while (valid == 0){
            for (j = 0; j < 18; j++){
                if (player->skillProf[j] == 0){
                    printf("\t%d - %s\n", j+1, player->skillNames[j]);
                }
            }
            printf("Enter the number for one skill you want proficiency in: \n");
            scanf("%d", &choice);

            if (choice > 0 && choice <= 18 && player->skillProf[choice-1] == 0){ //Only allow the player to choose a valid skill they do not already have
                valid = 1;
                player->skillProf[choice-1] = 1;
            }
            else {
                printf("Your choice was either invalid or a skill you already have proficiency in.  Try again.\n");
            }
        }
    }

    return;
 }

 //calculates skill scores
 //pre-conditions: score proficiencies have been determined
 //post-conditions: score skills will be stored in an array within the player structure
 void calcSkills(struct character *player){
    int i;

    //STR based skills
    player->skillScore[0] = player->abilityModifiers[STR] + (player->skillProf[0] * player->profBonus);

    //DEX based skills
    for (i = 1; i < 4; i++){
        player->skillScore[i] = player->abilityModifiers[DEX] + (player->skillProf[i] * player->profBonus);
    }

    //INT based skills
    for (i = 4; i < 9; i++){
        player->skillScore[i] = player->abilityModifiers[INT] + (player->skillProf[i] * player->profBonus);
    }

    //WIS based skills
    for (i = 9; i < 14; i++){
        player->skillScore[i] = player->abilityModifiers[WIS] + (player->skillProf[i] * player->profBonus);
    }

    //CHA based skills
    for (i = 14; i < 18; i++){
        player->skillScore[i] = player->abilityModifiers[CHA] + (player->skillProf[i] * player->profBonus);
    }

    return;
 }

 //Prints the character details to a file
 //pre-conditions: character has been generated using other functions
 //post-condition: will create a file [character name].txt and print details to it
 void printToFile(struct character player){
    FILE * ofp = NULL;
    char filename[25];
    int i;

    //create a file using the player's name
    strcpy(filename, player.name);
    strcat(filename, ".txt");
    ofp = fopen(filename, "w");

    //output information
    //name, level, class, race, other basic info
    fprintf(ofp, "%s, level %d %s %s\n", player.name, player.level, player.playerRace.raceName, player.playerClass.className);
    fprintf(ofp, "%d HP\t Proficiency Bonus: %d\n", player.hp, player.profBonus);

    //Ability scores and modifiers
    fprintf(ofp, "\nAbilities: \n");
    for (i = 0; i < 6; i++){
        fprintf(ofp, "%s: %d", player.abilityName[i], player.abilities[i]);
        if (player.abilityModifiers[i] >= 0){
            fprintf(ofp, " (modifier: +%d)", player.abilityModifiers[i]);
        }
        else{
            fprintf(ofp, " (modifier: %d)", player.abilityModifiers[i]);
        }
        fprintf(ofp, "\n");
    }

    //weapon, and armor proficiencies
    fprintf(ofp, "\n\nProficiencies: \n");
    fprintf(ofp, "\nWeapon proficiencies: \n");
    for (i = 0; i < 2; i++){
        if (player.playerClass.weaponProf[i] == 1){
            fprintf(ofp, "%s\n", player.playerClass.weaponTypes[i]);
        }
    }
    fprintf(ofp, "\nArmor proficiences:\n");
    for (i = 0; i < 4; i++){
        if (player.playerClass.armorProf[i] == 1){
            fprintf(ofp, "%s\n", player.playerClass.armorTypes[i]);
        }
    }

    //spell information
    if (player.playerClass.spellsKnown != 0 || player.playerClass.spellSlots != 0 || player.playerClass.cantrips != 0){
        fprintf(ofp, "\nSpells:\n");
        if (player.playerClass.spellsKnown != 0){
            fprintf(ofp, "Spells known: %d\n", player.playerClass.spellsKnown);
        }
        if (player.playerClass.spellSlots != 0){
            fprintf(ofp, "Spell slots: %d\n", player.playerClass.spellSlots);
        }
        if (player.playerClass.cantrips != 0){
            fprintf(ofp, "Cantrips: %d\n", player.playerClass.cantrips);
        }
    }

    //skills
    fprintf(ofp, "\n\nSkill scores:\n");
    for (i = 0; i < 18; i++){
        fprintf(ofp, "%s: %d", player.skillNames[i], player.skillScore[i]);
        if (player.skillProf[i] == 1){
            fprintf(ofp, " (proficient)");
        }
        fprintf(ofp, "\n");
    }


    //Speed, size, and languages

    fprintf(ofp, "\nOther information:\n");
    fprintf(ofp, "Speed: %d\t Size: %s\n", player.playerRace.speed, player.playerRace.size);
    fprintf(ofp, "\nLanguages:\n");
    for (i = 0; i < player.playerRace.numLang; i++){
        fprintf(ofp, "%s\n", player.playerRace.languages[i]);
    }

    fclose(ofp);

    return;
 }

 //Prints the character details to the screen
 //pre-conditions: character has been generated using other functions
 //post-condition: screen will display the character details
 //more or less identical to printToFile, just with printf instead of fprintf
 void printToScreen(struct character player){
    int i;

    //output information
    //name, level, class, race, other basic info
    printf("%s, level %d %s %s\n", player.name, player.level, player.playerRace.raceName, player.playerClass.className);
    printf("%d HP\t Proficiency Bonus: %d\n", player.hp, player.profBonus);

    //Ability scores and modifiers
    printf("\nAbilities: \n");
    for (i = 0; i < 6; i++){
        printf("%s: %d", player.abilityName[i], player.abilities[i]);
        if (player.abilityModifiers[i] >= 0){
            printf(" (modifier: +%d)", player.abilityModifiers[i]);
        }
        else{
            printf(" (modifier: %d)", player.abilityModifiers[i]);
        }
        printf("\n");
    }

    //weapon, and armor proficiencies
    printf("\n\nProficiencies: \n");
    printf("\nWeapon proficiencies: \n");
    for (i = 0; i < 2; i++){
        if (player.playerClass.weaponProf[i] == 1){
            printf("%s\n", player.playerClass.weaponTypes[i]);
        }
    }
    printf("\nArmor proficiences:\n");
    for (i = 0; i < 4; i++){
        if (player.playerClass.armorProf[i] == 1){
            printf("%s\n", player.playerClass.armorTypes[i]);
        }
    }

    //spell information
    if (player.playerClass.spellsKnown != 0 || player.playerClass.spellSlots != 0 || player.playerClass.cantrips != 0){
        printf("\nSpells:\n");
        if (player.playerClass.spellsKnown != 0){
            printf("Spells known: %d\n", player.playerClass.spellsKnown);
        }
        if (player.playerClass.spellSlots != 0){
            printf("Spell slots: %d\n", player.playerClass.spellSlots);
        }
        if (player.playerClass.cantrips != 0){
            printf("Cantrips: %d\n", player.playerClass.cantrips);
        }
    }

    //skills
    printf("\n\nSkill scores:\n");
    for (i = 0; i < 18; i++){
        printf("%s: %d", player.skillNames[i], player.skillScore[i]);
        if (player.skillProf[i] == 1){
            printf(" (proficient)");
        }
        printf("\n");
    }


    //Speed, size, and languages

    printf("\nOther information:\n");
    printf("Speed: %d\t Size: %s\n", player.playerRace.speed, player.playerRace.size);
    printf("\nLanguages:\n");
    for (i = 0; i < player.playerRace.numLang; i++){
        printf("%s\n", player.playerRace.languages[i]);
    }

    return;
 }

 //Makes the program wait for a certain number of seconds
 //Pre-conditions: time.h must be included
 //Post-conditions: the program wait the specified number of seconds and continues
 void waitFor(int sec){
    int retTime = time(0) + sec;
    while (time(0) < retTime);

    return;
}
