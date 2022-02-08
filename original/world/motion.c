#define EXTERN extern

#include "parame.inc"
#include "variab.h"
#include "arrays.h"
#include "trvtbl.inc"

/* World C Version 1.00 copyright 1987 J.D.McDonald 
   Use as you like for non-commercial purposes, but please
   leave this note, and document any changes you make as yours */

    static  int     rax[] = {556, 2445, 7552, 1105, 3111};
    static  int     dx[] = {2331, 4293, 2204, 6339, 3325};
    static  int     locx[] = {176, 177, 178, 179, 180};

vship()
{
    int             locy, i;

    if (wrdnum[0] == ACTIVA) {
	if ((loc < 175 || loc > 181) && loc != 184 && loc != 187)
	    speak(71);
	else if (lpill == 0)
	    speak(237);
	else if (obloc[YROD] != 2000 + YCLIP || obloc[BSAPPH] != BCLIP + 2000)
	    speak(539);
	else if ((obimpr[BDOOR] & 2) == 0)
	    speak(544);
	else if (spcloc != 2)
	    speak(542);
	else if (loc == 181 || loc == 184 || loc == 187) {
	    speak(536);
	    oldloc = 0;
	    oldlc2 = 0;
	    if (loc == 181)
		loc = 177;
	    else if (loc == 184)
		loc = 180;
	    else
		loc = 179;
	} else {
	    locy = 0;
	    for (i = 0; i < 5; i++) {
		if (rax[i] == raset && dx[i] == decset)
		    locy = locx[i];
	    }
	    if (locy == 0)
		speak(537);
	    else if (loc == locy)
		speak(500);
	    else {
		speak(538);
		if (loc == 175)
		    speak(545);
		for (i = 1; i <= MOVMAX; i++)
		    if (obloc[i] == loc)
			obloc[i] = locy;
		loc = locy;
		oldloc = 0;
		oldlc2 = 0;
	    }
	}
    } else if (loc < 176 || loc > 180)
	speak(71);
    else if (lpill == 0)
	speak(237);
    else if (obloc[YROD] != YCLIP + 2000 || obloc[BSAPPH]
	     != BCLIP + 2000)
	speak(539);
    else if ((obimpr[BDOOR] & 2) == 0)
	speak(544);
    else if (spcloc != 2)
	speak(542);
    else if (loc == 176)
	speak(540);
    else if (loc == 178)
	speak(541);
    else if (spcloc != 2)
	speak(543);
    else {
	oldloc = 0;
	oldlc2 = 0;
	speak(543);
	if (loc == 177)
	    locy = 181;
	else if (loc == 179)
	    locy = 187;
	else
	    locy = 184;
	for (i = 1; i <= MOVMAX; i++)
	    if (obloc[i] == loc)
		obloc[i] = locy;
	loc = locy;
    }
}


vcross()
{
    int             result;

    result = 0;
    /* the tree over the chasm */

    if ((loc == 19 || loc == 21) && (dobjs[0] == TREE ||
		       dobjs[0] == LOG || dobjs[0] == CHASM) && (prepdo == 0
			      || prepdo == OVER) && (iobj == TREE || iobj ==
						     LOG || iobj == 0)) {
	result = 1;
	prepdo = 0;
	dobjs[0] = NORTHE;
	if (loc == 21)
	    dobjs[0] = SOUTHW;
    }
    /* you can't "cross" the lake  */

    else if ((loc == 5 || loc == 6 || loc == 15 || loc == 17) && (
							 dobjs[0] == WATER))
	speak(47);
    else
	speak(94);
    return (result);
}


