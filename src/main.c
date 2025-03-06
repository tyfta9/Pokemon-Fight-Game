#include <stm32f031x6.h>
#include <stdlib.h>
#include "display.h"
#include "musical_notes.h"
#include "sound.h"

// screen width
#define SCREENWIDTH 128
// screen height
#define SCREENHEIGHT 160
// pokemon sprite size in pixels
// if pokemon sprites are squares
#define SPRITESIZE 32

//macros for arrow and move position


#define INC 14//detrmines the spacing between moves

#define X 65//constant x position of arrow

#define START 118 // Arrow starting position


// Pokemon sprites counter                              MATTER TO CHANGE IF WE ADD MORE POKEMON
#define SPRITECOUNT 2                 

void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void initADC();
int button_pressed();
void select_pika();
int move_up_func(void);
int move_down_func(void);
void scratch();
void pika_moves();
void select_charmder();
void charm_moves();
void playTune(uint32_t [], uint32_t [], int);






// let player choose a pokemon to play
// should return pointer to the sprite player has chosen
const uint16_t *UserChoosePokemon();
// random choice of cpu pokemon, except specified sprite
const uint16_t *CpuChoosePokemon(const uint16_t *userSprite);
// draw the menu frame, top line and bottom line
void DrawMenuFrame(uint8_t xPosition, uint8_t yPosition, uint8_t menuThickness, uint16_t menuColor);

volatile uint32_t milliseconds;

