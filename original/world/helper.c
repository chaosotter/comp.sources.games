#define EXTERN extern

#include <stdio.h>
#include "parame.inc"
#include "variab.h"
#include "arrays.h"
#include "qtext.inc"

#define Q1TEXT	"/usr/games/lib/q1text.dat"

/* World C Version 1.00 copyright 1987 J.D.McDonald 
   Use as you like for non-commercial purposes, but please
   leave this note, and document any changes you make as yours */

static int      start = 0;
int             q1text_dat;
static int      debugflg = 1;
static char     filenm[] = "world.sav";

static short    buffer[512];
static long     filepos, oldpos;
     

speak(point)
    int             point;
{
    /* 
     * this is the main routine to output text from the data file the word
     * rtext(point) points to the proper record in the file  
     */
    int             i, bi, t, kk;
    long            z;

    if (start == 0) {
	start += 1;
	oldpos = -1;
	q1text_dat = open(Q1TEXT, 0);
        if(q1text_dat == -1) {
            printf("Can't open text file q1text.dat.\n");
            exit(0);
        }
    }
    z = (long) ((unsigned long) rtext[point]);
    filepos = z * 2 & ~1023l;
    if (filepos != oldpos) {
	oldpos = filepos;
	lseek(q1text_dat, filepos, 0);
	read(q1text_dat, buffer, 512*sizeof(short));
    }
    bi = z & 511;
    do {
	kk = 0;
	while (1) {
	    if (bi == 512) {
		oldpos += 1024;
		lseek(q1text_dat, oldpos, 0);
		read(q1text_dat, buffer, 512*sizeof(short));
		bi = 0;
	    }
	    t = buffer[bi];
            z++;
            bi++;

	    if (t < 0) {
		t = -t;
		outst2[kk++] = t & 127;
		outst2[kk++] = t >> 8;
	    } else {
		outst2[kk++] = (t % 32) + 96;
		outst2[kk++] = ((t >> 5) % 32) + 96;
		outst2[kk++] = t / 1024 + 96;
		if (outst2[kk - 3] == '{') {
		    kmax = kk - 4;
	            break;
		} 
	    }
	    if (outst2[kk - 2] == '{') {
                kmax = kk - 3;
		break;
	    } else if (outst2[kk - 1] == '{') {
	        kmax = kk - 2;
		break;
	    }
	}
	for (i = 0; i <= kmax; i++) {
	    if (outst2[i] == '`')
		outst2[i] = ' ';
	    if (outst2[i] == '|')
		outst2[i] = '.';
	}
	if (wwflag == 0)
	    linout(outst2, kmax + 1);
    } while (z < (long) ( (unsigned long) rtext[point + 1] ) );
}

vlocat(i, h)
    int             i, h;
{
    int             k, p, r, nowir, xloc, xturn;
    /*
     * code for "l" or "locate"  but NOT "look" gtext(2) points to short
     * descriptions in the location area of the text file, while gtext(1)
     * points to the long ones  
     */

    if (brfflg == 2 || i == 1 || ((locdat[loc] & 16384) == 0
			      && brfflg != 0) || (loc >= 101 && loc <= 116))
	xloc = loc + gtext[1];
    else
	xloc = loc + gtext[2];
    speak(xloc);
    if (loc == 89) {
	if (easttm == 0) {
	    speak(372);
	    eastsc |= 1;
	} else if (easttm > 0 && easttm <= 50) {
	    speak(373);
	    eastsc |= 2;
	} else if (easttm == 51)
	    speak(374);
	else {
	    speak(375);
	    eastsc |= 4;
	}
    }
    xturn = turns % 100;
    if (turns < 8)
	xturn = 8;
    if (h == 0)
	return;
    /* this section prints out the objects at "loc"  */
    for (k = 1; k <= OBJMAX; k++) {
	p = obimpr[k];
	if (!((k <= MOVMAX && obloc[k] != loc) ||
	      (k > MOVMAX && (obw3[k] != loc && obw4[k] != loc)) ||
	      (k == HPUCK && loc == 38 && marflg[1]))) {
	    r = (p / 8) % 8;
	    if (r >= 1) {
		if ((p & 1) == 1)
		    speak(odistb[k] + r);
	    }
	    if (!(obpprp[k] / 2048 == 0 && k != ZROBOT))
		xcontn(k);
	} continue;
    }
    /* standing on something?  */
    if (spcloc == 1)
	speak(212);
    if (spcloc == 2) {
	if (dirty == 0)
	    speak(509);
	else
	    speak(510);
    }
    /* is the wire lying on the ground */
    nowir = 1;
    for (i = 0; i < 6; i++) {
	if (wirelc[i] == loc)
	    nowir = 0;
    }
    if (nowir == 0)
	speak(215);

    if ((loc == 76 || loc == 77 || loc == 79) && obloc[ZCAT] ==
	80)
	speak(280);
    /* flowers following sun  */
    if (loc == 86 && xturn > 6 && xturn < 69 && obimpr[ZSEED] > 4000)
	speak(341);
    /* vine describe   */
    if (loc == 74 && obimpr[ZVINE] == 9) {
	speak(339);
	obimpr[ZVINE] = 8201;
    } else if (loc == 74 && obimpr[ZVINE] == 8201)
	speak(259);
}

