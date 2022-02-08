#define EXTERN extern

#include "parame.inc"
#include "variab.h"
#include "arrays.h"

/* World C Version 1.00 copyright 1987 J.D.McDonald 
   Use as you like for non-commercial purposes, but please
   leave this note, and document any changes you make as yours */

vpunch()
{
    int             result, spk;
    result = 0;
    if (dobjs[0] == SEED || dobjs[0] == SPHERE) {
	result = 1;
	return (result);
    }
    spk = 286;
    if (dobjs[0] == LOUVER) {
	if (loc == 98 || loc == 142) {
	    spk = 282;
	    obimpr[ZLOUVE] = 9;
	} else
            spk = 66;
    } else if (dobjs[0] == GUARD) {
	if (loc == guardl || loc == 171 || (chaset != 0 && chaset <= 9))
	    spk = 562;
	else
	    spk = 561;
    } else if (dobjs[0] == ROBOT || dobjs[0] == FERRET)
	spk = 285;
    else if (dobjs[0] == MARTIA && loc >= 27 && loc <= 51)
	spk = 307;
    else if (dobjs[0] == VINE && loc == 74 && (obimpr[ZVINE] & 56) == 8) {
	spk = 306;
	if (pct(33))
	    spk = 302;
	else if (pct(50))
	    spk = 305;
	else;
    }
    speak(spk);
    return (result);
}