vcrawl()
{
    int             result;
    result = 0;
    /* you can crawl over the log   */

    if ((loc == 19 || loc == 21) && (dobjs[0] == TREE ||
				     dobjs[0] == LOG || dobjs[0] == CHASM) &&
	prepdo == OVER) {
	adverb = CAREFU;
	prepdo = 0;
	oldlc2 = oldloc;
	oldloc = loc;
	if (loc == 19)
	    loc = 21;
	else
	    loc = 19;
    }
    /* if "crawl direction" but not u or d, then call go  */

    else if (dobjs[0] <= NORTHW && prepdo == 0) {
	if (loc == 19 || loc == 21)
	    adverb = CAREFU;
	result = 1;
	speak(109);
    }
    /* otherwise, don't understand  */

    else
	speak(94);
    return (result);

}







vjump()
{
    int             result, ncarrd, nweigh, kcarrd, kweigh;
    result = 0;
    /* can't jump if dead */

    if (deadf) {
	speak(46);
	return (result);
    }
    /* jumping off the spire is fatal  */

    if ((loc == 13 || loc == 14) && ((dobjs[0] == SPIRE &&
				      prepdo == OFF) || dobjs[0] == 0)) {
	speak(49);
	speak(37);
	oldlc2 = 0;
	oldloc = 0;
	loc = 12;
	vdead();
	return (result);
    }
    /* trying to jump the chasm isn't too smart either  */

    if ((loc == 21 || loc == 19) && (dobjs[0] == 0 || ((dobjs[0] ==
		 CHASM || dobjs[0] == TREE || dobjs[0] == LOG) && (prepdo ==
						   0 || prepdo == OVER)))) {
	speak(49);
	speak(38);
	oldlc2 = 0;
	oldloc = 0;
	loc = 22;
	locdat[22] |= 16384;
	vdead();
	return (result);
    }
    /* but you can jump onto the tower  */

    if (loc == 48 && (dobjs[0] == 0 || dobjs[0] == TOWER) &&
	(prepdo == ON || prepdo == 0)) {
	/* if you're not carrying too much  */

	burden(&ncarrd, &nweigh, &kcarrd, &kweigh);
	if (ncarrd == 0 && (obloc[ZKNAPS] != 1000 && obloc[ZKNAPS]
			    != 3000) && wirelc[5] != 1000) {
	    speak(136);
	    oldlc2 = oldloc;
	    oldloc = loc;
	    loc = 49;
	} else
	    speak(139);

	return (result);
    }
    /* but not off the tower  */

    if ((loc == 49) && ((dobjs[0] == TOWER &&
			 prepdo == OFF) || dobjs[0] == 0)) {
	speak(49);
	speak(137);
	oldlc2 = 0;
	oldloc = 0;
	loc = 48;
	vdead();
	return (result);
    }
    /* jumping at the barrier   */

    if ((loc == 26 || loc == 27) && dobjs[0] == BARRIE) {
	if (prepdo == OVER || prepdo == 0) {
	    speak(170);
	    return (result);
	} else if (prepdo == THROUG) {
	    oldloc = 0;
	    oldlc2 = 0;
	    if (loc == 26)
		loc = 27;
	    else
		loc = 26;
	}
    }
    /* bottomless pit   */

    if (loc == 96) {
	oldloc = 0;
	oldlc2 = 0;
	if (dobjs[0] == TUNNEL)
	    loc = 102;
	else {
	    speak(266);
	    vdead();
	    loc = 97;
	}
    }
    /* wheeeeeee!!!  */

    speak(49);
    return (result);
}