vinven()
/* "inventory"  */
{
    int             carryn, wearn, i, k;

    carryn = 0;
    wearn = 0;
    if (wirelc[5] == 1000)
	carryn = 1;
    for (i = 1; i <= MOVMAX; i++) {
	if (obloc[i] == 1000)
	    carryn = 1;
	if (obloc[i] == 3000)
	    wearn = 1;
    }
    if (carryn == 1) {
	speak(62);
        xindnt += 2;
	if (wirelc[5] == 1000)
	    speak(256);
	for (k = 1; k <= MOVMAX; k++) {
	    if (obloc[k] != 1000)
		continue;
	    speak(odistb[k]);
	    if (obpprp[k] / 2048 != 0)
		xcontn(k);
	}
        xindnt -= 2;
    }
    if (wearn == 1) {
	speak(63);
        xindnt += 2;
	for (k = 1; k <= MOVMAX; k++) {
	    if (obloc[k] != 3000)
		continue;
	    speak(odistb[k]);
	    if (obpprp[k] / 2048 != 0)
		xcontn(k);
	}
        xindnt -= 2;
    }
    if (carryn == 0 && wearn == 0)
	speak(64);
    nonext = 1;
}

xcontn(k)                             /* RECURSIVE FUNCTION */
    int             k;
{
    int             p, empty, k1;
    /*
     * list contents of a container 
     * calls itself recursively to list things inside other containers
     */

    if (k == LFUNNE || k == MFUNNE || k == RFUNNE)
	return;
    p = obimpr[k];
    if ((p & 2) == 0 || (obpprp[k] & 128) == 0) {
	empty = 1;
	for (k1 = 1; k1 <= MOVMAX; k1++)
	    if (obloc[k1] == k + 2000)
		empty = 0;
	if (empty) {
	    xindnt += 2;
	    speak(odistb[k] + ((p / 512) % 8));
	    xindnt -= 2;
	} else {
	    xindnt += 2;
	    speak(odistb[k] + ((p / 512) % 8) + 1);
	    xindnt += 2;
	    for (k1 = 1; k1 <= MOVMAX; k1++) {
		if (obloc[k1] == k + 2000) {
		    speak(odistb[k1]);
         	    if (obpprp[k1] / 2048 != 0)
		        xcontn(k1);
                }
	    }
	    xindnt -= 4;
	}
    } else if ((p & 2) != 0 && (obpprp[k] & 128) != 0 && prepdo == IN)
	speak(386);
}

dbg()
{
    if(debugflg == 0) return;
    debugflg = 2;
    printf(" loc is %5d \n", loc);
    printf(" enter new loc ");
    scanf("%d", &loc);
}

scorng()
{
    int             result, i, q, v, l, n, plce, xplc;
    result = 0;
    if (loc >= 153)
	return (0);
    result = (diesc - 3) * 2;
    if (result < 0)
	result = 0;
    if (cactsc == 1 || cactsc == 2)
	result += 2;
    if (cactsc == 3)
	result += 4;
    if ((eastsc & 1) != 0)
	result += 2;
    if ((eastsc & 2) != 0)
	result += 2;
    if ((eastsc & 4) != 0)
	result += 2;
    if(debugflg == 2) 
        printf(" east+cact %5d out of 10\n",result);
    for (i = 1; i <= MOVMAX; i++) {
	q = obw4[i];
	v = q / 4096;
	l = q - v * 4096;
	if (v == 0)
	    continue;
        n = 0;
	itsher(i, &plce);
	if (i == GDISC) {
	    if (plce != 0 && rvtim == 2)
		n = v;
	} else if (l >= 4000 && l - 4000 != obloc[i])
	    n = v;
	else if (l == 3000 && obloc[i] == 3000)
	    n = v;
	else if (l == 1000 && (plce == 1 || plce == 3))
	    n = v;
	else if (l == 3000 && (plce == 1 || plce == 3))
	    n = v / 2;
	else if (l < 1000 && l == obloc[i])
	    n = v;
	else if (l > 2000 && l < 3000 && obloc[i] == l) {
	    itsher((l - 2000), &xplc);
	    if (xplc > 0 && xplc <= 3)
		n = v;
	}
        result += n;
        if(debugflg == 2)printf("obj %5d %5d points out of %5d\n",i,n,v);
    }
    if (deadf)
	result -= 5;
    if (result < 0)
	result = 0;
    result *= 5;
    return (result);
}

