// This is a literal translation of "vtxtcn.go" from the original distribution
// of World.  The intention is to replace this code entirely in the final Go
// version of the game, but we need to be able to run it in order to analyze
// the output files, and this is some seriously paleolithic C code.
//
// TODO(chaosotter): Massage this code into a comprehensible and civilized
// state.
//
// Original comment follows:
//
//     World C Version 1.00 copyright 1987 J.D.McDonald
//     Use as you like for non-commercial purposes, but please
//     leave this note, and document any changes you make as yours
package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

var (
	vtextDatPath  = flag.String("vtext_dat_path", "vtext.dat", "Path to the input 'vtext.dat' to index.")
	qtextIncPath  = flag.String("qtext_inc_path", "qtext.inc", "Path to the output 'qtext.inc' file, which contains text offsets.")
	objdesIncPath = flag.String("objdes_inc_path", "objdesc.inc", "Path to the output 'objdes.inc' file, which contains object description offset.")
	gtextIncPath  = flag.String("gtext_inc.path", "gtext.inc", "Path to the output 'gtext.inc' file, which contains the base record numbers for each type.")
	q1textDatPath = flag.String("q1text_dat_path", "q1text.dat", "Path to the output 'q1text.dat' file, which contains the compressed text strings.")
	goMode        = flag.Bool("go_mode", false, "If set, generate output for Go rather than the original C output.")
)

var (
	z, zbig, zsmall                  int
	htext                            [2000]int
	gtext                            [5]int
	chrbuf                           []byte
	vtext_dat                        *bufio.Reader
	qtext_inc, objdes_inc            *bufio.Writer
	gtext_inc                        *bufio.Writer
	q1text_dat                       *bufio.Writer
	packch                           int
	i, u, kk, nold, size, number, kq int
	buffer                           [512]int
	bi                               int
)

// MustRead opens the given file for reading or aborts the process.
func MustRead(path string) *bufio.Reader {
	f, err := os.Open(path)
	if err != nil {
		log.Fatalf("could not open %q: %v", path, err)
	}
	return bufio.NewReader(f)
}

// MustWrite opens the given file for writing or aborts the process.
func MustWrite(path string) *bufio.Writer {
	f, err := os.Create(path)
	if err != nil {
		log.Fatalf("could not create %q: %v", path, err)
	}
	return bufio.NewWriter(f)
}

func main() {
	vtext_dat = MustRead("vtext.dat")
	q1text_dat = MustWrite("q1text.dat")
	qtext_inc = MustWrite("qtext.inc")
	objdes_inc = MustWrite("objdes.inc")

	fmt.Fprintln(objdes_inc, " short odistb[] = { 0 ")
	for i := 0; i < 2000; i++ {
		htext[i] = 0
	}

	z = 0
	zbig = 0
	zsmall = 0
	u = 0
	nold = 0
	bi = 0

	chrbuf = make([]byte, 90)
	for number = 0; number != 9999; {
		buf, err := vtext_dat.ReadString('\n')
		copy(chrbuf, []byte(buf))
		if err != nil {
			log.Fatalf("Could not read vtext.dat: %v", err)
		}

		for i = 0; i < 85; i++ {
			if chrbuf[i] == '\012' || chrbuf[i] == '\015' || chrbuf[i] == '\000' {
				break
			}
		}

		size = i
		for i = size - 1; i > 3; i-- {
			if chrbuf[i] != ' ' {
				break
			}
		}

		size = i + 1
		if size < 9 {
			chrbuf[8] = ' '
			size = 9
		}

		number, err = strconv.Atoi(strings.TrimSpace(strings.Split(string(chrbuf), " ")[0]))
		if err != nil {
			log.Fatalf("cannot extract number from %q: %v", chrbuf, err)
		}

		if number != nold {
			u += 1
		}
		if number < 1001 {
			gtext[1] = u
		}
		if number < 2001 {
			gtext[2] = u
		}
		if number < 3001 {
			gtext[3] = u
		}
		if number < 9999 {
			gtext[4] = u
		}
		if number > 3000 && number < 3999 {
			fmt.Fprintf(objdes_inc, "  , %6d \n", u)
		} else if number > 5000 && number < 5999 {
			fmt.Fprintf(objdes_inc, "   , %6d \n", u)
		}

		if number != nold {
			htext[u] = z
		}
		nold = number

		chrbuf[size] = '{'
		for i = size + 1; i < 90; i++ {
			chrbuf[i] = '}'
		}
		for i = 8; i < size; i++ {
			if chrbuf[i] == ' ' {
				chrbuf[i] = '`'
			}
			if chrbuf[i] == '.' {
				chrbuf[i] = '|'
			}
		}

		kk = 8
		for {
			if bi == 512 {
				dump_buf()
			}
			if chrbuf[kk] < '`' || chrbuf[kk+1] < '`' || chrbuf[kk+2] < '`' {
				packch = -(int(chrbuf[kk]) + (int(chrbuf[kk+1]) << 8))
				buffer[bi] = packch
				kk += 2
				zbig++
				bi++
				z++
				if chrbuf[kk-1] == '{' || chrbuf[kk-2] == '{' {
					break
				}
			} else {
				packch = int(chrbuf[kk+2]-96)*1024 + int(chrbuf[kk+1]-96)*32 + int(chrbuf[kk]) - 96
				if (packch&0377) == 10 || ((packch>>8)&0377) == 10 {
					packch = -(int(chrbuf[kk]) + (int(chrbuf[kk+1]) << 8))
					buffer[bi] = packch
					kk += 2
					zbig++
					bi++
					z++
					if chrbuf[kk-1] == '{' || chrbuf[kk-2] == '{' {
						break
					}
				} else {
					buffer[bi] = packch
					kk += 3
					zsmall++
					bi++
					z++
					if chrbuf[kk-1] == '{' || chrbuf[kk-2] == '{' || chrbuf[kk-3] == '{' {
						break
					}
				}
			}
		}
	}

	dump_buf()
	fmt.Fprint(objdes_inc, "  } ; \n")
	u = ((u + 2) / 3) * 3
	fmt.Fprintf(qtext_inc, "#define RTSIZE %6d \n", u+1)
	fmt.Fprintf(qtext_inc, " unsigned short rtext[] = { 0 \n")

	for kk = 1; kk <= u/3; kk++ {
		kq = (kk - 1) * 3
		fmt.Fprintf(qtext_inc, " , %5d, %5d, %5d \n", htext[kq+1], htext[kq+2], htext[kq+3])
	}
	fmt.Fprintf(qtext_inc, "  }; \n")

	q1text_dat.Flush()
	qtext_inc.Flush()
	objdes_inc.Flush()

	gtext_inc = MustWrite("gtext.inc")
	fmt.Fprintf(gtext_inc, "  int gtext[5] = { 0, %6d, %6d, %6d, %6d };\n", gtext[1], gtext[2], gtext[3], gtext[4])
	gtext_inc.Flush()

	fmt.Printf(" packed: %8d unpacked: %8d \n", zsmall, zbig)
}

func dump_buf() {
	for i := 0; i < 512; i++ {
		// LSB, then MSB
		q1text_dat.WriteByte(byte(buffer[i] & 0xff))
		q1text_dat.WriteByte(byte((buffer[i] & 0xff00 >> 8)))
	}
	bi = 0
}