const uint16_t pikachu[]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,50209,50209,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,9257,58136,58136,0,0,0,0,0,0,0,0,0,0,0,50209,24327,24327,50209,50209,0,0,0,0,0,0,0,0,0,0,0,0,58136,24327,58136,58136,0,0,0,0,0,0,0,0,0,0,50209,24327,16181,16181,16181,16181,58136,0,0,0,0,0,0,0,0,0,0,0,58136,24327,58136,58136,0,0,0,0,0,0,0,0,0,0,50209,24327,16181,16181,16181,16181,58136,0,0,0,0,0,0,0,0,0,58136,58136,24327,24327,58136,58136,0,0,0,0,0,0,58136,58136,58136,58136,24327,16181,16181,16181,16181,16181,58136,0,0,0,0,0,0,0,0,0,58136,58136,24327,16181,58136,58136,0,0,0,58136,58136,58136,9257,9257,58136,58136,16181,16181,16181,16181,58136,58136,0,0,0,0,0,0,0,0,0,0,58136,58136,24327,16181,58136,58136,0,0,0,58136,58136,58136,9257,9257,58136,58136,16181,16181,16181,16181,58136,58136,0,0,0,0,0,0,0,0,0,58136,24327,24327,24327,24327,24327,24327,58136,58136,58136,24327,24327,24327,9257,58136,16181,16181,16181,16181,58136,58136,0,0,0,0,0,0,0,0,0,0,0,58136,24327,24327,24327,24327,24327,24327,58136,58136,58136,24327,24327,24327,9257,58136,16181,16181,16181,16181,58136,58136,0,0,0,0,0,0,0,0,0,58136,58136,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,58136,58136,16181,16181,16181,58136,0,0,0,0,0,0,0,0,0,0,58136,65535,65535,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,16181,16181,58136,0,58136,58136,16181,16181,58136,58136,0,0,0,0,0,0,0,0,58136,0,0,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,58136,0,58136,58136,16181,16181,58136,58136,0,0,0,0,0,0,0,0,58136,0,0,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,24327,58136,0,58136,58136,16181,16181,58136,58136,0,0,0,0,0,0,0,0,58136,24327,24327,24327,24327,24327,24327,65535,0,0,24327,24327,24327,24327,24327,24327,24327,58136,58136,58136,16181,50209,0,0,0,0,0,0,0,0,0,0,0,58136,58136,24327,24327,24327,24327,0,0,0,24327,24327,24327,24327,24327,24327,16181,58136,58136,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,24327,24327,24327,24327,0,0,0,24327,24327,24327,24327,24327,24327,16181,58136,58136,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,16181,16181,24327,24327,24327,24327,7936,7936,7936,24327,16181,16181,16181,16181,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,16181,16181,24327,24327,24327,24327,7936,16181,16181,16181,16181,16181,13067,13067,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,24327,16181,16181,16181,16181,16181,16181,16181,16181,16181,24327,16181,16181,16181,16181,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,24327,16181,16181,16181,16181,16181,16181,16181,16181,16181,24327,16181,16181,16181,16181,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,58136,24327,24327,24327,24327,24327,24327,24327,58136,16181,16181,16181,13067,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,16181,24327,24327,24327,24327,58136,58136,24327,16181,16181,16181,16181,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,16181,24327,24327,24327,24327,58136,58136,24327,16181,16181,16181,16181,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,16181,16181,58136,16181,16181,16181,16181,16181,16181,58136,16181,16181,16181,16181,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,16181,16181,58136,16181,16181,16181,16181,16181,16181,58136,16181,16181,16181,16181,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,58136,58136,58136,58136,58136,24327,24327,24327,24327,16181,16181,16181,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,58136,58136,16181,16181,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,24327,24327,24327,16181,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,24327,24327,24327,16181,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t charmander[]=
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,58136,58136,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,0,0,0,0,0,0,0,0,0,0,0,58136,58136,58136,58136,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,0,0,0,0,0,0,0,0,0,58136,58136,7940,7940,7940,7940,7940,7940,58136,0,0,0,0,0,0,0,0,0,0,0,58136,58136,64041,58136,0,0,0,0,0,0,0,58136,7940,7940,7940,7940,7940,7940,7940,7940,7940,58136,58136,0,0,0,0,0,0,0,0,0,58136,58136,64041,64041,58136,58136,0,0,0,0,0,58136,7940,7940,7940,7940,7940,7940,7940,7940,7940,58136,58136,0,0,0,0,0,0,0,0,0,58136,58136,64041,64041,58136,58136,0,0,0,0,0,58136,7940,7940,7940,7940,7940,7940,7940,7940,7940,58136,58136,0,0,0,0,0,0,0,0,0,58136,58136,64041,64041,58136,58136,0,0,0,0,58136,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,58136,58136,0,0,0,0,0,58136,58136,64041,64041,64041,64041,64041,64041,58136,0,0,0,58136,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,58136,58136,0,0,0,0,0,58136,58136,64041,64041,64041,64041,64041,64041,58136,0,58136,58136,7940,7940,7940,7940,7940,65535,65535,0,7940,7940,7940,7940,7940,58136,58136,0,0,0,0,0,58136,58136,64041,64041,64041,32575,64041,64041,58136,0,58136,58136,7940,7940,7940,7940,7940,0,0,0,7940,7940,7940,7940,7940,7940,7940,58136,0,0,0,0,58136,58136,64041,64041,32575,32575,64041,64041,58136,0,58136,58136,7940,7940,7940,7940,7940,0,0,0,7940,7940,7940,7940,7940,7940,7940,58136,0,0,0,0,0,0,58136,58136,32575,58136,58136,58136,0,0,58136,58136,7940,7940,7940,7940,7940,0,0,0,7940,7940,7940,7940,7940,7940,7940,58136,0,0,0,0,0,0,58136,58136,32575,58136,58136,58136,0,0,0,0,58136,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,58136,0,0,0,0,0,58136,58136,7940,58136,0,0,0,0,0,0,0,58136,58136,58136,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,58136,58136,0,58136,58136,7940,7940,7940,58136,0,0,0,0,0,0,0,58136,58136,58136,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,7940,58136,58136,0,58136,58136,7940,7940,7940,58136,0,0,0,0,0,0,0,0,0,0,58136,58136,58136,58136,7940,7940,7940,58136,58136,7940,7940,7940,7940,58136,58136,58136,7940,7940,7940,7940,58136,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,32575,32575,32575,58136,7940,7940,7940,7940,7940,7940,7940,7940,58136,7940,7940,7940,7940,58136,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,32575,32575,32575,58136,7940,7940,7940,7940,7940,7940,7940,7940,58136,7940,7940,7940,7940,58136,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,32575,32575,32575,32575,58136,58136,58136,58136,7940,7940,7940,7940,58136,7940,7940,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,58136,65535,65535,58136,32575,32575,32575,32575,32575,7940,7940,7940,7940,7940,7940,58136,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,58136,58136,58136,32575,32575,32575,7940,7940,7940,7940,58136,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,58136,58136,58136,32575,32575,32575,7940,7940,7940,7940,58136,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,58136,58136,58136,7940,58136,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,58136,58136,58136,7940,58136,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,65535,65535,7940,65535,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,58136,58136,58136,58136,58136,58136,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t select_arrow[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,30918,30918,0,0,0,0,0,0,0,0,30918,30918,30918,0,0,0,0,0,0,0,30918,30918,30918,30918,0,0,0,0,0,0,30918,30918,30918,0,0,0,0,0,0,0,30918,30918,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t deco1[]=
{
0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,65415,65415,65415,248,65415,0,0,0,0,0,0,0,65415,65415,65415,65415,65415,8068,0,0,0,0,0,0,65415,65415,65415,4096,4096,0,0,0,0,0,0,0,0,65415,65415,65415,0,0,0,0,0,0,0,0,0,7936,7936,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,7936,7936,7936,0,0,0,0,0,7936,7936,65535,65535,65535,65535,7936,0,0,0,0,0,7936,7936,7936,7936,7936,7936,7936,0,0,0,0,0,7936,7936,7936,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7936,65535,7936,0,0,0,0,0,0,0,0,0,7940,7940,7940,7940,0,0,0,
};
const uint16_t deco2[]= 
{
0,0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,65415,65415,65415,248,65415,0,0,0,0,0,0,0,65415,65415,65415,65415,65415,8068,0,0,0,0,0,0,65415,65415,65415,4096,4096,0,0,0,0,0,0,0,0,65415,65415,65415,0,0,0,0,0,0,0,0,7936,7936,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,7936,7936,7936,0,0,0,0,0,7936,7936,65535,65535,65535,65535,7936,0,0,0,0,0,7936,7936,7936,7936,7936,7936,7936,0,0,0,0,0,7936,7936,7936,7936,0,0,0,0,0,0,0,0,0,40224,7936,65535,7936,0,0,0,0,0,0,0,40224,40224,7936,65535,7936,0,0,0,0,0,0,65315,40224,40224,7936,65535,40224,0,0,0,0,0,0,0,65315,0,65315,65315,65315,65315,0,0,
};
const uint16_t deco3[]= 
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,65535,65535,4,4,4,4,4,4,65535,65535,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,24327,24327,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t dg1[]=
{
	0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,
};






int main()
{
	

	int hinverted = 0;
	int vinverted = 0;
	int toggle = 0;
	int hmoved = 0;
	int vmoved = 0;
	uint16_t x = 50;
	uint16_t y = 50;
	uint16_t oldx = x;
	uint16_t oldy = y;
	uint32_t pokemon_battle_theme[] = { AS4_Bb4, F4, DS4_Eb4, F4, AS4_Bb4, F4, DS4_Eb4, F4, 
		AS4_Bb4, DS5_Eb5, CS5_Db5, AS4_Bb4, GS4_Ab4, F4, DS4_Eb4, C4};
	uint32_t theme_durations[] = { 150, 150, 150, 150, 150, 150, 150, 150,
		300, 300, 300, 300, 300, 300, 300, 300};
	initADC();
	

	const uint16_t *userSprite = 0;
	const uint16_t *cpuSprite = 0;
	uint16_t color = RGBToWord(255,50,0);

	


	
	initClock();
	initSysTick();
	setupIO();
	
	initSound();
	
	// playNote(1000);

	playTune(pokemon_battle_theme, theme_durations, 16);
	// while(1)
	// {
	// 	for (int i = 0; i < 30; i++)
	// 	{
	// 		playNote(pokemon_battle_theme[i]);

	// 	}
		

	// }
	
	while(1)
	{
		// keep coding using those values as if
		// userSprite is a pointer to the sprite chosen by the player
		userSprite = UserChoosePokemon();
		// the pointer to sprite chosen by cpu
		cpuSprite = CpuChoosePokemon(userSprite);
		
		// checking if it works. 
		putImage(20, 80, SPRITESIZE, SPRITESIZE, userSprite, 1, 0);// can be altered or deleted 
		putImage(80, 20, SPRITESIZE, SPRITESIZE, cpuSprite, 0, 0);// can be altered or deleted

		
		

			
		
		//loops menu options
		while(1)
		{
			DrawMenuFrame(5, 80+32, 2, RGBToWord(255,50,0));//draw menu function

			

			move_down_func();

			//checks what pokemon user choose to determine move set
			if(userSprite == pikachu)
			{
				pika_moves();//draws pika's moves
				select_pika();
			}
			else
			{
				charm_moves();
				select_charmder();

			}
			

		}
			
		
		

		// while (1)
		// {
		// 	hmoved = vmoved = 0;
		// 	hinverted = vinverted = 0;
		// 	if ((GPIOB->IDR & (1 << 4)) == 0) // right pressed
		// 	{					
		// 		if (x < 110)
		// 		{
		// 			x = x + 1;
		// 			hmoved = 1;
		// 			hinverted = 0;
		// 		}						
		// 	}
		// 	if ((GPIOB->IDR & (1 << 5)) == 0) // left pressed
		// 	{				
		// 		if (x > 10)
		// 		{
		// 			x = x - 1;
		// 			hmoved = 1;
		// 			hinverted = 1;
		// 		}			
		// 	}
		// 	if ( (GPIOA->IDR & (1 << 11)) == 0) // down pressed
		// 	{
		// 		if (y < 140)
		// 		{
		// 			y = y + 1;			
		// 			vmoved = 1;
		// 			vinverted = 0;
		// 		}
		// 	}
		// 	if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
		// 	{			
		// 		if (y > 16)
		// 		{
		// 			y = y - 1;
		// 			vmoved = 1;
		// 			vinverted = 1;
		// 		}
		// 	}
		// 	if ((vmoved) || (hmoved))
		// 	{
		// 		// only redraw if there has been some movement (reduces flicker)
		// 		fillRectangle(oldx,oldy,12,16,0);
		// 		oldx = x;
		// 		oldy = y;					
		// 		if (hmoved)
		// 		{
		// 			if (toggle)
		// 				putImage(x,y,12,16,deco1,hinverted,0);
		// 			else
		// 				putImage(x,y,12,16,deco2,hinverted,0);

		// 			toggle = toggle ^ 1;
		// 		}
		// 		else
		// 		{
		// 			putImage(x,y,12,16,deco3,0,vinverted);
		// 		}
		// 		// Now check for an overlap by checking to see if ANY of the 4 corners of deco are within the target area
		// 		if (isInside(20,80,12,16,x,y) || isInside(20,80,12,16,x+12,y) || isInside(20,80,12,16,x,y+16) || isInside(20,80,12,16,x+12,y+16) )
		// 		{
		// 			printTextX2("GLUG!", 10, 20, RGBToWord(0xff,0xff,0), 0);
		// 		}
		// 	}	

		// 	delay(50);
		// }
	}
	return 0;
}

// let player choose a pokemon to play
// should return pointer to the sprite player has chosen 
const uint16_t *UserChoosePokemon()
{
	// menu x position and spacing
	uint8_t xPosition = 5; 
	// menu y position
	uint8_t yPosition = SCREENHEIGHT - 30;
	// radius of button prompts
	uint8_t buttonRadius = 8;
	// button color
	uint16_t buttonColor = RGBToWord(255,50,0);
	// button space between circle and filled circle
	uint8_t buttonSpacing = 4;
	// button space between circle and filled circle when the button is pressed
	uint8_t pressedButtonSpacing = buttonSpacing - 2;
	// delay after button is pressed
	uint8_t buttonDelay = 200;
	// thickness of the menu
	uint8_t menuThickness = 2;
	// color of menu and text
	uint16_t menuColor = RGBToWord(255,50,0);
	// prompt for user
	char *prompt = "Choose pokemon!";

	// draw pokemon to choose 
	putImage(xPosition, (yPosition/2-SPRITESIZE/2), SPRITESIZE, SPRITESIZE, pikachu, 0, 0);
	putImage((SCREENWIDTH-SPRITESIZE-xPosition), (yPosition/2-SPRITESIZE/2), SPRITESIZE, SPRITESIZE, charmander, 0, 0);

	// draw pointer at left pokemon to prompt user
	drawCircle((xPosition+SPRITESIZE+buttonRadius), yPosition/2, buttonRadius, buttonColor);
	fillCircle((xPosition+SPRITESIZE+buttonRadius), yPosition/2, buttonRadius-buttonSpacing, buttonColor);
	// draw pointer at right pokemon to prompt user
	drawCircle((SCREENWIDTH-(xPosition+SPRITESIZE+buttonRadius)), yPosition/2, buttonRadius, buttonColor);
	fillCircle((SCREENWIDTH-(xPosition+SPRITESIZE+buttonRadius)), yPosition/2, buttonRadius-buttonSpacing, buttonColor);
	
	// draw menu frame
	DrawMenuFrame(xPosition, yPosition, menuThickness, menuColor);

	// write a prompt for user to chose a pokemon
	printText(prompt, xPosition*2, ((SCREENHEIGHT-yPosition-xPosition-menuThickness)/2 + yPosition), menuColor, 0);

	while(1)
	{
		// if left button is pressed
		if((GPIOB->IDR & (1 << 5)) == 0)
		{
			// if the button is pressed, fill the circle more to show that it is selected
			fillCircle((xPosition+SPRITESIZE+buttonRadius), yPosition/2, buttonRadius-pressedButtonSpacing, buttonColor);

			// wait until the button is undone
			while((GPIOB->IDR & (1 << 5)) == 0)
			{
				delay(20);
			}

			// wait to show the player's selection
			delay(buttonDelay);

			// erase screen
			fillRectangle(0, 0, SCREENWIDTH, SCREENHEIGHT, 0x0);
			
			


			// return pointer that points at chosen sprite
			return pikachu;
			
		}

		// if right button is pressed
		if((GPIOB->IDR & (1 << 4)) == 0)
		{
			// if the button is pressed, fill the circle more to show that it is selected
			fillCircle((SCREENWIDTH-(xPosition+SPRITESIZE+buttonRadius)), yPosition/2, buttonRadius-pressedButtonSpacing, buttonColor);

			// wait until the button is undone
			while((GPIOB->IDR & (1 << 4)) == 0)
			{
				delay(20);
			}

			// wait to show the player's selection
			delay(buttonDelay);

			// erase screen
			fillRectangle(0, 0, SCREENWIDTH, SCREENHEIGHT, 0x0);
			

			// return pointer that points at chosen sprite
			return charmander;
		}
	}

	return 0;
}

// randomly choose cpu pokemon
// should return pointer to the sprite cpu has chosen
const uint16_t *CpuChoosePokemon(const uint16_t *userSprite)
{
	uint8_t choice = 0;

	// keep going until cpu chooses pokemon
	while(1)
	{
		// choose 1 or 0
		choice = rand() % SPRITECOUNT;

		switch (choice)
		{
		// if case is 0 and user have not chosen the same pokemon
		// return it
		case 0:
			if(userSprite != pikachu)
				return pikachu;
			break;
		// if case is 1 and user have not chosen the same pokemon
		// return it
		case 1:
			if(userSprite != charmander)
				return charmander;
			break;
		// if case is 2 and user have not chosen the same pokemon
		// return it
		case 2:
			if(userSprite != pikachu) //                                            Matter for change if we add more pokemon
				return pikachu; //                                            Matter for change if we add more pokemon
			break;
		// if case is 3 and user have not chosen the same pokemon
		// return it
		case 3:
			if(userSprite != charmander) //                                            Matter for change if we add more pokemon
				return charmander; //                                            Matter for change if we add more pokemon
			break;
		default:
			// do nothing
			break;
		}
	}

	return 0;
}

// draw the menu frame, x and y position is top left corner of the menu
void DrawMenuFrame(uint8_t xPosition, uint8_t yPosition, uint8_t menuThickness, uint16_t menuColor)
{
	// draw top line for menu
	fillRectangle(xPosition, yPosition, (SCREENWIDTH-xPosition*2), menuThickness, menuColor);
	// draw bottom line for menu
	fillRectangle(xPosition, (SCREENHEIGHT-menuThickness), (SCREENWIDTH-xPosition*2), menuThickness, menuColor);
}

void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void)
{
	milliseconds++;
}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}