getrob(don, doa)
    int             don, doa;
{
    int             result, i;
    /*
     * search table for object return object number if found and unambiguous
     * return 0 if not found return -1 if ambiguous  
     */

    for (i = 1; i <= OBJMAX; i++) {
	if (don != obnoun[i])
	    continue;
	if ((doa == abs(obadjv[i])) || (doa == 0 && obadjv[i] < 0))
	    return (i);
	else if (doa == 0 && obadjv[i] > 0)
	    result = -1;
	else
	    result = 0;
    }
    return (result);
}

    static  char    sstring[] =
    "Your score is     out of a total of     in      turns.";
    static  int     breakp[] = {0, 35, 70, 90, 130, 170, 210, 260, 330,
				380, 450, 900};
scorpt(scor, mxsc)
    int             scor, mxsc;
{
    char            cnum[4];
    int             i;

    numcvt(scor, cnum);
    sstring[14] = cnum[1];
    sstring[15] = cnum[2];
    sstring[16] = cnum[3];
    numcvt(mxsc, cnum);
    sstring[36] = cnum[1];
    sstring[37] = cnum[2];
    sstring[38] = cnum[3];
    numcvt(turns, cnum);
    sstring[43] = cnum[0];
    sstring[44] = cnum[1];
    sstring[45] = cnum[2];
    sstring[46] = cnum[3];
    linout(sstring, 54);
    for (i = 0; i < 10; i++) {
	if (scor >= breakp[i] && scor < breakp[i + 1])
	    break;
    }
    speak(481 + i);

}

numcvt(num, cnum)
    int             num;
    char           *cnum;
{
    int             lx, m;

    m = num / 1000;
    lx = (m) ? '0' + m : ' ';
    *cnum++ = lx;
    m = (num / 100) % 10;
    lx = (m == 0 && lx == ' ') ? ' ' : '0' + m;
    *cnum++ = lx;
    m = (num / 10) % 10;
    lx = (m == 0 && lx == ' ') ? ' ' : '0' + m;
    *cnum++ = lx;
    m = num % 10;
    *cnum = '0' + m;
}

robdsc()
{
    int             rempty, lempty, k;

    if (obloc[ZROBOT] >= 90 && obloc[ZROBOT] <= 95)
	speak(gtext[1] + obloc[ZROBOT]);
    else
	speak(226 + obloc[ZROBOT]);
    rempty = 1;
    lempty = 1;
    for (k = 1; k <= MOVMAX; k++) {
	if (obloc[k] == 2000 + ZROBOT)
	    rempty = 0;
	if (obloc[k] == obloc[ZROBOT] && k != ZROBOT)
	    lempty = 0;
    }
    if (rempty == 0) {
	speak(odistb[ZROBOT] + 5);
	for (k = 1; k <= MOVMAX; k++) {
	    xindnt += 2;
	    if (obloc[k] == 2000 + ZROBOT)
		speak(odistb[k]);
	    xindnt -= 2;
	}
    }
    if (lempty == 0) {
	speak(350);
	for (k = 1; k <= MOVMAX; k++) {
	    xindnt += 2;
	    if (k != ZROBOT && obloc[k] == obloc[ZROBOT])
		speak(odistb[k]);
	    xindnt -= 2;
	}
    }
    if (obloc[ZROBOT] == 90 && (obloc[ZORCHI] == 2000 + ZROBOT
				|| obloc[ZFLYTR] == 2000 + ZROBOT))
	speak(353);
}


endsl()
{
    /* dummy routine to maybe clear screen? */
}


