/*****************************************************************************
* lab2a.c for Lab2A of ECE 153a at UCSB
* Date of the Last Update:  October 23,2014
*****************************************************************************/

#define AO_LAB2A
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "qpn_port.h"
#include "bsp.h"
#include "lab2a.h"
#include "lcd.h"
#include "note.h"
#include "fft.h"

bool FFTActive = false;
bool Auto = false;
extern int extfreq;
extern int currOct;
extern int currNote;
extern int aValue;
extern int freq_list[63];
extern int cents;
int Old_freq_list[63];
extern int spacing;
extern int maxfrequency;
int menu = 0;
int octave = 4;
extern int freqOffset = 0;
static char integers[10] = {'0','1','2','3','4','5','6','7','8','9'};
static char notes[12][3]={"A ","A#","B ","C ","C#","D ","D#","E ","F ","F#","G ","G#"};
char octaveStr[1];
char FreqStr[4];
char A4Str[3];

typedef struct Lab2ATag  {               //Lab2A State machine
	QActive super;
}  Lab2A;

/* Setup state machines */
/**********************************************************************/
static QState Lab2A_initial (Lab2A *me);
static QState Lab2A_on      (Lab2A *me);
static QState Lab2A_stateA  (Lab2A *me);
static QState Lab2A_stateB  (Lab2A *me);
static QState Lab2A_stateC  (Lab2A *me);
static QState Lab2A_stateD  (Lab2A *me);
static QState Lab2A_SelectedA  (Lab2A *me);
static QState Lab2A_SelectedB  (Lab2A *me);
static QState Lab2A_SelectedC  (Lab2A *me);
static QState Lab2A_SelectedD  (Lab2A *me);
/**********************************************************************/


Lab2A AO_Lab2A;
XGpio led;

void Lab2A_ctor(void)  {
	Lab2A *me = &AO_Lab2A;
	QActive_ctor(&me->super, (QStateHandler)&Lab2A_initial);
}


QState Lab2A_initial(Lab2A *me) {
	xil_printf("\n\rInitialization");
    return Q_TRAN(&Lab2A_on);
}

QState Lab2A_on(Lab2A *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			xil_printf("\n\rOn");
			}

		case Q_INIT_SIG: {
			clrScr();
			ShowMenu();
			return Q_TRAN(&Lab2A_stateA);
			}
	}

	return Q_SUPER(&QHsm_top);
}


/* Create Lab2A_on state and do any initialization code if needed */
/******************************************************************/

QState Lab2A_stateA(Lab2A *me) {
	switch (Q_SIG(me)) {
	        case Q_ENTRY_SIG: {
	        	FFTActive = false;
	        	Auto = false;
	        	Menu_stateA();
	            return Q_HANDLED();
	        }
	        case ENCODER_UP: {
	        	return Q_TRAN(&Lab2A_stateD);
	        }
	        case ENCODER_DOWN: {
	            return Q_TRAN(&Lab2A_stateB);
	        }
	        case ENCODER_CLICK:  {
	            return Q_TRAN(&Lab2A_SelectedA);
	        }
	        case BUTTON_PRESS_UP:    {
	        	return Q_TRAN(&Lab2A_stateD);
	        }
	        case BUTTON_PRESS_DOWN:    {
	        	return Q_TRAN(&Lab2A_stateB);
	        }
	        case BUTTON_PRESS_CENTER:    {
	        	return Q_TRAN(&Lab2A_SelectedA);
	        }
	    }
	return Q_SUPER(&Lab2A_on);

}

QState Lab2A_stateB(Lab2A *me) { //menu select a4 STATE
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			FFTActive = false;
			Auto = false;
			Menu_stateB();
			return Q_HANDLED();
		}
		case ENCODER_UP: {
			return Q_TRAN(&Lab2A_stateA);
		}
		case ENCODER_DOWN: {
			return Q_TRAN(&Lab2A_stateC);
		}
		case ENCODER_CLICK:  {
			return Q_TRAN(&Lab2A_SelectedB);
		}
		case BUTTON_PRESS_UP:    {
			return Q_TRAN(&Lab2A_stateA);
		}
		case BUTTON_PRESS_DOWN:    {
			return Q_TRAN(&Lab2A_stateC);
		}
		case BUTTON_PRESS_CENTER:    {
			return Q_TRAN(&Lab2A_SelectedB);
		}
	}
	return Q_SUPER(&Lab2A_on);
}

