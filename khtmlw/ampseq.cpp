// -*- c++ -*-
/* This file is part of the KDE libraries
    Copyright (C) 1997 Martin Jones (mjones@kde.org)
              (C) 1997 Torben Weis (weis@kde.org)

    With some minor changes by:
        Norman Markgraf (Norman.Markgraf@rz.ruhr-uni-bochum.de)
 

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include "ampseq.h"
#define  Nrm (0)         // normal font face
#define  Sym (1)         // 'symbol' font face

// --> 165

ampseq_s AmpSequences[] = {
        {"AElig"   	,   0, 198},	// 31
	{"AMP"   	,   0,  38},
	{"Aacute"   	,   0, 193},
	{"Acirc"   	,   0, 194},
	{"Agrave"   	,   0, 192},
	{"Alpha"        , Sym,  65},
	{"Aring"   	,   0, 197},
	{"Atilde"   	,   0, 195},
	{"Auml"   	,   0, 196},
	{"Beta"         , Sym,  66},
	{"Ccedil"   	,   0, 199},
	{"Chi"          , Sym,  'C'},
	{"Delta"        , Sym,  68},
	{"ETH"   	,   0, 208},
	{"Eacute"   	,   0, 201},
	{"Ecirc"   	,   0, 202},
	{"Egrave"   	,   0, 200},
	{"Epsilon"      , Sym,  69},
	{"Eta"          , Sym,  'H'},
	{"Euml"   	,   0, 203},
	{"GT"   	,   0,  62},
	{"Gamma"        , Sym,  71},
	{"Iacute"   	,   0, 205},
	{"Icirc"   	,   0, 206},
	{"Igrave"   	,   0, 204},
	{"Iota"         , Sym,  73},
	{"Iuml"   	,   0, 207},
	{"Kappa"        , Sym,  'K'},
	{"LT"   	,   0,  60},
	{"Lambda"       , Sym,  76},
	{"Mu"           , Sym,  'M'},
	{"Ntilde"   	,   0, 209},
	{"Nu"           , Sym,  'N'},
	{"Oacute"   	,   0, 211},
	{"Ocirc"   	,   0, 212},
	{"Ograve"   	,   0, 210},
	{"Omega"        , Sym,  87},
	{"Omicron"      , Sym,  79},
	{"Oslash"   	,   0, 216},
	{"Otilde"   	,   0, 213},
	{"Ouml"   	,   0, 214},
	{"Phi"          , Sym,  'F'},
	{"Pi"           , Sym,  80},
	{"Psi"          , Sym,  'Y'},
	{"QUOT"   	,   0,  34},
	{"Rho"          , Sym,  'R'},
	{"Sigma"        , Sym,  83},
	{"THORN"   	,   0, 222},
	{"Tau"          , Sym,  84},
	{"Theta"        , Sym,  81},
	{"Uacute"   	,   0, 218},
	{"Ucirc"   	,   0, 219},
	{"Ugrave"   	,   0, 217},
	{"Upsilon"      , Sym,  85},
	{"Uuml"   	,   0, 220},
	{"Xi"           , Sym,  'X'},
	{"Yacute"   	,   0, 221},
	{"Zeta"         , Sym,  'Z'},
	{"acute"   	,   0, 180},
	{"aacute"   	,   0, 225},
	{"acirc"   	,   0, 226},
	{"aelig"   	,   0, 230},
	{"agrave"   	,   0, 224},
	{"alefsym"      , Sym, '@'},
	{"alpha"        , Sym, 'a'},
	{"amp"   	,   0,  38},
	{"aring"   	,   0, 229},
	{"atilde"   	,   0, 227},
	{"auml"   	,   0, 228},
	{"beta"         , Sym, 'b'},
	{"brvbar"   	,   0, 166},
	{"ccedil"   	,   0, 231},
	{"cedil"   	,   0, 184},
	{"cent"   	,   0, 162},
	{"chi"          , Sym, 'c'},
	{"copy"   	,   0, 169},
	{"curren"   	,   0, 164},
	{"dArr"         , Sym, '_'},
	{"deg"   	,   0, 176},
	{"delta"        , Sym, 'd'},
	{"divide"   	,   0, 247},
	{"eacute"   	,   0, 233},
	{"ecirc"   	,   0, 234},
	{"egrave"   	,   0, 232},
	{"epsilon"      , Sym, 'e'},
	{"eta"          , Sym, 'h'},
	{"eth"   	,   0, 240},
	{"euml"   	,   0, 235},
	{"exist"        , Sym, '$'},
	{"forall"       , Sym,'\"'},
	{"frac12"   	,   0, 189},
	{"frac14"   	,   0, 188},
	{"frac34"   	,   0, 190},
	{"gamma"        , Sym, 'g'},
	{"ge"           , Sym, '3'},
	{"gt"   	,   0,  62},
	{"iacute"   	,   0, 237},
	{"icirc"   	,   0, 238},
	{"iexcl"   	,   0, 161},
	{"igrave"   	,   0, 236},
	{"image"        , Sym, 'A'},
	{"iota"         , Sym, 'i'},
	{"iquest"   	,   0, 191},
	{"iuml"   	,   0, 239},
	{"kappa"        , Sym, 'k'},
	{"lArr"         , Sym, '\\'},
	{"lambda"       , Sym, 'l'},
	{"laqou"   	,   0, 171},
	{"lt"   	,   0,  60},
	{"macr"   	,   0, 175},
	{"micro"   	,   0, 181},
	{"middot"   	,   0, 183},
	{"mu"           , Sym, 'm'},
	{"nbsp"   	,   0,  32},
	{"not"   	,   0, 172},
	{"ntilde"   	,   0, 241},
	{"nu"           , Sym, 'n'},
	{"oacute"   	,   0, 243},
	{"ocirc"   	,   0, 244},
	{"ograve"   	,   0, 242},
	{"omega"        , Sym, 'w'},
	{"omnicron"     ,   0, 'o'},
	{"ordf"   	,   0, 170},
	{"ordm"   	,   0, 186},
	{"oslash"   	,   0, 248},
	{"otilde"   	,   0, 245},
	{"ouml"   	,   0, 246},
	{"para"   	,   0, 182},
	{"phi"          , Sym, 'j'},
	{"pi;"          , Sym, 'p'}, // NOT "pi" because there is a "piv" !
	{"piv"          , Sym, 'v'},
	{"plusmn"   	,   0, 177},
	{"pound"   	,   0, 163},
	{"psi"          , Sym, 'y'},
	{"quot"   	,   0,  34},
	{"raqou"   	,   0, 186},
	{"reg"   	,   0, 174},
	{"rho"          , Sym, 'r'},
	{"sect"   	,   0, 167},
	{"shy"   	,   0, 173},
	{"sigma;"       , Sym, 's'}, // NOT "sigma" because of "sigmaf" !
	{"sigmaf"       , Sym, 'V'},
	{"sub;"         , Sym, 'L'}, // NOT "sub" because of "sube" !
	{"sube"         , Sym, 'M'},
	{"sup;"         , Sym, 'I'}, // NOT "sup" because of "sub1",... !
	{"sup1"   	,   0, 185},
	{"sup2"   	,   0, 178},
	{"sup3"   	,   0, 179},
	{"szlig"   	,   0, 223},
	{"tau"          , Sym, 't'},
	{"theta;"       , Sym, 'q'}, // NOT "theta" because of "thetasym"
	{"thetasym"     , Sym, 'J'},
	{"thorn"   	,   0, 254},
	{"times"   	,   0, 215},
	{"trade"        , Sym, 'd'},
	{"uacute"   	,   0, 250},
	{"ucirc"   	,   0, 251},
	{"ugrave"   	,   0, 249},
	{"uml"   	,   0, 168},
	{"upsih"        , Sym, 'u'},
	{"upsilon"      , Sym, 'u'},
	{"uuml"   	,   0, 252},
	{"xi"           , Sym, 'x'},
	{"yacute"   	,   0, 253},
	{"yen"   	,   0, 165},
	{"zeta"         , Sym, 'z'}, // 196 - 31 = 165
};

char*   AmpSeqFontFaces[]=
{
        "",           // use the standard font as set by setStandardFont()
        "symbol", 
};

