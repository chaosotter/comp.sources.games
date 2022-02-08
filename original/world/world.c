#define EXTERN 

#include "parame.inc"
#include "variab.h"
#include "locdat.inc"
#include "objdat.inc"
#include "objdes.inc"
#include "gtext.inc"

/* World C Version 1.00 copyright 1987 J.D.McDonald 
   Use as you like for non-commercial purposes, but please
   leave this note, and document any changes you make as yours */

static int     i, xloc, mxscor, succes, mloc, ikk, verb, vretrn;
static int     scor, xretn, yretn, h, errflg;

main()
{

    rdinit();
    wwflag = 0;
    nomor = 0;
    speak(29);
    linout(" ", 1);
    slflag = 0;
    speak(gtext[1] + 1);
    xindnt = 0;
    for (i = 0; i < 9; i++)
	marflg[i] = 0;
    for (i = 0; i < 6; i++)
	wirelc[i] = 0;
    brfflg = 1;
    deadf = 0;
    dirty = 0;
    turns = 0;
    more = 0;
    diesc = 0;
    horflg = 0;
    nonext = 0;
    screef = 0;
    spcloc = 0;
    oldloc = 1;
    oldlc2 = 1;
    loc = 1;
    xloc = 0;
    oextim = 0;
    rdietm = 0;
    rvtim = 0;
    gvtim = 0;
    eattim = 0;
    easttm = 0;
    filmtm = 0;
    filmst = 0;
    machst = 0;
    fimage = 0;
    dial1x = -1;
    dial2x = -1;
    dial1 = 0;
    dial2 = 0;
    eastsc = 0;
    cactsc = 0;
    mxscor = 450;
    bonus = 0;
    jackpo = 0;
    lpill = 0;
    pbstat = 0;
    dcombi = -1;
    chaset = 0;
    chaser = 0;
    fshlif = 8;
    noshoe = 8;
    guardl = 0;
    eolflg = 1;
    linout(" ", 1);
    /* the main command loop!!!!!  */

lab1:
    if (eolflg) {
	getln();
	dotflg = 0;
	clause = 1;
	actor = 1;
	lptr = 0;
    }
    succes = scan();
    if (!succes)
	eolflg = 1;
    if (!succes)
	goto lab1;
    succes = parse();
    clause += 1;
    if (!succes)
	eolflg = 1;
    if (!succes)
	goto lab1;
    errflg = 0;
    /******        for "tell someone" or "ask someone"  */

    if (actor != 1) {

	otheract();	/* perform verbs for other than player */


	timer(xloc);
	/* this goto is for permanent death due to radiation poisoning */

	{
	    if (rdietm == 50)
		goto lab9000;
	    nonext = 1;
	    goto lab3000;
	}
    }
    /******/




    verb = wrdnum[0];

    playerv();	/* this does all the verbs for player */


    if (errflg == 1)
	goto lab9000;
    if (errflg == 2)
	goto lab9990;
    {
	if (deadf && loc >= 153)
	    goto lab9000;
    }
    timer(xloc);
    /*
     * this goto is for permanent death due to radiation poisoning or death
     * in endgame 
     */

    {
	if (rdietm == 50 || (loc >= 153 && deadf))
	    goto lab9000;
    }
    h = 0;
    {
	if (loc != xloc) {
	    /* get out of any special locations (like on glass) */

	    if (!(loc == 187 || loc == 184 || (loc >= 175
					       && loc <= 181)))
		spcloc = 0;
	    if (wirelc[5] == 1000 || obloc[ZWIRE] == 1000)
		wdemon();
	    h = 1;
	}
    }
    /* announce new location  */

    {
	if (!nonext || (loc >= 7 && loc <= 11) ||
	    (loc >= 100 && loc <= 116)) {
	    linout(" ", 1);
	    if (loc >= 7 && loc <= 11)
		h = 1;
	    vlocat(0, h);
	}
	if (loc >= 26 && loc <= 51) {
	    linout(" ", 1);
	    mdemon();
	}
    }

    /* cat function */

lab3000:
    {
	if (obloc[ZCAT] == 1000 || (obloc[ZCAT] == (2000 + ZKNAPS) &&
			 (obloc[ZKNAPS] == 3000 || obloc[ZKNAPS] == loc))) {
	    if (loc == 149 && obloc[ZCART] == 0) {
		speak(290);
		obloc[ZCAT] = loc;
		obloc[ZCART] = loc;
	    } else if (obloc[ZCAT] == 2000 + ZKNAPS && obloc[ZKNAPS] == 3000
		       && pct(5)) {
		speak(287);
		obloc[ZCAT] = 1000;
	    } else if (pct(3))
		speak(288);
	    else if (pct(3))
		speak(289);
	}
    }
    /* robot activation */

    {
	if ((obimpr[RSLOT] & 8192) == 8192) {
	    obimpr[RSLOT] -= 8192;
	    if (obloc[ZCART] == 2000 + RSLOT)
		speak(310);
	    else
		speak(365);
	}
    }
    /* give bonus for reaching cygnus or deneb */

    {
	if (loc == 183) {
	    if ((locdat[183] & 16384) == 0) {
		speak(549);
		bonus += 5;
	    } else if (xloc != loc)
		speak(550);
	} else if (loc == 186) {
	    if ((locdat[186] & 16384) == 0)
		speak(547);
	    else if (xloc != loc)
		speak(548);
	} else if (loc == 190) {
	    speak(551);
	    bonus += 5;
	    goto lab9000;
	}
    }
    {
	if (loc == xloc && loc == 171) {
	    speak(557);
	    vdead();
	    goto lab9000;
	}
    }
    xloc = loc;
    turns += 1;
    nonext = 0;
    /* we have visited this location  */

    locdat[loc] |= 16384;
    linout(" ", 1);
    goto lab1;
lab9000:
    linout(" ", 1);
    scor = scorng() + bonus * 5;
    scorpt(scor, mxscor);
    goto lab9999;
lab9990:
    endsl();
lab9999:
    return ;
}