QState Lab2A_stateC(Lab2A *me) { //menu select a4 STATE
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			FFTActive = false;
			Auto = false;
			Menu_stateC();
			return Q_HANDLED();
		}
		case ENCODER_UP: {
			return Q_TRAN(&Lab2A_stateB);
		}
		case ENCODER_DOWN: {
			return Q_TRAN(&Lab2A_stateD);
		}
		case ENCODER_CLICK:  {
			return Q_TRAN(&Lab2A_SelectedD);
		}
		case BUTTON_PRESS_UP:    {
			return Q_TRAN(&Lab2A_stateB);
		}
		case BUTTON_PRESS_DOWN:    {
			return Q_TRAN(&Lab2A_stateD);
		}
		case BUTTON_PRESS_CENTER:    {
			return Q_TRAN(&Lab2A_SelectedC);
		}
	}
	return Q_SUPER(&Lab2A_on);
}

QState Lab2A_stateD(Lab2A *me) { //menu select a4 STATE
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			FFTActive = false;
			Auto = false;
			Menu_stateD();
			return Q_HANDLED();
		}
		case ENCODER_UP: {
			return Q_TRAN(&Lab2A_stateC);
		}
		case ENCODER_DOWN: {
			return Q_TRAN(&Lab2A_stateA);
		}
		case ENCODER_CLICK:  {
			return Q_TRAN(&Lab2A_SelectedD);
		}
		case BUTTON_PRESS_UP:    {
			return Q_TRAN(&Lab2A_stateC);
		}
		case BUTTON_PRESS_DOWN:    {
			return Q_TRAN(&Lab2A_stateA);
		}
		case BUTTON_PRESS_CENTER:    {
			return Q_TRAN(&Lab2A_SelectedD);
		}
	}
	return Q_SUPER(&Lab2A_on);
}

QState Lab2A_SelectedA(Lab2A *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			clrScr();
			SelectedA();
			FFTActive = true;
			Auto = true;
			return Q_HANDLED();
		}
		case ENCODER_CLICK:  {
			clrScr();
			ShowMenu();
			return Q_TRAN(&Lab2A_stateA);
		}
		case BUTTON_PRESS_CENTER:    {
			clrScr();
			ShowMenu();
			return Q_TRAN(&Lab2A_stateA);
		}
		case IDLE:	{
			float scalar = 17.96*(extfreq/7902.13);
			freqOffset = (int)(scalar)*(aValue-440);
			UpdateFrequency(freqOffset + extfreq);
			UpdateOct(currOct);
			UpdateNote(currNote);
			int error = cents;
			if (((error/100)%10) == 0) {
				xil_printf("cents: %d\n\r",error);
				drawError(error);
				drawErrorBar(error);
			}
			return Q_HANDLED();
		}
	}
	return Q_SUPER(&Lab2A_on);
}

QState Lab2A_SelectedB(Lab2A *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			clrScr();
			SelectedB();
			FFTActive = false;
			Auto = false;
			updateA(aValue);
			return Q_HANDLED();
		}
		case ENCODER_UP: {
			if (aValue < 460) {
				aValue ++;
			}
			updateA(aValue);
			return Q_HANDLED();
		}
		case ENCODER_DOWN: {
			if (aValue > 420) {
				aValue --;
			}
			updateA(aValue);
			return Q_HANDLED();
		}
		case ENCODER_CLICK:  {
			clrScr();
			ShowMenu();
			return Q_TRAN(&Lab2A_stateB);
		}
		case BUTTON_PRESS_UP:    {
			if (aValue < 460) {
				aValue ++;
			}
			updateA(aValue);
			return Q_HANDLED();
		}
		case BUTTON_PRESS_DOWN:    {
			if (aValue > 420) {
				aValue --;
			}
			updateA(aValue);
			return Q_HANDLED();
		}
		case BUTTON_PRESS_CENTER:    {
			clrScr();
			ShowMenu();
			return Q_TRAN(&Lab2A_stateB);
		}
	}
	return Q_SUPER(&Lab2A_on);
}

