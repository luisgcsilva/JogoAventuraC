#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_PLAYER_NAME 50
#define MAX_OBJ_NAME 50
#define MAX_MONSTER_NAME 50
#define MAXLIN 80
#define INITIAL_PLAYER_ENERGY 100
#define INITIAL_PLAYER_CELL 0
#define NO_OBJECT 0
#define NO_TREASURE -1
#define MAX_CELL 500
#define MAX_CELL_DESCRIPTION 100
#define INVALID_WAY -1
#define MAX_COMMAND 10
#define MAX_OBJECTS 10

char * ways[6];
char * monsterWays[6] = {"north", "south", "west", "east", "up", "down"};

struct Player{
	char name[MAX_PLAYER_NAME];
	int energy;
	int cell;
	int object;
	int treasure;
};

struct Cell{
	int north;
	int south;
	int west;
	int east;
	int up;
	int down;
	int object;
	int treasure;
	char description[MAX_CELL_DESCRIPTION];
};

struct Object{
	char name[MAX_OBJ_NAME];
	int effectiveness;
};

struct Monster{
	char name[MAX_MONSTER_NAME];
	int energy;
	int cell;
};

struct Player player;
struct Object objects[MAX_OBJECTS];
struct Monster monster;
struct Cell map[MAX_CELL];

void initPlayer(){
	//Init Player
	printf("Hi player! What is your name?\n");
	scanf("%s", player.name);
	player.energy = INITIAL_PLAYER_ENERGY;
	player.cell = INITIAL_PLAYER_CELL;
	player.object = NO_OBJECT;
	player.treasure = NO_TREASURE;
}

void printPlayer(){
	//Print Player
	printf("----- PLAYER -----\n");
	printf("Player Name: %s\n", player.name);
	printf("Energy: %d\n", player.energy);
	printf("Location: %d\n", player.cell);
	printf("Object: %d\n", player.object);
	printf("Tresure: %d\n", player.treasure);
	printf("-------------------\n");
}

void initObject(FILE *f){
	//Init Objects
	char l[MAX_CELL];
	int counter = 0;
	char name[20] = " ";

	while(fgets(l, MAX_CELL, f) != NULL ){
			sscanf(l, "%d %s %s", &objects[counter].effectiveness, objects[counter].name, name);
			strcat(objects[counter].name, " ");
			strcat(objects[counter].name, name);
			//printf("%s tem poder %d\n", objects[counter].name, objects[counter].effectiveness);
			counter++;
	}
}

void initMonster(){
	//Init Object
	strcpy(monster.name, "Monstro");
	monster.energy = 15;
	monster.cell = 5;
}

void printMonster(){
	//Print Player
	printf("----- Monster -----\n");
	printf("Monster Name: %s\n", monster.name);
	printf("Energy: %d\n", monster.energy);
	printf("Cell: %d\n", monster.cell);
	printf("-------------------\n");
}

void initMap(FILE *f){
	//Init Map
	//Cell 0
	char l[MAX_CELL];

  	int bDescriptionLine;
	int counter = 0;

  	bDescriptionLine=0;
  
  	while( fgets(l, MAX_CELL, f) != NULL ){ //ler linha
    	//fputs(l, stdout);
	
		if( ! bDescriptionLine ){
			//obter cada um dos valores da linha
			sscanf( l, "%d %d %d %d %d %d %d %d", 
				&map[counter].north,
				&map[counter].south,
				&map[counter].west,
				&map[counter].east,
				&map[counter].up,
				&map[counter].down,
				&map[counter].object,
				&map[counter].treasure);
			
			bDescriptionLine = 1;
		}
		else{
			strcpy(map[counter].description, l);
			//printf("descrição: %s\n", map[counter].description);
			
			bDescriptionLine = 0;
			counter++;
		}
  	}

  	fclose(f);
}

