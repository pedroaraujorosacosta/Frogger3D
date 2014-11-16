#ifndef __FLARE_H
#define __FLARE_H

#define FLARE_MAXELEMENTSPERFLARE 
#define FLARE_MINELEMENTSPERFLARE
#define FLARE_RANGE(A,B)    ( (rand()%((B)-(A)+1)) + (A) )
#define FLARE_FRANGE(A,B)   ( ((float)(rand()&0xffffff)/(float)0xfffffe)*((B)-(A)) + (A) )
#define FLARE_MINCOLOUR        MAKEID(140, 100, 50, 100)
#define FLARE_MAXCOLOUR        MAKEID(255, 255, 200, 255)
#define FLARE_MINELEMENTSPERFLARE         8
#define FLARE_MAXSIZE                   0.3f
#define MAKEID(a,b,c,d)     (((a)<<24) | ((b)<<16) | ((c)<<8) | ((d)<<0))

class Flare
{

public:
	Flare();
	~Flare();
};

#endif