vput()
{
    int             result, plce, zplce, spk, obj, cplce, xiobj, ioplce, ambig;
    int             csize, cinsid, i, ncarrd, nweigh, kcarrd, kweigh;
    int             z, size, weigh, plural;

    result = 0;
    if ((prepdo == ON && prepio == 0) || (prepio == ON && iobj == ME)) {
	prepio = 0;
	prepdo = 0;
	result = 1;
	/* this results in calling vwear  */

	return (result);
    }
    if ((prepio == IN && iobj == BOWL) || prepdo == DOWN) {
	/* call drop to put things in bowl  */

	if (prepdo == DOWN)
	    prepdo = 0;
	result = 2;
	return (result);
    }
    if (dobjs[0] == WATER && dobjs[1] == 0 && iobj == FUNNEL) {
	result = 3;
	return (result);
    }
    /* code for put latex on pole   */

    if (dobjs[0] == LATEX && (doadjs[0] == 0 || doadjs[0] == GOOEY)
	&& loc == 76 && prepio == ON && iobj == POLE &&
	(obimpr[ZLATEX] & 56) == 8) {
	if (allflg || dobjs[1] != 0) {
	    speak(48);
	    return (result);
	}
	itsher(ZLATEX, &plce);
	if (plce == 0)
	    speak(66);
	else {
	    obloc[ZLATEX] = 76;
	    obimpr[ZLATEX] = 25 + 320;
	    speak(75);
	}
	return (result);
    }
    /* putting things in machines   */

    if ((iobj == MACHIN || iobj == OPENIN || iobj == SLOT) &&
	loc != 146) {
	if (loc != 143 && loc != 156 && loc != 158 &&
	    loc != 160 && loc != 162)
	    speak(90);
	else if (loc == 143) {
	    if (prepio != IN || allflg || dobjs[1] != 0
		|| dobjs[0] != CASSET || (doadjs[0] != 0 &&
					  doadjs[0] != HUGE))
		speak(48);
	    else {
		itsher(ZCASSE, &zplce);
		if (zplce <= 0)
		    speak(66);
		else {
		    obloc[ZCASSE] = 0;
		    machst |= 16;
		    if (obimpr[RLIGHT] == 73 && (machst & 1) == 0 &&
			(machst & 2) != 0) {
			speak(399);
			obimpr[RLIGHT] = 145;
			obimpr[GLIGHT] = 145;
		    }
		}
	    }
	} else {
	    spk = 0;
	    if (prepio != IN || allflg || dobjs[1] != 0
		|| dobjs[0] != COIN)
		spk = 48;
	    else {
		obj = getobj(COIN, doadjs[0]);
		if (obj < 0) {
		    speak(70);
		    return (result);
		}
		itsher(obj, &cplce);
		if (cplce == 0)
		    spk = 66;
		else if (loc == 156) {
		    if (obj != PCOIN)
			spk = 93;
		    else {
			spk = 512;
			obloc[PCOIN] = 0;
			obloc[BBALL] = 156;
		    }
		} else if (loc == 158) {
		    if (obj != NCOIN)
			spk = 93;
		    else {
			spk = 513;
			obloc[NCOIN] = 0;
			obloc[NNEWS] = 158;
		    }
		} else if (loc == 162) {
		    if (obj != CCOIN)
			spk = 93;
		    else {
			spk = 514;
			obloc[CCOIN] = 0;
			obloc[MMAP] = 162;
		    }
		} else if (loc == 160)
		    gamble(obj);
		else;
		if (spk != 0)
		    speak(spk);
	    }
	}
	return (result);
    }
    spk = 0;
    if ((prepio != IN || (prepio == IN && prepdo != 0)) && !
	(prepio == ON &&
	 (iobj == ENLARG || iobj == TRAY) && loc == 143))
	spk = 273;
    else {
	/* check on status of indirect object  */

	xiobj = getobj(iobj, ioadj);
	if (xiobj <= 0)
	    spk = 90;
	else if (obpprp[xiobj] / 2048 == 0 && xiobj != ZENLAR)
	    spk = 77;
	else if (iobj == BUCKET && (obimpr[ZBUCKE] == 1033 || (
		   obloc[ZLATEX] == (2000 + ZBUCKE) && (obimpr[ZLATEX] & 56)
							       == 8)))
	    spk = 276;
	else {
	    itsher(xiobj, &ioplce);
	    if (ioplce != 1 && ioplce != 2 && ioplce != 4)
		spk = 91;
	}
    }
    if (spk != 0) {
	speak(spk);
	return (result);
    }
    /* convert dobjs(i) (noun) to list of objects (pointers) */

    ambig = !cnvobj();
    if (allflg)
	getall();
    csize = obpprp[xiobj] / 2048;
    if (csize == 15)
	csize = 70;
    cinsid = 0;
    for (i = 1; i <= MOVMAX; i++)
	if (obloc[i] == 2000 + xiobj)
	    cinsid += (obw3[i] % 256);
    burden(&ncarrd, &nweigh, &kcarrd, &kweigh);
    if (obloc[ZKNAPS] == 1000 || obloc[ZKNAPS] == 3000)
	nweigh += kweigh;
    for (i = 0; i <= 29; i++) {
	z = dobjs[i];
	if (z == 0)
	    return (result);
	if (obloc[z] == z + 2000)
	    continue;
	itsher(z, &plce);
	size = (obw3[z] % 256);
	weigh = obw3[z] / 256;
	plural = (obpprp[z] & 256) == 256;
	if (allflg && (z > MOVMAX || obloc[z] == (2000 + xiobj)))
	    continue;
	if (allflg || dobjs[1] != 0)
	    speak(odistb[z]);
	if (z > MOVMAX) {
	    spk = odistb[z + 1] - 1;
	    if (plce != 4)
		spk = 113;
	} else if (obloc[z] == 2000 + xiobj) {
	    spk = 92;
	    if (plural)
		spk = 191;
	} else if (plce == 2) {
	    spk = 88;
	    if (plural)
		spk = 194;
	} else if (plce == 0) {
	    spk = 66;
	    if (plural)
		spk = 185;
	} else if ((plce == 4 || plce == 5) && z == ZWIRE && wirelc[0] != 0)
	    spk = 89;
	else if ((plce == 4 || plce == 5) && z == ZCART &&
		 obloc[ZCART] == 2000 + RSLOT)
	    spk = 311;
	else if (size + cinsid > csize) {
	    spk = 93;
	    if (plural)
		spk = 192;
	} else if ((nweigh + weigh > 125) && (xiobj == ZKNAPS &&
					      obloc[z] != 1000 &&
			(obloc[ZKNAPS] == 1000 || obloc[ZKNAPS] == 3000))) {
	    spk = 79;
	    if (plural)
		spk = 188;
	} else if (z == CFISH && obloc[z] ==
		   (2000 + PLBAG) && (obimpr[PLBAG] / 512 == 5))
	    spk = 76;
	else if (z == ZLATEX && (obimpr[ZLATEX] & 56) == 8)
	    spk = 277;
	else if (xiobj == RSLOT && !(z == ZCART || z == CCART))
	    spk = 351;
	else if (iobj == FUNNEL) {
	    spk = 396;
	    if (xiobj == LFUNNE && z == ZMETOL)
		machst |= 8;
	    if (xiobj == RFUNNE && z == ZHYPO)
		machst |= 4;
	    obloc[z] = 0;
	} else if (loc == 99 && z == ZSTATU
		   && obimpr[ZSEED] != 2 && obimpr[ZSEED] != 1)
	    spk = 405;
	else if (loc == 86 && obloc[ZSEED] == 86 && !
		 (obloc[RPANT] == 3000 && obloc[RSHIRT] == 3000)) {
	    if (obloc[LPANT] == 3000 || obloc[GSHIRT] == 3000) {
		if (daytim == 1)
		    speak(424);
		else
		    speak(425);
	    } else {
		if (daytim == 1)
		    speak(426);
		else
		    speak(427);
	    }
	    if (obloc[LPANT] == 3000)
		obloc[LPANT] = 0;
	    if (obloc[GSHIRT] == 3000)
		obloc[GSHIRT] = 0;
	    vdead();
	    return (result);
	} else {
	    if (obloc[z] == 2000 + RSLOT)
		obimpr[ZSCREE] = 9;
	    obloc[z] = 2000 + xiobj;
	    nweigh += weigh;
	    cinsid += size;
	    /* special increment of descriptor pointer if obimpr has 4096 set */

	    if (obimpr[z] / 4096 == 1)
		obimpr[z] = (obimpr[z] & ~4096) + 8;
	    /* ~4096=167777 octal  */

	    spk = 75;
	    if (z == ZLATEX && (obimpr[ZLATEX] & 56) == 24)
		obimpr[ZLATEX] = 337;
	    if ((z == CCART || z == ZCART) && xiobj == RSLOT) {
		obimpr[RSLOT] += 8192;
		obimpr[ZSCREE] = 145;
	    } else if (iobj == RECESS && z == GDISC)
		spk = 412;
	    else if (loc == 86 && z == ZSEED) {
		if (daytim == 1)
		    spk = 428;
		else
		    spk = 429;
	    } else if (xiobj == OCLIP && z == YROD)
		spk = 526;
	    else if (xiobj == VCLIP && z == LMINER)
		spk = 527;
	    else if (xiobj == YCLIP && z == YROD)
		spk = 528;
	    else if (xiobj == BCLIP && z == BSAPPH)
		spk = 529;
	    else;
	    if ((xiobj == YCLIP || xiobj == BCLIP) &&
	     obloc[YROD] == 2000 + YCLIP && obloc[BSAPPH] == 2000 + BCLIP) {
		speak(spk);
		spk = 531;
	    }
	}
	xindnt += 2;
	speak(spk);
	xindnt -= 2;
    }
    return (result);
}

