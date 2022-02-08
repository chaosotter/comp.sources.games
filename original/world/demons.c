#define EXTERN extern

#include "parame.inc"
#include "variab.h"
#include "arrays.h"

/* World C Version 1.00 copyright 1987 J.D.McDonald 
   Use as you like for non-commercial purposes, but please
   leave this note, and document any changes you make as yours */

mdemon()
{

    int             qtim, z, i, kkwr;
    /*
     * subroutine to care for comings and goings of martians 
     */

    qtim = (turns % 100) < 75;
    /*
     * the player must have entered mars once in the day and once at night,
     * and then wait 4 turns before they come close 
     */

    if (martim < 7) {
	if (loc == 27 || loc == 50 || loc == 41) {
	    oldloc = 0;
	    speak(152);
	}
	if (loc >= 26 && loc <= 50) {
	    if (qtim && (martim == 0 || martim == 2))
		martim |= 1;
	    if (!qtim && (martim == 0 || martim == 1))
		martim |= 2;
	    if (martim >= 3)
		martim += 1;
	}
    }
    if (qtim)
	return;
    /*
     * they will appear at once if he falls in their hockey rink 
     */

    if (loc == 38 && !marflg[5]) {
	marflg[5] = 1;
	martim = 7;
	speak(169);
	return;
    }
    for (i = 0; i < 5; i++)
	marflg[i] = 0;
    z = (turns % 100);
    /* wont come close if martim < 7 */

    if (martim < 7) {
	if (loc != 51)
	    speak(141);
	goto lab60;
    }
    if (z > 75 && z < 99)
	marflg[4] = 1;
    if (z > 76 && z < 98)
	marflg[0] = 1;
    if (z > 78 && z < 95)
	marflg[3] = 1;
    if (z > 78 && z < 96)
	marflg[2] = 1;
    if (z > 77 && z < 97 && obloc[HPUCK] == 38)
	marflg[1] = 1;
    /* announce the goings on of the martians */

    if (marflg[5] && z == 75 && loc != 38)
	speak(154);
    if (marflg[5] && z == 99 && loc != 38)
	speak(155);
    /* the first time they come close */

    if (!marflg[5] && loc < 51) {
	marflg[5] = 1;
	speak(142);
    }
    if (loc == 28) {
	if (marflg[3])
	    speak(143);
    } else if (loc >= 43 && loc <= 45) {
	if (marflg[4])
	    speak(144);
    }
    /* at home */

    else if (loc == 42 || loc == 30) {
	if (marflg[0]) {
	    if (!marflg[6] || pct(20)) {
		speak(145);
		marflg[6] = 1;
	    } else
		speak(146);
	}
    }
    /* the hockey game  */

    else if (loc == 29 || (loc >= 31 && loc <= 38)) {
	if (marflg[1]) {
	    if (!marflg[7] || pct(20)) {
		speak(147);
		marflg[7] = 1;
	    } else
		speak(148);
	}
    } else if (loc == 40) {
	if (marflg[2])
	    speak(149);
    } else;
    if (((loc == 42 && marflg[0]) || (loc == 38)) && !deadf) {
	/* oops! he ran inot one and got fried  */

	if (loc == 42)
	    speak(150);
	if (loc == 38) {
	    if (marflg[1])
		speak(151);
	    else
		speak(171);
	    for (i = 32; i <= 37; i++)
		locdat[i] |= 16384;
	}
	vdead();
	oldlc2 = 0;
	oldloc = 0;
    }
    /* move his belongings to the dump */

lab60:
    for (i = 1; i <= MOVMAX; i++) {
	if (obloc[i] == loc || (obloc[i] == 48 && loc == 49) || obloc[i] < 27 ||
	    obloc[i] > 50)
	    continue;
	if (i == ZWIRE) {
	    if (obloc[i] == 1000 || wirelc[0] == 1000 || wirelc[5]
		== 1000 || wirelc[0] == loc || wirelc[1] == loc
		|| wirelc[2] == loc || wirelc[3] == loc
		|| wirelc[4] == loc || wirelc[5] == loc)
		continue;
	    obimpr[ZWIRE] = (obimpr[ZWIRE] & ~56) + 8;
	    /* ~56=177707 octal */

	    for (kkwr = 0; kkwr < 6; kkwr++)
		wirelc[kkwr] = 0;
	}
	if (obloc[i] >= 27 && obloc[i] <= 50)
	    obloc[i] = 40;
	if (i == HPUCK)
	    obloc[i] = 38;
	if (i == ZDIAMO) {
	    obimpr[i] = (obimpr[i] & ~56) + 8;
	    obloc[i] = 49;
	}
	if (i == RBOULD)
	    obloc[i] = 27;
    }
    return;
}

