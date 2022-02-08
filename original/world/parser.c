#define EXTERN extern

#include "parame.inc"
#include "variab.h"
#include "arrays.h"
#include "vocab.inc"
#include "verbtb.inc"

/* World C Version 1.00 copyright 1987 J.D.McDonald 
   Use as you like for non-commercial purposes, but please
   leave this note, and document any changes you make as yours */

/* No apologies for all the goto's. If you can get rid of them, well,
   I guess some persons are just naturally masochists */

static int      typlst[4] = {VRBMIN, ADJMIN, NUNMIN, PRPMIN};
static int      index[5], indx;

scan()
/*
 * this function scans the input line for gross syntax errors it also expands
 * special direction commands such as "ne" into their full form. it also
 * works on adverbs, removes such expressions as "ask frog," etc. finally it
 * puts the word numbers and types in the wrdnum and wrdtyp arrays the input
 * is inbuf ,the output is wrdnum,wrdtyp,actor,adverb 
 *
 */
{
    int             result, offlg, askx, lptr0, wptr, i, lxx, errno;

    result = 0;
    offlg = 0;
    chgact = 0;
    askx = 0;

lab5:
    adverb = 0;
    for (i = 0; i < 30; i++) {
	wrdnum[i] = 0;
	wrdtyp[i] = 0;
    }
    /*
     * wordtyp is -1 for a period,-2 comma 1 for adverb,2 verb,3 adjective,4
     * noun,5 preposition numbers are type 4,and wrdnum=number+10000 
     */

    wptr = 0;
lab10:
    lptr0 = lptr;
    wscan();
    /*
     * on return from wscan punct contains: 0 for a word 1 for a period 2 for
     * a comma 3 for a number 4 if an illegal character 5 if a number follows
     * a letter directly 6 if a letter follows a number directly 
     */

    if (punct == 1)
	return (result);
    else if (punct == 4)
	goto lab9000;
    else if (punct == 2)
	goto lab9010;
    else if (punct == 3)
	goto lab9019;
    else if (punct == 5)
	goto lab9020;
    else if (punct == 6)
	goto lab9140;
    else;
    /* it's a real word, so find out what it is */

    find(VOCMAX);

    /* discard buzzword  */

    if (indx > 0 && indx <= BUZMAX)
	goto lab10;
    if (indx == 0)
	goto lab9040;

    if ((indx >= NORTH && indx <= NORTHW) || indx == UP
	|| indx == DOWN) {
	/* yes,it's a special word  */

	lptr0 = lptr;
	wscan();
	/* check if it's all that's on the line  */

	if (punct != 1)
	    goto lab9030;
	/* expand a direction word into full form */

	wrdtyp[1] = 4;
	wrdnum[1] = indx;
	wrdnum[0] = GO;
	wrdtyp[0] = 2;
	wrdtyp[2] = -1;
	goto lab9999;
    }
    if (index[0] != 0) {
	if (adverb != 0)
	    goto lab9070;
	/* adverbs are removed from the wrdtyp list and handled separately */

	adverb = index[0];
	if (adverb != WHERE && adverb != WHAT)
	    goto lab10;
	if (actor == 1 || askx != ASK)
	    goto lab9130;
	goto lab10;
    }
    if (index[1] == 0)
	goto lab9050;
    wrdtyp[wptr] = 2;
    wrdnum[wptr] = index[1];
    /*
     * we've found the verb check for legal question  
     */

    if ((adverb == WHAT || adverb == WHERE) - (wrdnum[0] == IS))
	goto lab9130;
    if (wrdnum[0] == YELL) {
	/* the player is told to use "ask" or "tell" */

lab131:
	wscan();
	if (punct != 1)
	    goto lab131;
	wrdtyp[0] = 2;
	wrdnum[0] = YELL;
	result = 1;
	return (result);
    }
    if (wrdnum[0] < SHIT)
	goto lab200;
    /*
     * special handling for words which take no objects or modifiers they
     * should be alone on a line. they are verbs >= shit  
     */

    lptr0 = lptr;
    wscan();
    {
	if (punct == 1)
	    goto lab9998;
	else if (punct == 5)
	    goto lab9120;
	else
	    goto lab9150;
    }
    /* inner loop for all words after verb  */

lab200:
    wptr += 1;
    if (wptr > 27)
	goto lab9180;

lab201:
    lptr0 = lptr;
    wscan();
    if (punct != 0)
	goto lab210;
    find(BUZMAX);
    /* special code for "noun1 of noun2" construct  */

    if (indx == 4 && wrdtyp[wptr - 1] == 4)
	offlg = 1;
    if (indx != 0)
	goto lab201;
    goto lab215;
lab210:
    if (punct == 4)
	goto lab9000;
    if (punct == 5)
	goto lab9020;
    if (punct == 6)
	goto lab9140;
    if (punct != 2)
	goto lab215;
    if (wrdtyp[wptr - 1] != 4)
	goto lab9010;
    wrdtyp[wptr] = -2;
    goto lab200;

lab215:
    if (wrdtyp[wptr - 1] != 2 || (wrdnum[wptr - 1] != ASK
				  && wrdnum[wptr - 1] != TELL))
	goto lab220;
    if (punct == 3)
	goto lab9019;
    if (punct != 0)
	goto lab9010;
    if (actor != 1)
	goto lab9100;
    /*
     * this code is to set up addressing a second "person" the phrases "ask
     * frog" or "tell frog" etc are discarded and the variable "actor" tells
     * us who is addressed  
     */

    askx = wrdnum[wptr - 1];
    find(VOCMAX);
    if ((index[3] != 0) &&
	(index[3] < ROBOT || index[3] > FERRET))
	goto lab9090;
    if (index[3] == 0)
	goto lab9160;
    actor = index[3];
    lptr0 = lptr;
    chgact = 1;
    wscan();
    if (punct != 2)
	goto lab9110;
    goto lab5;
    /* code for numbers inputted in a statement */

lab220:
    if (punct != 3)
	goto lab240;
    wrdtyp[wptr] = 4;
    wrdnum[wptr] = three[0] + 10000;
    goto lab200;
    /* all regular words go here */

lab240:

    if (punct != 1)
	goto lab250;
    wrdtyp[wptr] = -1;
    goto lab9999;
lab250:
    find(VOCMAX);
    if (indx == 0)
	goto lab9040;
    /* special code for "of" handler */

    if (offlg) {
	offlg = 0;
	if (index[3] != 0 && wrdtyp[wptr - 2] != 3) {
	    /* exchange two nouns and tell parser the first is an adjective */

	    wrdtyp[wptr] = wrdtyp[wptr - 1];
	    wrdnum[wptr] = wrdnum[wptr - 1];
	    wrdtyp[wptr - 1] = 3;
	    wrdnum[wptr - 1] = index[3];
	    if (wrdnum[wptr] == PHOTOG && wrdnum[wptr - 1] == FERRET) {
		wrdnum[wptr - 1] = FAMILY;
            }
            goto lab200;
        }
	else
            goto lab9200;
    }
    if (indx != AND)
	goto lab270;
    if (wrdtyp[wptr - 1] == -2)
	goto lab201;
    if (wrdtyp[wptr - 1] != 4)
	goto lab9120;
    wrdtyp[wptr] = -2;
    goto lab200;
lab270:
    if (index[1] != 0 && index[0] == 0 && index[2] == 0
	&& index[3] == 0 && index[4] == 0)
	goto lab9080;
    if (index[0] == 0)
	goto lab280;
    if (adverb != 0)
	goto lab9070;
    adverb = indx;
    if (adverb == WHERE || adverb == WHAT)
	goto lab9130;
    goto lab201;
    /* at this point the word must be an adjectine, noun or preposition */

lab280:
    if (wrdtyp[wptr - 1] == 3 && index[2] != 0)
	goto lab9170;
    if (index[2] != 0) {
	wrdtyp[wptr] = 3;
	wrdnum[wptr] = index[2];
    } else if (index[3] != 0) {
	wrdtyp[wptr] = 4;
	wrdnum[wptr] = index[3];
    } else {
	wrdtyp[wptr] = 5;
	wrdnum[wptr] = index[4];
    }
    if ((wrdtyp[wptr] == 4 && wrdtyp[wptr - 1] == 3) &&
	wrdnum[wptr] < SAPPHI)
	goto lab9190;
    goto lab200;


    /***********       error processing  */

lab9000:
    lxx = lptr;
    errno = 1;
    goto lab9900;
lab9010:
    lxx = lptr;
    errno = 2;
    goto lab9900;
lab9019:
    lptr -= 1;
lab9020:
    lptr += 1;
    lxx = lptr;
    errno = 3;
    goto lab9900;
lab9030:
    lxx = lptr;
    errno = 4;
    goto lab9900;
lab9040:
    lxx = lptr;
    errno = 5;
    goto lab9900;
lab9050:
    lxx = lptr;
    errno = 6;
    goto lab9900;
lab9070:
    lxx = lptr;
    errno = 8;
    goto lab9900;
lab9080:
    lxx = lptr;
    errno = 9;
    goto lab9900;
lab9090:
    lxx = 0;
    errno = 10;
    goto lab9900;
lab9100:
    lxx = 0;
    errno = 11;
    goto lab9900;
lab9110:
    lxx = lptr0 + 1;
    errno = 12;
    goto lab9900;
lab9120:
    lxx = lptr;
    errno = 13;
    goto lab9900;
lab9130:
    lxx = 0;
    errno = 14;
    goto lab9900;
lab9140:
    lxx = lptr;
    errno = 15;
    goto lab9900;
lab9150:
    lxx = lptr0;
    errno = 16;
    goto lab9900;
lab9160:
    lxx = 0;
    errno = 17;
    goto lab9900;
lab9170:
    lxx = lptr;
    errno = 18;
    goto lab9900;
lab9180:
    lxx = 0;
    errno = 25;
    goto lab9900;
lab9190:
    lxx = lptr;
    errno = 107;
    goto lab9900;
lab9200:
    lxx = lptr;
    errno = 112;
lab9900:
    carerr(lxx, errno);
    return (result);
lab9998:
    wrdtyp[1] = -1;
lab9999:
    if ((askx == TELL && (adverb == WHERE || adverb == WHAT))
	|| (askx == ASK && (adverb != WHERE && adverb != WHAT)))
	goto lab9130;
    if (adverb == WHERE || adverb == WHAT)
	dotflg = 1;
    result = 1;
    return (result);

}