gamble(c)
    int             c;
{
    int             rann, spk, x;
    rann = qrand();
    if (jackpo == 1) {
	obloc[c] = 0;
	spk = 515;
    } else if (c != PCOIN) {
	if (rann > 3750) {
	    spk = 516;
	    x = PCOIN;
	} else if (rann > 3317) {
	    x = SCOIN;
	    spk = 517;
	} else if (rann > 2212) {
	    x = CCOIN;
	    spk = 518;
	} else if (rann > 1105) {
	    x = BCOIN;
	    spk = 519;
	} else {
	    x = NCOIN;
	    spk = 520;
	}
    } else {
	if (rann > 3276) {
	    spk = 521;
	    jackpo = 1;
	} else if (rann > 2048) {
	    x = PCOIN;
	    spk = 516;
	} else {
	    x = SCOIN;
	    spk = 517;
	}
    }
    if (spk == 521) {
	obloc[PCOIN] = 160;
	obloc[SCOIN] = 160;
	obloc[CCOIN] = 160;
	obloc[BCOIN] = 160;
	obloc[NCOIN] = 160;
	bonus += 5;
    } else {
	obloc[c] = 0;
	obloc[x] = 160;
    }
    speak(spk);
}

vscrat()
{
    int             spk, plce;
    if ((iobj != DIAMON && iobj != SAPPHI && iobj != KNIFE) || prepio != WITH)
	spk = 196;
    /* sapphire has no edge */

    else if (iobj == SAPPHI) {
	itsher(BSAPPH, &plce);
	if (plce == 0)
	    spk = 197;
	else
	    spk = 198;
    }
    /*
     * diamond cuts glass window but not vine names of window and glass plate
     * are all mixed up  
     */

    else if (iobj == DIAMON) {
	itsher(ZDIAMO, &plce);
	if (plce == 0)
	    spk = 199;
	else if (dobjs[0] == WINDOW || dobjs[0] == GLASS) {
	    if (loc != 30 && loc != 42)
		spk = 200;
	    else if (((obimpr[ZWINDO] / 8) % 8) != 0)
		spk = 201;
	    else {
		spk = 202;
		obimpr[ZWINDO] = 145;
		obloc[ZDIAMO] = 1000;
		if (loc == 30)
		    obloc[PGLASS] = 42;
		else
		    obloc[PGLASS] = 30;
	    }
	} else if (dobjs[0] == VINE && loc == 74 && (obimpr[ZVINE] & 56) == 8)
	    spk = 306;
	else
	    spk = 203;
    } else {
	itsher(ZKNIFE, &plce);
	if (plce == 0)
	    spk = 89;
	else if (dobjs[0] == WINDOW || dobjs[0] == GLASS)
	    spk = 309;
	else if (dobjs[0] == VINE && loc == 74 && (obimpr[ZVINE] & 56) == 8)
	    spk = 305;
	else
	    spk = 286;
    }
    speak(spk);
}

vread()
{
    int             m, plce, n;
    /* if it's light, read whatever is written on  */

    if (daytim == 0) {
	speak(121);
	return;
    }
    m = getobj(dobjs[0], doadjs[0]);
    if (prepio == TO || m < 1 || (obpprp[m] & 16) != 16) {
	speak(71);
	return;
    }
    itsher(m, &plce);
    n = (obimpr[m] / 64) % 8;
    if (n == 0 || plce == 0)
	speak(98);
    else {
	if (loc >= 153 && lpill == 1 && (m == MMAP || m ==
					 NNEWS || m == NNOTE || m == TMACH))
	    n += 1;
	speak(odistb[m] + n);
	if (m == NNOTE && lpill == 1)
	    nreadx();
    }
}

    static   char   string[] = "  Large ship XX XX XX";

nreadx()
{
    int             l, n, m;
    char            cnum[4];
    l = dcombi;
    n = (l & 31);
    l = l / 32;
    m = (l & 31);
    l = l / 32;
    numcvt(n, cnum);
    string[19] = cnum[2];
    string[20] = cnum[3];
    numcvt(m, cnum);
    string[16] = cnum[2];
    string[17] = cnum[3];
    numcvt(l, cnum);
    string[13] = cnum[2];
    string[14] = cnum[3];
    linout(string, 21);
}