/* end of main program  */


otheract()
{
    static int      actok;

    if (actor == MARTIA) {
	if (martim == 7 && loc >= 27 && loc <= 51)
	    speak(167);
	else
	    speak(168);
    } else if (actor == ROBOT) {
	{
	    if (chgact == 1)
		actok = 1;
	}
	/* test to see if robot can "hear"  */

	if (chgact == 1 && obloc[CCART] == (2000 + RSLOT) &&
	    (loc == 146 || obloc[ZROBOT] == loc)) {
	    speak(366);
	    actok = 0;
	} else if (chgact == 1 && !((loc == obloc[ZROBOT] ||
			   loc == 146) && obloc[ZCART] == (2000 + RSLOT))) {
	    speak(314);
	    actok = 0;
	} else if (chgact == 0 && actok == 0);
	else if (wrdnum[0] == GO) {
	    mloc = obloc[ZROBOT];
	    vgo();
	    {
		if (obloc[ZROBOT] == 90)
		    obimpr[ZROBOT] = 2257;
	    }
	    {
		if (rmove == 1 && (loc == mloc))
		    speak(316);
		else if (rmove == 1 && mloc != loc && obloc[ZROBOT] == loc)
		    speak(317);
		else if (rmove == 1 && loc == 146)
		    robdsc();
	    }
	    {
		if (obloc[ZROBOT] == 90) {
		    if (obloc[ZORCHI] == 2000 + ZROBOT)
			obloc[ZORCHI] = 0;
		    else if (obloc[ZFLYTR] == 2000 + ZROBOT)
			obloc[ZFLYTR] = 0;
		}
	    }
	} else if (wrdnum[0] == TAKE || wrdnum[0] == DROP ||
		   wrdnum[0] == PUT || wrdnum[0] == IS || wrdnum[0] ==
		   TRANSL || wrdnum[0] == WASH || wrdnum[0] == QWATER
		   || wrdnum[0] == HOLD || wrdnum[0] == GIVE)
	    vrobot();
	else
	    speak(315);
    } else if (actor == FERRET) {
	if (loc != 131 && loc != 99 && loc != 100)
	    speak(435);
	else
	    speak(436);
    } else if (actor == CACTUS) {
	if (loc != 82)
	    speak(437);
	else if (cactsc != 3)
	    speak(438);
	/* cactus freezing scree sclope */

	else if ((wrdnum[0] == HOLD || wrdnum[0] == FREEZE) &&
		 dobjs[0] == GRAVEL) {
	    screef = 4;
	    speak(441);
	} else
	    speak(237);
    } else if (actor == GUARD) {
	if (loc == guardl || loc == 171 || (chaset != 0 &&
					    chaset <= 9))
	    speak(562);
	else
	    speak(561);
    } else
	speak(251);
}