vdead()
{
    int             i;
    /*
     * "you're dead,fred" it is incumbent on the calling routine to get to
     * where the body is supposed to lie, and to set oldloc and oldlc2 to
     * ,usually, 0 this routine sets deadf to .true. and drops all his
     * possessions where he is now 
     */

    eattim = 0;
    rdietm = 0;
    deadf = 1;
    for (i = 1; i <= MOVMAX; i++)
	if (obloc[i] == 1000 || obloc[i] == 3000)
	    obloc[i] = loc;
    if (loc < 153)
	speak(55);
    else
	speak(552);
    nonext = 1;
    eolflg = 1;
    spcloc = 0;
    for (i = 0; i < 6; i++)
	if (wirelc[i] == 1000)
	    wirelc[i] = loc;
    if (wirelc[0] != obloc[ZWIRE])
	return;
    for (i = 0; i < 6; i++)
	wirelc[i] = 0;
    diesc += 1;
}

itsher(objt, plce)
    int             objt;
    int            *plce;
{
    int             n1;
    /*
     * this subroutine tells if an object is either: being carried     plce=1
     * being worn        plce=2 inside something being carried,and not hidden
     * from view (3) at "loc" and not hidden  (4) inside something at loc (5) 
     */

    *plce = 0;
    if (objt <= MOVMAX) {
	if (obloc[objt] == 1000)
	    *plce = 1;
	else if (obloc[objt] == 3000)
	    *plce = 2;
	else if (obloc[objt] == loc && (obimpr[objt] & 1) == 1)
	    *plce = 4;
	else if (obloc[objt] > 2000 && obloc[objt] < 3000) {
	    n1 = obloc[objt] - 2000;
	    if (n1 <= MOVMAX) {
		if ((obloc[n1] == 1000 || obloc[n1] == 3000)
		    && (obimpr[n1] & 2) == 0)
		    *plce = 3;
		else if (obloc[n1] == loc && ((obimpr[n1] & 1) == 1)
			 && ((obimpr[n1] & 2) == 0))
		    *plce = 5;
		else;
	    } else if ((obw3[n1] == loc || obw4[n1] == loc) &&
		       ((obimpr[n1] & 1) == 1) && ((obimpr[n1] & 2) == 0))
		*plce = 5;
	    else;
	} else;
    } else if ((obw3[objt] == loc || obw4[objt] == loc) &&
	       ((obimpr[objt] & 1) == 1))
	*plce = 4;
}

burden(ncarrd, nweigh, kcarrd, kweigh)
    int            *ncarrd, *nweigh, *kcarrd, *kweigh;
{
    int             i;
    /*
     * calculate weight and number of objects on adventurer and in knapsack  
     */


    *ncarrd = 0;
    *nweigh = 0;
    for (i = 1; i <= MOVMAX; i++) {
	if (obloc[i] != 1000)
	    continue;
	*ncarrd += 1;
	*nweigh += obw3[i] / 256;
    }
    *kcarrd = 0;
    *kweigh = 0;
    for (i = 1; i <= MOVMAX; i++) {
	if (i == ZKNAPS || obloc[i] != (2000 + ZKNAPS))
	    continue;
	*kcarrd += 1;
	*kweigh += obw3[i] / 256;
    }
}