vgo()
{
    int             aloc, xretr, dir, errno, nloc, k, kkk, indx1, ix;
    int             m, n, xgox, kk, indx2, s;


    /* the entrance to the cave at the waterfall */

    rmove = 0;
    if (prepdo == BEHIND && loc == 24 && dobjs[0] == HORSET) {
	horflg = 1;
	oldlc2 = oldloc;
	oldloc = loc;
	loc = 25;
	return;
    }
    aloc = loc;
    if (actor == ROBOT)
	aloc = obloc[ZROBOT];
    /* translate go through barrier to direction  */

    if (dobjs[0] == BARRIE && prepdo == THROUG) {
	prepdo = 0;
	if (aloc == 26)
	    dobjs[0] = NORTHE;
	else if (aloc == 19)
	    dobjs[0] = SOUTHW;
	else if (aloc == 62 || aloc == 50)
	    dobjs[0] = NORTH;
	else if (aloc == 86 || aloc == 63)
	    dobjs[0] = SOUTH;
	else if (aloc == 68)
	    dobjs[0] = WEST;
	else if (aloc == 90)
	    dobjs[0] = EAST;
	else
	    prepdo = THROUG;
    }
    /*

     * all prepositional expressions must be handled by special code above

     * this point  

     
*/

    if (prepdo > DOWN) {
	speak(94);
	return;
    }
    xretr = 0;
    dir = 0;
    errno = 0;
    if (prepdo == UP || prepdo == DOWN) {
	if (dobjs[0] == 0)
	    dir = prepdo + 9 - PRPMIN;
	else if (dobjs[0] > 0 && dobjs[0] <= NORTHW)
	    errno = 57;
	else if (dobjs[0] == SPIRE && (aloc >= 12 && aloc <= 14))
	    dobjs[0] = 0;
	else
	    errno = 28;
    } else if (dobjs[0] > 0 && dobjs[0] <= NORTHW)
	dir = dobjs[0] + 1 - NUNMIN;
    else
	errno = 28;
    if (errno != 0)
	goto lab9000;
    /****** we have reached the point where we use the travel table  */
    xgox = 0;
    k = dir;
    kkk = k;
    if (adverb == QUICKL)
	k = k + 64;
    if (adverb == SLOWLY)
	k = k + 32;
    if (adverb == CAREFU)
	k = k + 16;
    /*
     * if he says "go quickly" that will match "go quickly" or just "go" but
     * "go" will not match "go quickly" . same for other adverbs 
     */

    if (deadf)
	k = kkk;
    indx1 = dispat[aloc];
    indx2 = dispat[aloc + 1];
    for (ix = indx1; ix <= indx2 - 2; ix += 2)
	if (trvtbl[ix] == k || kkk == trvtbl[ix])
	    goto lab200;
    errno = 58;
    goto lab9000;
lab200:
    indx1 = ix + 1;
    m = trvtbl[indx1] / 512;
    n = trvtbl[indx1] - m * 512;
    if (actor != 1 && actor != ROBOT)
	return;
    else if (actor == ROBOT) {
	if (m == 1) {
	    xgox = 1;
	    nloc = n;
	} else if (m == 3 && n == 13 && aloc == 68) {
	    xgox = 1;
	    nloc = 90;
	} else if (m == 3 && n == 6 && aloc == 74) {
	    xgox = 1;
	    nloc = 70;
	} else;
    }
    /* unconditional motion */

    else if (m == 1) {
	xgox = 1;
	nloc = n;
	xretr = 1;
	/* unconditional stay where is */

    } else if (m == 2) {
	errno = n;
	/* forced to get out of chair */

	if (aloc == 184 || aloc == 187 || aloc == 166 ||
	    (aloc >= 175 && aloc <= 181))
	    spcloc = 0;
	else;
    } else if (m == 3) {
	/***    special conditions */

	if (n == 1) {
	    /*
	     * to climb the spire you must wear shoes, but nothing else, and
	     * it must be daytime  
             */

	    if (daytim == 0 || (turns % 100) > 73) {
		speak(123);
		return;
	    }
	    if (obloc[CSHOES] == 3000) {
		for (kk = 1; kk <= MOVMAX; kk++) {
		    if (kk == CSHOES)
			continue;
		    if (obloc[kk] == 1000 || (obloc[kk] == 3000 && kk != CKEY))
			errno = 60;
		}
		if (errno != 60) {
		    xgox = 1;
		    nloc = 13;
		}
	    }
	}
	/* to get behind the horsetails */

	else if (n == 2) {
	    if (horflg || (locdat[25] & 16384) != 0) {
		xgox = 1;
		nloc = 25;
	    }
	}
	/* into the bar  */

	else if (n == 3) {
	    if (daytim == 1 || deadf) {
		xgox = 1;
		xretr = 1;
		nloc = 42;
	    } else if (marflg[0]) {
		speak(128);
		return;
	    }
	}
	/* underground from mars */

	else if (n == 4 || n == 11) {
	    if ((obimpr[MDOOR] & 2) == 0) {
		xgox = 1;
		xretr = 1;
		nloc = 135;
		if (n == 11)
		    nloc = 41;
	    }
	} else if (n == 5) {
	    /* warehouse  */

	    if ((obimpr[RDOOR] & 2) != 2) {
		xgox = 1;
		xretr = 0;
		nloc = 47;
		if (aloc == 47)
		    nloc = 51;
	    }
	} else if (n == 6) {
	    if ((obimpr[ZVINE] & 56) == 16) {
		xgox = 1;
		nloc = 70;
		xretr = 1;
	    }
	} else if (n == 7) {
	    /* rabbit hole  */

	    if (obimpr[RHOLE] == 17) {
		xgox = 1;
		nloc = 96;
	    }
	} else if (n == 8) {
	    /* going down the pole c */

	    if ((obimpr[ZLATEX] & 56) == 24) {
		xgox = 1;
		nloc = 80;
	    }
	} else if (n == 9 || n == 12) {
	    /* glass door  */

	    if ((obimpr[GDOOR] & 2) == 0) {
		xgox = 1;
		xretr = 1;
		nloc = 136;
		if (aloc == 136)
		    nloc = 85;
	    }
	}
	/* beehive  */

	else if (n == 10) {
	    if (obimpr[ZDEET] >= 4096) {
		xgox = 1;
		nloc = 88;
		if (aloc == 88)
		    nloc = 87;
	    }
	} else if (n == 13);
	else if (n == 14) {
	    if (obimpr[ZLOUVE] == 9) {
		xgox = 1;
		xretr = 1;
		nloc = 142;
		if (aloc == 142)
		    nloc = 98;
	    }
	}
	/* the scree slope  */

	else if (n == 15) {
	    if (screef > 0) {
		xgox = 1;
		xretr = 1;
		nloc = 151;
	    }
	} else if (n == 16) {
	    if ((obimpr[ODOOR] & 2) == 0) {
		xgox = 1;
		xretr = 1;
		nloc = 166;
		if (aloc == 166)
		    nloc = 165;
	    }
	} else if (n == 17) {
	    if ((obimpr[BDOOR] & 2) == 0) {
		xgox = 1;
		xretr = 1;
		if (aloc == 174)
		    nloc = 175;
		else if (aloc == 175)
		    nloc = 174;
		else if (aloc == 181)
		    nloc = 182;
		else if (aloc == 184)
		    nloc = 185;
		else if (aloc == 187)
		    nloc = 188;
		else;
	    }
	} else if (n == 18) {
	    if (obimpr[HMURAL] == 209) {
		xgox = 1;
		xretr = 1;
		nloc = 170;
	    }
	} else {
	    linout("bug in travel table", 19);
	    return;
	}
    }
    /***    end special conditions

            he dies. code give his final location */

    else if (m == 4) {
	if (deadf)
	    errno = 46;
	else {
	    oldlc2 = 0;
	    oldloc = 0;
	    if (loc == 13 || loc == 14)
		loc = 12;
	    if (loc == 19 || loc == 21)
		loc = 22;
	    if (loc == 76)
		loc = 79;
	    /* if he moved when he dies, special code goes here  */

	    locdat[loc] |= 16384;
	    vdead();
	    if (loc == 96) {
		loc = 97;
		locdat[97] |= 16384;
	    }
	    errno = n;
	}
    }
    /* goes to newloc if he is dead and has been there before */

    else if (m == 5) {
	if (deadf && (locdat[n] & 16384) != 0) {
	    xgox = 1;
	    nloc = n;
	}
    }
    /* motion with some probability */

    else if (m >= 16 && m <= 31) {
	s = (m - 15) * 6;
	if (pct(s)) {
	    xgox = 1;
	    nloc = n;
	}
    } else {
	speak(252);
	return;
    }


    if (errno != 0)
	goto lab9000;
    if (xgox && actor == 1) {
	if ((locdat[nloc] & 25) == 0 && obloc[25] != 1000 && !deadf)
	    errno = 258;
	else if ((locdat[nloc] & 32) != 0) {
	    oldlc2 = oldloc;
	    oldloc = loc;
	    loc = nloc;
	    if (!xretr || loc == 38 || loc == 97 || loc == 127
		|| loc == 88 || loc == 74 || loc == 96) {
		oldloc = 0;
		oldlc2 = 0;
	    }
	} else
	    errno = 59;
    } else if (xgox && actor == ROBOT) {
	if ((locdat[nloc] & 64) != 0) {
	    obloc[ZROBOT] = nloc;
	    rmove = 1;
	} else
	    errno = 299;
    } else {
	ix = indx1 + 1;
	if (trvtbl[ix] == kkk || trvtbl[ix] == k)
	    goto lab200;
	errno = 58;
	if (actor == 1)
	    errno = 299;
    }
    /***   all errors and "you are dead" or "impossibility" messages go here */

lab9000:
    if (errno != 0)
	speak(errno);
}