vrobot()
{
    int             spk, vspk, dj, empty, i, bplce, gplce, tloc, iplce, ij, plce;
    spk = 0;
    vspk = 0;
    if (dobjs[0] != 0)
	dj = getrob(dobjs[0], doadjs[0]);
    if (iobj != 0)
	ij = getrob(iobj, ioadj);
    empty = 1;
    for (i = 1; i <= MOVMAX; i++)
	if (obloc[i] == 2000 + ZROBOT)
	    empty = 0;
    if (dobjs[1] != 0)
	spk = 343;
    else if (wrdnum[0] == TAKE || wrdnum[0] == HOLD) {
	if (dobjs[0] == DISC && dj < 0) {
	    tloc = loc;
	    loc = obloc[ZROBOT];
	    itsher(GDISC, &gplce);
	    itsher(BDISC, &bplce);
	    loc = tloc;
	    if (gplce == 4)
		gplce = 5;
	    if (bplce == 4)
		bplce = 5;
	    if (loc == obloc[ZROBOT]) {
		if (gplce == 1)
		    gplce = 5;
		if (bplce == 1)
		    bplce = 5;
	    }
	    if (gplce != 5 && bplce != 5)
		dj = 0;
	    else if (gplce == 5)
		dj = GDISC;
	    else
		dj = BDISC;
	}
	if (prepdo != 0 || prepio != 0 || allflg)
	    spk = 342;
	else if (empty == 0)
	    spk = 343;
	else if (dj < 0)
	    spk = 70;
	else if (dj > MOVMAX || dj == 0)
	    spk = 344;
	else {
	    tloc = loc;
	    loc = obloc[ZROBOT];
	    itsher(dj, &plce);
	    loc = tloc;
	    if (!(plce == 4 || plce == 5 || (plce == 1
                                        && obloc[ZROBOT] == loc)))
		spk = 420;
	    else if (obw3[dj] / 256 > 1 || (obw3[dj] & 255) > 2)
		spk = 352;
	    else {
		spk = 345;
		obloc[dj] = 2000 + ZROBOT;
		if (dj == ZINSEC && obimpr[ZINSEC] == 201)
		    obimpr[ZINSEC] = 209;
	    }
	}
    } else if (wrdnum[0] == DROP) {
	if (dj < 0) {
	    if (dobjs[0] == DISC && obloc[GDISC] == 2000 + ZROBOT)
		dj = GDISC;
	    else if (dobjs[0] == DISC && obloc[BDISC] == 2000 + ZROBOT)
		dj = BDISC;
	    else;
	}
	if (prepdo != 0 || prepio != 0 || allflg)
	    spk = 342;
	else if (dj < 0)
	    spk = 70;
	else if (dj == 0)
	    spk = 344;
	else if (obloc[dj] != 2000 + ZROBOT)
	    spk = 347;
	else {
	    spk = 346;
	    obloc[dj] = obloc[ZROBOT];
	}
    } else if (wrdnum[0] == GIVE) {
	if (dj < 0) {
	    if (dobjs[0] == DISC && obloc[GDISC] == 2000 + ZROBOT)
		dj = GDISC;
	    else if (dobjs[0] == DISC && obloc[BDISC] == 2000 + ZROBOT)
		dj = BDISC;
	    else;
	}
	if (obloc[ZROBOT] != loc)
	    spk = 443;
	else if (prepdo != 0 || prepio != TO || allflg)
	    spk = 342;
	else if (dj < 0)
	    spk = 70;
	else if (dj == 0)
	    spk = 344;
	else if (obloc[dj] != 2000 + ZROBOT)
	    spk = 347;
	else {
	    spk = 444;
	    obloc[dj] = 1000;
	}
    } else if (wrdnum[0] == WASH || wrdnum[0] == QWATER) {
	if (dobjs[0] == DISC && obloc[GDISC] == 2000 + ZROBOT)
	    dj = GDISC;
	else if (dobjs[0] == DISC && obloc[BDISC] == 2000 + ZROBOT)
	    dj = BDISC;
	else;
	if (dj <= 0 || !((dj <= MOVMAX && (obloc[dj] == 2000 +
		      ZROBOT || obloc[dj] == obloc[ZROBOT])) || (dj > MOVMAX
	      && (obw3[dj] == obloc[ZROBOT] || obw4[dj] == obloc[ZROBOT]))))
	    spk = 344;
	else {
	    if (wrdnum[0] == WASH) {
		if (dj == GDISC && rvtim == 0)
		    spk = 414;
		else if (dj == GDISC && rvtim == 1) {
		    spk = 410;
		    rvtim = 2;
		} else
		    spk = 348;
	    } else
		spk = 349;
	    if (loc == 74 && (obimpr[ZVINE] & 56) == 8 &&
		dj == ZVINE) {
		vspk = 1;
		obimpr[ZVINE] = obimpr[ZVINE] + 8;
	    }
	}
    } else if (wrdnum[0] == TRANSL) {
	if (dobjs[0] >= DEET && dobjs[0] <= STORAG)
	    spk = 354 - DEET + dobjs[0];
	else if (dobjs[0] == CYGNAN || dobjs[0] == TERRAN)
	    spk = 362 - TERRAN + dobjs[0];
	else
	    spk = 364;
    } else if (wrdnum[0] == PUT) {
	if (dobjs[0] == DISC && obloc[GDISC] == 2000 + ZROBOT)
	    dj = GDISC;
	else if (dobjs[0] == DISC && obloc[BDISC] == 2000 + ZROBOT)
	    dj = BDISC;
	else;
	if (dobjs[0] == DISC && obloc[GDISC] != 2000 + ZROBOT &&
	    obloc[BDISC] != ZROBOT)
	    spk = 347;
	else if (prepdo != 0 || prepio != IN || allflg)
	    spk = 342;
	else if (dj < 0 || ij < 0)
	    spk = 70;
	else if (dj == 0 || ij == 0)
	    spk = 344;
	else if (obloc[dj] != 2000 + ZROBOT)
	    spk = 347;
	else {
	    tloc = loc;
	    loc = obloc[ZROBOT];
	    itsher(ij, &iplce);
	    loc = tloc;
	    if (obloc[ZROBOT] == loc && obloc[ij] == 1000)
		iplce = 6;
	    if (iplce < 4 || iplce > 6)
		spk = 344;
	    else {
		spk = 75;
		obloc[dj] = 2000 + ij;
		if (ij == ZMUD) {
		    spk = 407;
		    obloc[dj] = 0;
		} else if (ij == FPIT) {
		    spk = 408;
		    obloc[dj] = 0;
		} else if (ij == ZACID && (dj == GDISC || dj == BDISC)) {
		    spk = 413;
		    obloc[dj] = 0;
		} else if (ij == GVAPOR && gvtim == 0 && dj == GDISC) {
		    spk = 409;
		    gvtim = 1;
		} else if (ij == RVAPOR && dj == GDISC && rvtim == 0) {
		    spk = 409;
		    if (gvtim == 1)
			rvtim = 1;
		}
	    }
	}
    } else if (wrdnum[0] == IS && adverb == WHERE)
	spk = 477;
    else if (wrdnum[0] == IS && adverb == WHAT) {
	if (dobjs[0] == COMBIN && (doadjs[0] == 0 || doadjs[0] == BOX))
	    spk = 476;
	else if (doadjs[0] != 0)
	    spk = 342;
	else if (dobjs[0] == DEET)
	    spk = 473;
	else if (dobjs[0] == METOL)
	    spk = 474;
	else if (dobjs[0] == HYPO)
	    spk = 475;
	else
	    spk = 477;
    } else
	spk = 251;
    if (spk != 0 && (obloc[ZROBOT] == loc || loc == 146)) {
	speak(spk);
	if (vspk != 0)
	    speak(301);
    }
}

