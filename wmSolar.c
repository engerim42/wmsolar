/* Copyright (c) 2002, Thomas Kuiper <tkuiper@inxsoft.net */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <X11/X.h>
#include <X11/xpm.h>
#include <math.h>
#include <values.h>
#include "xutils.h"
#include "wmSolar_master.xpm"
#include "wmSolar_mask.xbm"

#include "DateOps.h"
#include "Lunar.h"
#include "PlanetData.h"

typedef struct Coordinates {
    double x;
    double y;
} Coordinates;

#define PI 3.141592653589793238462643383279502884197169399375105
#define BIGZOOM 0.6

double val = 9.0;
double jd = 0;
int automatic = 1;
long inc = 0;

long cday;
long cmonth;
long cyear;

void GetPoint(double angle, double radius, int planet)
{
    double x;
    double y;
    int xpos;
    int ypos;
    double rads;
    int width = 2;


    angle = angle * -1;
    rads = angle;
    x = cos(rads) * radius * val;
    y = sin(rads) * radius * val;

    xpos = (int) floor(x);
    ypos = (int) floor(y);
    xpos += 32;
    ypos += 32;

    if ((xpos > 4) && (xpos < 60) && (ypos > 4) && (ypos < 60)) {
	switch (planet) {
	case SUN:		// YELLOW
	    copyXPMArea(76, 88, width, width, xpos, ypos);
	    break;
	case MERCURY:		// GREEN
	    copyXPMArea(76, 90, width, width, xpos, ypos);
	    break;
	case VENUS:		// WHITE
	    copyXPMArea(72, 88, width, width, xpos, ypos);
	    break;
	case EARTH:		// CYAN
	    copyXPMArea(74, 90, width, width, xpos, ypos);
	    break;
	case MARS:		// RED
	    copyXPMArea(74, 88, width, width, xpos, ypos);
	    break;
	case JUPITER:		// GRAY
	    copyXPMArea(72, 90, width, width, xpos, ypos);
	    break;
	case SATURN:		// GREEN
	    copyXPMArea(76, 90, width, width, xpos, ypos);
	    break;
	case URANUS:		// PINK
	    copyXPMArea(78, 88, width, width, xpos, ypos);
	    break;
	case NEPTUNE:		// CYAN
	    copyXPMArea(74, 90, width, width, xpos, ypos);
	    break;
	}
    }

}


void drawplanets()
{
    PlanetData p;
    ObsInfo loc(0.0, 0.0, 0);

/*        SUN=0, MERCURY=1, VENUS=2, EARTH=3, MARS=4, JUPITER=5,
        SATURN=6, URANUS=7, NEPTUNE=8, PLUTO=9, LUNA=10 ;*/

    p.calc(SUN, jd, loc);
    GetPoint(p.eclipticLon(), p.radius(), SUN);

    if (val != BIGZOOM) {
	p.calc(MERCURY, jd, loc);
	GetPoint(p.eclipticLon(), p.radius(), MERCURY);
	p.calc(VENUS, jd, loc);
	GetPoint(p.eclipticLon(), p.radius(), VENUS);
	p.calc(EARTH, jd, loc);
	GetPoint(p.eclipticLon(), p.radius(), EARTH);
	p.calc(MARS, jd, loc);
	GetPoint(p.eclipticLon(), p.radius(), MARS);
    } else {
	p.calc(JUPITER, jd, loc);
	GetPoint(p.eclipticLon(), p.radius(), JUPITER);
	p.calc(SATURN, jd, loc);
	GetPoint(p.eclipticLon(), p.radius(), SATURN);
	p.calc(URANUS, jd, loc);
	GetPoint(p.eclipticLon(), p.radius(), URANUS);
	p.calc(NEPTUNE, jd, loc);
	GetPoint(p.eclipticLon(), p.radius(), NEPTUNE);
    }

}

void getcurrentjd()
{
    jd = DateOps::dmyToDay(cday, cmonth, cyear);
}

/* 
 *  Delay between refreshes (in microseconds) 
 */
#define DELAY 10000L
#define DEFAULT_UPDATEDELAY 900L

void ButtonPressEvent(XButtonEvent *);
void KeyPressEvent(XKeyEvent *);


int UpToDate = 0;
long UpdateDelay;


char LabelColor[30] = "#79bdbf";
char WindGustColor[30] = "#ff0000";
char DataColor[30] = "#ffbf50";
char BackColor[30] = "#181818";
char StationTimeColor[30] = "#c5a6ff";


void settoday()
{
    struct tm *tmworld = NULL;

    time_t t;

    t = time(0);
    t += inc;
    tmworld = gmtime(&t);
    cday = tmworld->tm_mday;
    cmonth = tmworld->tm_mon + 1;
    cyear = tmworld->tm_year + 1900;
}