QState Lab2A_SelectedC(Lab2A *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			clrScr();
			SelectedA();
			FFTActive = true;
			Auto = false;
			return Q_HANDLED();
		}
		case ENCODER_UP: {
			if (octave < 8) {
				octave ++;
			}
			return Q_HANDLED();
		}
		case ENCODER_DOWN: {
			if (octave > 1) {
				octave --;
			}
			return Q_HANDLED();
		}
		case ENCODER_CLICK:  {
			clrScr();
			ShowMenu();
			return Q_TRAN(&Lab2A_stateC);
		}
		case BUTTON_PRESS_UP:    {
			if (octave < 8) {
				octave ++;
			}
			return Q_HANDLED();
		}
		case BUTTON_PRESS_DOWN:    {
			if (octave < 1) {
				octave --;
			}
			return Q_HANDLED();
		}
		case BUTTON_PRESS_CENTER:    {
			clrScr();
			ShowMenu();
			return Q_TRAN(&Lab2A_stateC);
		}
		case IDLE:	{
			float scalar = 17.96*(extfreq/7902.13);
			freqOffset = (int)(scalar)*(aValue-440);
			xil_printf("%d\n\r", freqOffset);
			UpdateFrequency(freqOffset + extfreq);
			UpdateOct(octave);
			UpdateNote(currNote);
			int error = cents;
			if (((error/100)%10) == 0) {
				xil_printf("cents: %d\n\r",error);
				drawError(error);
				drawErrorBar(error);
			}
			return Q_HANDLED();
		}
	}
	return Q_SUPER(&Lab2A_on);
}

QState Lab2A_SelectedD(Lab2A *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			clrScr();
			SelectedD();
			FFTActive = true;
			Auto = true;
			return Q_HANDLED();
		}
		case ENCODER_UP: {
			return Q_HANDLED();
		}
		case ENCODER_DOWN: {
			return Q_HANDLED();
		}
		case ENCODER_CLICK:  {
			clrScr();
			ShowMenu();
			return Q_TRAN(&Lab2A_stateD);
		}
		case BUTTON_PRESS_UP:    {
			return Q_HANDLED();
		}
		case BUTTON_PRESS_DOWN:    {
			return Q_HANDLED();
		}
		case BUTTON_PRESS_CENTER:    {
			clrScr();
			ShowMenu();
			return Q_TRAN(&Lab2A_stateD);
		}
		case IDLE: {
			updateHistogram();
			UpdateOct(currOct);
			return Q_HANDLED();
		}
	}
	return Q_SUPER(&Lab2A_on);
}


void updateHistogram() {
	int yinitial = 249;
	int bin_spacing = floor(200/63);
	for (int i = 0;i<63;i++) {
		int diff = 0;
		if (freq_list[i] > 120) {
			freq_list[i] = 120;
		}
		if (Old_freq_list[i] > freq_list[i]) {
			diff = Old_freq_list[i] - freq_list[i];
			setColor(0,0,0);
			fillRect(25+(i*bin_spacing),yinitial-Old_freq_list[i],25+((i+1)*bin_spacing),yinitial-Old_freq_list[i]+diff);
		}
		if (Old_freq_list[i] < freq_list[i]) {
			diff = freq_list[i] - Old_freq_list[i];
			setColor(0,195,255);
			fillRect(25+(i*bin_spacing),yinitial-Old_freq_list[i],25+((i+1)*bin_spacing),yinitial-diff);
		}
		Old_freq_list[i] = freq_list[i];
	}
	updateSpacing();
}

void updateSpacing() {
	setFont(SmallFont);
	setColor(0,195,255);
	setColorBg(0,0,0);
	char spacingStr[2];
	itoa(spacing, spacingStr, 10);
	lcdPrint(spacingStr, 140, 280);
}

void MenuBackground() {
	clrScr();
	for (int Row = 0; Row<8; Row++) {
		int NewRow = Row*40;
		for (int Col = 0; Col<6; Col++) {
			int NewCol = Col*40;
			for (int y = 0; y<40; y++) {
				int blue = 2*ceil(y/2);
				setColor(56,215,227);
				drawHLine(0+NewCol, y+NewRow, 20-(blue/2));
				setColor(84, 104, 110);
				drawHLine(20-(blue/2)+NewCol, y+NewRow, blue);
				setColor(175,215,227);
				drawHLine(20+(blue/2)+NewCol, y+NewRow, 20-(blue/2));

			}
		}
	}
//	setColor(0, 255, 0);
//	setColorBg(0, 0, 0);
//	setFont(BigFont);
//	lcdPrint("VOLUME", 73, 50);
}