getobj(don, doa)
    int             don, doa;
{
    /*
     * search table for object return object number if found and unambiguous
     * return 0 if not found return -1 if ambiguous  
     */

    int             result, i, g1, g2, ggg, ddd, sum;
    int             aplce, bplce, cplce, dplce, fplce, gplce, mplce, nplce, oplce, pplce;
    int             splce, tplce;
    result = 0;
    for (i = 1; i <= OBJMAX; i++) {

	if (don != obnoun[i])
	    continue;
	if ((doa == abs(obadjv[i]) && doa != BEAUTI && doa !=
	     VENDIN) || (doa == 0 && obadjv[i] < 0)) {
	    result = i;
	    return (result);
	} else if ((doa == 0 && obadjv[i] > 0) || (doa == VENDIN &&
			 don == MACHIN) || (doa == BEAUTI && don == BIRD)) {
	    if (don == FISH) {
		if (obloc[CFISH] != 0)
		    result = CFISH;
		else
		    result = DFISH;
	    } else if (don == PANTS) {
		itsher(RPANT, &g1);
		itsher(LPANT, &g2);
		if (g1 != 0 && g2 == 0)
		    result = RPANT;
		else if (g1 == 0 && g2 != 0)
		    result = LPANT;
		else if ((wrdnum[0] == TAKE && prepdo == OFF) ||
			 wrdnum[0] == REMOVE) {
		    if (g1 == 2)
			result = RPANT;
		    else
			result = LPANT;
		} else if (wrdnum[0] == DROP) {
		    if (((g1 == 1) || (g1 == 3)) && g2 != 1 && g2 != 3)
			result = RPANT;
		    if (((g2 == 1) || (g2 == 3)) && g1 != 1 && g1 != 3)
		        result = LPANT;
		} else
		    result = -1;
	    } else if (don == SHIRT) {
		itsher(RSHIRT, &g1);
		itsher(GSHIRT, &g2);
		if (g1 != 0 && g2 == 0)
		    result = RSHIRT;
		else if (g1 == 0 && g2 != 0)
		    result = GSHIRT;
		else if ((wrdnum[0] == TAKE && prepdo == OFF) ||
			 wrdnum[0] == REMOVE) {
		    if (g1 == 2)
			result = RSHIRT;
		    else
			result = GSHIRT;
		} else if (wrdnum[0] == DROP) {
		    if (((g1 == 1) || (g1 == 3)) && g2 != 1 && g2 != 3)
			result = RSHIRT;
		    if (((g2 == 1) || (g2 == 3)) && g1 != 1 && g1 != 3)
			result = GSHIRT;
		} else
		    result = -1;
	    } else if (don == DOOR) {
		if (loc == 41 || loc == 135)
		    result = MDOOR;
		else if (loc == 85 || loc == 136)
		    result = GDOOR;
		else if (loc == 47 || loc == 51)
		    result = RDOOR;
		else if (loc == 165 || loc == 166)
		    result = ODOOR;
		else if (loc == 173)
		    result = PDOOR;
		else if (loc == 175)
		    result = BDOOR;
		else
		    result = -1;
	    } else if (don == CARTRI) {
		itsher(ZCART, &tplce);
		itsher(CCART, &cplce);
		if (tplce > 0 && cplce <= 0)
		    result = ZCART;
		else if (tplce <= 0 && cplce > 0)
		    result = CCART;
		else
		    result = -1;
	    } else if (don == PHOTOG) {
		itsher(PPHOTO, &pplce);
		itsher(MPHOTO, &mplce);
		itsher(OPHOTO, &oplce);
		itsher(APHOTO, &aplce);
		itsher(FPHOTO, &fplce);
		pplce = (pplce < 1) ? pplce : 1;
		mplce = (mplce < 1) ? mplce : 1;
		oplce = (oplce < 1) ? oplce : 1;
		aplce = (aplce < 1) ? aplce : 1;
		fplce = (fplce < 1) ? fplce : 1;
		sum = pplce + mplce + oplce + aplce + fplce;
		{
		    if (sum == 0)
			result = 0;
		    else if (sum > 1)
			result = -1;
		    else {
			if (pplce != 0)
			    result = PPHOTO;
			if (mplce != 0)
			    result = MPHOTO;
			if (oplce != 0)
			    result = OPHOTO;
			if (aplce != 0)
			    result = APHOTO;
			if (fplce != 0)
			    result = FPHOTO;
		    }
		}
	    } else if (don == BIRD) {
		if (loc == 65 || loc == 66)
		    result = RBIRD;
		else if (loc == 78 || loc == 79)
		    result = XBIRD;
		else if (loc == 77 || loc == 80)
		    result = YBIRD;
		else if (loc == 75 || loc == 76)
		    result = ZBIRD;
		else
		    result = 0;
	    } else if (don == BUTTON) {
		if (loc == 146 || loc == 134)
		    result = -1;
		else if (loc == 143)
		    result = RDBUTT;
		else if (loc == 169)
		    result = GBUTTO;
		else
		    result = 0;
	    } else if (don == CABLE || don == DISC) {
		if (don == CABLE) {
		    ddd = DCABLE;
		    ggg = GCABLE;
		} else {
		    ddd = BDISC;
		    ggg = GDISC;
		}
		itsher(ddd, &dplce);
		itsher(ggg, &gplce);
		if (dplce == 0 && gplce == 0)
		    result = 0;
		else {
		    if (wrdnum[0] == DROP) {
			if ((dplce == 1 || dplce == 3) && (gplce != 1 
                                                                 && gplce != 3))
			    result = ddd;
			else if ((gplce == 1 || gplce == 3) && 
                                                     (dplce != 1 && dplce != 3))
			    result = ggg;
			else
			    result = -1;
		    } else if (wrdnum[0] == TAKE) {
			if ((dplce == 4 || dplce == 5) && (gplce != 4 
                                                                 && gplce != 5))
			    result = ddd;
			else if ((gplce == 4 || gplce == 5) &&
                                                     (dplce != 4 && dplce != 5))
			    result = ggg;
			else
			    result = -1;
		    } else {
			if (gplce == 0 && dplce != 0)
			    result = ddd;
			else if (dplce == 0 && gplce != 0)
			    result = ggg;
			else
			    result = -1;
		    }
		}
	    } else if (don == MACHIN) {
		if (loc == 153)
		    result = PMACH;
		else if (loc == 156)
		    result = TMACH;
		else if (loc == 158)
		    result = NMACH;
		else if (loc == 160)
		    result = SMACH;
		else if (loc == 162)
		    result = MMACH;
	    } else if (don == SLOT) {
		if (loc == 146)
		    result = RSLOT;
		else if (loc == 156)
		    result = TSLOT;
		else if (loc == 158)
		    result = NSLOT;
		else if (loc == 160)
		    result = SSLOT;
		else if (loc == 162)
		    result = MSLOT;
	    } else if (don == COIN) {
		if (loc < 153)
		    result = SCOIN;
		else {
		    itsher(SCOIN, &splce);
		    itsher(BCOIN, &bplce);
		    itsher(CCOIN, &cplce);
		    itsher(NCOIN, &nplce);
		    itsher(PCOIN, &pplce);
		    pplce = (pplce < 1) ? pplce : 1;
		    nplce = (nplce < 1) ? nplce : 1;
		    cplce = (cplce < 1) ? cplce : 1;
		    bplce = (bplce < 1) ? bplce : 1;
		    splce = (splce < 1) ? splce : 1;
		    sum = pplce + nplce + cplce + bplce + splce;
		    if (sum == 0)
			result = 0;
		    else if (sum > 1)
			result = -1;
		    else {
			if (splce != 0)
			    result = SCOIN;
			if (bplce != 0)
			    result = BCOIN;
			if (cplce != 0)
			    result = CCOIN;
			if (nplce != 0)
			    result = NCOIN;
			if (pplce != 0)
			    result = PCOIN;
		    }
		}
	    } else
		result = -1;
	    return (result);
	}
    }
    return (result);
}
    static  char    strng1[] = "    I can't do that to item    on your list.";
    static  char    strng2[] = "    Item number    on your list is ambiguous";