void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}


void initADC()
{
	// Turn on ADC 
	RCC->APB2ENR |= (1 << 9);		
    // Enable the reference voltage
	ADC->CCR |= (1 << 22);	
	// Begin ADCCalibration	
	ADC1->CR = ( 1 << 31);
	// Wait for calibration complete:  
	while ((ADC1->CR & ( 1 << 31)));
	// Select Channel 7
	ADC1->CR |= (1 << 7);	
	// Enable the ADC
	ADC1->CR |= (1 << 0);  
}

void select_pika()//selects pokemon move
{
	
	
	
	uint16_t color = RGBToWord(255,50,0);
	int choice = move_down_func();//storing the users choice

	if(((GPIOB->IDR & (1 << 4)) == 0) )
	{
		delay(500);//stops extra input
		char *promt1 = "Pika used Scratch!";
		char *prompt2  = "Pika used Thunder!";
		char *prompt3 = "Pika used Heal!";
		int width = 125;
		int height =40;

		fillRectangle(X, START + (1 - 1) * INC, 10, 10, 0x0);//removes arrow drawn by move down func

		switch (choice)
		{
		case 1:

			fillRectangle(X, 115, width, height, 0x0);//erases moves
			printText(promt1, 4, 126, color, 0);

			while (((GPIOB->IDR & (1 << 4)) != 0))//waits for right button input
			{
				//wait
			}
			fillRectangle(4, 115, width, height, 0x0);//erases promt
			break;

		case 2:
			fillRectangle(X, 115, width, height, 0x0);//eraases moves
			printText(prompt2, 4, 126, color, 0);

			while (((GPIOB->IDR & (1 << 4)) != 0))//waits for right button input
			{
				//wait
			}
			fillRectangle(4, 115, width, height, 0x0);//erases promt
			break;
		
			case 3:
			fillRectangle(X, 115, width, height, 0x0);//eraases moves
			printText(prompt3, 4, 126, color, 0);

			while (((GPIOB->IDR & (1 << 4)) != 0))//waits for right button input
			{
				//wait
			}
			fillRectangle(4, 115, width, height, 0x0);//erases promt
			break;
			
		
		default:
			break;
		}
	}
		

}