void MenuTitle() {
	for (int Row = 1; Row<2; Row++) {
		int NewRow = Row*40;
		for (int Col = 1; Col<5; Col++) {
			int NewCol = Col*40;
			for (int y = 0; y<40; y++) {
				int blue = 2*ceil(y/2);
				setColor(0,0,0);
				drawHLine(0+NewCol, y+NewRow, 20-(blue/2));
				setColor(0, 0, 0);
				drawHLine(20-(blue/2)+NewCol, y+NewRow, blue);
				setColor(0,0,0);
				drawHLine(20+(blue/2)+NewCol, y+NewRow, 20-(blue/2));

			}
		}
	}
	setFont(BigFont);
	setColorBg(0,0,0);
	setColor(0,195,255);
	lcdPrint("CHROMATIC", 48,40);
	lcdPrint("TUNER", 80, 60);
}

void ShowMenu() {
	MenuBackground();
	MenuTitle();
}

void drawErrorBar(int error) {
	setColor(255, 0, 255);
	fillRect(125, 229, 125 + (error * 0.75), 239);
	setColor(0,0,0);
	if (error < 0) {
		fillRect(50 - (error * 0.75), 229, 50, 239);
		fillRect(125, 229, 200, 239);
	} else {
		fillRect(200 - (error * 0.75), 229, 200, 239);
		fillRect(50, 229, 125, 239);
	}
}

void drawError(int error) {
	setFont(SmallFont);
	setColor(0,195,255);;
	setColorBg(0,0,0);
	char errStr[10];
	itoa(error, errStr, 10);
	strcat(errStr, "     ");
	lcdPrint(errStr, 140, 280);
	setColor(0,0,0);
}

void SelectedMenu() {
	setColor(0,34,204);
	setColorBg(25,217,255);
}

void NotSelectedMenu() {
	setColor(25,217,255);
	setColorBg(0,34,204);
}

void BackMenu() {
	int yCoord = 300;
	int xCoord = 70;

	setFont(SmallFont);
	SelectedMenu();
	lcdPrint("SELECT TO GO BACK", xCoord, yCoord);
}

void A4_Title() {
	setFont(BigFont);
	setColorBg(0,0,0);
	setColor(0,195,255);
	lcdPrint("A4 TUNING", 40,40);
	lcdPrint("SELECTION", 50, 60);
}

void Octave_Title() {
	setFont(BigFont);
	setColor(0,195,255);;
	setColorBg(0,0,0);
	lcdPrint("Frequency:", 50, 20);

	setFont(SmallFont);
	lcdPrint("Cents:", 90, 280);
	setColor(255, 215, 0);
	lcdPrint("Octave:", 90, 265);

	int yCoord = 240;
	setColor(25,255,25);//green
	fillRect(100, yCoord, 140, yCoord+5);

	setColor(255,25,25);//red
	fillRect(0, yCoord, 60, yCoord+5);
	fillRect(180, yCoord, 240, yCoord+5);

	setColor(255,255,25);//yellow
	fillRect(60, yCoord, 100, yCoord+5);
	fillRect(140, yCoord, 180, yCoord+5);
}

void Tuner_Title() {
	setFont(BigFont);
	setColor(0,195,255);
	setColorBg(0,0,0);
	lcdPrint("Frequency:", 50, 20);

	setFont(SmallFont);
	lcdPrint("Cents:", 90, 280);
	setColor(255, 215, 0);
	lcdPrint("Octave:", 90, 265);

	int yCoord = 240;
	setColor(25,255,25);//green
	fillRect(100, yCoord, 140, yCoord+5);

	setColor(255,25,25);//red
	fillRect(0, yCoord, 60, yCoord+5);
	fillRect(180, yCoord, 240, yCoord+5);

	setColor(255,255,25);//yellow
	fillRect(60, yCoord, 100, yCoord+5);
	fillRect(140, yCoord, 180, yCoord+5);
}

