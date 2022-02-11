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
	vtextPath  = flag.String("vtext_path", "vtext.dat", "Path to the input 'vtext.dat' to index.")
	qtextPath  = flag.String("qtext_path", "qtext.inc", "Path to the output 'qtext.inc' file, which contains text offsets.")
	objdesPath = flag.String("objdes_path", "objdes.inc", "Path to the output 'objdes.inc' file, which contains object description offsets.")
	gtextPath  = flag.String("gtext_path", "gtext.inc", "Path to the output 'gtext.inc' file, which contains the base record numbers for each type.")
	q1textPath = flag.String("q1text_path", "q1text.dat", "Path to the output 'q1text.dat' file, which contains the compressed text strings.")
	goMode     = flag.Bool("go_mode", false, "If set, generate output for Go rather than the original C output.")
)

var (
	z, zbig, zsmall int
	packch          int
	i, kk, size     int
	buffer          [512]int
	bi              int
)

//-----------------------------------------------------------------------------

// A RecordID identifies a particular record ID from "vtext.dat".  These are
// four-digit numbers, with the leading digit identifying the type of record.
type RecordID int

// Sentinel is used to identify the end of input in "vtext.dat".
const Sentinel = RecordID(9999)

// MustNewRecordID extracts the record number from a line of input text from
// "vtext.dat".  Each line begins with a four-digit record number,
// left-justified within an eight-column field (e.g., "0056    Blah").
func MustNewRecordID(line string) RecordID {
	text := strings.Split(strings.TrimSpace(line), " ")[0]
	val, err := strconv.Atoi(text)
	if err != nil {
		log.Fatalf("invalid record number in %q: %v", line, err)
	}
	return RecordID(val)
}

// IsObject checks if this record ID is for an object, either moveable (3xxx)
// or fixed (5xxx).
func (id RecordID) IsObject() bool {
	return (3000 < id && id < 4000) || (5000 < id && id < 6000)
}

//-----------------------------------------------------------------------------

// A RecordIndex is the 0-based index of a packed record in "v1text.dat".
type RecordIndex int

// RecordBlocks contains the initial record index for each block of object in
// the game (messages, rooms, room names, objects).  The final entry is the
// total number of records.
type RecordBlocks [5]RecordIndex

// Update updates the table of initial record indices for the given record ID
// and index.
func (rb *RecordBlocks) Update(id RecordID, i RecordIndex) {
	if id != Sentinel {
		rb[block(id)+1] = i
	}
}

// block returns the block for the record associated with this ID
// (message=0, ...).
func block(id RecordID) int {
	if c := int(id) / 1000; c < 3 {
		return c
	}
	return 3
}

// MustWrite outputs the current state of the RecordBlocks to the given file,
// using the selected format.
func (rb *RecordBlocks) MustWrite(file string) {
	out := MustWrite(file)
	fmt.Fprintf(out, "  int gtext[5] = { 0, %6d, %6d, %6d, %6d };\n", rb[1], rb[2], rb[3], rb[4])
	out.Flush()
}

//-----------------------------------------------------------------------------

// RecordOffset is a byte offset into the contents of "q1text.dat".
type RecordOffset int

// RecordOffsets accumulates the initial byte offset of every record *after*
// the first record.
type RecordOffsets []RecordOffset

// Add adds this record offset to the table.
func (offs *RecordOffsets) Add(off RecordOffset) {
	*offs = append(*offs, off)
}

// MustWrite outputs the current state of the RecordOffsets table to the given
// file, using the selected format.
func (offs RecordOffsets) MustWrite(file string) {
	// Round up to the next multiple of 3 to avoid fragmented lines.
	recs := ((len(offs) + 2) / 3) * 3
	offs = append(offs, 0, 0)

	out := MustWrite(file)
	fmt.Fprintf(out, "#define RTSIZE %6d \n", recs+1)
	fmt.Fprintf(out, " unsigned short rtext[] = { 0 \n")
	for i := 0; i < recs; i += 3 {
		fmt.Fprintf(out, " , %5d, %5d, %5d \n", offs[i], offs[i+1], offs[i+2])
	}
	fmt.Fprintf(out, "  }; \n")
	out.Flush()
}

//-----------------------------------------------------------------------------

// ObjectIndices accumulates the record indices of the objects we encounter.
type ObjectIndices []RecordIndex

// Add adds this record index to the table.
func (is *ObjectIndices) Add(i RecordIndex) {
	*is = append(*is, i)
}

// MustWrite outputs the current state of the ObjectIndices to the given file,
// using the selected format.
func (is ObjectIndices) MustWrite(file string) {
	out := MustWrite(file)
	fmt.Fprintln(out, " short odistb[] = { 0 ")
	for _, i := range is {
		fmt.Fprintf(out, "  , %6d \n", i)
	}
	fmt.Fprint(out, "  } ; \n")
	out.Flush()
}

//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------

func main() {
	vtext := MustRead(*vtextPath)
	q1text := MustWrite(*q1textPath)

	z = 0
	zbig = 0
	zsmall = 0
	bi = 0

	chrbuf := make([]byte, 90)
	var blocks RecordBlocks
	var offsets RecordOffsets
	var objects ObjectIndices
	var lastID RecordID
	var rec RecordIndex

	for id := RecordID(0); id != Sentinel; lastID = id {
		buf, err := vtext.ReadString('\n')
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

		id = MustNewRecordID(buf)
		if id != lastID {
			rec++
			offsets.Add(RecordOffset(z))
		}
		blocks.Update(id, rec)
		if id.IsObject() {
			objects.Add(rec)
		}

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
				dump_buf(q1text)
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

	dump_buf(q1text)
	q1text.Flush()

	offsets.MustWrite(*qtextPath)
	objects.MustWrite(*objdesPath)
	blocks.MustWrite(*gtextPath)

	fmt.Printf(" packed: %8d unpacked: %8d \n", zsmall, zbig)
}

func dump_buf(out *bufio.Writer) {
	for i := 0; i < 512; i++ {
		// LSB, then MSB
		out.WriteByte(byte(buffer[i] & 0xff))
		out.WriteByte(byte((buffer[i] & 0xff00 >> 8)))
	}
	bi = 0
}
