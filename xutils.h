#ifndef WMGENERAL_H_INCLUDED
#define WMGENERAL_H_INCLUDED

#define MAX_MOUSE_REGION 100

/*
 *   Typedefs 
 */
typedef struct {
	Pixmap		pixmap;
	Pixmap		mask;
	XpmAttributes	attributes;
} XpmIcon;

typedef struct {
        int             enable;
        int             top;
        int             bottom;
        int             left;
        int             right;
} MOUSE_REGION;

extern MOUSE_REGION    mouse_region[MAX_MOUSE_REGION];


extern Display     *display;
extern Window          Root;
extern Window          iconwin, win;
extern int             screen;
extern int             DisplayDepth;
extern GC NormalGC;


/*
 *   Global variable
 */


/*
 *   Function Prototypes
 */
void 		AddMouseRegion(int, int, int, int, int);
int  		CheckMouseRegion(int, int);
Window		openXwindow(int, char **, char **, char *, int, int, char *, char *, char *, char *,char *);
void 		initXwindow(int, char **);
void 		RedrawWindow(void);
void 		RedrawWindowXY(int, int);
void  		copyXPMArea(int, int, int, int, int, int);
void  		copyXBMArea(int, int, int, int, int, int);
void  		setMaskXY(int, int);
unsigned long 	getColor(char *, float);
void 		RedrawWindow(void);


#endif