void printMap(int nCells){
	//Print Map
	int i = 0;
	printf("----- MAP -----\n");
	printf("---- CELL %d-----\n", i);
	printf("North: %d\n", map[i].north);
	printf("South: %d\n", map[i].south);
	printf("West: %d\n", map[i].west);
	printf("East: %d\n", map[i].east);
	printf("Up: %d\n", map[i].up);
	printf("Down: %d\n", map[i].down);
	printf("Description: %s\n", map[i].description);
	printf("Object: %d\n", map[i].object);
	printf("Tresure: %d\n", map[i].treasure);
	printf("-------------\n");
}

void printPossibleRoutes(){
	int counter = 0;
	memset(ways,0,sizeof(ways));
	if(map[player.cell].north != INVALID_WAY){
		ways[counter] = "North";
		counter++;
	}
	if(map[player.cell].south != INVALID_WAY){
		ways[counter] = "South";
		counter++;
	}
	if(map[player.cell].east != INVALID_WAY){
		ways[counter] = "East";
		counter++;
	}
	if(map[player.cell].west != INVALID_WAY){
		ways[counter] = "West";
		counter++;
	}
	if(map[player.cell].up != INVALID_WAY){
		ways[counter] = "Up";
		counter++;
	}
	if(map[player.cell].down != INVALID_WAY){
		ways[counter] = "Down";
		counter++;
	}

	for(int i = 0; i < 6; i++){
		if(ways[i] != NULL){
			printf("%s ", ways[i]);
		}
	}
}

int movePlayer(char direction[MAX_COMMAND]){
	int newPlayerCell;

	if(strcasecmp(direction, "North") == 0){
		newPlayerCell = map[player.cell].north;
	}
	else if(strcasecmp(direction, "South") == 0){
		newPlayerCell = map[player.cell].south;
	}
	else if(strcasecmp(direction, "East") == 0){
		newPlayerCell = map[player.cell].east;
	}
	else if(strcasecmp(direction, "West") == 0){
		newPlayerCell = map[player.cell].west;
	}
	else if(strcasecmp(direction, "Up") == 0){
		newPlayerCell = map[player.cell].up;
	}
	else if(strcasecmp(direction, "Down") == 0){
		newPlayerCell = map[player.cell].down;
	}
	return newPlayerCell;
};

int checkTreasure(){
	if (map[player.cell].treasure == 1){
		player.treasure = 1;
		return player.treasure;
	}
}

int checkObject(){
	
	char l[MAX_CELL];
	int effectiveness;
	int object;
	char objectName[20];
	char choice[1];

	if (map[player.cell].object != -1){
		object = map[player.cell].object;
		strcpy(objectName, objects[object].name);
		effectiveness = objects[object].effectiveness;
		printf("\nEncontrou uma arma nova: %s, com poder %d. Deseja trocar com a sua? Y/N\n", objectName, effectiveness);
		scanf("%s", choice);
		if (strcasecmp(choice, "Y") == 0) {
			map[player.cell].object = player.object;
			player.object = object;
			return player.object;
		}
		else if (strcasecmp(choice, "N") == 0) {
			return player.object;
		}
		else{
			printf("Invalid Input\n");
		}
	}
	else {
		return player.object;
	}
	
}

int moveMonster(){
	int turnMove = rand()%5 + 1;
	int newMonsterCell;
	char * monsterMove = monsterWays[turnMove];

	if(strcasecmp(monsterMove, "North") == 0){
		if (map[monster.cell].north != 1)
			newMonsterCell = map[monster.cell].north;
	}
	else if(strcasecmp(monsterMove, "South") == 0){
		if (map[monster.cell].south != 1)
			newMonsterCell = map[monster.cell].south;
	}
	else if(strcasecmp(monsterMove, "East") == 0){
		if (map[monster.cell].east != 1)
			newMonsterCell = map[monster.cell].east;
	}
	else if(strcasecmp(monsterMove, "West") == 0){
		if (map[monster.cell].west != 1)
			newMonsterCell = map[monster.cell].west;
	}
	else if(strcasecmp(monsterMove, "Up") == 0){
		if (map[monster.cell].up != 1)
			newMonsterCell = map[monster.cell].up;
	}
	else if(strcasecmp(monsterMove, "Down") == 0){
		if (map[monster.cell].down != 1)
			newMonsterCell = map[monster.cell].down;
	}

	return newMonsterCell;
}

