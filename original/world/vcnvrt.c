#include <stdio.h>

/* World C Version 1.00 copyright 1987 J.D.McDonald 
   Use as you like for non-commercial purposes, but please
   leave this note, and document any changes you make as yours */

int             three[3], vrbpdo[100], vrbpio[100], vrbobj[100];
int             pp1[15], pp2[15], pp3[8];
int             obloc[200], obw3[200];
int             obw4[200], obpprp[200], obimpr[200];
int             dispat[300];
char            chrbuf[81];
char            astr[2], bstr[2], cstr[3], dstr[7];
char            outstr[81], estr[8], fstr[7];
char            obnoun[1200], obadjv[1400];
char           *nounptr, *adjptr;

int             pnum, number, i, q, numx, vocdim, nvrbob, vocsz, m, q1;
int             loxx, loxxx, ppq1, ppq2, zz1, zz2, ttabsz, locat, qq1, qq2;
int             objmax, movmax, z, wt, size, value, floc, pprp, imprp;
int             iloc, cval, qqqq, ib;

FILE           *vocab_dat, *vocab_inc, *parame_inc, *verbtb_inc;
FILE           *data_dat, *trvtbl_inc, *locdat_inc, *objdat_inc;


main()
{

    astr[1] = 0;
    bstr[1] = 0;
    cstr[2] = 0;
    dstr[6] = 0;
    estr[7] = 0;
    fstr[6] = 0;
    obloc[0] = 0;
    obnoun[0] = 0;
    obadjv[0] = 0;
    obw3[0] = 0;
    obw4[0] = 0;
    obpprp[0] = 0;
    obimpr[0] = 0;
    dispat[0] = 0;
    vocab_dat = fopen("vocab.dat", "r");
    vocab_inc = fopen("vocab.inc", "w");
    parame_inc = fopen("parame.inc", "w");
    verbtb_inc = fopen("verbtb.inc", "w");
    fprintf(parame_inc, "#define ACTMAX     1\n");
    number = 0;
    pnum = 0;
lab110:
    for (i = 0; i < 81; i++)
	chrbuf[i] = ' ';
    if (fgets(chrbuf, 80, vocab_dat) == NULL)
	goto lab500;
    q = strlen(chrbuf) - 1;
    for (i = q; i < 80; i++)
	chrbuf[i] = ' ';
    if (chrbuf[0] == '*')
	goto lab110;
    if (chrbuf[0] < '0' || chrbuf[0] >= '9')
	goto lab111;
    numx = pnum + 1;
    if (chrbuf[0] == '1') {
	fprintf(parame_inc, "#define BUZMAX %5d\n", pnum);
	fprintf(parame_inc, "#define ADVMIN %5d\n", numx);
    } else if (chrbuf[0] == '2') {
	fprintf(parame_inc, "#define ADVMAX %5d\n", pnum);
	fprintf(parame_inc, "#define VRBMIN %5d\n", numx);
    } else if (chrbuf[0] == '3') {
	fprintf(parame_inc, "#define VRBMAX %5d\n", pnum);
	fprintf(parame_inc, "#define ADJMIN %5d\n", numx);
    } else if (chrbuf[0] == '4') {
	fprintf(parame_inc, "#define ADJMAX %5d\n", pnum);
	fprintf(parame_inc, "#define NUNMIN %5d\n", numx);
    } else if (chrbuf[0] == '5') {
	fprintf(parame_inc, "#define NUNMAX %5d\n", pnum);
	fprintf(parame_inc, "#define PRPMIN %5d\n", numx);
    } else if (chrbuf[0] == '6') {
	fprintf(parame_inc, "#define PRPMAX %5d\n", pnum);
    } else;

    if (chrbuf[0] != '6')
	goto lab110;
    goto lab500;
lab111:
    number += 1;
    if (chrbuf[0] == '+')
	goto lab110;
    pnum += 1;
    for (i = 0; i < 6; i++)
	outstr[i] = chrbuf[i];
    if (chrbuf[59] >= 'A' && chrbuf[59] <= 'Z') {
	for (i = 0; i < 6; i++)
	    outstr[i] = chrbuf[59 + i];
    }
    fprintf(parame_inc, "#define %6s %5d\n", outstr, pnum);
    goto lab110;
lab500:
    fprintf(parame_inc, "#define VOCMAX %5d\n", number);
    vocdim = number * 3;

    fseek(vocab_dat, 0l, 0);

    fprintf(vocab_inc, "    short vocab[%5d] = { 0 \n", vocdim + 1);
    nvrbob = 0;
lab600:
    fgets(chrbuf, 80, vocab_dat);
    q = strlen(chrbuf) - 1;
    for (i = q; i < 80; i++)
	chrbuf[i] = ' ';
    if (chrbuf[0] == '*')
	goto lab600;
    vocsz = 0;
lab820:
    if (fgets(chrbuf, 80, vocab_dat) == NULL)
	goto lab6000;
    q = strlen(chrbuf) - 1;
    for (i = q; i < 80; i++)
	chrbuf[i] = ' ';
    if (chrbuf[0] == '*')
	goto lab820;
    if (chrbuf[0] > '0' && chrbuf[0] < '9')
	goto lab820;
    m = 0;
    if (chrbuf[0] == '+')
	m = 1;
    cvt(&chrbuf[m]);
    if (m == 1)
	three[0] = -three[0];
    vocsz += 3;
    q1 = (q < 9) ? q : 9;	/* q1 = min (q,9)  */
    fprintf(vocab_inc, "  , %6d, %6d, %6d  /* %12.13s */\n", three[0],
	    three[1], three[2], chrbuf);
    if (chrbuf[15] == ' ')
	goto lab820;
    for (i = 0; i < 15; i++) {
	pp1[i] = chrbuf[i + 14] - '0';
	pp2[i] = chrbuf[i + 31] - '0';
    }
    for (i = 0; i < 8; i++)
	pp3[i] = chrbuf[i + 49] - '0';

    nvrbob += 1;
    vrbpdo[nvrbob] = 0;
    vrbpio[nvrbob] = 0;
    vrbobj[nvrbob] = 0;
    for (i = 0; i < 15; i++) {
	vrbpdo[nvrbob] = vrbpdo[nvrbob] * 2 + pp1[i];
	vrbpio[nvrbob] = vrbpio[nvrbob] * 2 + pp2[i];
    }
    for (i = 0; i < 8; i++)
	vrbobj[nvrbob] = vrbobj[nvrbob] * 2 + pp3[i];
    goto lab820;
lab6000:
    fprintf(verbtb_inc, "   short vrbpdo[ %3d ] = { 0 \n", nvrbob + 1);
    for (i = 1; i <= nvrbob; i++)
	fprintf(verbtb_inc,
		"   , %7d \n", vrbpdo[i]);
    fprintf(verbtb_inc, "   } ; \n");

    fprintf(verbtb_inc, "   short vrbpio[ %3d ] = { 0 \n", nvrbob + 1);
    for (i = 1; i <= nvrbob; i++)
	fprintf(verbtb_inc,
		"   , %7d \n", vrbpio[i]);
    fprintf(verbtb_inc, "   } ; \n");

    fprintf(verbtb_inc, "   short vrbobj[ %3d ] = { 0 \n", nvrbob + 1);
    for (i = 1; i <= nvrbob; i++)
	fprintf(verbtb_inc,
		"   , %7d \n", vrbobj[i]);
    fprintf(verbtb_inc, "   } ; \n");


    fprintf(vocab_inc, "  } ;  \n");
    fclose(vocab_dat);
    fclose(vocab_inc);
    fclose(verbtb_inc);

    data_dat = fopen("data.dat", "r");
    trvtbl_inc = fopen("trvtbl.inc", "w");
    locdat_inc = fopen("locdat.inc", "w");
    objdat_inc = fopen("objdat.inc", "w");

    ttabsz = 0;
    fprintf(trvtbl_inc, "  short trvtbl[] = { 0  \n");
lab1000:
    fgets(chrbuf, 80, data_dat);
    if (chrbuf[0] == '*')
	goto lab1000;
    locat = atoi(&chrbuf[1]);
    qq1 = atoi(&chrbuf[13]);
    qq2 = atoi(&chrbuf[16]);
    astr[0] = chrbuf[0];
    cstr[0] = chrbuf[8];
    cstr[1] = chrbuf[9];
    bstr[0] = chrbuf[11];
    if (locat == 9999)
	goto lab1200;
    ttabsz += 1;
    if (strcmp(cstr, "N ") == 0)
	ppq1 = 1;
    else if (strcmp(cstr, "NE") == 0)
	ppq1 = 2;
    else if (strcmp(cstr, "E ") == 0)
	ppq1 = 3;
    else if (strcmp(cstr, "SE") == 0)
	ppq1 = 4;
    else if (strcmp(cstr, "S ") == 0)
	ppq1 = 5;
    else if (strcmp(cstr, "SW") == 0)
	ppq1 = 6;
    else if (strcmp(cstr, "W ") == 0)
	ppq1 = 7;
    else if (strcmp(cstr, "NW") == 0)
	ppq1 = 8;
    else if (strcmp(cstr, "U ") == 0)
	ppq1 = 9;
    else if (strcmp(cstr, "D ") == 0)
	ppq1 = 10;
    else
	printf(" error in travel table\n");
    if (bstr[0] == 'C')
	ppq1 += 16;
    if (bstr[0] == 'S')
	ppq1 += 32;
    if (bstr[0] == 'Q')
	ppq1 += 64;
    ppq2 = 512 * qq1 + qq2;
    zz2 = ttabsz * 2;
    zz1 = zz2 - 1;
    {
	if (locat != 0) {
	    dispat[locat] = zz1;
	    loxx = locat;
	}
    }

    fprintf(trvtbl_inc, "  , %7d , %7d \n", ppq1, ppq2);

    goto lab1000;
lab1200:
    loxxx = loxx + 1;
    fprintf(trvtbl_inc, " } ;\n");
    fprintf(trvtbl_inc, " short dispat[] = { 0 \n");
    for (i = 1; i < loxxx; i++)
	fprintf(trvtbl_inc, "   , %5d \n", dispat[i]);
    fprintf(trvtbl_inc, "   , %5d }; \n", zz2 + 1);
    fprintf(parame_inc, "#define LOCNUM %5d \n", loxx);
    fprintf(parame_inc, "#define TTABSZ %5d \n", zz2);

    fclose(trvtbl_inc);

    fprintf(locdat_inc, "   short locdat[] = { 0 \n");

    while (1) {
	do
	    fgets(chrbuf, 80, data_dat);
	while (chrbuf[0] == '*');
	locat = atoi(&chrbuf[1]);
	if (locat == 9999)
	    break;
	ppq1 = atoi(&chrbuf[6]);
	fprintf(locdat_inc, "   , %6d \n", ppq1);
    }
    fprintf(locdat_inc, "  } ; \n");
    fclose(locdat_inc);

    objmax = 0;
    movmax = 0;
    nounptr = obnoun;
    adjptr = obadjv;

    while (1) {
	do
	    fgets(chrbuf, 80, data_dat);
	while (chrbuf[0] == '*');
	z = atoi(&chrbuf[1]);
	if (z == 9999)
	    break;
	for (i = 0; i < 6; i++)
	    *nounptr++ = chrbuf[i + 7];
	for (i = 0; i < 7; i++)
	    *adjptr++ = chrbuf[i + 16];
	wt = atoi(&chrbuf[24]);
	size = atoi(&chrbuf[30]);
	value = atoi(&chrbuf[36]);
	floc = atoi(&chrbuf[42]);
	pprp = atoi(&chrbuf[48]);
	imprp = atoi(&chrbuf[54]);
	iloc = atoi(&chrbuf[60]);
	cval = atoi(&chrbuf[66]);
	for (i = 0; i < 6; i++)
	    fstr[i] = chrbuf[i + 72];
	objmax += 1;
	movmax += 1;
	obw3[z] = wt * 256 + size;
	obw4[z] = value * 4096 + floc;
	obpprp[z] = pprp + 2048 * cval;
	obloc[z] = iloc;
	obimpr[z] = imprp;
	for (i = 0; i < 6; i++)
	    if (fstr[i] == '\n')
		fstr[i] = ' ';
	fprintf(parame_inc, "#define  %7.6s %5d\n", fstr, z);
    }

    while (1) {
	do
	    fgets(chrbuf, 80, data_dat);
	while (chrbuf[0] == '*');
	z = atoi(&chrbuf[1]);
	if (z == 9999)
	    break;
	for (i = 0; i < 6; i++)
	    *nounptr++ = chrbuf[i + 7];
	for (i = 0; i < 7; i++)
	    *adjptr++ = chrbuf[i + 16];
	objmax += 1;
	obw3[objmax] = atoi(&chrbuf[30]);
	obw4[objmax] = atoi(&chrbuf[42]);
	pprp = atoi(&chrbuf[48]);
	obimpr[objmax] = atoi(&chrbuf[54]);
	cval = atoi(&chrbuf[66]);
	for (i = 0; i < 6; i++)
	    fstr[i] = chrbuf[i + 72];
	for (i = 0; i < 6; i++)
	    if (fstr[i] == '\n')
		fstr[i] = ' ';
	obpprp[objmax] = pprp + 2048 * cval;

	fprintf(parame_inc, "#define  %7.6s %5d\n", fstr, objmax);
    }
    fclose(data_dat);

    fprintf(parame_inc, "#define OBJMAX %5d\n", objmax);
    fprintf(parame_inc, "#define MOVMAX %5d\n", movmax);
    fprintf(parame_inc, "#define OBJMX2 %5d\n", objmax + 1);
    fclose(parame_inc);

    fprintf(objdat_inc, "   short obloc[] = { 0 \n");
    for (i = 1; i <= movmax; i++) {
	qqqq = obloc[i];
	if (qqqq > 6000)
	    qqqq += movmax - 4000;
	fprintf(objdat_inc, "  , %7d \n", qqqq);
    }
    fprintf(objdat_inc, " } ;\n");

    fprintf(objdat_inc, "   short obw3[] = { 0 \n");
    for (i = 1; i <= objmax; i++) {
	fprintf(objdat_inc, "  , %7d \n", obw3[i]);
    }
    fprintf(objdat_inc, " } ;\n");

    fprintf(objdat_inc, "   short obw4[] = { 0 \n");
    for (i = 1; i <= objmax; i++) {
	fprintf(objdat_inc, "  , %7d \n", obw4[i]);
    }
    fprintf(objdat_inc, " } ;\n");

    fprintf(objdat_inc, "   short obpprp[] = { 0 \n");
    for (i = 1; i <= objmax; i++) {
	fprintf(objdat_inc, "  , %7d \n", obpprp[i]);
    }
    fprintf(objdat_inc, " } ;\n");

    fprintf(objdat_inc, "   short obimpr[] = { 0 \n");
    for (i = 1; i <= objmax; i++) {
	fprintf(objdat_inc, "  , %7d \n", obimpr[i]);
    }
    fprintf(objdat_inc, " } ;\n");

    nounptr = obnoun;
    fprintf(objdat_inc, "   short obnoun[] =  { 0 \n");
    for (i = 1; i <= objmax; i++) {
	for (ib = 0; ib < 6; ib++)
	    dstr[ib] = *nounptr++;
	fprintf(objdat_inc, "  , %7s \n", dstr);
    }
    fprintf(objdat_inc, " } ;\n");

    adjptr = obadjv;
    fprintf(objdat_inc, "   short obadjv[] = { 0 \n");
    for (i = 1; i <= objmax; i++) {
	for (ib = 0; ib < 7; ib++)
	    estr[ib] = *adjptr++;
	fprintf(objdat_inc, "  , %8s \n", estr);
    }
    fprintf(objdat_inc, " } ;\n");


    fclose(objdat_inc);

}


cvt(strptr)
    char           *strptr;
{
    int             ii, ij, k;

    for (ii = 0; ii < 3; ii++) {
	three[ii] = 0;
	for (ij = 0; ij < 3; ij++) {
	    k = *strptr++;
	    if (k == 45)
		k = 91;
	    k -= 64;
	    if (k < 1 || k > 27)
		k = 0;
	    three[ii] = three[ii] * 32 + k;
	}
    }
}
