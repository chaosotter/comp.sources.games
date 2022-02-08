#define EXTERN extern

#include "parame.inc"
#include "variab.h"
#include "arrays.h"

/* World C Version 1.00 copyright 1987 J.D.McDonald 
   Use as you like for non-commercial purposes, but please
   leave this note, and document any changes you make as yours */


vattac()
{
    int             i, spk, cplce, n, plce;
    /*
     * "attach" or "tie"   not "attack" a mess since it involves the wire  
     */

    if (dobjs[0] == CABLE && iobj == ANTENN) {
	if (loc != 69)
	    spk = 379;
	else if (obloc[GCABLE] == 2000 + ZANTEN || obloc[DCABLE] == 2000 + ZANTEN)
	    spk = 381;
	else {
	    n = getobj(dobjs[0], doadjs[0]);
	    if (n <= 0)
		spk = 70;
	    else {
		itsher(n, &cplce);
		if (cplce != 1 && cplce != 3 && cplce != 4)
		    spk = 380;
		else {
		    obloc[n] = 2000 + ZANTEN;
		    spk = 75;
		}
	    }
	}
    } else if (dobjs[0] != WIRE)
	spk = 48;
    else if (iobj == TREE || iobj == LOG || iobj == STOOL)
	spk = 204;
    else if (iobj != KNOB && iobj != TOWER)
	spk = 48;
    else {
	itsher(ZWIRE, &plce);
	if (plce == 0)
	    spk = 213;
	else if (iobj == TOWER) {
	    if (loc != 48)
		spk = 205;
	    else {
		spk = 75;
		obloc[ZWIRE] = loc;
		/* -57=177707 octal  */

		obimpr[ZWIRE] = (obimpr[ZWIRE] & -57) + 24;
		for (i = 0; i < 6; i++)
		    wirelc[i] = 1000;
	    }
	} else {
	    if (loc != 42)
		spk = 206;
	    else if (spcloc == 1) {
		spk = 75;
		obloc[ZWIRE] = loc;
		/* ~56=177707 octal  */

		obimpr[ZWIRE] = (obimpr[ZWIRE] & ~56) + 32;
		for (i = 0; i < 6; i++)
		    wirelc[i] = 1000;
	    } else
		spk = 207;
	}
    }
    speak(spk);
}

vattak()
{
    int             spk;
    spk = 286;
    if (dobjs[0] == VINE && loc == 74 && (obimpr[ZVINE] & 56) == 8) {
	spk = 306;
	if (pct(33))
	    spk = 302;
	else if (pct(50))
	    spk = 305;
    } else if (dobjs[0] == GUARD) {
	if (loc == guardl || loc == 171 || (chaset != 0 && chaset <= 9))
	    spk = 562;
	else
	    spk = 561;
    } else if (dobjs[0] == MARTIA && loc >= 27 && loc <= 51)
	spk = 307;
    else if (dobjs[0] == ROBOT || dobjs[0] == FERRET)
	spk = 285;
    speak(spk);
}

vbreak()
{
    int             j, hplce, xplce;
    if (iobj != 0 && iobj != HAMMER && iobj != BOULDE) {
	speak(48);
	return;
    }
    if (iobj == HAMMER) {
	itsher(ZHAMME, &hplce);
	if (hplce == 0) {
	    speak(329);
	    return;
	}
    }
    if (iobj == BOULDE) {
	speak(330);
	return;
    }
    if (dobjs[0] == LOUVER && (loc == 98 || loc == 142)) {
	speak(282);
	obimpr[ZLOUVE] = 9;
	if (iobj == HAMMER)
	    obloc[ZHAMME] = 1000;
	return;
    }
    if (dobjs[0] == FERRET || dobjs[0] == ROBOT || dobjs[0] ==
	MARTIA || dobjs[0] == CAT || dobjs[0] == FISH) {
	speak(286);
	return;
    }
    if (dobjs[0] == GUARD) {
	if (loc == guardl || loc == 171 || (chaset != 0 && chaset <= 9))
	    speak(562);
	else
	    speak(561);
	return;
    }
    j = getobj(dobjs[0], doadjs[0]);
    if (j == -1) {
	speak(70);
	return;
    } else {
	itsher(j, &xplce);
	if (xplce == 0)
	    speak(66);
	else if (j > MOVMAX)
	    speak(332);
	else if (dobjs[0] == WIRE || dobjs[0] == KNAPSA || dobjs[0] ==
		 SHIRT || dobjs[0] == PANTS || dobjs[0] == BOULDE ||
		 dobjs[0] == PUCK || dobjs[0] == LATEX || dobjs[0] ==
		 NUGGET || dobjs[0] == KNIFE || dobjs[0] == BEAD) {
	    speak(48);
	    return;
	} else if ((dobjs[0] == KEY || dobjs[0] == SAPPHI || dobjs[0] ==
		    DIAMON || dobjs[0] == SPHERE) && iobj != HAMMER)
	    speak(385);
	else if (j == ZSEED) {
	    if (loc != 131 && loc != 99 && loc != 100) {
		speak(337);
		obimpr[ZSEED] = 0;
	    } else if (loc != 100) {
		speak(338);
		obimpr[ZSEED] = 0;
	    } else {
		speak(340);
		obimpr[ZSEED] = 3;
		/* seed function here  */

	    }
	    obloc[ZSEED] = 0;
	} else if (j == ZSTATU && loc == 99 && obloc[j] == 99)
	    speak(89);
	else if (j == CEGG) {
	    speak(480);
	    obloc[CEGG] = 0;
	    obloc[SCOIN] = loc;
	} else {
	    speak(331);
	    if (j == GSPHER) {
		speak(333);
		obloc[ZBEAD] = obloc[j];
	    }
	    if (obloc[j] == 2000 + RSLOT)
		obimpr[RSLOT] = 9;
	    obloc[j] = 0;
	}
    }
}