void Histogram_Title() {
	setFont(BigFont);
	setColorBg(0,0,0);
	setColor(0,195,255);
	lcdPrint("FFT", 100,40);
	lcdPrint("HISTOGRAM", 50, 60);
	setColor(230, 230, 230);
	drawHLine(20, 250, 200);
	setFont(SmallFont);
	setColor(255, 215, 0);
	lcdPrint("Octave:", 90, 265);
	setColor(0,195,255);
	lcdPrint("Spacing:", 75, 280);
}

void Menu_stateA() {
	int yCoord = 230;
	int xCoord = 70;

	setFont(SmallFont);
	SelectedMenu();
	lcdPrint("TUNER", xCoord + 30, yCoord);

	NotSelectedMenu();
	lcdPrint("SET A4", xCoord + 25, yCoord+13);

	NotSelectedMenu();
	lcdPrint("SELECT OCTAVE", xCoord, yCoord+26);

	NotSelectedMenu();
	lcdPrint("HISTOGRAM", xCoord + 15, yCoord+39);
}

void Menu_stateB() {
	int yCoord = 230;
	int xCoord = 70;

	setFont(SmallFont);
	NotSelectedMenu();
	lcdPrint("TUNER", xCoord + 30, yCoord);

	SelectedMenu();
	lcdPrint("SET A4", xCoord + 25, yCoord+13);

	NotSelectedMenu();
	lcdPrint("SELECT OCTAVE", xCoord, yCoord+26);

	NotSelectedMenu();
	lcdPrint("HISTOGRAM", xCoord + 15, yCoord+39);
}

void Menu_stateC() {
	int yCoord = 230;
	int xCoord = 70;

	setFont(SmallFont);
	NotSelectedMenu();
	lcdPrint("TUNER", xCoord + 30, yCoord);

	NotSelectedMenu();
	lcdPrint("SET A4", xCoord + 25, yCoord+13);

	SelectedMenu();
	lcdPrint("SELECT OCTAVE", xCoord, yCoord+26);

	NotSelectedMenu();
	lcdPrint("HISTOGRAM", xCoord + 15, yCoord+39);
}

void Menu_stateD() {
	int yCoord = 230;
	int xCoord = 70;

	setFont(SmallFont);
	NotSelectedMenu();
	lcdPrint("TUNER", xCoord + 30, yCoord);

	NotSelectedMenu();
	lcdPrint("SET A4", xCoord + 25, yCoord+13);

	NotSelectedMenu();
	lcdPrint("SELECT OCTAVE", xCoord, yCoord+26);

	SelectedMenu();
	lcdPrint("HISTOGRAM", xCoord + 15, yCoord+39);
}

void SelectedA() {
	Tuner_Title();
	BackMenu();
}

void SelectedB() {
	A4_Title();
	BackMenu();
}

void UpdateFrequency(int frequency) {
	setFont(BigFont);
	setColor(255, 255, 255);
	setColorBg(0, 0, 0);
	char frequencyStr[6] = "00000";
	char temp = '0';
	int i;
	for (i = 4; i > -1; i--) {
		temp = (int)frequency%10 + 48;
		frequencyStr[i] = temp;
		frequency /= 10;
	}
	lcdPrint(frequencyStr, 50, 70);
}

void UpdateOct(int octave) {
	setFont(SmallFont);
	setColor(255, 215, 0);
	setColorBg(0,0,0);
	octaveStr[0] = integers[octave];
	lcdPrint(octaveStr, 150, 265);
}

void UpdateNote(int currNote) {
	setFont(BigFont);
	setColor(255, 255, 255);
	setColorBg(0,0,0);
	if (extfreq > 80) {
		lcdPrint(notes[currNote], 50, 130);
		setColor(0,0,0);
		fillRect(82,130,230,146);
	}
}

void updateA(int Alpha) {
	setFont(BigFont);
	setColor(255, 255, 255);
	setColorBg(0, 0, 0);
	char A4[4] = "000";
	char temp = '0';
	int i;
	for (i = 2; i > -1; i--) {
		temp = (int)Alpha%10 + 48;
		A4Str[i] = temp;
		Alpha /= 10;
	}
	lcdPrint(A4Str, 70, 100);
}
void SelectedC() {
	Tuner_Title();
	BackMenu();
}

void SelectedD() {
	Histogram_Title();
	BackMenu();
}

/**********************************************************************/