vretre()
/* retreat or back */

{
    if (oldloc == 0)
	speak(65);
    else {
	loc = oldloc;
	oldloc = oldlc2;
	oldlc2 = 0;
    }
}

vclimb()
{
    int             result;

    result = 0;
    /*

     * if result is 1 call vgo rubber tree jungle trees 

     
*/

    if ((loc == 64 || loc == 67) && (dobjs[0] == TREE || dobjs[0]
				     == 0))
	speak(430);
    else if ((loc == 78 || loc == 79) && (dobjs[0] == TREE || dobjs[0]
					  == 0))
	speak(431);
    else if (loc == 65 && (prepdo == 0 || prepdo == UP) && (dobjs[0]
						== TREE || dobjs[0] == 0)) {
	oldlc2 = oldloc;
	oldloc = loc;
	loc = 66;
    } else if (loc == 66 && (prepdo == DOWN || prepdo == 0) && (dobjs[0]
						== 0 || dobjs[0] == TREE)) {
	oldlc2 = oldloc;
	oldloc = loc;
	loc = 65;
    }
    /* pole  */

    else if ((loc == 80 || loc == 79 || loc == 76) && (dobjs[0] == POLE
						       || dobjs[0] == 0)) {
	result = 1;
	wrdnum[0] = GO;
	dobjs[0] = 0;
	if (prepdo == 0)
	    prepdo = UP;
    }
    /*

     * you can climb the spire if you are careful but the actual motion is

     * done by "vgo" 

     
*/

    else if ((loc >= 12 && loc <= 14) && (dobjs[0] == 0 || dobjs[0]
					  == SPIRE)) {
	wrdnum[0] = GO;
	dobjs[0] = 0;
	result = 1;
	if ((loc == 12 && (prepdo == 0 || prepdo == UP)) ||
	    (loc == 13 && (prepdo == 0 || prepdo == UP)))
	    prepdo = UP;
	else if (((loc == 13 || loc == 14) && prepdo == DOWN) ||
		 (loc == 14 && prepdo == 0))
	    prepdo = DOWN;
	else;
    }
    /* you can climb over the log but not up a tree!  */

    else if (loc == 19 || loc == 21) {
	if ((dobjs[0] == TREE || dobjs[0] == LOG) && prepdo == OVER) {
	    prepdo = 0;
	    result = 1;
	    dobjs[0] = NORTHE;
	    {
		if (loc == 21)
		    dobjs[0] = SOUTHW;
	    }
	} else if (dobjs[0] == TREE && (prepdo == UP || prepdo == 0))
	    speak(108);
	else
	    speak(94);
    }
    /* you can't climb the tower  */

    else if (loc == 48 && (prepdo == 0 || prepdo == UP) &&
	     (dobjs[0] == 0 || dobjs[0] == TOWER))
	speak(138);
    /* but you can climb down the tower */

    else if (loc == 49 && (prepdo == 0 || prepdo == DOWN) &&
	     (dobjs[0] == TOWER || dobjs[0] == 0)) {
	oldlc2 = oldloc;
	oldloc = loc;
	loc = 48;
    }
    /* you can't climb the barrier  */

    else if ((loc == 26 || loc == 27) && dobjs[0] == BARRIE)
	speak(170);
    else if (dobjs[0] == TREE) {
	if ((locdat[loc] & 2048) == 2048)
	    speak(244);
	else
	    speak(245);
    } else if (loc == 20 || (loc >= 52 && loc <= 56)) {
	wrdnum[0] = GO;
	{
	    if (prepdo == 0)
		prepdo = UP;
	}
	result = 1;
    } else
	speak(50);
    return (result);
}