wscan()
/*
 * this routine reads the input ascii code and scans for punctuation or
 * numbers and then converts the first 9 letters of a word to three integer*2
 * words of radix 32 form  
 */

{
    char            m;
    static int      pow32[] = {1, 32, 1024, 32768};
    int             plcpnt, wpnt, kchar;

    if (dotflg) {
	actor = 1;
	dotflg = 0;
    }
    plcpnt = 1;
    punct = 0;
    wpnt = 1;
    three[0] = 0;
    three[1] = 0;
    three[2] = 0;
    while (inbuf[lptr] == ' ')
	lptr++;
    /*
     * punct =1 is period, 2 is comma, 3 is number,4 is illegal character and
     * 5 means a number directly follows a word 6 means letter follows a
     * number 
     */

    if (inbuf[lptr] == '.' || inbuf[lptr] == '?') {
	dotflg = 1;
	punct = 1;
	lptr += 1;
	return (0);
    }
    if (inbuf[lptr] == ',') {
	punct = 2;
	lptr += 1;
	return (0);
    }
    while (inbuf[lptr] >= '0' && inbuf[lptr] <= '9') {
	/* number handling */

	punct = 3;
	if (three[0] < 3200)
	    three[0] = three[0] * 10 + inbuf[lptr] - '0';
	lptr += 1;
    }
    if (punct == 3 && (inbuf[lptr] != '.' &&
		       inbuf[lptr] != ',' && inbuf[lptr] != '?' &&
		       inbuf[lptr] != ' '))
	punct = 6;
    if (punct == 3 || punct == 6)
	return (0);
    do {
	if (!((inbuf[lptr] >= 'a' && inbuf[lptr] <= 'z') ||
	      (inbuf[lptr] >= 'A' && inbuf[lptr] <= 'Z'))) {
	    lptr += 1;
	    punct = 4;
	    return (0);
	    /* the actual letter packing is done here  */

	}
	kchar = inbuf[lptr];
	if (kchar >= 'a' && kchar <= 'z')
	    kchar -= ' ';
	if (wpnt < 4)
	    three[wpnt - 1] = three[wpnt - 1] +
                                (kchar - '@') * pow32[3 - plcpnt];
	plcpnt += 1;
	if (plcpnt >= 4) {
	    wpnt += 1;
	    plcpnt = 1;
	}
	lptr += 1;
	m = inbuf[lptr];
	if (m == ' ' || m == ',' || m == '.' || m == '?') {
	    if (three[0] != 20741 || three[1] != 14336 ||
		three[2] != 0)
		return (0);
	    punct = 1;
	    return (0);
	}
    }
    while (m < '0' || m > '9');
    punct = 5;
    return (0);
}