playerv()
{
    if (verb < TAKE) {
	vretrn = 1;
	{
	    if (verb == RUN)
		vretrn = vrun();
	    else if (verb == CRAWL)
		vretrn = vcrawl();
	    else if (verb == CROSS)
		vretrn = vcross();
	    else if (verb == JUMP)
		vretrn = vjump();
	    else if (verb == ASCEND) {
		prepdo = UP;
		vretrn = vclimb();
	    } else if (verb == DESCEN) {
		prepdo = DOWN;
		vretrn = vclimb();
	    } else if (verb == CLIMB)
		vretrn = vclimb();
	    else if (verb == SWIM)
		vretrn = vswim();
	}

	/*
	 * if vretrn=0, then the action is complete if vretrn=1, then a
	 * translation to "go" occured or the verb was actually "go" 
         */

	{
	    if (vretrn == 1)
		vgo();
	}
    } else if (verb == QUIT) {
	if (yesx(101))
	    errflg = 1;	/* out after scoring */

    } else if (verb == DAMN)
	dirty += 1;
    else if (verb == SUSPEN)
	if (chaset == 0)
	    vsuspe(0);
	else
	    speak(559);
    else if (verb == RESTOR) {
	if (vsuspe(1)) {
	    eolflg = 1;
	    nonext = 0;
	    actor = 1;
	    xloc = loc;
	}
    } else if (verb == BRIEF)
	brfflg = 1;
    else if (verb == VERBOS)
	brfflg = 2;
    else if (verb == SUPERB)
	brfflg = 0;
    else if (verb == QMORE)
	nomor = 0;
    else if (verb == NOMORE)
	nomor = 1;
    else if (verb == SCORE) {
	scor = scorng() + bonus * 5;
	scorpt(scor, mxscor);
    } else if (verb == HELP)
	vhelp();
    else if (verb == RETREA)
	vretre();
    else if (verb == PRAY) {
	if (!deadf || loc != 61)
	    speak(44);
	else {
	    deadf = 0;
	    speak(45);
	    nonext = 1;
	    noshoe = 8;
	}
    } else if (verb == DEBUG)
	dbg();
    else if (verb == LOCATE) {
	vlocat(1, 1);
	nonext = 1;
    } else if (!deadf) {
	if (verb == TAKE || verb == REMOVE || verb == CARRY ||
	    verb == HOLD)
	    vtake();
	else if (verb == DROP)
	    vdrop();
	else if (verb == THROW) {
	    if (!vthrow())
		vdrop();
	} else if (verb == SIT)
	    vsit();
	else if (verb == STAND)
	    vstand();
	else if (verb == GET) {
	    xretn = vget();
	    {
		if (xretn == 1)
		    vtake();
	    }
	} else if (verb == SHOOT)
	    vshoot();
	else if (verb == OPEN)
	    vopen();
	else if (verb == SHUT)
	    vshut();
	else if (verb == LAND || verb == ACTIVA)
	    vship();
	else if (verb == SCRATC)
	    vscrat();
	else if (verb == TRANSL)
	    speak(71);
	else if (verb == ATTACH)
	    vattac();
	else if (verb == POUR || verb == QWATER) {
	    if (vpour() == 1)
		vput();
	} else if (verb == WASH)
	    vwash();
	else if (verb == EAT)
	    veat();
	else if (verb == DRINK)
	    vdrink();
	else if (verb == FILL) {
	    if (vfill() == 1)
		vpour();
	} else if (verb == IGNITE)
	    speak(39);
	else if (verb == TURN)
	    vturn();
	else if (verb == READ)
	    vread();
	else if (verb == BREAK)
	    vbreak();
	else if (verb == FREEZE)
	    speak(442);
	else if (verb == PICK) {
	    xretn = vpick();
	    {
		if (xretn == 1)
		    vtake();
	    }
	} else if (verb == WEAR)
	    vwear();
	else if (verb == PUT) {
	    xretn = vput();
	    {
		if (xretn == 1)
		    vwear();
		if (xretn == 2)
		    vdrop();
		if (xretn == 3)
		    vpour();
	    }
	} else if (verb == LOCK || verb == UNLOCK)
	    vlocks();
	else if (verb == LOOK || verb == EXAMIN)
	    vlooks();
	else if (verb == PUSH || verb == KISS) {
	    {
		if (verb == KISS)
		    yretn = vkiss();
	    }
	    if (yretn == 1 || verb == PUSH) {
		xretn = vpush();
		/* oops! the nova button */

		if (xretn == 1)
		    errflg = 2;	/* instant out */

		else if (xretn > 1) {
		    bonus = xretn * 2;
		    {
			if (xretn < 5)
			    errflg = 1;	/* out after scoring */

		    }
		    /* change to the "endgame" */

		    scor = scorng();
		    bonus += scor / 5;
		    loc = 153;
		    oldloc = 0;
		    oldlc2 = 0;
		    {
			for (ikk = 1; ikk <= SCOIN; ikk++)
			    if (obloc[ikk] != 1000 && obloc[ikk] != 3000)
				obloc[ikk] = 0;
		    }
		    obloc[ZKNAPS] = 0;
		}
	    }
	} else if (verb == PUNCH) {
	    if (vpunch() == 1)
		vbreak();
	} else if (verb == ATTACK)
	    vattak();
	else if (verb == DIG)
	    vdig();
	else if (verb == GIVE || verb == FEED)
	    vgive();
	else if (verb == IS)
	    speak(467);
	else if (verb == FUCK) {
	    dirty += 1;
	    vfuck();
	} else if (loc == 127 && (verb == SHIT || verb == EXHALE
				  || verb == PISS)) {
	    loc = 126;
	    oldloc = 0;
	    oldlc2 = 0;
	    speak(312);
	} else if (verb == EXHALE)
	    speak(313);
	else if (verb == PISS) 
            vpiss();
	else if (verb == SHIT) 
            vshit();
	else if (verb == INVENT)
	    vinven();
	else;
    }
    else speak(46);

}