vdig()
{
    int             splce, pplce;
    if (dobjs[0] != 0 && dobjs[0] != SHOVEL && dobjs[0] != PITCHF)
	speak(48);
    else if (loc != 73)
	speak(284);
    else {
	itsher(ZSHOVE, &splce);
	itsher(ZPITCH, &pplce);
	if ((dobjs[0] == SHOVEL && splce != 0) ||
	    (dobjs[0] == PITCHF && pplce != 0) ||
	    (dobjs[0] == 0 && (splce > 0 || pplce > 0))) {
	    if (obimpr[RHOLE] != 17)
		speak(283);
	    else
		speak(434);
	    obimpr[RHOLE] = 17;
	} else if ((dobjs[0] == SHOVEL && splce == 0) || (dobjs[0] ==
						      PITCHF && pplce == 0))
	    speak(89);
	else
	    speak(234);
    }
}


vdrink()
{
    int             spk;
    /* pervert!!!!  */

    if (dobjs[0] == NPISS) {
	dirty += 1;
	spk = 183;
    }
    /* water is o.k. */

    else if (dobjs[0] == WATER) {
	if ((locdat[loc] & 1024) == 1024)
	    spk = 184;
	else
	    spk = 74;
    } else
	spk = 48;
    speak(spk);
}

vdrop()
{
    int             d, i, ambig, ddflg, empty, plce, plural, kkwr, kk, spk;

    /* special code for "drop something into bowl" */

    if (prepio != 0 && !(prepio == IN && iobj == BOWL)) {
	speak(28);
	return;
    }
    /* convert dobjs(1) (noun) list into list of objects (pointers) */

    ambig = !cnvobj();
    if (allflg)
	getall();
    for (i = 0; i < 30; i++) {
	ddflg = 0;
	empty = 1;
	d = dobjs[i];
	if (d == 0)
	    continue;
	/* see if it is here */

	itsher(d, &plce);
	if (allflg && ((plce != 1 && plce != 2) || d > MOVMAX))
	    continue;
	if (dobjs[1] != 0 || allflg)
	    speak(odistb[d]);
	plural = (obpprp[d] & 256) == 256;
	if (d > MOVMAX)
	    spk = 48;
	else if (plce == 2) {
	    spk = 88;
	    if (plural)
		spk = 194;
	} else if (d == ZWIRE && plce != 1) {
	    for (kkwr = 0; kkwr < 6; kkwr++)
		if (wirelc[kkwr] == 1000)
		    wirelc[kkwr] = loc;
	    if (obloc[d] == loc && wirelc[5] == loc) {
		for (kkwr = 0; kkwr < 6; kkwr++)
		    wirelc[kkwr] = 0;
	    }
	    spk = 82;
	} else if (plce == 0 || plce == 4 || plce == 5) {
	    spk = 89;
	    if (plural)
		spk = 195;
	} else if (d == ZSEED && (wrdnum[0] == DROP || wrdnum[0] == THROW)) {
	    if (loc == 100) {
		spk = 340;
		obimpr[ZSEED] = 0;
	    } else if (loc == 131 || loc == 99) {
		spk = 338;
		obimpr[ZSEED] = 3;
	    } else {
		obimpr[ZSEED] = 0;
		spk = 337;
	    }
	    obloc[ZSEED] = 0;
	} else if (d == CFISH && obloc[d] ==
		   (2000 + PLBAG) && (obimpr[PLBAG] / 512 == 5))
	    spk = 76;
	/*
	 * code for knocking the puck out of the arena loc38 is center of
	 * bowl, 23-37 are the rim  
         */

	else if (iobj == BOWL) {
	    if (loc < 32 || loc > 38)
		spk = 156;
	    else {
		if (d == RBOULD && loc != 38) {
		    itsher(RBOULD, &plce);
		    /* marflg[1] tells if martians are there */

		    if (marflg[1]) {
			speak(157);
			vdead();
			oldloc = 0;
			oldlc2 = 0;
			return;
		    } else {
			if (obloc[HPUCK] == 38) {
			    /*
			     * the next line tells if other rim is low or
			     * high 
                             */

			    if ((loc & 1) == 1) {
				/* we shoot the puck out */

				spk = 158;
				obloc[RBOULD] = 38;
				obloc[HPUCK] = loc + 3;
				if (obloc[HPUCK] > 37)
				    obloc[HPUCK] -= 6;
			    } else {
				spk = 159;
				obloc[RBOULD] = 38;
			    }
			} else {
			    /* in this  case it settles back in  */

			    if ((loc & 1) == 1) {
				spk = 160;
				obloc[RBOULD] = loc + 3;
				if (obloc[RBOULD] > 37)
				    obloc[RBOULD] -= 6;
			    } else {
				spk = 161;
				obloc[RBOULD] = 38;
			    }
			}
			/* smash the other items in the bowl  */

			for (kk = 1; kk <= MOVMAX; kk++) {
			    if (kk == RBOULD || kk == HPUCK)
				continue;
			    if (obloc[kk] == 38) {
				obloc[kk] = 0;
				empty = 0;
			    }
			}
		    }
		} else {
		    spk = 75;
		    obloc[d] = 38;
		    ddflg = 1;
		}
	    }
	} else {
	    if (d == ZCAT)
		spk = 334;
	    else if (d == RBOULD)
		spk = 335;
	    else if (d == ZORCHI && loc == 87)
		spk = 432;
	    else if (d == ZFLYTR && loc == 87)
		spk = 433;
	    else
		spk = 82;
	    obloc[d] = loc;
	    ddflg = 1;
	}
	if (d == ZLATEX && ddflg == 1)
	    obimpr[d] = 337;
	xindnt += 2;
	speak(spk);
	xindnt -= 2;
	if (empty != 1) {
	    xindnt += 4;
	    speak(162);
	    xindnt -= 4;
	}
    }
}