wdemon()
{
    int             i, n, i2, vloc;
    /*
     * take care of playing out and taking in the wire take it in  
     */

    if (wirelc[5] == 1000) {
	for (i = 1; i <= 4; i++) {
	    n = 5 - i;
	    if (wirelc[n] == oldloc && wirelc[n - 1] == loc) {
		wirelc[n] = 1000;
		speak(216);
		if (wirelc[1] == 1000 && obloc[ZWIRE] == 1000 && wirelc[0] == loc) {
		    for (i2 = 0; i2 < 5; i2++)
			wirelc[i2] = 0;
		    speak(219);
		}
		return;
	    }
	}
	if (wirelc[1] == 1000 && wirelc[0] == oldloc && obloc[ZWIRE]
	    == loc) {
	    wirelc[0] = 1000;
	    speak(216);
	    return;
	}
	/* play it out  */

	if (loc >= 32 && loc <= 38) {
	    speak(257);
	    if (oldloc == 31 || oldloc == 32)
		vloc = 32;
	    else
		vloc = 37;
	    for (i = 0; i < 6; i++)
		if (wirelc[i] == 1000)
		    wirelc[i] = vloc;
	    return;
	}
	for (i = 0; i < 4; i++) {
	    if (wirelc[i] == oldloc && wirelc[i + 1] == 1000) {
		wirelc[i + 1] = loc;
		speak(217);
		return;
	    }
	}
	if (wirelc[0] == 1000 && obloc[ZWIRE] == oldloc) {
	    wirelc[0] = loc;
	    speak(217);
	    return;
	}
	/* out of wire   */

	if (wirelc[4] == oldloc) {
	    speak(218);
	    wirelc[5] = wirelc[4];
	    return;
	}
    } else;
}