find(m)
    int             m;
{
    /*
     * this routine searches a vocabulary to find a word. index, on output,
     * gives the position of the word in the list. synonyms do not increment
     * the counter, so index*3-2 is **not** the actual pointer to the word in
     * the list 
     */

    int             i, t, k, type;

    for (i = 0; i < 5; i++)
	index[i] = 0;
    t = 0;
    indx = 0;

    for (i = 1; i <= m * 3 - 2; i += 3) {

	if (vocab[i] > 0)
	    indx += 1;
	if (abs(vocab[i]) != three[0])
	    continue;
	if (vocab[i + 1] != three[1])
	    continue;
	if (vocab[i + 2] == three[2]) {
	    t = indx;
	    type = 1;
	    for (k = 1; k <= 4; k++)
		if (indx >= typlst[k - 1])
		    type = type + 1;
	    index[type - 1] = indx;
	}
    }
    indx = t;
}

lbit(arg, pow)
    int             arg, pow;
{
    return ((arg & (1 << pow)) != 0);
}

parse()
{
    /*
     * subroutine parse this mess deciphers the various direct and indirect
     * (i.e. second, usually, but in 'give frog cup' the frog is the i.o.)
     * objects. it associates them with their respective adjectives (dobjs(i)
     * and doadjs(i) correspond) butflg is set if the "all but" construct is
     * encountered allflg is set if the "all" construct is enconntered the
     * d.o. and i.o prepositions are put in prepdo and prepio 
     */

    int             result, wptr, thevrb, vrbind, vdo, vio, vobj, i;
    int             frstaj, frstnn, errno, tprp;
    result = 0;
    thevrb = wrdnum[0];
    vrbind = thevrb - VRBMIN + 1;

    vdo = 0;
    vio = 0;
    vobj = 0;
    if (thevrb < SHIT) {
	vdo = vrbpdo[vrbind];
	vio = vrbpio[vrbind];
	vobj = vrbobj[vrbind];
    }
    butflg = 0;
    allflg = 0;
    numdo = 0;
    prepdo = 0;
    prepio = 0;
    iobj = 0;
    ioadj = 0;
    for (i = 0; i < 12; i++) {
	dobjs[i] = 0;
	doadjs[i] = 0;
    }
    /* this block is a test for "sit down" or "get up" or "look up" etc */

    if (((lbit(vobj, 1) && wrdnum[1] == DOWN) ||
	 (lbit(vobj, 2) && wrdnum[1] == UP)) && wrdtyp[2] == -1) {
	prepdo = wrdnum[1];
	result = 1;
	goto finalout;
    }
    /* yell allows anything after it */

    if (thevrb == YELL)
	goto testout;
    wptr = 1;
    frstaj = 0;
    frstnn = 0;
    /* a preposition immediately follows verb */

    if (wrdtyp[wptr] == 5) {
	prepdo = wrdnum[wptr];
	if (prepdo == BUT) {
	    errno = 21;
	    goto errorout;
	}
	if (!lbit(vdo, (prepdo - PRPMIN))) {
	    errno = 21;
	    goto errorout;
	}
	wptr += 1;
    }
    /*
     * adjective follows verb *** special case of "terran" and "cygnan" as
     * nouns 
     */

    if (wrdtyp[wptr] == 3) {
	if ((wrdnum[wptr] == TERRAN || wrdnum[wptr] == CYGNAN)
	    && wrdnum[0] == TRANSL)
	    wrdtyp[wptr] = 4;
	else {
	    frstaj = wrdnum[wptr];
	    doadjs[0] = frstaj;
	    wptr += 1;
	}
    }
    /* this takes care of the case of a verb alone on a line */

    if (wrdtyp[wptr] != 4) {
	if (wrdtyp[wptr] != -1) {
	    errno = 28;
	    goto errorout;
	} else {
	    {
		if (thevrb >= SHIT) {
		    result = 1;
		    goto finalout;
		}
		if (!lbit(vobj, 7)) {
		    result = 1;
		    goto finalout;
		}
	    }
	    errno = 27;
	    goto errorout;
	}
    }
    frstnn = wrdnum[wptr];
    /* look at first set of objects (not always d.o.) */

    dobjs[0] = frstnn;
    numdo += 1;
    if (numdo == 9) {
	errno = 25;
	goto errorout;
    }
    wptr += 1;


    /* the following takes care of multiple objects */

    if (frstnn == ALL) {
	if (!lbit(vobj, 5)) {
	    errno = 22;
	    goto errorout;
	}
	if (frstaj != 0) {
	    errno = 26;
	    goto errorout;
	}
	allflg = 1;
	if (wrdnum[wptr] == BUT) {
	    butflg = 1;
	    /* check for adjective before noun */

	    if (!(wrdtyp[wptr + 1] == 4 || (wrdtyp[wptr + 1] == 3
					    && wrdtyp[wptr + 2] == 4))) {
		errno = 23;
		goto errorout;
	    }
	    wptr += 1;
	}
    }
    if (butflg || (wrdtyp[wptr] == -2)) {
	if (!lbit(vobj, 5)) {
	    errno = 22;
	    goto errorout;
	}
	mulobj(&wptr);
	if (numdo > 9) {
	    errno = 25;
	    goto errorout;
	}
    }
    /********** end multiple obj processor  */


    if (wrdtyp[wptr] == -1)
	goto testout;

    /* if the verb is "is" we may have a final adjective  */

    if (thevrb == IS && wrdtyp[wptr] == 3) {
	doadjs[1] = wrdnum[wptr];
	if (wrdtyp[wptr + 1] == -1)
	    goto testout;
	errno = 20;
	goto errorout;
    }
    /*
     * if the next word is a noun or adjective, and verb is not "is" we have
     * a non-prep indirect object such as the frog in "give frog water" 
     */

    if (wrdtyp[wptr] == 4 || (wrdtyp[wptr] == 3 && wrdtyp[wptr + 1]
			      == 4)) {
	if (numdo > 1 || prepdo != 0) {
	    errno = 22;
	    goto errorout;
	}
	if (!lbit(vobj, 3)) {
	    errno = 24;
	    goto errorout;
	}
	prepio = TO;
	ioadj = frstaj;
	iobj = frstnn;
	doadjs[0] = 0;
	if (wrdtyp[wptr] == 3) {
	    doadjs[0] = wrdnum[wptr];
	    wptr += 1;
	}
	dobjs[0] = wrdnum[wptr];
	numdo = 1;
	wptr += 1;
	/****** repeat the multiple obj processor */

	if (dobjs[0] == ALL) {
	    if (!lbit(vobj, 5)) {
		errno = 22;
		goto errorout;
	    }
	    if (doadjs[0] != 0) {
		errno = 26;
		goto errorout;
	    }
	    allflg = 1;
	    if (wrdtyp[wptr] == 5 && wrdnum[wptr] == BUT) {
		butflg = 1;
		if (!(wrdtyp[wptr + 1] == 4 || (wrdtyp[wptr + 1] == 3
						&& wrdtyp[wptr + 2] == 4))) {
		    errno = 23;
		    goto errorout;
		}
	    }
	}
	if (butflg || wrdtyp[wptr] == -2) {
	    if (!lbit(vobj, 5)) {
		errno = 22;
		goto errorout;
	    }
	    mulobj(&wptr);
	    if (numdo > 9) {
		errno = 25;
		goto errorout;
	    }
	}
	/***** end multiple object processor */

	if (wrdtyp[wptr] != -1) {
	    errno = 20;
	    goto errorout;
	}
	goto testout;
    }
    /* the only thing left that is legal is a perpositional construct */

    if (wrdtyp[wptr] != 5 || wrdnum[wptr] == BUT || (
			wrdtyp[wptr + 1] == 5 && wrdnum[wptr + 1] == BUT)) {
	errno = 20;
	goto errorout;
    }
    tprp = wrdnum[wptr];
    wptr += 1;
    /*
     * check for end of line or two preps in a row (e.g. fill the bottle up
     * with water) 
     */

    if (wrdtyp[wptr] == -1 || wrdtyp[wptr] == 5) {
	if (prepdo != 0 || (!lbit(vdo, (tprp - PRPMIN)))
	    || (!lbit(vobj, 0))) {
	    errno = 20;
	    goto errorout;
	}
	prepdo = tprp;
	if (wrdtyp[wptr] == -1)
	    goto testout;
	wptr += 1;
    }
    if (!lbit(vio, (wrdnum[wptr - 1] - PRPMIN))) {
	errno = 20;
	goto errorout;
    }
    prepio = wrdnum[wptr - 1];
    if (wrdtyp[wptr] == 3) {
	ioadj = wrdnum[wptr];
	wptr += 1;
    }
    if (wrdtyp[wptr] != 4) {
	errno = 20;
	goto errorout;
    }
    iobj = wrdnum[wptr];
    wptr += 1;
    if (wrdtyp[wptr] != -1) {
	errno = 20;
	goto errorout;
    }
testout:
    if ((dobjs[0] == 0 && lbit(vobj, 7)) || (iobj == 0 &&
					     lbit(vobj, 6))) {
	errno = 20;
	goto errorout;
    }
    if (!lbit(vobj, 4) && dobjs[0] != 0 && prepdo == 0) {
	errno = 19;
	goto errorout;
    }
    result = 1;
finalout:

    if (wrdnum[0] == AGAIN) {
	for (i = 0; i < 12; i++) {
	    doadjs[i] = zadjs[i];
	    dobjs[i] = zobjs[i];
	}
	ioadj = ziadj;
	iobj = ziobj;
	prepdo = zpdo;
	prepio = zpio;
	actor = zactor;
	adverb = zadvrb;
	wrdnum[0] = zverb;
	numdo = znumb;
	allflg = zall;
	butflg = zbut;
    } else {
	for (i = 0; i < 12; i++) {
	    zadjs[i] = doadjs[i];
	    zobjs[i] = dobjs[i];
	}
	ziadj = ioadj;
	ziobj = iobj;
	zpdo = prepdo;
	zpio = prepio;
	zactor = actor;
	zadvrb = adverb;
	zverb = wrdnum[0];
	zall = allflg;
	zbut = butflg;
    }
    return (result);
errorout:
    carerr(0, errno);
    return (result);
}

mulobj(wptr)
    int            *wptr;
{
    /****   multiple opject subroutine from "parse" */

    while (1) {
	if (wrdtyp[*wptr] == 3) {
	    doadjs[numdo] = wrdnum[*wptr];
	    *wptr += 1;
	}
	if (wrdtyp[*wptr] == 4) {
	    numdo += 1;
	    if (numdo > 10)
		return (1);
	    dobjs[numdo - 1] = wrdnum[*wptr];
	    *wptr += 1;
	    if (wrdtyp[*wptr] != -2)
		return (1);
	}
	*wptr += 1;
    }
}