vshit()
{
    dirty += 1;
    {
	if (loc == 74 && (obimpr[ZVINE] & 56) == 8)
	    speak(303);
	else if (dirty >= 5 && dirty < 10)
	    speak(40);
	else if (dirty >= 10)
	    if (obloc[LPANT] == 3000 || obloc[RPANT] == 3000)
		speak(43);
	    else
		speak(42);
    }
}

vshoot()
{
    int             spk, plce;
    spk = 0;
    itsher(ICAMER, &plce);
    /* camera isn't here  */

    if (plce != 1 && plce != 4)
	spk = 114;
    /* see if film is fogged   */

    else if (oextim == 8192)
	spk = 237;
    else if (oextim >= 45) {
	spk = 320;
	obloc[OPHOTO] = loc;
	obimpr[OPHOTO] += 64;
	oextim = 8192;
    }
    /* shot the pterodactyls */

    else if (loc == 89 || loc == 68 || loc == 62)
	spk = 382;
    else if (loc == 24) {
	if (obloc[PPHOTO] != 0)
	    spk = 115;
	else {
	    spk = 116;
	    obloc[PPHOTO] = loc;
	}
    }
    /* phograph the martians at the bar  */

    else if (loc == 30 && marflg[0]) {
	if (obloc[MPHOTO] != 0)
	    spk = 135;
	else {
	    spk = 134;
	    obloc[MPHOTO] = loc;
	}
    }
    /* shoot the hockey game  */

    else if (((loc >= 31 && loc <= 38) || loc == 29) && marflg[1]) {
	if (obloc[MPHOTO] != 0)
	    spk = 135;
	else {
	    spk = 134;
	    obloc[MPHOTO] = loc;
	    obimpr[MPHOTO] += 64;
	}
    }
    /* martians, alone aren't too interesting */

    else if ((loc == 28 && marflg[3]) ||
	     (loc == 40 && marflg[2]) ||
	     (loc >= 43 && loc <= 45 && marflg[4]))
	spk = 140;
    else if (loc == 131) {
	if (obloc[FPHOTO] != 0)
	    spk = 115;
	else {
	    spk = 318;
	    obloc[FPHOTO] = loc;
	    obimpr[FPHOTO] += 64;
	}
    } else if (loc == 100) {
	if (obloc[APHOTO] != 0)
	    spk = 115;
	else {
	    spk = 319;
	    obloc[APHOTO] = loc;
	    obimpr[APHOTO] += 64;
	}
    } else
	spk = 117;
    if (spk != 0)
	speak(spk);
}

vshut()
{
    int             spk, n, plce;
    if (prepdo != 0 && prepio != 0)
	spk = 21;
    /* the door to the bar is always open  */

    else if ((loc == 30 || loc == 42) && dobjs[0] == DOOR)
	spk = 224;
    /* doors or containers must be openable, and not already shut  */

    else {
	n = getobj(dobjs[0], doadjs[0]);
	if (dobjs[0] == DOOR && (loc == 184 || loc == 187 ||
				 (loc >= 176 && loc <= 181)))
	    n = BDOOR;
	if (n < 0)
	    spk = 70;
	else if (n == 0)
	    spk = 66;
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
	    } else if ((obimpr[n] & 2) == 2) {
		if (dobjs[0] == DOOR)
		    spk = 227;
		else
		    spk = 390;
	    } else {
		spk = 232;
		obimpr[n] |= 2;
		if (n == RDOOR || n == MDOOR || n == GDOOR || n == ODOOR
		    || n == BDOOR)
		    obimpr[n] -= 8;
		if (n == ZBOX) {
		    obimpr[ZBOX] |= 4;
		    dial1 = 0;
		    dial2 = 0;
		}
	    }
	}
    }
    speak(spk);
}

vsit()
{
    int             spk;
    /* sit is usually a joke */

    if (dobjs[0] == 0 && prepdo == DOWN) {
	if (loc == 166 || loc == 184 || loc == 187 ||
                                         (loc <= 181 && loc >= 175)) {
	    spcloc = 2;
	    spk = 510;
	} else
	    spk = 220;
    } else if (prepdo != IN && prepdo != ON)
	spk = 21;
    else if (prepdo == IN && dobjs[0] == CHAIR && (loc == 166
			      || loc == 184 || (loc <= 181 && loc >= 175) ||
						   loc == 187)) {
	spcloc = 2;
	spk = 510;
    } else if ((dobjs[0] == TREE || dobjs[0] == LOG || dobjs[0] ==
		BOULDE) && prepdo == ON)
	spk = 220;
    else if ((prepdo == ON || prepdo == IN) && (dobjs[0] == STOOL
			       || dobjs[0] == STOOLS || dobjs[0] == BOWL)) {
	/* except the case of sitting on electric seat  */

	if (loc == 42)
	    spk = 221;
	else if (loc == 28)
	    spk = 222;
	/* sit in bowl, when at hockey rink, slides him in it */

	else if (dobjs[0] == BOWL && (loc >= 32 && loc <= 38)) {
	    loc = 38;
	    spk = 75;
	    oldloc = 0;
	    oldlc2 = 0;
	} else if (dobjs[0] == BOWL)
	    spk = 156;
	else
	    spk = 71;
    } else
	spk = 48;
    speak(spk);
}
vstand()
{
    int             spk;
    if (dobjs[0] == 0 && prepdo == UP) {
	if (spcloc == 2) {
	    spcloc = 0;
	    spk = 511;
	} else
	    spk = 208;
    } else if (prepdo != ON)
	spk = 94;
    else if (dobjs[0] == TREE || dobjs[0] == LOG && (loc >= 4 &&
			  loc <= 19 && loc != 16 && loc != 13 && loc != 14))
	spk = 209;
    /* stand on glass plate  */

    else if (dobjs[0] == GLASS) {
	if (obloc[PGLASS] == 1000 || obloc[PGLASS] == 3000)
	    spk = 211;
	else if (obloc[PGLASS] != loc)
	    spk = 66;
	else {
	    spk = 75;
	    spcloc = 1;
	}
    } else
	spk = 210;
    speak(spk);
}