//function to move down the button options
int move_down_func(void)
{
	

	static int move_down = 1;  
	

	// puts arrow at start move
	if ((GPIOA->IDR & (1 << 11)) == 0) 
	{
		delay(500);  //stops multiple button presses

		// clears prev arrow sprite
		fillRectangle(X, START + (move_down - 1) * INC, 10, 10, 0x0);

		move_down++;  // Move to the next move
		
		// handles overflow
		if (move_down > 3)
		{
			move_down = 1;
		}
	}

	// draw the new arrow at the selected position
	putImage(X, START + (move_down - 1) * INC, 10, 10, select_arrow, 0, 0);


	delay(120);  //delay for asthetics
	return move_down;//returnimg current arrow position

}


void pika_moves()//pikachu move set
{
	uint16_t color = RGBToWord(255,50,0);
	char *scratch_mv_txt = "Scratch";
	char *Heal_mv_txt = "Heal";
	char *Spark_mv_txt = "Thunder";


	printText(scratch_mv_txt, 75, 118, color, 0);
	printText(Spark_mv_txt, 75, 132, color, 0);
	printText(Heal_mv_txt, 75, 146, color, 0);

}


void charm_moves()//charmander move set
{
	uint16_t color = RGBToWord(255,50,0);
	char *scratch_mv_txt = "Scratch";
	char *Heal_mv_txt = "Heal";
	char *Spark_mv_txt = "Ember";


	printText(scratch_mv_txt, 75, 118, color, 0);
	printText(Spark_mv_txt, 75, 132, color, 0);
	printText(Heal_mv_txt, 75, 146, color, 0);

}