vrun()
{
    int             result;
    result = 0;
    /* ru translates to "go quickly"  */

    if (adverb == SLOWLY)
	speak(51);
    else {
	result = 1;
	adverb = QUICKL;
	wrdnum[0] = GO;
    }
    return (result);
}


vswim()
{
    int             spk, m, i, xloc;
    spk = 0;
    /* he can't swim at night (for no real reason)  */

    if (daytim == 0 && (loc == 5 || loc == 6 || loc == 15 || loc
			== 17))
	spk = 122;
    /* he can't swim while dead */

    else if (deadf)
	spk = 46;
    /* you must swim in the lake or poool  */

    else if ((loc < 5 || loc > 17 || (loc > 6 && loc < 15))
	     && loc != 122)
	spk = 52;
    else;
    if (spk != 0) {
	speak(spk);
	return (0);
    }
    m = dobjs[0];
    if ((m == WATER && prepdo != IN) || ((m == ISLAND || m == SHORE)
			    && prepdo != TO) || (m == POOL && prepdo != IN))
	spk = 28;
    else if ((loc == 5 && (m == WEST || m == ISLAND)) ||
	     (loc == 15 && m == EAST) ||
	     (loc == 16 && m == SOUTH))
	xloc = 6;
    else if (loc == 6 && (m == EAST || m == SHORE))
	xloc = 5;
    else if ((loc == 6 && m == NORTH) ||
	     (loc == 17 && (m == SOUTH || m == ISLAND)) ||
	     (loc == 16 && m == WEST))
	xloc = 15;
    else if ((loc == 6 && m == SOUTH) || (loc == 15 && m == WEST))
	xloc = 16;
    else if (loc == 15 && (m == NORTH || m == SHORE))
	xloc = 17;
    else if ((loc == 5 || loc == 6 || (loc <= 17 && loc >= 15))
	     && (m == WATER)) {
	if (dirty < 10)
	    spk = 110;
	else
	    spk = 111;
    } else if (loc == 122 && m == POOL)
	xloc = 123;
    else
	spk = 54;
    if (spk == 0) {
	/* but not if you are wearing too much  */

	for (i = 1; i <= MOVMAX; i++) {
	    if (i == CSHOES || i == CKEY || i == CFISH || i == DFISH
		|| i == BNET || i == GSPHER)
		continue;
            if(obloc[i] == 1000 || obloc[i] == 3000) {
        	speak(53);
	        return (0);
            }
	}
	oldlc2 = 0;
	oldloc = 0;
	loc = xloc;
	return (0);
    }
    speak(spk);
    return (0);
}