veat()
{
    int             d, l, spk, plce;
    /* don't eat shit  */

    if (dobjs[0] == NSHIT) {
	dirty += 1;
	spk = 179;
    }
    /* id love for you to do it, but please don't make mama mad  */

    else if (dobjs[0] == ME) {
	dirty += 1;
	spk = 183;
    } else {
	d = getobj(dobjs[0], doadjs[0]);
	/* see if its here */

	if (d <= 0) {
	    speak(48);
	    return;
	}
	itsher(d, &plce);
	if (plce <= 0) {
	    spk = 66;
	    if ((obpprp[d] & 256) == 256)
		spk = 185;
	} else {
	    /* poison!   */

	    if ((obpprp[d] & 8) == 8) {
		if (d == CFISH && obloc[d] ==
		    (2000 + PLBAG) && (obimpr[PLBAG] / 512 == 5)) {
		    speak(76);
		    return;
		}
		speak(181);
		oldloc = 0;
		oldlc2 = 0;
		if (d <= MOVMAX)
		    obloc[d] = 0;
		vdead();
		return;
	    }
	    /* it's edible, and delicious   */

	    else if ((obpprp[d] & 4) == 4) {
		spk = 180;
		if (d <= MOVMAX)
		    obloc[d] = 0;
		if (d != BBALL)
		    eattim = 0;
		if (d == CEGG) {
		    spk = 478;
		    obloc[SCOIN] = 1000;
		}
		if (d == LETTUC)
		    eattim = 150;
		if (d == BBALL) {
		    lpill = 1;
		    spk = 522;
		    bonus += 5;
		    dcombi = (qrand() % 31);
		    if (dcombi == 0)
			dcombi = 1;
		    l = (qrand() % 31);
		    if (l == 0)
			l = 22;
		    dcombi = dcombi * 32 + l;
		    l = (qrand() % 31);
		    if (l == 0)
			l = 3;
		    dcombi = dcombi * 32 + l;
                }
		/* inedible  */

	    } else
		spk = 182;
	}
    }
    speak(spk);
}



vfill()
{
    int             result, z, z2, spk, i, empty, plce;
    result = 0;
    z = dobjs[0];
    spk = 48;
    if (dobjs[0] == FUNNEL && iobj == WATER) {
	result = 1;
	dobjs[0] = WATER;
	iobj = FUNNEL;
	ioadj = doadjs[0];
	doadjs[0] = 0;
	prepio = IN;
	return (result);
    }
    if (dobjs[0] == TUBE) {
	speak(494);
	return (result);
    }
    /* fill the bag or bucket with water or latex  */

    empty = 1;
    if (z == BUCKET) {
	for (i = 1; i <= OBJMAX; i++)
	    if (obloc[i] == 2039)
		empty = 0;
    }
    if (z == BUCKET && iobj == LATEX && obloc[ZLATEX] == 66 &&
	prepio == WITH && loc == 66) {
	if (obimpr[ZBUCKE] == 1033)
	    spk = 274;
	else if (empty == 0)
	    spk = 275;
	else {
	    obloc[ZLATEX] = 2000 + ZBUCKE;
	    obimpr[ZBUCKE] = 1545;
	    obimpr[ZLATEX] = 265;
	    spk = 75;
	}
    } else if ((z == BAG && (doadjs[0] == 0 || doadjs[0] == PLASTI)) ||
	       (z == BUCKET && doadjs[0] == 0)) {
	if ((prepio == WITH && iobj == WATER) ||
	    (((locdat[loc] & 1024) == 1024) && prepio == 0
	     && iobj == 0)) {
	    z2 = ZBUCKE;
	    if (z == BAG)
		z2 = PLBAG;
	    itsher(z2, &plce);
	    if (plce == 0)
		spk = 73;
	    else if ((locdat[loc] & 1024) != 1024 && ((obloc[ZBUCKE]
			  != loc && obloc[ZBUCKE] != 1000) || obimpr[ZBUCKE]
						      != 1033))
		spk = 74;
	    else if (empty == 1) {
		if (z2 == PLBAG)
		    obimpr[z2] = 2697;
		else
		    obimpr[z2] = 1033;
		spk = 75;
		obloc[z2] = 1000;
	    } else
		spk = 275;
	} else if (iobj == 0)
	    spk = 278;
    }
    speak(spk);
    return (result);
}