vtake()
{
    int             i, spk, xiobj, qq, xplce, kplce, ncarrd, kcarrd;
    int             nweigh, kweigh, ambig, z, plce, plural, t, kkwr;
    spk = 0;
    if (prepdo != 0 && prepio != 0)
	spk = 48;

    /* remove clothing means take off  */

    if (wrdnum[0] == REMOVE && prepdo == 0) {
	qq = getobj(dobjs[0], doadjs[0]);
	if (qq > 0 && obloc[qq] == 3000)
	    prepdo = OFF;
    }
    xiobj = getobj(iobj, ioadj);
    if (xiobj == ZKNAPS)
	itsher(ZKNAPS, &kplce);
    /* handle remove something from container */

    if (prepio == FROM) {
	if (obpprp[xiobj] / 2048 == 0)
	    spk = 77;
	else {
	    itsher(xiobj, &xplce);
	    if (xplce == 0)
		spk = 78;
	}
    }
    if (spk != 0) {
	speak(spk);
	return;
    }
    /* handle multiple objects   */

    ambig = !cnvobj();
    if (butflg && ambig)
	return;
    if (allflg)
	getall();

    /* check weight  */

    burden(&ncarrd, &nweigh, &kcarrd, &kweigh);
    if (obloc[ZKNAPS] == 1000 || obloc[ZKNAPS] == 3000)
	nweigh = nweigh + kweigh;
    for (i = 0; i < 30; i++) {
	z = dobjs[i];
	if (z == 0)
	    return;
	plural = (obpprp[z] & 256) == 256;
	if (z > MOVMAX) {
	    if (!allflg) {
		speak(odistb[z]);
		itsher(z, &plce);
		if (plce == 4)
		    spk = odistb[z + 1] - 1;
		else {
		    if (plural)
			spk = 185;
		    else
			spk = 66;
		}
		xindnt += 2;
		speak(spk);
		xindnt -= 2;
	    }
	    continue;
	}
	itsher(z, &plce);
	if (allflg && (plce == 0 || plce == 1 || (prepdo ==
			    OFF && plce != 2) || (prepio == FROM && xiobj ==
				   ZKNAPS && (obloc[z] != (2000 + ZKNAPS) ||
		kplce == 0)) || (prepio == 0 && obloc[z] == (2000 + ZKNAPS))
		       || (plce == 2 && prepdo != OFF)))
	    continue;
	/* emit name of object if "all" or multiple */

	if (dobjs[1] != 0 || allflg || ambig)
	    speak(odistb[z]);
	if (prepdo == 0) {
	    /*
	     * the wire is very,very special... don't even try to figure it
	     * out!!! *************************************************** 
             */


	    if (plce == 1 || (z == ZWIRE && wirelc[5] == 1000
			      && wirelc[0] != 1000)) {
		spk = 67;
		if (plural)
		    spk = 186;
	    } else if (plce == 0 && !(z == ZWIRE && (wirelc[4]
			      == loc || wirelc[3] == loc || wirelc[2] == loc
				|| wirelc[1] == loc || wirelc[0] == loc))) {
		spk = 66;
		if (plural)
		    spk = 185;
		if (z == RBOULD && loc == 27)
		    spk = 153;
		if (z == GMOSS && loc == 19)
		    spk = 243;
		if (z == PGLASS && (loc == 30 || loc == 42))
		    spk = 247;
		if ((z == PPHOTO && loc == 24) || (z == MPHOTO &&
						   loc >= 26 && loc <= 50))
		    spk = 249;
	    } else if (plce == 2) {
		spk = 68;
		if (plural)
		    spk = 187;
	    } else if (prepio == WITH && (z != CFISH ||
					  xiobj != BNET))
		spk = 48;
	    else if (prepio == FROM && !((z == CKEY &&
				iobj == SPIRE && ioadj == 0) || (obloc[z] ==
			(2000 + xiobj)) || (z == ZWIRE && ((iobj == ZKNOB &&
				    (obimpr[ZWIRE] & ~56) == 32) || (iobj ==
				  ZTOWER && (obimpr[ZWIRE] & -57) == 24)))))
		/* ~56=177707 octal  */

		spk = 71;
	    else if (ncarrd > 5)
		spk = 72;
	    else if (nweigh + obw3[z] / 256 > 125) {
		spk = 79;
		if (plural)
		    spk = 188;
	    }
	    /* take fish (from sea) with net.  */
	    else if (z == CFISH && loc == 16 && obimpr[CFISH]/4096 == 1) {
	        if (prepio != WITH || xiobj != BNET)
		    spk = 80;
                else if (obloc[BNET] != 1000)
                    spk = 91;
                else {
                    obloc[CFISH] = 2000 + BNET;
                    spk = 81;
                    obimpr[CFISH] -= 4088;
                }
            }
	    /* prevent him from killing wish in watery bag  */

	    else if (z == CFISH && obloc[z] ==
		     (2000 + PLBAG) && (obimpr[PLBAG] / 512 == 5))
		spk = 76;
	    /* cannot take latex if still liquid   */

	    else if (z == ZLATEX && (obimpr[z] & 56) == 8)
		spk = 268;
	    /* cant remove cartridge from slot  */

	    else if (z == ZCART && obloc[ZCART] == 2000 + RSLOT)
		spk = 311;
	    /* cant take statue unless ferrets blinded  */

	    else if (loc == 99 && z == ZSTATU
		     && obimpr[ZSEED] != 2 && obimpr[ZSEED] != 1)
		spk = 405;
	    /* diamond must land on moss to be safe   */

	    else if (z == ZDIAMO && loc == 49) {
		if (obloc[GMOSS] == 48) {
		    spk = 165;
		    obloc[z] = 48;
		    obimpr[z] += 8;
		} else {
		    spk = 164;
		    obloc[z] = 0;
		}
	    } else if (z == ZSEED && loc == 86 && obloc[ZSEED] == 86 &&
		       !(obloc[RPANT] == 3000 && obloc[RSHIRT] == 3000)) {
		if (obloc[LPANT] == 3000 || obloc[GSHIRT] == 3000) {
		    if (daytim == 1)
			speak(424);
		    else
			speak(425);
		} else {
		    if (daytim == 1)
			speak(426);
		    else
			speak(427);
		}
		if (obloc[LPANT] == 3000)
		    obloc[LPANT] = 0;
		if (obloc[GSHIRT] == 3000)
		    obloc[GSHIRT] = 0;
		vdead();
		return;
	    } else {
		nweigh = nweigh + obw3[z] / 256;
		ncarrd += 1;
		if (z == ZKNAPS) {
		    spk = 87;
		    obloc[z] = 3000;
		}
		/* specail code for wire  */

		else {
		    spk = 81;
		    if (z == ZWIRE) {
			if (obloc[ZWIRE] == loc) {
			    obimpr[ZWIRE] = (obimpr[ZWIRE] & ~56) + 8;
			    if (wirelc[0] == 0)
				obloc[z] = 1000;
			    /*
			     * if he takes fastened end, all the pointers must
			     * have their order reversed  
                             */

			    else {
				t = obloc[z];
				obloc[z] = wirelc[4];
				wirelc[4] = t;
				t = wirelc[3];
				wirelc[3] = wirelc[0];
				wirelc[0] = t;
				t = wirelc[2];
				wirelc[2] = wirelc[1];
				wirelc[1] = t;
				wirelc[5] = 1000;
				if (obloc[ZWIRE] == 1000)
				    goto lab240;
			lab239:
				if (obloc[z] != wirelc[0])
				    goto lab240;
				obloc[ZWIRE] = wirelc[0];
				for (kkwr = 0; kkwr < 5; kkwr++)
				    wirelc[kkwr] = wirelc[kkwr + 1];
				goto lab239;
				/*
				 * if it's all coiled up, i.e. all the
				 * wirelc's are 1000, clear wirelc   
                                 */

			lab240:
				if (wirelc[0] == 1000) {
				    for (kkwr = 0; kkwr < 6; kkwr++)
					wirelc[kkwr] = 0;
				}
			    }
			} else if (wirelc[5] == loc) {
			    /*
			     * if pick up far end of wire, 1000(held by him)
			     * propagates back from end of wirelc  
                             */

			    for (kkwr = 1; kkwr <= 5; kkwr++) {
				if (wirelc[6 - kkwr] == loc &&
                                             wirelc[5 - kkwr] == loc)
                                                       wirelc[6 - kkwr] = 1000;
			    }
			    if (wirelc[0] == loc && obloc[ZWIRE] == loc)
				wirelc[0] = 1000;
			} else
			    spk = 214;
			if (wirelc[0] == 1000 && obloc[ZWIRE] == 1000) {
			    for (kkwr = 0; kkwr < 6; kkwr++)
				wirelc[kkwr] = 0;
			}
		    } else {
			/*******    the actual taking occurs here  */

			if (obloc[z] == 2000 + RSLOT)
			    obimpr[ZSCREE] = 9;
			obloc[z] = 1000;
			if (z == ZLATEX)
			    obimpr[z] = 337;
		    }
		}
		/* special message for seed  */

		if (loc == 86 && z == ZSEED) {
		    if (daytim == 1)
			spk = 428;
		    else
			spk = 429;
		}
		/*
		 * special increment of descriptor pointer if obimpr has 4096
		 * set 
                 */

		if (obimpr[z] / 4096 == 1)
		    obimpr[z] = (obimpr[z] & ~4096) + 8;
		/* ~4096=167777 octal  */

	    }
	} else {
	    /*
	     * the prepdo is off take off clothes only  
             */

	    if (plce == 2) {
		obloc[z] = loc;
		spk = 82;
	    } else if ((obpprp[z] & 64) != 64)
		spk = 71;
	    else {
		spk = 83;
		if (plural)
		    spk = 189;
	    }
	}
	xindnt += 2;
	speak(spk);
	xindnt -= 2;
    }
}