void fight(){
	int monsterAtt = monster.energy * 0.2;

	if(player.cell == monster.cell){
		printf("Encontrou um monstro, começou uma luta\n");

		while(monster.energy != 0 && player.energy != 0){
			if (monster.energy > 0)
			{			
				player.energy -= monsterAtt;
				monster.energy -= objects[player.object].effectiveness;
				printf("Monstro tirou %d de vida ao jogador! Ficando com %d energia!\n", monsterAtt, player.energy);
				sleep(2);
				printf("O jogador tirou %d de energia ao monstro! Ficando com %d energia\n", objects[player.object].effectiveness, monster.energy);
				sleep(2);
			}
			else{
				printf("The monster has been defeated! Congratulations!!\n");
				break;
			}

		}
	}
}

int randNumber()
{
    return (rand() % 6);
}

void gameStart(){
	printf("Your name is %s, and you just woke up feeling lost, your body hurts.\n", player.name);
	sleep(2);
	printf("Your last memory was getting caught by someone and being kept in captivity.\n");
	sleep(2);
	printf("Your adventures starts here!\n");
}

void  *thread1_function(void *ptr)
{
	int newMonsterCell = 0;
	int random;
	srand((unsigned)time(NULL));

	while (monster.energy != 0)
	{
		random = randNumber();
		char * monsterMove = monsterWays[random];

		if(strcasecmp(monsterMove, "North") == 0){
			newMonsterCell = map[monster.cell].north;
		}
		else if(strcasecmp(monsterMove, "South") == 0){
			newMonsterCell = map[monster.cell].south;
		}
		else if(strcasecmp(monsterMove, "West") == 0){
			newMonsterCell = map[monster.cell].west;
		}
		else if(strcasecmp(monsterMove, "East") == 0){
			newMonsterCell = map[monster.cell].east;
		}
		else if(strcasecmp(monsterMove, "Up") == 0){
			newMonsterCell = map[monster.cell].up;
		}
		else if(strcasecmp(monsterMove, "Down") == 0){
			newMonsterCell = map[monster.cell].down;
		}

		if (newMonsterCell != -1)
		{
			monster.cell = newMonsterCell;
			fight(player, monster);//Verifica a presença de monstro na cell do jogador, se existir, luta
		}

		sleep(5);
	}
}

int main(int argc, char *argv[]){
	int nCells;
	char command[MAX_COMMAND];
	FILE *mapFile;
	FILE *objectFile;
	pthread_t thread1;

	mapFile = fopen("mapa.txt", "r");
	objectFile = fopen("objects.txt", "r");

	initPlayer();
	initObject(objectFile);//initObject
	initMonster();//initMonster
	initMap(mapFile);//Init Map

	gameStart();//Começo do jogo
	
	//Prints
	printPlayer();//Print Player
	//printObject();//print dos Objetos
	//printMonster();//print do monstro
	//printMap();//print do mapa

	pthread_create( &thread1, NULL, thread1_function, (void*) NULL);
	
	while(player.treasure == -1){
		printf("%s", map[player.cell].description); //Mostrar a descrição da sala
		printf("Movimentos possiveis:\n");
		printPossibleRoutes(map, player);//Mostra os movimentos possiveis
		printf("\nO que pretende fazer?\n");
		scanf("%s", command);
		for(int i = 0; i < 6; i++){
			if(strcasecmp(command, ways[i]) == 0){
				player.cell = movePlayer(command);//Movimento jogador
				fight(player, monster);//Verifica a presença de monstro na cell do jogador, se existir, luta
				player.treasure = checkTreasure(map, player);//Verifica a existencia de tesouro na cell que o jogador está
				player.object = checkObject(objects, map, player);//Verifica a existencia de objeto, caso exista é perguntado se o jogador quer apanha-lo
				break;
			}
			else if(ways[i] == NULL){
				break;
			}
			else{
				printf("Invalid Input\n");
			}
		}
		
		if(player.treasure == 1){
			printf("You arrived at the last Room where the treasure is!! You won the game!!! Congratulations!!!\n");
			sleep(10);
			pthread_join( thread1, NULL);
			break;
		}
	}
}