vfuck()
{
    if (dirty < 10)
	speak(495);
    else if (dirty > 20)
	speak(497);
    else
	speak(496);
}


vget()
{
    int             spk, result;
    result = 0;
    spk = 0;
    if (prepdo == IN && dobjs[0] == CHAIR && (loc == 166 ||
					      loc == 175)) {
	spcloc = 2;
	spk = 510;
    }
    /* get up is a joke  */

    else if (prepdo == UP) {
	if (dobjs[0] == 0 && spcloc == 2) {
	    spk = 511;
	    spcloc = 0;
	} else
	    spk = 208;
    }
    /* get on or off glass plate   */

    else if (dobjs[0] == GLASS && (doadjs[0]
			     == 0 || doadjs[0] == GLASS) && dobjs[1] == 0) {
	if (prepdo == OFF && spcloc == 1) {
	    spcloc = 0;
	    spk = 75;
	} else if (prepdo == ON && spcloc == 0) {
	    spcloc = 1;
	    spk = 75;
	}
    }
    /* get something  means take it */

    else if (prepdo == ON || prepdo == OFF)
	spk = 71;
    else
	result = 1;
    /* this results in calling vtake  */

    if (spk != 0)
	speak(spk);
    return (result);
}

vgive()
{
    int             spk, ij, iplce, dj, dplce;
    spk = 0;
    if (iobj == ME)
	spk = 445;
    else if (iobj == GUARD) {
	if (loc == guardl || loc == 171 || (chaset != 0 &&
					    chaset <= 9))
	    spk = 562;
	else
	    spk = 561;
    } else if (iobj == MARTIA) {
	if (martim == 7 && loc >= 27 && loc <= 51)
	    spk = 446;
	else
	    spk = 168;
    } else if (iobj == FERRET && !(loc == 131 || loc == 99
				   || loc == 100))
	spk = 447;
    else if (iobj != CAT && iobj != BIRD && iobj != BEES &&
	     iobj != FROG && iobj != ROBOT)
	spk = 448;
    else {
	ij = getobj(iobj, ioadj);
	if (iobj == BIRD && ij < 0)
	    ij = YBIRD;
	itsher(ij, &iplce);
	if (iplce == 0) {
	    if (iobj == CAT)
		spk = 449;
	    else if (iobj == BEES)
		spk = 450;
	    else if (iobj == FROG)
		spk = 451;
	    else if (iobj == ROBOT)
		spk = 452;
	    else if (iobj == BIRD)
		spk = 453;
	} else {
	    if (iobj == FROG)
		spk = 454;
	    else if (iobj == BIRD)
		spk = 455;
	}
    }
    if (spk != 0) {
	speak(spk);
	return;
    }
    dj = getobj(dobjs[0], doadjs[0]);
    if (dj < 0)
	spk = 70;
    else if (dj == 0)
	spk = 28;
    else {
	itsher(dj, &dplce);
	if (dplce == 0) {
	    if ((obimpr[dj] & 256) != 0)
		spk = 185;
	    else
		spk = 89;
	} else if (obloc[dj] == 2000 + ij)
	    spk = 456;
	else if (dj > MOVMAX)
	    spk = odistb[dj + 1] - 1;
	else if (dplce > 3)
	    spk = 458;
	else;
    }
    if (spk != 0) {
	speak(spk);
	return;
    }
    /* at this point you have it, and someone is here to take it. */

    if (iobj == CAT) {
	if (dobjs[0] == FISH) {
	    spk = 459;
	    obloc[dj] = 0;
	} else if ((obpprp[dj] & 12) != 0)
	    spk = 460;
	else
	    spk = 461;
    } else if (iobj == FERRET) {
	if (dobjs[0] == EGG || dobjs[0] == FISH || dobjs[0] == CAT) {
	    spk = 462;
	    obloc[dj] = 0;
	} else if ((obpprp[dj] & 12) != 0)
	    spk = 464;
	else {
	    spk = 463;
	    obloc[dj] = 0;
	}
    } else if (iobj == BEES) {
	if (iobj != ORCHID && iobj != FLYTRA)
	    spk = 465;
	else if (iobj == ORCHID) {
	    spk = 432;
	    obloc[dj] = loc;
	} else {
	    spk = 433;
	    obloc[dj] = loc;
	}
    } else if (iobj == ROBOT) {
	if (obloc[ZCART] != RSLOT + 2000 && obloc[CCART] != RSLOT + 2000)
	    spk = 466;
	else if (obw3[dj] / 256 > 1 || (obw3[dj] & 255) > 2)
	    spk = 352;
	else {
	    spk = 345;
	    obloc[dj] = 2000 + ZROBOT;
	}
    }
    speak(spk);
}

vhelp()
{
    speak(105);
}


vkiss()
{
    int             result, spk, cplce;
    /* if vkiss ends up 1, call vpush */

    result = 0;
    if (dobjs[0] == FROG && loc == 67)
	spk = 468;
    else if (dobjs[0] == GUARD) {
	if (loc == guardl || loc == 171 || (chaset != 0 && chaset <= 9))
	    spk = 562;
	else
	    spk = 561;
    } else if (dobjs[0] == FERRET && (loc == 131 || loc == 100 || loc == 99))
	spk = 469;
    else if (dobjs[0] == CAT) {
	itsher(ZCAT, &cplce);
	if (cplce != 0)
	    spk = 470;
	else
	    spk = 449;
    } else if (dobjs[0] == ME)
	spk = 471;
    else if (dobjs[0] == BIRD || dobjs[0] == BEES || dobjs[0] == ROBOT)
	spk = 472;
    else
	result = 1;
    if (result == 0)
	speak(spk);
    return (result);
}