cnvobj()
{
    char            cnum[4];
    int             result, many, i, k, n, p, z;

    /*
     * this routine searches the list of objects and compares with the list
     * of d.o.'s if an object exists and is unambiguous it's entry in dobjs
     * is replaced by its object number otherwise its entry in dobjs is set
     * to zero the entries in dobsj are then squeezed up if the result was
     * totally unambiguous cnvobj is .true. 
     */

    result = 1;
    n = 0;
    if (allflg)
	n = 1;
    many = 0;
    if ((butflg && dobjs[2] != 0) || ((!butflg) &&
				      dobjs[1] != 0))
	many = 1;
    k = 0;
    z = -1;
    for (i = n; i < 12; i++) {
	k += 1;
	if (dobjs[i] == 0)
	    break;
	p = getobj(dobjs[i], doadjs[i]);
	if (p > 0) {
	    z += 1;
	    dobjs[z] = p;
	} else if (p == 0) {
	    if (many == 1) {
		numcvt(k, cnum);
		strng1[28] = cnum[2];
		strng1[29] = cnum[3];
		linout(strng1, 44);
	    } else
		speak(422);
	} else {
	    result = 0;
	    if (many == 1) {
		numcvt(k, cnum);
		strng2[16] = cnum[2];
		strng2[17] = cnum[3];
		linout(strng2, 44);
	    } else
		speak(70);
	    speak(250);
	}
    }
    while (++z < 30)
	dobjs[z] = 0;
    return (result);
}

getall()
{

    int             temp[30], i, k, d, m;
    /*
     * routine to get "all" or "all but" objects it only works on moveable
     * objects because the only verbs which call it work only on those
     * objects (drop,take, put) 
     */

    for (i = 0; i < 30; i++) {
	temp[i] = dobjs[i];
	dobjs[i] = 0;
    }
    k = 0;
    for (i = 1; i <= MOVMAX; i++) {
	itsher(i, &d);
	if (d == 0)
	    continue;
	for (m = 0; m < 30; m++)
	    if (temp[m] == i)
		goto endloop;
	if (k == 29) {
	    speak(106);
	    return;
	}
	dobjs[k++] = i;
endloop:
	;
    }
}