vthrow()
{
    int             xobj, result, plce, kobj;
    result = 0;
    /* if vthrow remains false, must call vdrop  */

    xobj = getobj(dobjs[0], doadjs[0]);
    if (xobj <= 0 || xobj > MOVMAX) {
	speak(71);
	result = 0;
	return (result);
    }
    itsher(xobj, &plce);
    if (plce == 0) {
	if ((obpprp[xobj] & 256) == 256)
	    speak(185);
	else
	    speak(66);
	result = 1;
	return (result);
    }
    /* throw things at the shimmering barrier  */

    if ((loc == 26 || loc == 27) && prepio != OUT &&
	iobj == BARRIE && dobjs[0] != WIRE && dobjs[0] != FISH) {
	speak(172);
	obloc[xobj] = loc;
	result = 1;
    }
    /* throwing things at martians is not allowed  */

    else if ((loc >= 27 && loc <= 51) && daytim == 0 &&
	     (prepio == AT || prepio == TO) && iobj == MARTIA) {
	speak(178);
	result = 1;
	/*
	 * the window, or the glass, can't be broken by throwing things at it 
         */

    } else if ((prepio == THROUG || prepio == AT) && (iobj == WINDOW
						      || iobj == GLASS)) {
	if (loc != 30 && loc != 42)
	    speak(200);
	else if (xobj == ZSEED) {
	    speak(337);
	    obloc[xobj] = 0;
	    obimpr[ZSEED] = 0;
	} else if (obloc[PGLASS] != 0) {
	    speak(240);
	    obloc[xobj] = 42;
	    if (loc == 42)
		obloc[xobj] = 30;
	} else if (xobj == RBOULD) {
	    speak(241);
	    obloc[xobj] = loc;
	} else {
	    speak(242);
	    obloc[xobj] = loc;
	}
	result = 1;
	/*
	 * throwing other things results in a joke (usually)  
         */

    } else if (prepio == AT && ((iobj == FERRET && (loc == 99 || loc ==
		      100 || loc == 131)) || (iobj == BEES && loc == 87))) {
	prepio = 0;
	iobj = 0;
	ioadj = 0;
    } else if (prepio == AT) {
	if (iobj == ROBOT || iobj == FERRET)
	    speak(285);
	else if (iobj == GUARD) {
	    if (loc == guardl || loc == 171 || (chaset != 0 &&
						chaset <= 9))
		speak(562);
	    else
		speak(561);
	} else if (iobj == VINE && loc == 74) {
	    speak(308);
	    obloc[xobj] = loc;
	} else {
	    kobj = getobj(iobj, ioadj);
	    if (kobj <= 0)
		speak(71);
	    else;
	    speak(48);
	}
	return (1);
    }
    return (result);
}