vlocks()
{
    int             n, plce, spk;
    n = getobj(dobjs[0], doadjs[0]);
    if (n < 0)
	spk = 70;
    else if (n == 0)
	spk = 28;
    else if ((obpprp[n] & 512) != 512 && n != ZBOX)
	spk = 233;
    else {
	itsher(n, &plce);
	if (plce <= 0) {
	    if (dobjs[0] == DOOR)
		spk = 225;
	    else
		spk = 66;
	} else if (n == ZBOX)
	    spk = 389;
	else if (n == ODOOR || n == PDOOR)
	    spk = 524;
	else if (prepio == WITH && iobj == KEY) {
	    if (n == RDOOR || (n == MDOOR && loc == 41))
		spk = 235;
	    else if (n == MDOOR || n == GDOOR) {
                if(obloc[CKEY] == 1000 || obloc[CKEY] == 3000) { 
 		    if (wrdnum[0] == LOCK)
		        obimpr[n] |= 4;
		    else
		        obimpr[n] = (obimpr[n] & ~4);
		    /* ~4=177773 octal */
		spk = 75;
                } else
                spk = 104;
	    } else
		spk = 48;
	} else
	    spk = 234;
    }
    speak(spk);
}

    static   char   strng3[] = "The left dial is set to     and the right to    .";
    static   char   strng4[] = "are     for the left and     for the right.";
vlooks()
/*
 * look or examine 
 */