rdinit()
{
    long            t;
    long            tloc;
    long            time();
    int             ti;
    t = time(&tloc);
    ti =( (int) t) & 32767;
    srand(ti);
}

qrand()
{
    return ((rand() & 16383) / 4);
}


pct(q)
    int             q;
{
    int             j;

    j = rand();
    return ((q * 327) > (j & 32767));
}

extern char *gets();

yesx(spk)
    int             spk;
{
    char *eoferr;
    char            ans[90];
    /* ask question, and wait for reply from him */


    speak(spk);
    more = 0;
    while (1) {
	linout("?", 2001);
	eoferr = gets(ans);
	more = 1;
	if (strcmp(ans, "Y") == 0 || strcmp(ans, "YES") == 0
	    || strcmp(ans, "y") == 0 || strcmp(ans, "yes") == 0
            || eoferr == (char *) 0)
	    return (1);
	else if (strcmp(ans, "N") == 0 || strcmp(ans, "NO") == 0
		 || strcmp(ans, "n") == 0 || strcmp(ans, "no") == 0)
	    return (0);
	else
	    speak(102);
    }
}

getln()
{
    /* write a prompt, then read a line from the terminal */

    char *eoferr;
    int             i, lastpos;
    linout(">", 7001);

    eoferr = gets(inbuf);
    if(eoferr == (char *) 0) exit(0);
    eolflg = 0;
    lastpos = strlen(inbuf);
    lastpos = (lastpos > 78) ? 78 : lastpos;
    for (i = lastpos; i < 83; i++)
	inbuf[i] = '.';
    more = 1;
    inbuf[127] = lastpos;
}


carerr(x, z)
    int             x, z;
{
    /*
     * this nifty routine put a caret on the terminal at the indicated
     * position of a line and then outputs a message  
     */
    /* if not the first sentence on a line, rewrite line.  */

    if (clause != 1) {
	linout(" ", 1001);
	linout(inbuf, inbuf[127]);
    }
    if (x > 0) {
	xindnt = x;
	linout("^", 1);
	xindnt = 0;
    } else {
	if (clause != 1) {
	    xindnt = lptr - 1;
	    linout("*", 1);
	    xindnt = 0;
	    speak(423);
	}
    }
    speak(z);
}

linout(ustring, num)
    char           *ustring;
    int             num;

{
    int             num1, i;
    char            buff[80];
    char           *cptr;

    cptr = buff;
    num1 = num % 1000;
    for (i = 0; i < xindnt; i++)
	*cptr++ = ' ';

    for (i = 0; i < num1; i++)
	*cptr++ = *ustring++;

    *cptr++ = '\0';
    if (num < 1000) {
        if(more == 19 && nomor == 0) {
            i = strlen(buff);
            for( ; i < 74; i++)
               buff[i] = ' '; 
            strcpy(&buff[74],"MORE");
            printf("%1s", buff);
            gets(buff);
            more = 0;
        }
        else {
            printf("%1s\n", buff);
            more += 1;
        }
    }
    else
	printf("%1s", buff);
}