timer(xloc)
    int             xloc;
{
    int             bplce, cplce, gplce, iplce, objen, qq, i;

    /* prevent him from being outside the valley at sunset */

    if (loc <= 3 && loc * 15 < turns) {
	speak(255);
	oldlc2 = oldloc;
	oldloc = loc;
	loc += 1;
    }
    /*
     * daytim=1 if it is intrinsically light here (i.e. lights are always on
     * or it is outside and the sun is up or he is carrying sphere or bead  
     */

    if ((locdat[loc] & 17) != 0 || ((locdat[loc] & 8) != 0
			     && (turns % 100) < 75) || obloc[GSPHER] == 1000
	|| obloc[GSPHER] == loc || obloc[ZBEAD] == 1000 ||
	obloc[ZBEAD] == loc)
	daytim = 1;
    else
	daytim = 0;
    /* he is killed if on tower in daytime  */

    if (loc == 49 && daytim == 1 && !deadf) {
	speak(166);
	oldlc2 = 0;
	oldloc = 0;
	loc = 48;
	vdead();
    }
    /* announce rising and setting of both suns  */

    if ((locdat[loc] & 8) != 0) {
	if (daytim == 0)
	    speak(124);
	else if ((turns % 100) >= 72 && (turns % 100) < 75)
	    speak(125);
	else if ((turns % 100) < 3 && turns > 4)
	    speak(126);
	else;
    }
    /*
     * check to see if he is not wearing shoes or boots if too long without
     * them, he dies  
     */

    if ((obloc[CSHOES] != 3000 && obloc[HBOOTS] != 3000) &&
	!deadf && (loc != 16 && loc < 153)) {
	noshoe -= 1;
	if (noshoe == 0)
	    speak(99);
	else if (noshoe < -6 && pct(10 * (-6 - noshoe))) {
	    speak(100);
	    vdead();
	}
    }
    /* fish need water to live!  */

    if (obloc[DFISH] == 0) {
	if (obloc[CFISH] != 16 && !(obloc[CFISH] == (2000 + PLBAG)
				    && (((obimpr[PLBAG] / 512) % 8) == 5))) {
	    fshlif -= 1;
	    if (fshlif == 0) {
		obloc[DFISH] = obloc[CFISH];
		obloc[CFISH] = 0;
		obimpr[CFISH] = 0;
		obimpr[DFISH] = 137;
	    }
	}
    }
    /* check on status of bug repellent  */

    if (obimpr[ZDEET] >= 4096)
	obimpr[ZDEET] -= 4096;
    /* check on radioactive exposure of film in camera  */

    itsher(ZBEAD, &bplce);
    itsher(GSPHER, &gplce);
    if (oextim < 8000) {
	itsher(ICAMER, &iplce);
	if (obloc[GSPHER] == obloc[ICAMER])
	    oextim += 1;
	else {
	    if (iplce != 0 && gplce != 0)
		oextim += 1;
	}
	if (obloc[ZBEAD] == obloc[ICAMER])
	    oextim += 7;
	else {
	    if (iplce != 0 && bplce != 0)
		oextim += 4;
	}
    }
    /* check on radiation poisoning  */

    if (!deadf && bplce != 0 && obloc[ZBEAD] != 2000 + ZSINK) {
	rdietm = (rdietm / 5) * 5;
	rdietm += 5;
	if (rdietm > 0 && (rdietm % 10) == 0)
	    speak(rdietm / 10 + 320);
    } else {
	rdietm -= 1;
	if (rdietm <= 0)
	    rdietm = 0;
	if (rdietm == 36)
	    speak(326);
	if (rdietm == 24)
	    speak(327);
	if (rdietm == 12)
	    speak(328);
    }
    /* check on changing of loc 89 */

    if (easttm == 1)
	speak(376);
    if (easttm == 15)
	speak(377);
    if (easttm == 50)
	speak(378);
    if (easttm >= 1 && easttm <= 50)
	easttm += 1;
    if (!deadf)
	eattim += 1;
    if (eattim == 170)
	speak(367);
    if (eattim == 240)
	speak(368);
    if (eattim == 270)
	speak(369);
    if (eattim == 300) {
	speak(384);
	vdead();
    }
    /* exposing the x-ray film  */

    itsher(ZCASSE, &cplce);
    if (obloc[ZCASSE] == 2000 + ZRACK && obloc[ZBEAD] == 2000 + ZTRAY) {
	objen = 0;
	for (i = 0; i <= MOVMAX; i++) {
	    if (obloc[i] == ZENLAR + 2000) {
		if (fimage != 0 && fimage != i)
		    filmst = 1;
		qq = i;
		objen += 1;
	    }
	}
	if (objen > 1)
	    filmst = 1;
	if (objen == 0)
	    filmtm += 10;
	if (objen >= 1) {
	    filmtm += 1;
	    fimage = qq;
	}
    } else if (obloc[ZBEAD] == obloc[ZCASSE] || (obloc[ZBEAD] != ZSINK
					+ 2000 && bplce != 0 && cplce != 0))
	filmtm += 1000;
    else;
    if (filmtm > 3000)
	filmtm = 3000;
    /*
     * dispersing of the cloud from the seed we are using obimpr(zseed) as a
     * clock; this is ok since it no longer exists (  o.e. obloc(zseed)=0 )
     * despite this being stupid  
     */

    if (obloc[ZSEED] == 0 && obimpr[ZSEED] > 0) {
	if ((loc == 99 || loc == 131) && obimpr[ZSEED] == 1)
	    speak(406);
	obimpr[ZSEED] -= 1;
    }
    if (screef > 0)
	screef -= 1;
    /* the chase is on!!!!    */

    if (chaset != 0) {
	if (adverb == QUICKL)
	    chaser += 1;
	if (chaset <= 8) {
	    if (xloc == loc || loc == oldlc2 || loc == 174 || loc == 166) {
		if (chaset != 1) {
		    speak(557);
		    vdead();
		} else
		    speak(554);
	    } else if (chaset >= 5 && chaser < 3)
		speak(556);
	    else
		speak(555);
	} else if (chaset == 9) {
	    if (chaser >= 3) {
		speak(558);
		guardl = loc;
	    } else {
		speak(557);
		vdead();
	    }
	} else if ((chaset > 9 && chaset < 18) && loc == guardl) {
	    speak(563);
	    vdead();
	} else if (loc < 176 && chaset == 18) {
	    if (!(loc == 175 && (obimpr[BDOOR] & 2) != 0)) {
		speak(560);
		vdead();
	    }
	}
	chaset += 1;
	if (chaset == 19)
	    chaset = 0;
    }
}