{
    char            cnum[4];

    int             kk, zplce, spk, kkwr, lwire, n, plce, u, empty;

    if ((prepdo == THROUG && dobjs[0] == TELESC && wrdnum[0] ==
	 LOOK) || (wrdnum[0] == EXAMIN && dobjs[0] == PLANET &&
		   prepio == WITH && iobj == TELESC)) {
	if (loc >= 175 && loc <= 181)
	    spk = loc + 326;
	else if (loc == 184 || loc == 187)
	    spk = 507;
	else
	    spk = 48;
	speak(spk);
	return;
    }
    if ((loc >= 43 && loc <= 45) && (dobjs[0] == HOLE || dobjs[0]
				     == DOOR)) {
	if (daytim == 1)
	    speak(173);
	else
	    speak(174);
	return;
    }
    if (dobjs[0] == HOUSE && (doadjs[0] == 0 || doadjs[0] == DOLL)
	&& prepdo == IN) {
	if (loc != 132)
	    speak(481);
	else if (obimpr[CEGG] > 4096) {
	    speak(479);
	    obloc[CEGG] = 132;
	} else
	    speak(480);
	return;
    }
    /* you can't examine things at night unless they glow  */

    if (daytim == 0 && dobjs[0] != SPHERE) {
	speak(120);
	return;
    }
    if (wrdnum[0] == EXAMIN)
	prepdo = AT;
    if (dobjs[0] == TELESC) {
	if ((loc >= 175 && loc <= 181) || loc == 184 || loc == 187)
	    speak(534);
	else
	    speak(535);
	return;
    }
    /* if you look behind horsetails, there is a cave  */

    if (prepio == 0 && prepdo == BEHIND && loc == 169 &&
	dobjs[0] == CABINE) {
	speak(523);
	return;
    }
    if (prepio == 0 && prepdo == BEHIND && loc == 24) {
	if (dobjs[0] == HORSET) {
	    speak(119);
	    horflg = 1;
	    return;
	} else if (dobjs[0] == WATERF) {
	    speak(493);
	    horflg = 1;
	    return;
	}
    }
    /*
     * in the (hockey) bowl you can only see if something is there, not what
     * it is  
     */

    if (prepdo == IN && dobjs[0] == BOWL && (loc >= 32 && loc <= 37)) {
	empty = 1;
	for (kk = 1; kk <= MOVMAX; kk++)
	    if (obloc[kk] == 38)
		empty = 0;
	if (empty == 1)
	    speak(7);
	else
	    speak(163);
	return;
    }
    if ((loc == 28) && (dobjs[0] == STOOLS || dobjs[0] == STOOL ||
	     dobjs[0] == BOWL) && (doadjs[0] == 0 || doadjs[0] == CERAMI)) {
	speak(176);
	return;
    }
    /* various martian things  */

    if (loc == 42 && (dobjs[0] == STOOLS || dobjs[0] == BOWL)) {
	speak(177);
	spcloc = 0;
	return;
    }
    if (loc == 30 && (dobjs[0] == BUILDI || dobjs[0] == WINDOW)
	&& prepdo == IN) {
	speak(174);
	return;
    }
    if (dobjs[0] == CACTUS && loc == 82 && wrdnum[0] == EXAMIN) {
	if (pct(25) && (cactsc & 2) == 0) {
	    speak(371);
	    cactsc += 2;
	    if (cactsc == 3)
		speak(439);
	} else if ((cactsc & 1) == 0) {
	    speak(370);
	    cactsc += 1;
	    if (cactsc == 3)
		speak(439);
	} else
	    speak(237);
	return;
    }
    if (dobjs[0] == DIAL) {
	itsher(ZBOX, &zplce);
	if (zplce == 0)
	    speak(388);
	else {
	    numcvt(dial1, cnum);
	    strng3[24] = cnum[1];
	    strng3[25] = cnum[2];
	    strng3[26] = cnum[3];
	    numcvt(dial2, cnum);
	    strng3[45] = cnum[1];
	    strng3[46] = cnum[2];
	    strng3[47] = cnum[3];
	    linout(strng3, 49);
	}
	return;
    }
    /*
     * special code for anything other than "look at object" or "look in
     * container" gotes above here 
     */

    n = getobj(dobjs[0], doadjs[0]);
    if (prepio != 0 || (prepdo != AT && prepdo != IN)) {
	speak(94);
	return;
    }
    if (n == 0) {
	speak(95);
	return;
    } else if (n < 0) {
	speak(70);
	return;
    }
    itsher(n, &plce);
    lwire = 0;
    /* special code for wire in several places at once  */

    if (n == ZWIRE) {
	for (kkwr = 0; kkwr < 6; kkwr++)
	    if (wirelc[kkwr] == loc)
		lwire = 1;
    }
    if (plce < 1 && !lwire) {
	if ((obpprp[n] & 256) == 256)
	    speak(185);
	else
	    speak(66);
	return;
    }
    if (plce < 1 && n == ZWIRE && lwire) {
	speak(215);
	return;
    }
    /* the generic "look at" processor  */

    if (prepdo == AT) {
	u = (obimpr[n] / 64 % 8);
	if (u == 0) {
	    if ((obpprp[n] & 256) == 256)
		speak(193);
	    else
		speak(96);
	}
	/* zzzzzzap if not standing on glass while looking at knob  */

	else if ((n == ZSTOOL || n == ZKNOB) && spcloc == 1)
	    speak(237);
	else if (n == ZANTEN) {
	    if (obloc[DCABLE] == 2000 + ZANTEN) {
		speak(odistb[n] + 1);
		obimpr[DCABLE] = 137;
	    } else if (obloc[GCABLE] == 2000 + ZANTEN)
		speak(odistb[n] + 2);
	    else
		speak(odistb[n] + 3);
	} else {
	    if ((n == MMAP || n == NNEWS || n == TMACH || n == NNOTE
		 ) && loc >= 153 && lpill == 1)
		u += 1;
	    speak(odistb[n] + u);
	    if (n == NNOTE && lpill == 1)
		nreadx();
	    if (n == ZSCREE && obimpr[n] == 145 && loc == 146)
		robdsc();
	    if (dobjs[0] == BOX) {
		numcvt(dial1, cnum);
		strng3[24] = cnum[1];
		strng3[25] = cnum[2];
		strng3[26] = cnum[3];
		numcvt(dial2, cnum);
		strng3[45] = cnum[1];
		strng3[46] = cnum[2];
		strng3[47] = cnum[3];
		linout(strng3, 49);
	    }
	    if (dobjs[0] == FILM && obimpr[ZFILM] == 201) {
		wwflag = 1;
		speak(odistb[fimage]);
		wwflag = 0;
		outst2[0] |= 32;
                outst2[kmax] = '.';
                outst2[kmax+1] = '\0';
		linout(outst2, kmax+1);
		if (fimage == ZBOX) {
		    speak(404);
		    numcvt(dial1x, cnum);
		    strng4[4] = cnum[1];
		    strng4[5] = cnum[2];
		    strng4[6] = cnum[3];
		    numcvt(dial2x, cnum);
		    strng4[25] = cnum[1];
		    strng4[26] = cnum[2];
		    strng4[27] = cnum[3];
		    linout(strng4, 49);
		}
	    }
	    /*
	     * special code for flags set by looking at or in object goes
	     * below here  
             */

	    if (n == PPHOTO)
		horflg = 1;
	}
    }
    if (obpprp[n] / 2048 != 0)
	xcontn(n);
    else if (prepdo == IN)
	speak(97);
    return;
}

vopen()
{
    int             spk, n, plce;
    if (prepdo != 0 && prepio != 0)
	spk = 21;
    /* there is no door on the bar to open or close  */

    else if ((loc == 30 || loc == 42) && dobjs[0] == DOOR)
	spk = 224;
    /* open doors or containers  */

    else {
	n = getobj(dobjs[0], doadjs[0]);
	if (dobjs[0] == DOOR && (loc == 184 || loc == 187 ||
				 (loc >= 176 && loc <= 181)))
	    n = BDOOR;
	if (n < 0)
	    spk = 70;
	else if (n == 0)
	    spk = 66;
	/* if they can be opened */

	else if ((obpprp[n] % 2) != 1) {
	    if (obpprp[n] / 2048 != 0)
		spk = 254;
	    else
		spk = 230;
	} else {
	    itsher(n, &plce);
	    if (dobjs[0] == DOOR && (loc == 184 || loc == 187 ||
				     (loc >= 176 && loc <= 181)))
		plce = 5;
	    if (plce <= 0) {
		if (dobjs[0] == DOOR)
		    spk = 225;
		else
		    spk = 66;
	    }
	    /* and aren't locked ,but are closed  */

	    else if ((obimpr[n] & 4) == 4) {
		if (n == ZBOX)
		    spk = 389;
		else
		    spk = 228;
	    } else if ((obimpr[n] & 2) == 0)
		spk = 226;
	    else if (dobjs[0] == DOOR && (loc <= 180 && loc >= 176))
		spk = 546;
	    else if (dobjs[0] == CABINE && adverb != CAREFU && adverb !=
		     QUIETL)
		spk = 553;
	    else {
		spk = 231;
		obimpr[n] = (obimpr[n] & ~2);
		/* ~2=177775 octal */

		if (n == RDOOR || n == MDOOR || n == GDOOR || n ==
		    ODOOR || n == BDOOR)
		    obimpr[n] += 8;
		if (n == ZDESK && dial1x == -1) {
		    /*
		     * establish the proper setting of the dials when the box
		     * is first seen 
                     */

		    dial1x = (qrand() % 999) + 1;
		    dial2x = (qrand() % 999) + 1;
		}
	    }
	}
    }
    speak(spk);
    if (spk == 553)
	vdead();
}