vsuspe(inout)
    int             inout;
{
    short            sbuffer[256];
    int             result, filedes, i, iret;
    short           *sbptr, *iptr;
    result = 0;

    if (inout != 1) {
	/* "suspend" or "save"  */

	filedes = creat(filenm, 0600);
	if (filedes == -1) {
	    linout("I failed to create your save file.", 34);
	    return (result);
	}
	sbptr = sbuffer;
	*sbptr++ = horflg;
	for (i = 0; i < 6; i++)
	    *sbptr++ = wirelc[i];
	*sbptr++ = turns;
	*sbptr++ = loc;
	*sbptr++ = oldloc;
	*sbptr++ = oldlc2;
	*sbptr++ = brfflg;
	*sbptr++ = deadf;
	*sbptr++ = dirty;
	*sbptr++ = nonext;
	*sbptr++ = spcloc;
	*sbptr++ = fshlif;
	*sbptr++ = noshoe;
	*sbptr++ = daytim;
	*sbptr++ = martim;
	for (i = 0; i < 9; i++)
	    *sbptr++ = marflg[i];
	*sbptr++ = oextim;
	*sbptr++ = rdietm;
	*sbptr++ = rvtim;
	*sbptr++ = gvtim;
	*sbptr++ = eattim;
	*sbptr++ = easttm;
	*sbptr++ = filmtm;
	*sbptr++ = screef;
	*sbptr++ = eastsc;
	*sbptr++ = cactsc;
	*sbptr++ = diesc;
	*sbptr++ = filmst;
	*sbptr++ = machst;
	*sbptr++ = dial1;
	*sbptr++ = dial2;
	*sbptr++ = dial1x;
	*sbptr++ = dial2x;
	*sbptr++ = bonus;
	*sbptr++ = jackpo;
	*sbptr++ = lpill;
	*sbptr++ = pbstat;
	*sbptr++ = decset;
	*sbptr++ = raset;
	*sbptr++ = dcombi;
	*sbptr++ = chaset;
	*sbptr++ = guardl;
        *sbptr++ = fimage;
	write(filedes, sbuffer, 64*sizeof(short));
	sbptr = sbuffer;
	iptr = locdat;
	for (i = 0; i <= LOCNUM; i++)
	    *sbptr++ = *iptr++;
	write(filedes, sbuffer, 256*sizeof(short));
	sbptr = sbuffer;
	iptr = obimpr;
	for (i = 0; i <= OBJMAX; i++)
	    *sbptr++ = *iptr++;
	write(filedes, sbuffer, 256*sizeof(short));
	sbptr = sbuffer;
	iptr = obloc;
	for (i = 0; i <= OBJMAX; i++)
	    *sbptr++ = *iptr++;
	write(filedes, sbuffer, 256*sizeof(short));
	close(filedes);
	return (result);
    } else {
	/* "restore"    */

	filedes = open(filenm, 0);
	if (filedes == -1)
	    goto lab1;
	iret = read(filedes, sbuffer, 64*sizeof(short));
	if (iret != 64*sizeof(short))
	    goto lab1;
	sbptr = sbuffer;
	horflg = *sbptr++;
	for (i = 0; i < 6; i++)
	    wirelc[i] = *sbptr++;
	turns = *sbptr++;
	loc = *sbptr++;
	oldloc = *sbptr++;
	oldlc2 = *sbptr++;
	brfflg = *sbptr++;
	deadf = *sbptr++;
	dirty = *sbptr++;
	nonext = *sbptr++;
	spcloc = *sbptr++;
	fshlif = *sbptr++;
	noshoe = *sbptr++;
	daytim = *sbptr++;
	martim = *sbptr++;
	for (i = 0; i < 9; i++)
	    marflg[i] = *sbptr++;
	oextim = *sbptr++;
	rdietm = *sbptr++;
	rvtim = *sbptr++;
	gvtim = *sbptr++;
	eattim = *sbptr++;
	easttm = *sbptr++;
	filmtm = *sbptr++;
	screef = *sbptr++;
	eastsc = *sbptr++;
	cactsc = *sbptr++;
	diesc = *sbptr++;
	filmst = *sbptr++;
	machst = *sbptr++;
	dial1 = *sbptr++;
	dial2 = *sbptr++;
	dial1x = *sbptr++;
	dial2x = *sbptr++;
	bonus = *sbptr++;
	jackpo = *sbptr++;
	lpill = *sbptr++;
	pbstat = *sbptr++;
	decset = *sbptr++;
	raset = *sbptr++;
	dcombi = *sbptr++;
	chaset = *sbptr++;
	guardl = *sbptr++;
        fimage = *sbptr++;
	iret = read(filedes, sbuffer, 256*sizeof(short));
	if (iret != 256*sizeof(short))
	    goto lab1;
	sbptr = sbuffer;
	iptr = locdat;
	for (i = 0; i <= LOCNUM; i++)
	    *iptr++ = *sbptr++;
	iret = read(filedes, sbuffer, 256*sizeof(short));
	if (iret != 256*sizeof(short))
	    goto lab1;
	sbptr = sbuffer;
	iptr = obimpr;
	for (i = 0; i <= OBJMAX; i++)
	    *iptr++ = *sbptr++;
	iret = read(filedes, sbuffer, 256*sizeof(short));
	if (iret != 256*sizeof(short))
	    goto lab1;
	sbptr = sbuffer;
	iptr = obloc;
	for (i = 0; i <= OBJMAX; i++)
	    *iptr++ = *sbptr++;
        close(filedes);
	result = 1;
	chaser = 0;
	linout(" ", 1);
	return (result);
    }
lab1:
    linout("Restore failed.", 15);
    return (result);
}