void select_charmder()//removable if any of you find a better solution
{
	uint16_t color = RGBToWord(255,50,0);
	int choice = move_down_func();//storing the users choice

	if(((GPIOB->IDR & (1 << 4)) == 0) )
	{
		delay(500);//stops extra input
		char *promt1 = "Char used Scratch!";
		char *prompt2  = "Char used Ember!";
		char *prompt3 = "Char used Heal!";
		int width = 125;
		int height =40;

		fillRectangle(X, START + (1 - 1) * INC, 10, 10, 0x0);//removes arrow draw by move down func

		switch (choice)
		{
		case 1:

			fillRectangle(X, 115, width, height, 0x0);//erases moves
			printText(promt1, 4, 126, color, 0);

			while (((GPIOB->IDR & (1 << 4)) != 0))//waits for right button input
			{
				//wait
			}
			fillRectangle(4, 115, width, height, 0x0);//erases promt
			break;

		case 2:
			fillRectangle(X, 115, width, height, 0x0);//eraases moves
			printText(prompt2, 4, 126, color, 0);

			while (((GPIOB->IDR & (1 << 4)) != 0))//waits for right button input
			{
				//wait
			}
			fillRectangle(4, 115, width, height, 0x0);//erases promt
			break;


			case 3:
			fillRectangle(X, 115, width, height, 0x0);//eraases moves
			printText(prompt3, 4, 126, color, 0);

			while (((GPIOB->IDR & (1 << 4)) != 0))//waits for right button input
			{
				//wait
			}
			fillRectangle(4, 115, width, height, 0x0);//erases promt
			break;
			
		
		default:
			break;
		}
	}
		

}
	

void playTune(uint32_t notes[], uint32_t durations[], int count)
{
	int index=0;
	while(index < count)
	{
		playNote(notes[index]);
		delay(durations[index]);
		playNote(0);
		delay(100);
		index++;
	}
}