vpick()
{
    int             result;
    result = 0;
    /* pick up means take  */

    if (prepdo == UP) {
	prepdo = 0;
	result = 1;
    }
    /* this results in calling take  */

    else if (dobjs[0] == NLOCK && loc == 47)
	speak(235);
    else
	speak(95);
    return(result);
}


vpiss()
{
    dirty += 1;
    {
	if (loc == 74 && (obimpr[ZVINE] & 56) == 8 && (dobjs[0]
					== 0 || dobjs[0] == VINE)) {
	    speak(301);
	    obimpr[ZVINE] += 8;
	} else if (dirty >= 5 && dirty < 10)
	    speak(40);
	else if (dirty >= 10) {
	    if (obloc[LPANT] == 3000 || obloc[RPANT] == 3000)
		speak(41);
	    else
		speak(42);
	}
    }
}


vpour()
{
    int             result, bplce, pplce, n, plce;
    result = 0;
    /* if result set = 1, then call vput  */


    if (wrdnum[0] == QWATER) {
	prepio = ON;
	iobj = dobjs[0];
	ioadj = doadjs[0];
	dobjs[0] = WATER;
	doadjs[0] = 0;
    }
    if (dobjs[0] == DEET && prepio == ON && (iobj == ME
					  || (iobj == VINE && loc == 74))) {
	itsher(ZDEET, &plce);
	if (plce == 0)
	    speak(66);
	else {
	    speak(75);
	    obimpr[ZDEET] = 28762;
	    obloc[ZDEET] = 0;
	}
	return (result);
    }
    if ((dobjs[0] == DEET || dobjs[0] == METOL || dobjs[0] == HYPO)
	&& iobj == FUNNEL) {
	wrdnum[0] = PUT;
	prepio = IN;
	result = 1;
	return (result);
    }
    if (iobj != 0 && iobj != POLE) {
	n = getobj(iobj, ioadj);
	if (n >= 0) {
	    itsher(n, &plce);
	}
	if (iobj == FUNNEL && n < 0) {
	    speak(70);
	    return (result);
	}
	if (n <= 0 || plce == 0) {
	    speak(48);
	    return (result);
	}
    }
    itsher(ZBUCKE, &bplce);
    itsher(PLBAG, &pplce);
    if (dobjs[0] == WATER) {
	if (bplce != 0 && obimpr[ZBUCKE] == 1033) {
	    if (iobj == FUNNEL) {
		if (n == LFUNNE || n == RFUNNE) {
		    speak(402);
		    if (obimpr[RLIGHT] != 73) {
			speak(400);
			obimpr[RLIGHT] = 73;
			obimpr[GLIGHT] = 73;
		    }
		    machst |= machst;
		    return(result);
		} else {
		    if ((machst & 2) == 0)
			speak(397);
		    machst |= 2;
		    obimpr[ZINDIC] = 145;
		    if ((machst & 16) != 0 && (machst & 1) == 0 &&
                                       obimpr[RLIGHT] == 73) {
			speak(399);
			obimpr[RLIGHT] = 145;
			obimpr[GLIGHT] = 145;
		    }
		}
	    } else if (loc != 74 || (obimpr[ZVINE] & 56) != 8)
		speak(247);
	    else {
		speak(301);
		obimpr[ZVINE] = obimpr[ZVINE] + 8;
	    }
	    obimpr[ZBUCKE] = 1545;
	} else if (pplce != 0 && obimpr[PLBAG] == 2697)
	    speak(248);
	else
	    speak(48);
    } else if (dobjs[0] == LATEX && obloc[ZLATEX] == 2000 + ZBUCKE
	       && bplce != 0 && (obimpr[ZLATEX] & 56) == 8) {
	if (loc != 76 || iobj != POLE) {
	    speak(279);
	    obimpr[ZLATEX] = obimpr[ZLATEX] + 72;
	} else {
	    speak(75);
	    obimpr[ZLATEX] = obimpr[ZLATEX] + 80;
	}
	obloc[ZLATEX] = loc;
    } else
	speak(48);
    return (result);
}