int main(int argc, char *argv[])
{

    XEvent event;
    int m, dt1, dt2, dt3, i, q, digit;
    char tempstr[255];
    Window myWindow;
    m = 101;
    initXwindow(argc, argv);
    myWindow =
	openXwindow(argc, argv, wmSolar_master, wmSolar_mask_bits,
		    wmSolar_mask_width, wmSolar_mask_height, BackColor,
		    LabelColor, WindGustColor, DataColor,
		    StationTimeColor);

    UpToDate = 0;
    settoday();
    AddMouseRegion(0, 4, 52, 20, 59);
    AddMouseRegion(1, 21, 52, 33, 59);
    AddMouseRegion(2, 34, 52, 60, 59);


    while (1) {


	/* 
	 *   Process any pending X events.
	 */
	while (XPending(display)) {
	    XNextEvent(display, &event);
	    switch (event.type) {
	    case Expose:
		RedrawWindow();
		break;
	    case ButtonPress:
		ButtonPressEvent(&event.xbutton);
		break;
	    case KeyPress:
		KeyPressEvent(&event.xkey);
		break;
	    case ButtonRelease:
		break;
	    case EnterNotify:
		XSetInputFocus(display, PointerRoot, RevertToParent,
			       CurrentTime);
		break;
	    case LeaveNotify:
		XSetInputFocus(display, PointerRoot, RevertToParent,
			       CurrentTime);
		break;

	    }
	}

	copyXPMArea(5, 69, 54, 54, 5, 5);
	getcurrentjd();

	q = 0;
	sprintf(tempstr, "%02i.%02i.%02i", (int) cday, (int) cmonth,
		(int) cyear);

	for (i = 0; i < 10; i++) {
	    if (tempstr[i] == '\0')
		break;
	    digit = tempstr[i] - 48;
	    if (digit >= 0) {
		copyXPMArea(digit * 5 + 66, 57, 5, 6, 6 + q, 50);
		q += 5;
	    } else {
		copyXPMArea(10 * 5 + 66, 57, 5, 6, 6 + q, 50);
		q += 5;
	    }
	}

	drawplanets();
	if (val != BIGZOOM) {
	    copyXPMArea(('I' - 65) * 5 + 2, 128, 5, 6, 7, 6);
	    copyXPMArea(('N' - 65) * 5 + 2, 128, 5, 6, 12, 6);
	} else {
	    copyXPMArea(('O' - 65) * 5 + 2, 128, 5, 6, 7, 6);
	    copyXPMArea(('U' - 65) * 5 + 2, 128, 5, 6, 12, 6);
	    copyXPMArea(('T' - 65) * 5 + 2, 128, 5, 6, 17, 6);
	}



	if (m > 100) {
	    if (automatic == 1)
		settoday();

	    m = 0;
	    ++dt1;
	    ++dt2;
	    ++dt3;


	} else {

	    /*
	     *  Increment counter
	     */
	    ++m;

	}

	RedrawWindow();

	usleep(DELAY);
    }
}




void ButtonPressEvent(XButtonEvent * xev)
{

    /*
     *  Process single clicks.
     */
    int region;

    if ((xev->button == Button1) && (xev->type == ButtonPress)) {
	region = CheckMouseRegion(xev->x, xev->y);

	switch (region) {
	case -1:
	    if (val == BIGZOOM)
		val = 9.0;
	    else
		val = BIGZOOM;
	    return;
	    break;
	case 0:
	    cday++;
	    break;
	case 1:
	    cmonth++;
	    break;
	case 2:
	    cyear++;
	    break;

	}
	automatic = 0;

	if (cmonth == 13) {
	    cmonth = 1;
	    cyear++;
	}
	YearEndDays days;
	MonthDays md;
	DateOps::getJulGregYearData(cyear, days[0], md, false);
	if (cday > md[cmonth - 1]) {
	    cday = 1;
	    cmonth++;
	    if (cmonth == 13) {
		cmonth = 1;
		cyear++;
	    }
	}


    } else if ((xev->button == Button2) && (xev->type == ButtonPress)) {


    } else if ((xev->button == Button3) && (xev->type == ButtonPress)) {

	region = CheckMouseRegion(xev->x, xev->y);
	switch (region) {
	case -1:
	    automatic = 1;
	    settoday();
	    return;
	    break;
	case 0:
	    cday--;
	    break;
	case 1:
	    cmonth--;
	    break;
	case 2:
	    cyear--;
	    break;
	}

	automatic = 0;

	if (cmonth == 0) {
	    cmonth = 12;
	    cyear--;
	}

	YearEndDays days;
	MonthDays md;
	DateOps::getJulGregYearData(cyear, days[0], md, false);


	if (cday == 0) {
	    cmonth--;
	    cday = md[cmonth - 1];
	}

	if (cmonth == 0) {
	    cday = 31;
	    cmonth = 12;
	    cyear--;
	}

	if (cday > md[cmonth - 1])
	    cday = md[cmonth - 1];

    }
    return;


}

/*
 *  This routine handles key presses.
 *
 */
void KeyPressEvent(XKeyEvent * xev)
{

    return;

}