vturn()
{
    int             numbr;
    if ((dobjs[0] == RA || dobjs[0] == DECLIN) && ((loc >= 175
				 && loc <= 181) || loc == 184 || loc == 187)
	&& prepio == TO && iobj > 9999) {
	numbr = iobj - 10000;
	if (dobjs[0] == RA)
	    raset = numbr;
	if (dobjs[0] == DECLIN)
	    decset = numbr;
	if (lpill == 1)
	    speak(533);
	return;
    }
    if ((dobjs[0] == LIGHT || dobjs[0] == MACHIN) && (prepdo ==
						      ON || prepdo == OFF)) {
	speak(391);
	return;
    } else if (dobjs[0] != DIAL)
	speak(387);
    else {
	if (doadjs[0] != LEFT && doadjs[0] != RIGHT)
	    speak(392);
	else if (prepio == 0 && iobj == 0)
	    speak(393);
	else if (prepio != TO || iobj < 9999)
	    speak(28);
	else if (iobj < 10000 || iobj > 10999)
	    speak(394);
	else if (doadjs[0] == LEFT)
	    dial1 = iobj - 10000;
	else
	    dial2 = iobj - 10000;
    }
    if (dial1 == dial1x && dial2 == dial2x && (obimpr[ZBOX] & 6) == 6) {
	speak(395);
	obimpr[ZBOX] -= 6;
    }
}
vwash()
{
    int             bplce, n, nplce;
    itsher(ZBUCKE, &bplce);
    if ((locdat[loc] & 1024) == 0 && (bplce == 0 || obimpr[ZBUCKE] != 1033)) {
	speak(74);
	return;
    }
    n = getobj(dobjs[0], doadjs[0]);
    if (n < 0)
	speak(70);
    else if (n == 0)
	speak(28);
    else {
	itsher(n, &nplce);
	if (nplce == 0) {
	    if ((obimpr[n] & 256) != 0)
		speak(185);
	    else
		speak(89);
	} else if (n == GDISC && rvtim == 0)
	    speak(414);
	else if (n == GDISC && rvtim == 1) {
	    speak(410);
	    rvtim = 2;
	} else
	    speak(336);
    }
}

vwear()
{
    int             ambig, i, d, plural, plce, spk;
    /* wear clothes  */

    ambig = !cnvobj();
    if (allflg || ambig) {
	speak(84);
	return;
    }
    for (i = 0; i < 30; i++) {
	d = dobjs[i];
	if (d == 0)
	    continue;
	plural = (obpprp[d] & 256) == 256;
	if (d != ZDEET)
	    speak(odistb[d]);
	itsher(d, &plce);
	if (plce == 0) {
	    spk = 66;
	    if (plural)
		spk = 185;
	} else if (plce == 2) {
	    spk = 68;
	    if (plural)
		spk = 187;
	} else if ((obpprp[d] & 64) != 64 && d != ZDEET)
	    spk = 85;
	else if ((d == RPANT && obloc[LPANT] == 3000) ||
		 (d == LPANT && obloc[RPANT] == 3000) ||
		 (d == RSHIRT && obloc[GSHIRT] == 3000) ||
		 (d == GSHIRT && obloc[RSHIRT] == 3000) ||
		 (d == HBOOTS && obloc[CSHOES] == 3000) ||
		 (d == CSHOES && obloc[HBOOTS] == 3000))
	    spk = 86;
	else {
	    if (d == ZDEET) {
		spk = 75;
		obimpr[ZDEET] = 28672;
		obloc[ZDEET] = 0;
	    } else {
		spk = 87;
		if (plural)
		    spk = 190;
		obloc[d] = 3000;
		/*
		 * taking key changes descriptor ~4152=167707 octal  
                 */

		if (d == CKEY)
		    obimpr[d] = (obimpr[d] & ~4152) + 16;
	    }
	}
	xindnt += 2;
	speak(spk);
	xindnt -= 2;
    }
}