vpush()
{
    int             result, numbr, ndo, nio, spk, xfilm;
    result = 0;
    /* push is also touch   */

    if ((loc == 165 || loc == 173) && dobjs[0] > 9999) {
	numbr = dobjs[0] - 10000;
	if ((loc == 165 && numbr > 30) || (loc == 173 && numbr > 99))
	    speak(532);
	else if (loc == 165) {
	    pbstat = (pbstat & 1023) * 32 + numbr;
	    if (pbstat == dcombi) {
		speak(530);
		/* ~4=177773 octal */

		obimpr[ODOOR] = (obimpr[ODOOR] & ~4);
	    } else
		speak(238);
	} else
	    speak(238);
	return (result);
    }
    if (dobjs[0] == CACTUS && loc == 82) {
	if (pct(25) && (cactsc & 2) == 0) {
	    speak(371);
	    cactsc = cactsc + 2;
	    if (cactsc == 3)
		speak(439);
	} else if ((cactsc & 1) == 0) {
	    speak(370);
	    cactsc = cactsc + 1;
	    if (cactsc == 3)
		speak(439);
	} else
	    speak(237);
	return (result);
    }
    if (dobjs[0] == GUARD) {
	if (loc == guardl || loc == 171 || (chaset != 0 && chaset <= 9))
	    speak(562);
	else
	    speak(561);
	return (result);
    }
    ndo = 0;
    nio = 0;
    if (dobjs[0] != 0)
	ndo = getobj(dobjs[0], doadjs[0]);
    if (iobj != 0)
	nio = getobj(iobj, ioadj);
    /*
     * unlocking the door to warehouse by touching wire to it if wire is
     * connected to knob in bar 
     */

    if ((ndo == MDIMPL && nio == ZWIRE && prepio == WITH) ||
	(ndo == ZWIRE && nio == MDIMPL && prepio == TO) &&
	loc == 47) {
	if (obloc[ZWIRE] != loc && wirelc[0] != loc && wirelc[1] !=
	    loc && wirelc[2] != loc && wirelc[3] != loc &&
	    wirelc[4] != loc && wirelc[5] != 1000 && obloc[ZWIRE]
	    != 1000)
	    spk = 213;
	/* first test is for connection to knob */

	else if (((obimpr[ZWIRE] / 8) % 8) == 4 && (wirelc[5] == 1000
						    || wirelc[5] == loc)) {
	    spk = 238;
	    /* ~4=177773 octal */

	    obimpr[RDOOR] = (obimpr[RDOOR] & ~4);
	} else
	    spk = 237;
    } else if ((ndo == ZKNOB || nio == ZKNOB) && loc == 42 && spcloc != 1)
	spk = 207;
    else if (ndo == ZLOUVE && (loc == 98 || loc == 142) &&
	     obimpr[ZLOUVE] == 1) {
	spk = 282;
	obimpr[ZLOUVE] = 9;
    } else if (dobjs[0] == BUTTON) {
	if (ndo == -1)
	    spk = 70;
	else if (ndo == SBUTTO && loc == 146) {
	    locdat[144] |= 16;
	    locdat[145] |= 16;
	    locdat[147] |= 16;
	    locdat[148] |= 16;
	    locdat[150] |= 16;
	    spk = 238;
	} else if (ndo == RBUTTO && loc == 146) {
	    speak(383);
	    result = 1;
	    return(result);
	} else if (ndo == HBUTTO && loc == 146) {
	    if (easttm == 0)
		easttm = 1;
	    spk = 238;
	} else if (ndo == TBUTTO && loc == 146) {
	    if (easttm == 51)
		easttm = 52;
	    spk = 238;
	} else if (ndo == GBUTTO && loc == 169) {
	    if (adverb == TWICE) {
		spk = 525;
		obimpr[HMURAL] = 209;
		bonus = bonus + 5;
		chaset = 1;
		oldlc2 = 0;
		oldloc = 0;
	    } else
		spk = 238;
	} else if (ndo == RDBUTT && loc == 143) {
	    if ((machst & 51) != 18)
		spk = 237;
	    else {
		speak(398);
		xfilm = (filmtm % 1000);
		if (xfilm >= 3 && xfilm <= 6 && filmtm < 2000 && machst == 30) {
		    if (filmst == 0)
			obimpr[ZFILM] = obimpr[ZFILM] + 64;
		    else
			obimpr[ZFILM] = obimpr[ZFILM] + 128;
		} else if ((machst & 12) != 12)
		    obimpr[ZFILM] = obimpr[ZFILM] + 192;
		else;
		obloc[ZFILM] = 2000 + ZHOPPE;
		machst = 33;
		return (result);
	    }
	} else if (loc == 134 && (ndo == VBUTTO || ndo == OBUTTO ||
				  ndo == WBUTTO || ndo == BBUTTO)) {
	    if (obloc[GDISC] == 2000 + ZRECES && rvtim != 2)
		spk = 411;
	    else if (obloc[GDISC] == 2000 + ZRECES && rvtim == 2 &&
		     obloc[GCABLE] != 2000 + ZANTEN)
		spk = 421;
	    else if (obloc[GDISC] == 2000 + ZRECES && rvtim == 2) {
		if (ndo == VBUTTO) {
		    spk = 415;
		    result = 2;
		} else if (ndo == OBUTTO) {
		    spk = 416;
		    result = 3;
		} else if (ndo == BBUTTO) {
		    spk = 417;
		    result = 4;
		} else {
		    spk = 418;
		    result = 5;
		}
	    } else
		spk = 238;
	} else
	    spk = 419;
    } else
	spk = 236;
    speak(spk);
    return (result);
}
