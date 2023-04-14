! ==============================================================================
!   GERMAN:  Language Definition File
!
!   Supplied for use with deform 6 -- Release 6/11 -- Serial number 050825
!
!   Inform 6 und Original-Library 6/11:
!   Copyright Graham Nelson 1993-2004 but freely usable (see manuals)
!
!   deform 6 -- deutsche Übersetzung der Library, (C) 2005 Martin Oehm
!   Weitergabe unter denselben Bedingungen wie Inform 6 und die Original-Lib.
!
!   Diese Datei wird automatisch von "parserm" eingebunden. (Im Gegensatz zur
!   eigentlichen Lib muss dazu aber nicht +language_name=German setzen. Es wird
!   empfohlen, diese Lib in einem separaten Verzeichnis zu lagern und am Anfang
!   der Spieldatei auf dieses Verzeichnis zu verweisen. Nach dem Einbinden von
!   GermanG.h wird überprüft, ob alle Dateien der Lib eingebunden wurden.
!
! ==============================================================================

System_file;

! ------------------------------------------------------------------------------
!   Part I.   Preliminaries
! ------------------------------------------------------------------------------

Constant LanguageVersion = "deform Release 6/11 (2010-03-16)";

Constant definite = -1;         ! Immer bestimmte Artikel verwenden
Constant yours = 1;             ! Possessivpronomen: deine alte Handtasche
Constant no_article = 0;        ! Kein unbestimmter Artikel
Constant no_short_name = -1;    ! Kein Short_name (aber Adjektive!)

Constant Dativ_n = -1;          ! Neue Deklinationstypen
Constant Akkusativ_en = -2;
Constant Genitiv_s = -3;
Constant Genitiv_es = -4;

Property additive disambig;     ! Für das aufgebohrte ChooseObjects
Property additive init;         ! Zur einfachen Initialisierung
Property dekl;
Property suffixes;
Property adj;
Property post;

Constant MALE_GENDER = 'm.';    ! Ein paar "untypable words", um die
Constant FEMALE_GENDER = 'f.';  ! Warnungen des Compilers zu unterdrücken.
Constant NEUTER_GENDER = 'n.';
Constant PLURAL_GENDER = 'p.';

#ifdef TARGET_ZCODE;
Constant CHAR_AE = 'ä';         ! ZSCII-Zeichen, allerdings nicht als
Constant CHAR_OE = 'ö';         ! Zahlencode, damit die (selten gebrauchte)
Constant CHAR_UE = 'ü';         ! Direktive Zcharacter weiterhin funktioniert
Constant CHAR_SS = 'ß';
#ifnot;
Constant CHAR_AE = 228;         ! ASCII-Codes (ISO 8859-1) der Umlaute
Constant CHAR_OE = 246;         ! für Glulx
Constant CHAR_UE = 252;
Constant CHAR_SS = 223;
#endif;

Default INVENTORY_INDENT = 2;   ! Einzug für eine Stufe der Inventarliste
                                ! Default ist zwei wie in der englischen Lib

! ------------------------------------------------------------------------------
!   Hier wird eine Kopie der ursprünglichen Eingabe angelegt, um eventuell
!   Mehrdeutigkeiten auflösen zu können oder damit Parser-Routinen sich den
!   ursprünglichen Text anschauen können, wenn zum Beispiel weiße Würfel
!   Namen haben, die auf 'e' aufhören oder so.
! ------------------------------------------------------------------------------

#ifdef DEBUG;
Global parser_echo = 0;         ! Flagge zur Ausgabe der beschnittenen Eingabe
#endif;

#ifdef TARGET_ZCODE;
Array orig_buffer -> 123;       ! Kopie der ursprünglichen Eingabe
Array orig_parse -> 65;         ! Kopie der ursprünglichen Wortpositionen
Array orig_position --> 17;     ! Wortreferenz, eventuell durch Synonyme
                                ! verschoben

[ OriginalAddress wordnum;
    if (wordnum > orig_position-->0)
        return orig_buffer + orig_buffer->1 + 1;
    wordnum = orig_position-->wordnum;
    return orig_buffer + orig_parse->(wordnum*4+1);
];

[ OriginalLength wordnum;
    if (wordnum > orig_position-->0) rfalse;
    wordnum = orig_position-->wordnum;
    return orig_parse->(wordnum*4);
];
#ifnot;
Array orig_buffer -> INPUT_BUFFER_LEN;
Array orig_parse --> PARSE_BUFFER_LEN/WORDSIZE;
Array orig_position --> MAX_BUFFER_WORDS + WORDSIZE;

[ OriginalAddress wordnum;
    if (wordnum > orig_position-->0)
        return orig_buffer + orig_buffer-->0 + 1;
    wordnum = orig_position-->wordnum;
    return orig_buffer + orig_parse-->(wordnum*3);
];

[ OriginalLength wordnum;
    if (wordnum > orig_position-->0) rfalse;
    wordnum = orig_position-->wordnum;
    return orig_parse-->(wordnum*3-1);
];
#endif;

[ PrintOriginal wordnum   buffer length i;
    buffer = OriginalAddress(wordnum);
    length = OriginalLength(wordnum);

    for (i = 0 : i < length : i++) print (char) buffer->i;
];

Global genitive_list = 0;             ! Flagge, ob ein Plural im Genitiv steht
                                      ! Das sollte eine lokale Flagge sein, aber
                                      ! in WriteListR waren keine mehr frei,
                                      ! und Plurale verschachteln sich nicht.
Global dative_mode = 0;               ! Ist das nächste Token im Dativ?
Global article_word;                  ! Gibt an, ob und welches Wort als
                                      ! Artikel verstanden wurde.
Global parse_noun_from;               ! Gibt an, ab welchem Wort eine Noun
                                      ! Phrase untersucht wird.
Global imperative_form;               ! Art des verwendeten Imperativs

Constant IMP_DU  0;                   ! hebe den Apfel auf
Constant IMP_IHR 1;                   ! hebt den Apfel auf
Constant IMP_SIE 2;                   ! heben Sie den Apfel auf
Constant IMP_INF 3;                   ! den Apfel aufheben

! ------------------------------------------------------------------------------
!   Diese Routine ist neu, hier werden am Anfang die richtigen Strings für die
!   verschiedenen Eszett-Stile gesetzt. (Diese Idee ist von Ralf Herrmann.)
!
!   Außerdem wird hier die Property init für jedes Objekt aufgerufen.
! ------------------------------------------------------------------------------

[ LanguageInitialise    o;
#ifdef DIALECT_SWISS;
    string 30 "ss";
#ifnot;
    string 30 "ß";
#endif;
#ifdef DIALECT_TRADITIONAL;
    string 31 "ß";
#ifnot;
    string 31 "ss";
#endif;

    objectloop (o provides init) o.init();
];

Class   CompassDirection
  with  number 0, article definite,
        description [;
            if (location provides compass_look
                && location.compass_look(self)) rtrue;
            if (self.compass_look()) rtrue;
            L__M(##Look, 7, self);
        ],
        compass_look false,
  has   male scenery;

Object Compass "Windrose" has female concealed;

!   Die Namen der Windrichtungen sind so gewählt, dass man immer "nach x"
!   sagen kann: "nach Westen", "nach oben/unten", "nach drinnen/draußen".

#Ifndef WITHOUT_DIRECTIONS;
CompassDirection -> n_obj "Norden"
                    with door_dir n_to,
                         name 'n//' 'norden';
CompassDirection -> s_obj "Süden"
                    with door_dir s_to,
                         name 's//' 'sueden';
CompassDirection -> e_obj "Osten"
                    with door_dir e_to,
                         name 'e//' 'o//' 'osten';
CompassDirection -> w_obj "Westen"
                    with door_dir w_to,
                         name 'w//' 'westen';
CompassDirection -> ne_obj "Nordosten"
                    with door_dir ne_to,
                         name 'ne' 'no' 'nordosten';
CompassDirection -> nw_obj "Nordwesten"
                    with door_dir nw_to,
                         name 'nw' 'nordwesten';
CompassDirection -> se_obj "Südosten"
                    with door_dir se_to,
                         name 'se' 'so' 'suedosten';
CompassDirection -> sw_obj "Südwesten"
                    with door_dir sw_to,
                         name 'sw' 'suedwesten';
CompassDirection -> u_obj "oben"
                    with door_dir u_to,
                         name 'h//' 'u//' 'hoch' 'oben' 'rauf',
                    has  proper;
CompassDirection -> d_obj "unten"
                    with door_dir d_to,
                         name 'r//' 'd//' 'runter' 'unten' 'hinunter',
                    has  proper;
#endif; ! WITHOUT_DIRECTIONS

CompassDirection -> in_obj "drinnen"
                    with door_dir in_to,
                         name 'rein' 'drinnen',
                    has  proper;
CompassDirection -> out_obj "drau@30en"
                    with door_dir out_to,
                         name 'raus' 'draussen',
                    has  proper;

! ------------------------------------------------------------------------------
!   Part II.   Vocabulary
! ------------------------------------------------------------------------------

! ------------------------------------------------------------------------------
!   Die XXX__WDs aus der Original-Lib wurden durch Routinen ersetzt, die ich
!   flexibler finde. Kein Undo? Einfach is_undo_word ersetzen, so dass es immer
!   false zurückgibt. Auch Erweiterungen sind schnell möglich. Ausnahme sind die
!   THEN__WDs, da THEN1__WD oft als Synonym für den Punkt benutzt wird.
! ------------------------------------------------------------------------------

[ Is_again_word w;
    if (w=='nochmal') rtrue;
    if (w=='wieder') rtrue;
    if (w=='nm') rtrue;
    if (w=='wdh') rtrue;
    if (w=='g//') rtrue;
    rfalse;
];

[ Is_oops_word w;
    ! wird vor LTI geparst, Umlaute berücksichtigen
    if (w=='o//' && NumberOfWords()==2 && oops_from) rtrue;
    if (w=='oh') rtrue;
    if (w=='aeh') rtrue;
    if (w=='äh') rtrue;
    if (w=='oops') rtrue;
    if (w=='hoppla') rtrue;
    if (w=='huch') rtrue;
    rfalse;
];

[ Is_undo_word w;
    ! wird vor LTI geparst, Umlaute berücksichtigen
    if (w=='undo') rtrue;
    if (w=='rueckgaengig') rtrue;                  ! 'rueckgaen'
    if (w=='rueckgängig') rtrue;                   ! 'rueckg...'
    if (w=='rückgaengig') rtrue;                   ! 'rückga'
    if (w=='rückgängig') rtrue;                    ! 'rückg...'
    if (w=='zurueck') rtrue;
    if (w=='zurück') rtrue;
    rfalse;
];

[ Is_all_word w;
    if (w=='alles') rtrue;
    if (w=='all') rtrue;
    rfalse;
];

[ Is_but_word w;
    if (w=='ausser') rtrue;
    if (w=='ohne') rtrue;
    rfalse;
];

[ Is_other_word w;
    if (w=='andere') rtrue;
    rfalse;
];

[ Is_me_word w;
    if (w=='ich') rtrue;
    if (w=='mich') rtrue;
    if (w=='mir') rtrue;
    if (actor==player) {
        if (w=='du') rtrue;
        if (w=='dich') rtrue;
        if (w=='dir') rtrue;
    }
    rfalse;
];

[ Is_of_word w;
    if (w=='von') rtrue;
    rfalse;
];

[ Is_and_word w;
    if (w=='und') rtrue;
    if (w=='sowie') rtrue;
    rfalse;
];

[ Is_yes_word w;
    if (w=='j//') rtrue;
    if (w=='ja') rtrue;
    if (w=='jawohl') rtrue;
    if (w=='y//') rtrue;
    rfalse;
];

[ Is_no_word w;
    if (w=='n//') rtrue;
    if (w=='nein') rtrue;
    rfalse;
];

Constant THEN1__WD      = 'dann';      ! Das ist zwar nicht ganz richtig, da es
Constant THEN2__WD      = 'dann';      ! nicht heißt "nimm Brot dann iss es", aber
Constant THEN3__WD      = 'dann';      ! THEN1__WD wird häufig verwendet, auch in
                                       ! infix.h, und das fasse ich nicht an.

Default AMUSING__WD    = 'nachwort';
Default FULLSCORE1__WD = 'komplett';
Default FULLSCORE2__WD = 'k//';
Default QUIT1__WD      = 'q//';
Default QUIT2__WD      = 'ende';
Default RESTART__WD    = 'neustart';
Default RESTORE__WD    = 'laden';

Default LIT__TX        = "Licht spendend";

Array LanguagePronouns table

   !  word       possible GNAs                   connected
   !             to follow:                      to:
   !             a     i
   !             s  p  s  p
   !             mfnmfnmfnmfn

      'er'     $$100000100000                    NULL   ! m-Nom
      'sie'    $$010111010111                    NULL   ! f-Nom/Akk
      'es'     $$001000001000                    NULL   ! n-Nom/Akk
      'ihn'    $$100000100000                    NULL   ! m-Akk
      'ihm'    $$101000101000                    NULL   ! m/n-Dat
      'ihr'    $$010000010000                    NULL   ! f-Dat
      'ihnen'  $$000111000111                    NULL   ! p/Dat
      'spez.'  $$000000111111                    NULL   ! Adverbialpron.
      'ihm/r'
               $$000000111111                    NULL   ! Adverbialpron.
      ;

#ifdef TRADITIONAL_DESCRIPTORS;

Array LanguageDescriptors table

   !  word       possible GNAs   descriptor      connected
   !             to follow:      type:           to:
   !             a     i
   !             s  p  s  p
   !             mfnmfnmfnmfn

      'der'    $$110111110111    DEFART_PK       NULL   ! m-Nom, f-Dat, f/p-Gen
      'die'    $$010111010111    DEFART_PK       NULL   ! f/p-Nom/Akk
      'das'    $$001000001000    DEFART_PK       NULL   ! n-Nom/Akk
      'dem'    $$101000101000    DEFART_PK       NULL   ! m/n-Dat
      'den'    $$100111100111    DEFART_PK       NULL   ! m-Akk, p-Dat
      'des'    $$101000101000    DEFART_PK       NULL   ! m/n-Gen

      'einen'  $$100000100000    INDEFART_PK     NULL   ! m-Akk
      'ein'    $$101000101000    INDEFART_PK     NULL   ! m/n-Nom, n-Akk
      'einem'  $$101000101000    INDEFART_PK     NULL   ! m/n-Dat
      'eine'   $$010000010000    INDEFART_PK     NULL   ! f-Nom/Akk
      'einer'  $$010000010000    INDEFART_PK     NULL   ! f-Dat/Gen
      'eines'  $$101000101000    INDEFART_PK     NULL   ! m/n-Gen

      'dies'   $$001000001000    POSSESS_PK      0      ! Demonstrativpronomen
      'diese'  $$010111010111    POSSESS_PK      0      ! für "hier"
      'dieser' $$110111110111    POSSESS_PK      0
      'diesem' $$101000101000    POSSESS_PK      0
      'diesen' $$100111100111    POSSESS_PK      0
      'dieses' $$101000101000    POSSESS_PK      0

      'jene'   $$010111010111    POSSESS_PK      1      ! Demonstrativpronomen
      'jener'  $$110111110111    POSSESS_PK      1      ! für "dort"
      'jenem'  $$101000101000    POSSESS_PK      1
      'jenen'  $$100111100111    POSSESS_PK      1
      'jenes'  $$101000101000    POSSESS_PK      1

      'mein'   $$101000101000    POSSESS_PK      0      ! Possesivpronomen
      'meine'  $$010111010111    POSSESS_PK      0
      'meiner' $$010111010111    POSSESS_PK      0
      'meinem' $$101000101000    POSSESS_PK      0
      'meinen' $$100111100111    POSSESS_PK      0
      'meines' $$101000101000    POSSESS_PK      0

      'dein'   $$101000101000    POSSESS_PK      1      ! Possesivpronomen
      'deine'  $$010111010111    POSSESS_PK      1
      'deiner' $$010111010111    POSSESS_PK      1
      'deinem' $$101000101000    POSSESS_PK      1
      'deinen' $$100111100111    POSSESS_PK      1
      'deines' $$101000101000    POSSESS_PK      1

      'sein'   $$001000001000    POSSESS_PK      'him'
      'seine'  $$010111010111    POSSESS_PK      'him'
      'seiner' $$110111110111    POSSESS_PK      'him'
      'seinem' $$101000101000    POSSESS_PK      'him'
      'seinen' $$100111100111    POSSESS_PK      'him'
      'seines' $$101000101000    POSSESS_PK      'him'

      'ihr'    $$001000001000    POSSESS_PK      'her'
      'ihre'   $$010111010111    POSSESS_PK      'her'
      'ihrer'  $$110111110111    POSSESS_PK      'her'
      'ihrem'  $$101000101000    POSSESS_PK      'her'
      'ihren'  $$100111100111    POSSESS_PK      'her'
      'ihres'  $$101000101000    POSSESS_PK      'her'


      'deren'  $$111111111111    POSSESS_PK      'them'
      'dessen' $$111111111111    POSSESS_PK      'it'
      ;

#ifnot;

Array LanguageDescriptors table

   !  Nur Dativ und Akkusativ werden berücksichtigt, da man (fast)
   !  nie Tokens im Nominativ und Genitiv braucht. (Der Nominativ
   !  wird zwar in der Anrede verwendet, aber vom Parser nicht
   !  gesondert behandelt.)

   !  word       possible GNAs   descriptor      connected
   !             to follow:      type:           to:
   !             a     i
   !             s  p  s  p
   !             mfnmfnmfnmfn

      'der'    $$010111010111    DEFART_PK       NULL   ! m-Nom, f-Dat, f/p-Gen
      'die'    $$010111010111    DEFART_PK       NULL   ! f/p-Nom/Akk
      'das'    $$001000001000    DEFART_PK       NULL   ! n-Nom/Akk
      'dem'    $$101000101000    DEFART_PK       NULL   ! m/n-Dat
      'den'    $$100111100111    DEFART_PK       NULL   ! m-Akk, p-Dat
      'des'    $$101000101000    DEFART_PK       NULL   ! m/n-Gen

      'einen'  $$100000100000    INDEFART_PK     NULL   ! m-Akk
      'ein'    $$001000001000    INDEFART_PK     NULL   ! m/n-Nom, n-Akk
      'einem'  $$101000101000    INDEFART_PK     NULL   ! m/n-Dat
      'eine'   $$010000010000    INDEFART_PK     NULL   ! f-Nom/Akk
      'einer'  $$010000010000    INDEFART_PK     NULL   ! f-Dat/Gen
      'eines'  $$101000101000    INDEFART_PK     NULL   ! m/n-Gen

      'dies'   $$000000000000    POSSESS_PK      0      ! Demonstrativpronomen
      'jene'   $$000000000000    POSSESS_PK      1      ! Demonstrativpronomen
      ;

#endif;

Array LanguageNumbers table
    'eins'         1
    'zwei'         2
    'drei'         3
    'vier'         4
    'fuenf'        5
    'sechs'        6
    'sieben'       7
    'acht'         8
    'neun'         9
    'zehn'        10
    'elf'         11
    'zwoelf'      12
    'dreizehn'    13
    'vierzehn'    14
    'fuenfzehn'   15
    'sechzehn'    16
    'siebzehn'    17
    'achtzehn'    18
    'neunzehn'    19
    'zwanzig'     20
    ;

! ------------------------------------------------------------------------------
!   Part III.   Translation
! ------------------------------------------------------------------------------

Array LanguageSynonyms table
    'am'        "an dem"
    'ans'       "an das"
    'aufs'      "auf das"
    'beim'      "bei dem"
    'durchs'    "durch das"
    'hinterm'   "hinter dem"
    'hinters'   "hinter das"
    'im'        "in dem"
    'ins'       "in das"
    'nebens'    "neben das"
    'uebers'    "ueber das"
    'ueberm'    "ueber dem"
    'unters'    "unter das"
    'unterm'    "unter dem"
    'vom'       "von dem"
    'vors'      "vor das"
    'vorm'      "vor dem"
    'zum'       "zu dem"
    'zur'       "zu der"
    'darin'     "in ihm/r"
    'damit'     "mit ihm/r"
    ;

Array LanguageTwins table
    'bis' 'auf'     "ausser"
    ;

!   Die Einträge in LanguageVerbPreps sind mögliche Kandidaten für den
!   zweiten Teil zusammengesetzter Verben: an-machen, zusammen-kleben,
!   fort-fahren, usw.

Array LanguageVerbPreps table
    "ab"          "an"          "auf"         "aus"
    "auseinander" "bei"         "darauf"      "daraus"
    "darein"      "drauf"       "durch"       "ein"
    "entzwei"     "fort"        "herauf"      "heraus"
    "herum"       "herunter"    "hin"         "hinauf"
    "hinaus"      "hinein"      "hinueber"    "hinweg"
    "hoch"        "kaputt"      "mit"         "nach"
    "nieder"      "rauf"        "raus"        "rein"
    "runter"      "ueber"       "um"          "umher"
    "unter"       "weg"         "zu"          "zusammen"
    ;

[ CheckIWantTo     wd;

!   Diese Routine prüft, ob es vor dem eigentlichen Satz Angaben gibt.
!   Wenn ja, werden diese übersprungen und die globale Variable
!   verb_wordnum wird korrekt gesetzt. Zurückgegeben wird der neue
!   Wert von verb_word, also die Vokabel-Adresse des Worts an
!   der Stelle verb_wordnum.
!
!   Muster:                                     vw              vwnum
!   > ich will/möchte Kohlrabi essen            'kohlrabi' *    3
!   > ich esse Kohlrabi                         'ess'           2
!   > ich, esse Kohlrabi                        'ich' **        1
!   > iss kohlrabi                              'iss'           1
!
!   *   Das wird später noch umgestellt
!   **  Hier wird "ich," als Befehl an mich gewertet, was aber
!       letzten Endes auch richtig geparst wird.

    wn = verb_wordnum;
    if (NextWord() ~= 'ich') return verb_word;
    wd = NextWord();
    if (wd == comma_word) return verb_word;
    if (wd ~= 'will' or 'moechte') wn--;
    verb_wordnum = wn;
    wd = NextWordStopped();
    if (wd == -1 or THEN1__WD) return -1;
    return wd;
];


[ CheckInfinitiveClause    ws we wd wl wa1 wa2 swap i j ll length olength;

!   Hier wird der Satz umgewandelt von "Tasche aufheben" in "heb tasche auf"
!   Das wird gemacht, bevor der Satz auf eine Himmelsrichtung ("> nw") hin
!   untersucht wird und bevor geprüft wird, ob ein Akteur angesprochen wird.
!
!   Die Infinitivkonstruktion hat eh einige Nachteile:
!
!   "> uschi, kein quatsch machen" wird richtig umgeleitet zu "> uschi,
!   machen kein quatsch", was interpretiert wird als Ansage an Uschi, sie
!   möge bitte keinen Quatsch machen, krude Grammatik hin oder her.
!   Das geht erst seit einem Fix, den mir Christian Blümke eingesandt
!   hat: Nicht nur das then-word, sondern auch das comma-word wird als
!   Kriterium fürs Beibehalten der Satzstruktur interpretiert.
!
!   "> ziegelstein, seidenhemd und rote pudelmütze nehmen" wird allerdings
!   mit "Du kannst nicht mit dem Ziegelstein reden" beschieden, und zwar
!   mit und ohne Komma als Knock-out-Kriterium.

    wn = ws = verb_wordnum;
    while (NextWordStopped() ~= THEN1__WD or -1 or comma_word);
    we = wn = wn - 2;
    if (we < ws) return verb_word;

    wd = NextWord();
    if (wd && ((wd->#dict_par1) & 1) ~= 0) {
        swap = true;
    } else {
        ll = LanguageVerbPreps-->0;
        olength = WordLength(we);
        for (i = 1 : i <= ll : i++) {
            wn = we;
            length = WordMatch(LanguageVerbPreps-->i);
            if (length == 0) continue;
            if (length == olength) continue;
            wa2 = WordAddress(we) - buffer;
            LTI_Insert(wa2 + length, ' ');
            we++;
            Tokenise__(buffer, parse);
            (orig_position-->0)++;
            (orig_position-->(orig_position-->0))--;
            for (j == NumberOfWords() : j > we : j--) {
                (orig_position-->j)++;
            }
            PruneWord(we);
            wn = we;
            wd = NextWord();
            if (wd && ((wd->#dict_par1) & 1) ~= 0) swap = true;
            break;
        }
    }
    if (swap) {
        wa1 = WordAddress(ws) - buffer;
        wa2 = WordAddress(we) - buffer;
        wl = WordLength(we);
        LTI_Insert(wa1, ' ');
        i = wl;
        while (i--) LTI_Insert(wa1, LTI_Delete(wa2 + wl));
        Tokenise__(buffer, parse);
        wn = ws;

        ll = orig_position-->we;
        for (j = we : j > ws : j--) {
            orig_position-->j = orig_position-->(j - 1);
        }
        orig_position-->ws = ll;

        num_words = NumberOfWords();
        #ifdef DEBUG;
        ws = WORDSIZE;
        #ifdef TARGET_ZCODE;
        we = WORDSIZE + buffer->1;
        #ifnot; ! TARGET_GLULX
        we = WORDSIZE + buffer-->0;
        #endif; ! TARGET_

        if (parser_echo) {
            print "[Echo: ~";
            for (i = ws: i < we: i++) print (char) buffer->i;
            print "~]^";
        }
        #endif; ! DEBUG

        return NextWordStopped();
    }
    return verb_word;
];

Array SynonymBuffer string 24;

[ printSynonymBuffer s;
    #ifdef TARGET_ZCODE;
    @output_stream 3 SynonymBuffer;
    print (string) s;
    @output_stream -3;
    return SynonymBuffer-->0;
    #ifnot;
    return PrintAnyToArray(Synonymbuffer, 24, s);
    #endif;
];

[ CheckSynonym w s   i n wd start length newlength offset;
    wn = w; wd = NextWord(); if (~~wd) return;
    #ifdef TARGET_ZCODE;
    offset = WORDSIZE;
    #ifnot;
    offset = 0;
    #endif;

    !   Die Tabelle durchackern
    n = (s-->0);
    for (i = 1 : i<=n : i = i + 2) {
        if (wd ~= s-->i) continue;
        start = WordAddress(w);
        length = WordLength(w);
        newlength = printSynonymBuffer(s-->(i+1));
        while (newlength > length) {
            LTI_Insert(start + length - buffer,
                SynonymBuffer->(offset + (--newlength)));
        }
        while (newlength < length) {
            start->(--length) = ' ';
        }
        for (i = 0 : i<length : i++) {
            start->i = SynonymBuffer->(offset + i);
        }
        ! Satz neu in Tokens ausfplitten
        length = NumberOfWords();
        Tokenise__(buffer, parse);

        ! Referenzpositionen merken
        n = NumberOfWords();
        length = length - n;
        for (w++ : w<=n : w++)
            orig_position-->w = orig_position-->w + length;
        orig_position-->0 = orig_position-->0 - length;
        return;
    }
];

[ CheckTwin w s   i n wd1 wd2 start length newlength offset;
    wn = w; wd1 = NextWord(); wd2 = NextWord();
    #ifdef TARGET_ZCODE;
    offset = WORDSIZE;
    #ifnot;
    offset = 0;
    #endif;
    if (~~wd1) return; if (~~wd2) return;

    !   Die Tabelle durchackern
    n = (s-->0);
    for (i = 1 : i<=n : i = i + 3) {
        if (wd1 ~= s-->i) continue;
        if (wd2 ~= s-->(i+1)) continue;
        start = WordAddress(w);
        length = WordAddress(w+1) + WordLength(w+1) - start;
        newlength = printSynonymBuffer(s-->(i+2));
        start->(newlength - 1) = ' ';
        while (newlength > length) {
            LTI_Insert(start + length - buffer,
                SynonymBuffer->(offset + (--newlength)));
        }
        while (newlength < length) {
            start->(--length) = ' ';
        }
        for (i = 0 : i<length : i++) {
            start->i = SynonymBuffer->(offset + i);
        }
        ! Satz neu in Tokens ausfplitten
        length = NumberOfWords();
        Tokenise__(buffer, parse);

        ! Referenzpositionen merken
        n = NumberOfWords();
        length = length - n;
        for (w++ : w<=n : w++)
            orig_position-->w = orig_position-->w + length;
        orig_position-->0 = orig_position-->0 - length;
        return;
    }
];

#ifdef CompoundHeads;
[ CheckCompoundHeads w   n i start length olength;
    wn = w;

    !   Die Tabelle durchackern
    n = (CompoundHeads-->0);
    for (i = 1 : i<=n : i = i + 2) {
        olength = WordLength(wn);
        length = WordMatch(CompoundHeads-->i);
        if (length==0) continue;
        if (length==olength) continue;

        start = WordAddress(w);
        if (start->length == '-') {
            LTI_Insert(start + length - buffer + 1, ' ');
        } else {
            LTI_Insert(start + length - buffer, ' ');
            if (CompoundHeads-->(i+1)==0)
                LTI_Insert(start + length - buffer, '-');
        }

        ! Satz neu in Tokens ausfplitten
        Tokenise__(buffer, parse);

        ! Referenzpositionen merken
        length = -1;
        for (w++ : w<=16 : w++)
            orig_position-->w = orig_position-->w + length;
        orig_position-->0 = orig_position-->0 - length;

        ! Eventuell den Rest beschneiden
        PruneWord(wn);
        return;
    }
];
#endif;

#ifdef CompoundTails;
[ CheckCompoundTails w   n i start length olength;
    wn = w;

    !   Die Tabelle durchackern
    n = (CompoundTails-->0);
    for (i = 1 : i<=n : i = i + 2) {
        olength = WordLength(wn);
        length = WordMatch(CompoundTails-->i, 0, olength);

        start = WordAddress(w);
        if (length == 0 && start->(olength - 1) == 'e' or 's' or 'n') {
            length = WordMatch(CompoundTails-->i, 0, olength-1);
            if (length) olength--;
        }
        if (length == 0 && start->(olength - 2) == 'e'
            && start->(olength - 1) == 'm' or 'n' or 'r' or 's') {
            length = WordMatch(CompoundTails-->i, 0, olength-2);
            if (length) olength = olength - 2;
        }

        if (length==0) continue;
        if (length==olength) continue;

        start = WordAddress(w) + olength - length;
        if ((start - 1)->0 == '-') {
            LTI_Insert(start - buffer, ' ');
        } else {
            LTI_Insert(start - buffer, ' ');
            if (CompoundTails-->(i+1)==0)
                LTI_Insert(start - buffer, '-');
        }

        ! Satz neu in Tokens ausfplitten
        Tokenise__(buffer, parse);

        ! Referenzpositionen merken
        length = -1;
        for (w++ : w<=16 : w++)
            orig_position-->w = orig_position-->w + length;
        orig_position-->0 = orig_position-->0 - length;

        ! Eventuell den Rest beschneiden
        PruneWord(wn);
        return;
    }
];
#endif;

! ------------------------------------------------------------------------------
!   Hier werden Ausrufe- und Fragezeichen entweder (a) herausgenommen, (b)
!   so separiert, dass sie eigene Wörter bilden oder (c) in Punkte umgewandelt.
! ------------------------------------------------------------------------------

#ifdef NO_PUNCTUATION;
Constant IGNORE_PUNCTUATION;
#endif;

[ is_Punctuation x;
    if (x=='?' or '!') rtrue;
    rfalse;
];

[ HandlePunctuation     i zs ze;

    zs = WORDSIZE;
    #ifdef TARGET_ZCODE;
    ze = 2 + buffer->1;
    #ifnot; ! TARGET_GLULX
    ze = WORDSIZE + buffer-->0;
    #endif; ! TARGET_
    i = 0;

    #ifdef IGNORE_PUNCTUATION;
    ! (a) Satzzeichen werden einfach zu Leerzeichen umgewandelt
    for (i = zs : i < ze : i++) {
        if (is_Punctuation(buffer->i)) buffer->i = ' ';
    }
    #endif;

    #ifdef SEPARATE_PUNCTUATION;
    ! (b) Satzzeichen werden von angrenzenden Wörtertn abgerückt
    for (i = zs : i < ze : i++) {
        if (is_Punctuation(buffer->i)) {
            if (i+1 <ze && buffer->(i+1) ~= ' ') {
                LTI_Insert(i+1, ' '); ze++;
            }
            if (i > zs && buffer->(i-1) ~= ' ') {
                LTI_Insert(i, ' '); ze++; i++;
            }
        }
    }
    #endif;

    #ifdef REPLACE_PUNCTUATION;
    ! (c) Satzzeichen werden zu Punkten umgewandelt
    for (i = zs : i < ze : i++) {
        if (is_Punctuation(buffer->i)) buffer->i = '.';
    }
    #endif;

];

! ------------------------------------------------------------------------------
!   Diese Routine beschneidet ein Wort so, dass eine sinnvolle Endung
!   abgeschnitten wird und ein bekanntes Wort übrig bleibt.
! ------------------------------------------------------------------------------

[ PruneWord w    start length;
    !   Wenn es das Wort gibt, gibt es hier nichts zu tun.
    wn = w; if (NextWord()) return;

    start = WordAddress(w);
    length = WordLength(w);

    !   Schauen, ob man -e, -s oder -n abschneiden kann
    if (start->(length - 1) == 'e' or 'n' or 's'
        && DictionaryLookup(start, length - 1)) {
        start->(length - 1) = ' ';
        Tokenise__(buffer, parse);
        return;
    }

    !   Schauen, ob man -em, -en, -er oder -es abschneiden kann
    if (start->(length - 2) == 'e'
        && start->(length - 1) == 'm' or 'n' or 'r' or 's'
        && DictionaryLookup(start, length - 2)) {
        start->(length - 1) = ' ';
        start->(length - 2) = ' ';
        Tokenise__(buffer, parse);
        return;
    }

    !   Durch Abschneiden können wir kein gültiges Wort erzeugen
    !   Nichts passiert...
];

[ NumberOfWords;
    #ifdef TARGET_ZCODE;
    return parse->1;
    #ifnot; ! TARGET_GLULX
    return parse-->0;
    #endif; ! TARGET_
];

[ LanguageToInformese    zs ze i;

!   (i)
!   Der Autor bekommt Kontrolle über die Informisierung

    if (PreInformese()) rtrue;

!   (ii)
!   Alle Umlaute und Eszetts in 'ae', 'oe', 'ue', 'ss' umwandeln

    zs = WORDSIZE;
    #ifdef TARGET_ZCODE;
    ze = 2 + buffer->1;
    #ifnot; ! TARGET_GLULX
    ze = WORDSIZE + buffer-->0;
    #endif; ! TARGET_

    for (i = zs : i < ze : i++) {
        if (buffer->i > 128) {
            switch(buffer->i) {
                CHAR_AE: buffer->i = 'a'; LTI_Insert(i+1, 'e'); ze++;
                CHAR_OE: buffer->i = 'o'; LTI_Insert(i+1, 'e'); ze++;
                CHAR_UE: buffer->i = 'u'; LTI_Insert(i+1, 'e'); ze++;
                CHAR_SS: buffer->i = 's'; LTI_Insert(i+1, 's'); ze++;
            }
        }
    }
    Tokenise__(buffer, parse);

!   (iii)
!   Kopie der Originaleingabe (allerdings ohne Umlaute) anlegen

    for (i = 0 : i < ze : i++) orig_buffer->i = buffer->i;

    ze = NumberOfWords();

    #ifdef TARGET_ZCODE;
    for (i=1 : i<=ze*2 : i++) orig_parse-->i = parse-->i;
    #ifnot;
    for (i=1 : i<=ze*3 : i++) orig_parse-->i = parse-->i;
    #endif;
    orig_parse-->0 = parse-->0;

    #ifdef TARGET_GLULX;
    for (i=1 : i<=MAX_BUFFER_WORDS : i++) orig_position-->i = i;
    #ifnot;
    for (i=1 : i<=16 : i++) orig_position-->i = i;
    #endif;
    orig_position-->0 = ze;

!   (iv)
!   Endungen (-e, -em, -en, -er, -es, -n, -s) abschneiden. Hierbei wird so lange
!   abgeschnitten, bis ein gültiges Wort gefunden wird. Wenn es also eine
!   'laute' gibt, so wird "Laute" belassen, ansonsten wird das '-e' abgestrennt.
!
!   Das kann bei einigen Paarungen von Substantiven mit Adjektiven (jung/Junge)
!   oder Verben (pump/Pumpe) Konflikte geben. Dann muss das Vokabular angepasst
!   werden. (Nicht schön, aber praktikabel.)

    for (i=0 : i<ze : i++) PruneWord(i + 1);

!   (v)
!   Umwandeln der Synonyme, hauptsächlich Kontraktionen aus Präpositionen und
!   bestimmten Artikeln. Hierzu wird das Feld Synonyms herangezogen.

    for (i=0 : i<ze : i++) {
        #ifdef Synonyms;
        CheckSynonym(i + 1, Synonyms);
        #endif;
        CheckSynonym(i + 1, LanguageSynonyms);
        #ifdef TARGET_ZCODE;
        ze = parse->1;
        #ifnot; ! TARGET_GLULX
        ze = parse-->0;
       #endif; ! TARGET_
    }

    for (i=0 : i<ze-1 : i++) {
        #ifdef Twins;
        CheckTwin(i + 1, Twins);
        #endif;
        CheckTwin(i + 1, LanguageTwins);
        #ifdef TARGET_ZCODE;
        ze = parse->1;
        #ifnot; ! TARGET_GLULX
        ze = parse-->0;
        #endif; ! TARGET_
    }

    #ifdef CompoundHeads;
    for (i=0 : i<ze : i++) {
        CheckCompoundHeads(i + 1);
        #ifdef TARGET_ZCODE;
        ze = parse->1;
        #ifnot; ! TARGET_GLULX
        ze = parse-->0;
        #endif; ! TARGET_
    }
    #endif;

    #ifdef CompoundTails;
    for (i=0 : i<ze : i++) {
        CheckCompoundTails(i + 1);
        #ifdef TARGET_ZCODE;
        ze = parse->1;
        #ifnot; ! TARGET_GLULX
        ze = parse-->0;
        #endif; ! TARGET_
    }
    #endif;

!   (vi)
!   Ausrufe- und Fragezeichen behandeln, falls gewünscht.

    HandlePunctuation();

!   (vii)
!   Der Autor bekommt noch einmal Kontrolle über die Informisierung.

    if (PostInformese()) rtrue;

!   (viii)
!   Echo der neuen Zeile, wenn das Parser-Tracing gesetzt ist
    #ifdef DEBUG;
    zs = WORDSIZE;
    #ifdef TARGET_ZCODE;
    ze = WORDSIZE + buffer->1;
    #ifnot; ! TARGET_GLULX
    ze = WORDSIZE + buffer-->0;
    #endif; ! TARGET_
    if (parser_echo) {
        print "[Echo: ~";
        for (i = zs: i < ze: i++) print (char) buffer->i;
        print "~]^";
    }
    #endif; ! DEBUG
];

! ------------------------------------------------------------------------------
!   Part IV.   Printing
! ------------------------------------------------------------------------------

Constant LanguageAnimateGender   = male;
Constant LanguageInanimateGender = neuter;

[ LanguageDirection d;
    switch (d) {
      n_to:    print "Norden";
      s_to:    print "Süden";
      e_to:    print "Osten";
      w_to:    print "Westen";
      ne_to:   print "Nordosten";
      nw_to:   print "Nordwesten";
      se_to:   print "Südosten";
      sw_to:   print "Südwesten";
      u_to:    print "oben";
      d_to:    print "unten";
      in_to:   print "drinnen";
      out_to:  print "drau@30en";
      default: return RunTimeError(9,d);
    }
];

[ LanguageDirectionAdj d;
    switch (d) {
      n_to:    print "nördlich";
      s_to:    print "südlich";
      e_to:    print "östlich";
      w_to:    print "westlich";
      ne_to:   print "nordöstlich";
      nw_to:   print "nordwestlich";
      se_to:   print "südöstlich";
      sw_to:   print "südwestlich";
      u_to:    print "rauf";
      d_to:    print "runter";
      in_to:   print "rein";
      out_to:  print "raus";
      default: return RunTimeError(9,d);
   }
];

[ LanguageNumber n f;
  if (n==0) {
      print "null"; rfalse;
  }
  if (n<0) {
      print "minus "; n = -n;
  }
  if (n>=1000) {
      print (LanguageNumber) n/1000, "tausend"; n=n%1000; f=1;
  }
  if (n>=100) {
      if (f==1); print (LanguageNumber) n/100, "hundert"; n=n%100;
  }
  if (n==0) rfalse;
  switch(n)
  {   1:  print "ein";
      2:  print "zwei";
      3:  print "drei";
      4:  print "vier";
      5:  print "fünf";
      6:  print "sechs";
      7:  print "sieben";
      8:  print "acht";
      9:  print "neun";
      10: print "zehn";
      11: print "elf";
      12: print "zwölf";
      13: print "dreizehn";
      14: print "vierzehn";
      15: print "fünfzehn";
      16: print "sechzehn";
      17: print "siebzehn";
      18: print "achtzehn";
      19: print "neunzehn";
      20 to 99:
          if (n%10 ~= 0) print (LanguageNumber) n%10, "und";
          switch(n/10)
          {  2: print "zwanzig";
             3: print "drei@30ig";
             4: print "vierzig";
             5: print "fünfzig";
             6: print "sechzig";
             7: print "siebzig";
             8: print "achtzig";
             9: print "neunzig";
          }

  }
];

[ LanguageTimeOfDay hours mins;
   if (hours < 0 || hours > 23 || mins < 0 || mins > 59) rfalse;
   print hours/10, hours%10, ":", mins/10, mins%10;
];

! ----------------------------------------------------------------------------
!  Diese LanguageVerb wurde im Forum erarbeitet und von Christoph Winkler
!  in diese kompakte Form gebracht. Danke!
!
!  Hier soll aber nicht die Befehlsform, sondern der Infinitiv ausgegeben
!  werden, damit man die Routine in PrintCommand() verwenden kann.
! ----------------------------------------------------------------------------

Array UmlautAux -> 12;

[ UmlautAddress i cap  letter next last skip start length;

    ! Vokabel auf Hilfsfeld schreiben
    #ifdef TARGET_GLULX;
    length = PrintAnyToArray(UmlautAux, 24, i);
    start = 0;
    #ifnot;
    @output_stream 3 UmlautAux;
    print (address) i;
    @output_stream -3;
    length = UmlautAux-->0;
    start = 2;
    #endif;

    ! Vokabel mit ersetzten Umlauten ausgeben
    ! *** und wenn (cap==true) mit Großbuchstaben am Anfang
    for (i=0 : i < length : i++) {
        letter = UmlautAux->(i+start);
        if(i + 1 >= length) next = 0;
        else next = UmlautAux ->(i+start+1);

        if (letter == 'a' && next == 'e') {
            if (i==0 && cap) print "Ä"; else print "ä"; skip = true;
        } else if (letter == 'o' && next == 'e') {
            if (i==0 && cap) print "Ö"; else print "ö"; skip = true;
        } else if (letter == 'u' && next == 'e' && last ~= 'q' or 'e') {
           if (i==0 && cap) print "Ü"; else print "ü"; skip = true;
        } else if (skip) skip = false;
        else if (i==0 && cap) print (char) UpperCase(letter); else print (char) letter;
        last = letter;
    }
];

[ CUmlautAddress i; UmlautAddress(i, true); ];

[ IsDirectionWord w    o;
    objectloop (o in Compass) {
        if (WordInProperty(w, o, name)) return o;
    }
    rfalse;
];

! *** LastCharacterAddress() ermittelt die n-letzten Zeichen einer Vokabel.
!     LanguageVerb() benutzt dies, um den Infinitv genauer zu
!     rekonstruieren.


[ LastCharacterAddress i j       length start ch;  ! *** hinzugefügt von CB
    ! Vokabel auf Hilfsfeld schreiben
    #ifdef TARGET_GLULX;
    length = PrintAnyToArray(UmlautAux, 24, i);
    start = 0;
    #ifnot;
    @output_stream 3 UmlautAux;
    print (address) i;
    @output_stream -3;
    length = UmlautAux-->0;
    start = 2;
    #endif;

    ch = UmlautAux->(length-1+start-j); ! letzes Zeichen - j
    !!!print " {ch == ", (char) ch, "} ";
    return ch;
];

[ LanguageVerb i    o last;
    switch (i) {
        'l//': print "schauen";
        'z//': print "warten";
        'j//': print "ja";
        'x//','u//','b//': print "betrachten";
        'v//': print "verlassen";
        'i//', 'inv', 'inventar': print "Inventar anzeigen";
        'raus', 'hinaus', 'heraus': print "raus gehen";

        ! Wörter, die zu lang sind oder in denen ein ß ist

        'giess':         print "gie@30en";
        'oeffne':        print "öffnen";
        'ueberreich':    print "überreichen";
        'praesentier':   print "präsentieren";
        'konsultier':    print "konsultieren";
        'durchwuehl':    print "durchsuchen";
        'durchstoeber':  print "durchstöbern";
        'schliess':      print "schlie@30en";
        'verschliess':   print "verschlie@30en";
        'verriegel':     print "verriegeln";
        'zertruemmer':   print "zertrümmern";
        'unterricht':    print "unterrichten";
        'zerdrueck':     print "zerdrücken";
        'zerquetsch':    print "zerquetschen";
        'durchschneid':  print "durchschneiden";
        'zerschneid':    print "zerschneiden";

        ! Starke Verben

        'betritt':       print "betreten";
        'tritt':         print "treten";
        'nimm':          print "nehmen";
        'gib':           print "geben";
        'wirf':          print "werfen";
        'brich':         print "brechen";
        'zerbrich':      print "zerbrechen";
        'iss':           print "essen";
        'friss':         print "fressen";
        'sprich':        print "sprechen";
        'sieh':          print "sehen";
        'lies':          print "lesen";
        'erwirb':        print "erwerben";
        'klettre':       print "erklimmen";
        'wedle':         print "wedeln";
        'baumle':        print "baumeln";
        'streichl':      print "streicheln";

        default:

        ! Himmelsrichtung

        o = IsDirectionWord(i); if (o) {
            print "nach ", (WithoutArt) o, " gehen";
            rtrue;
        }

        ! Schwache Verben: Infinitiv ist Form ohne 'e' und '-en'
        ! *** oder auch mit 'e' und ohne 'n'.

        !*** falls doch noch ein 'e' am Verb dranhängt wird nur ein "n"
        !    ausgegeben (s. UmlautAddress(), die jetzt das letzte Zeichen
        !    zurückgibt)
        !
        !    NextToLast() ermittelt die vorletzten Zeichen, um den Infinitiv
        !    etwas genauer erraten zu können.

        if(PrintVerb(i) == 0) {
            UmlautAddress(i);
            last = LastCharacterAddress(i); ! letztes Zeichen
            if (last == 'e'
                ||  (last == 'l' or 'r'
                    && LastCharacterAddress(i,1) == 'e' ! vorletztes Zeichen
                    && LastCharacterAddress(i,2) ~=     ! vorvorletztes Zeichen
                        'i' or 'a' or 'o' or 'u')) print "n";
            else print "en";
        }
    }
];

! ----------------------------------------------------------------------------
!  LanguageVerbIsDebugging is called by SearchScope.  It should return true
!  if word w is a debugging verb which needs all objects to be in scope.
! ----------------------------------------------------------------------------

#Ifdef DEBUG;
[ LanguageVerbIsDebugging w;
    if (w == 'purloin' or 'tree' or 'abstract' or 'gonear'
                       or 'scope' or 'showobj' or 'dekl') rtrue;
    rfalse;
];
#Endif;

! ----------------------------------------------------------------------------
!  LanguageVerbLikesAdverb is called by PrintCommand when printing an UPTO_PE
!  error or an inference message.  Words which are intransitive verbs, i.e.,
!  which require a direction name as an adverb ('walk west'), not a noun
!  ('I only understood you as far as wanting to touch /the/ ground'), should
!  cause the routine to return true.
! ----------------------------------------------------------------------------

![ LanguageVerbLikesAdverb w;
 !   w = w; rfalse;
!];

[ LanguageVerbLikesAdverb w;
    !w = w; rfalse;
    if (w == 'geh' or 'lauf' or 'renn' or 'wander'
                   or 'fluecht' or 'flieh' or 'schreit'
                   or 'spazier') rtrue;

    if (w == 'schau' or 'seh' or 'sieh'
                     or 'blick' or 'lug' or 'guck' or 'kuck') rtrue;


    rfalse;
];

! ----------------------------------------------------------------------------
!  LanguageVerbMayBeName is called by NounDomain when dealing with the
!  player's reply to a "Which do you mean, the short stick or the long
!  stick?" prompt from the parser. If the reply is another verb (for example,
!  LOOK) then then previous ambiguous command is discarded /unless/
!  it is one of these words which could be both a verb /and/ an
!  adjective in a 'name' property.
!
!  In deform wird das ganz pragmatisch gehandhabt: Alle möglichen Objekte sind
!  bekannt. Wenn das Wort zu dessen name-Einträgen gehört, so soll das Wort
!  nicht als Verb bahendelt werden, ansonsten schon. Das fängt keine komplexen
!  parse_name-Konstrukte ab, aber schließlich wird dies auch nur aufgerufen,
!  wenn w ein Verb sein kann. Außerdem ist die Abfrage so kontextsensitiv.
! ----------------------------------------------------------------------------

[ LanguageVerbMayBeName w    i;
    for (i=0 : i < number_matched : i++) {
        if (WordInProperty(w, match_list-->i, name)) rtrue;
    }
    rfalse;
];

! ----------------------------------------------------------------------------
!  LanguageIsVerb ist eine Krücke, um zu erkennen, ob ein Wort in buffer2, das
!  nicht informisiert wurde, ein Verb ist. Hier muss etwas gemacht werden, da
!  Verben mit Umlauten und die Form mit 'e' sonst nicht erkannt werden.
! ----------------------------------------------------------------------------

[ LanguageIsVerb b p wordnum    wa wl i zs ze;

    if (b == buffer or buffer2) {
        ! Zuerst die Umlaute herausfischen
        zs = WORDSIZE;
        #ifdef TARGET_ZCODE;
        ze = 2 + b->1;
        #ifnot; ! TARGET_GLULX
        ze = WORDSIZE + b-->0;
        #endif; ! TARGET_

        for (i = zs : i < ze : i++) {
            switch(b->i) {
                CHAR_AE: b->i = 'a'; LTI_Insert(i+1, 'e', b); ze++;
                CHAR_OE: b->i = 'o'; LTI_Insert(i+1, 'e', b); ze++;
                CHAR_UE: b->i = 'u'; LTI_Insert(i+1, 'e', b); ze++;
                CHAR_SS: b->i = 's'; LTI_Insert(i+1, 's', b); ze++;
            }
        }
        Tokenise__(b, p);

        ! Dann schauen, ob das Wort wordnum erkannt wird
        #ifdef TARGET_ZCODE;
        i = 2*wordnum - 1;
        #ifnot;
        i = 3*wordnum - 2;
        #endif;
        if (p-->i) return p-->i;

        #ifdef TARGET_ZCODE;
        wa = b + p->(wordnum*4 + 1);
        wl = p->(wordnum*4);
        #ifnot;
        wa = b + p-->(wordnum*3);
        wl = p-->(wordnum*3-1);
        #endif;

        ! Wenn nicht, ein 'e' abschneiden und nochmal schauen
        if (wa->(wl - 1) == 'e') {
            wa->(wl - 1) = ' ';
            Tokenise__(b, p);
            return p-->i;
        }
        ! *** oder nach einem 't' gucken
        !     (dieser Block blieb bislang unberücksichtigt, weil eine
        !     geschweifte Klammer verrutscht war)
        else {

            !wa = WordAddress(wordnum);
            !wl = WordLength(wordnum);
            if (wa->(wl-1) == 't') {
                i = DictionaryLookup(wa, wl - 1);
                if (i && (i->#dict_par1 & 1)) {
                    imperative_form = IMP_IHR;
                    return i;
                }
            }

            !wa = OriginalAddress(wordnum);
            !wl = OriginalLength(wordnum);
        }
    }

    rfalse;
];

Constant NKEY__TX     = "N = nächstes";
Constant PKEY__TX     = "V = voriges";
Constant QKEY1__TX    = "     Z = zurück";
Constant QKEY2__TX    = "     Z = zurück";
Constant RKEY__TX     = "EINGABE = Artikel lesen";

Constant NKEY1__KY      = 'N';
Constant NKEY2__KY      = 'n';
Constant PKEY1__KY      = 'V';
Constant PKEY2__KY      = 'v';
Constant QKEY1__KY      = 'Z';
Constant QKEY2__KY      = 'z';

Constant SCORE__TX      = "Punkte: ";
Constant MOVES__TX      = "Züge: ";
Constant TIME__TX       = "Zeit: ";
Constant CANTGO__TX     = "Du kannst nicht in diese Richtung gehen.";
Constant FORMER__TX     = "dein früher@00 Ich";
Constant YOURSELF__TX   = "du";
Constant YOU__TX        = "du";
Constant DARKNESS__TX   = "Dunkelheit";

Constant THOSET__TX     = "all das";
Constant THAT__TX       = "das";
Constant OR__TX         = " oder ";
Constant NOTHING__TX    = "nichts";
Constant IS__TX         = " ist";
Constant ARE__TX        = " sind";
Constant IS2__TX        = "ist ";
Constant ARE2__TX       = "sind ";
Constant AND__TX        = " und ";
Constant WHOM__TX       = "";
Constant WHICH__TX      = "";
Constant COMMA__TX      = ", ";

[ print_yourself k;
  switch (k) {
      0: print "du";
      1: print "deiner";
      2: print "dir";
      3: print "dich";
  }
  rtrue;
];

[ IsDativeToken p; ! *** hinzugefügt von CB
    if (line_ttype-->(p) == GPR_TT && line_tdata-->(p) == dative) rtrue;
    rfalse;
];

[ PrintWomitCommand x1; ! *** siehe L_M ##Miscellany, 49
    x1 = pcount;
    if (pattern-->(pcount - 1) == PATTERN_NULL) pcount--;
        if (pattern-->(pcount - 1) > REPARSE_CODE) {
            ! Wenn das letzte Element eines unvollständigen Satzes eine
            ! Präposition, also eine Vokabel anstatt eines Objekts ist,
            ! dann soll sie als Fragewort ("Womit" usw. ) benutzt werden.
            ! pcount muss nachher wieder zurückgesetzt werden, damit die
            ! Antwort richtig analysiert werden kann.

            ! *** Dativ-Token berücksichtigen, damit in GWomit() "worin" und "worein"
            !     unterschieden werden können.
            GWomit(No__Dword(pattern-->(pcount-1)-REPARSE_CODE), IsDativeToken(pcount-1)); print " ";
            pcount--;
        } else print "Was ";
        if (actor ~= player) print "soll ", (der) actor, " ";
        else print "willst du ";
        PrintCommand(); print "?^";
        pcount = x1;
];

[PrintWemCommand x1; ! *** siehe L_M ##Miscellany, 48
    x1 = pcount;
    if (pattern-->(pcount - 1) == PATTERN_NULL) pcount--;
    if (pattern-->(pcount - 1) > REPARSE_CODE) {
        ! Siehe Erklärung unter PrintWomitCommand.
        ! *** Großbuchstabe am Anfang
        CUmlautAddress(No__Dword(pattern-->(pcount-1)-REPARSE_CODE));
        print " wem ";
        pcount--;
    } else {
            if (IsDativeToken(pcount-1)) print "Wem ";
            else print "Wen ";
    }
    if (actor ~= player) print "soll ", (der) actor, " ";
    else print "willst du ";
    PrintCommand(); print "?^";
    pcount = x1;
];

! ----------------------------------------------------------------------------
!  Felder für die Ausgabe der Objekte im richtigen Fall
!
!               p       m       f       n
!       Nom     ...     ...     ...     ...
!       Gen     ...     ...     ...     ...
!       Dat     ...     ...     ...     ...
!       Akk     ...     ...     ...     ...
! ----------------------------------------------------------------------------

Constant Nom = 0;
Constant Gen = 1;
Constant Dat = 2;
Constant Akk = 3;

Array LanguageArticles -->
        "die "    "der "    "die "    "das "     ! bestimmte Artikel
        "der "    "des "    "der "    "des "
        "den "    "dem "    "der "    "dem "
        "die "    "den "    "die "    "das "
        0         "ein "    "eine "   "ein "     ! unbestimmte Artikel
        0         "eines "  "einer "  "eines "
        0         "einem "  "einer "  "einem "
        0         "einen "  "eine "   "ein "
        "keine "  "kein "   "keine "  "kein "    ! verneinte
        "keiner " "keines " "keiner " "keines "  ! unbestimmte Artikel
        "keinen " "keinem " "keiner " "keinem "
        "keine "  "keinen " "keine "  "kein "
        ;
Constant def_article_base 0;
Constant indef_article_base 16;
Constant neg_article_base 32;

Array LanguageSuffixes -->
        "en"   "en"   "e"    "es"       ! mit bestimmtem Artikel
        "en"   "en"   "en"   "en"
        "en"   "en"   "en"   "en"
        "en"   "en"   "e"    "es"
        "e"    "er"   "e"    "es"       ! mit unbestimmtem Artikel
        "er"   "en"   "en"   "en"
        "en"   "en"   "en"   "en"
        "e"    "en"   "e"    "es"
        "e"    "er"   "e"    "es"       ! ohne Artikel
        "er"   "en"   "er"   "en"
        "en"   "em"   "er"   "em"
        "e"    "en"   "e"    "es"
        "e"    "er"   "e"    "es"       ! Demonstrativpronomen.
        "er"   "es"   "er"   "es"
        "en"   "em"   "er"   "em"
        "e"    "en"   "e"    "es"
        "e"    ""     "e"    ""         ! Possessivpronimen
        "er"   "es"   "er"   "es"
        "en"   "em"   "er"   "em"
        "e"    "en"   "e"    ""
        ;

Constant def_suffix_base 0;
Constant indef_suffix_base 16;
Constant plain_suffix_base 32;
Constant demonstrative_suffix_base 48;
Constant possessive_suffix_base 64;

Array PersonalPronouns -->
        "sie"     "er"      "sie"     "es"
        "ihrer"   "seiner"  "ihrer"   "seiner"
        "ihnen"   "ihm"     "ihr"     "ihm"
        "sie"     "ihn"     "sie"     "es"
        ;

Constant MAX_CG_STACK = 8;

Array CG_buffer --> 2 * MAX_CG_STACK;
Global CG_pointer;

[ GenderChanged o   i;
    for (i = 0 : i < MAX_CG_STACK : i++) {
        if (CG_buffer-->(2*i) == o) return CG_buffer-->(2*i + 1);
    }
    rfalse;
];

[ GenderNotice o g   i;
    !!!print "{GN: ", (object) o, ", ", g, "}";

    ! Objekt und Genus löschen
    for (i=0 : i < MAX_CG_STACK : i++) {
        if (CG_buffer-->(2*i) == o) {
            CG_buffer-->(2*i) = 0;
        }
    }

    if (g) {
        ! Objekt und Genus setzen
        CG_buffer-->(CG_pointer*2) = o;
        CG_buffer-->(CG_pointer*2 + 1) = g;
        CG_pointer++;
        CG_pointer = CG_pointer % MAX_CG_STACK;
    }
];

[ Gender obj flag   g;
    if (obj == nothing) return;

    ! Wenn flag gesetzt ist, nach Gender-Änderungen schauen
    if (flag) {
        g = GenderChanged(obj);
        if (g) {
            switch (g) {
                pluralname: return 0;
                male:       return 1;
                female:     return 2;
                neuter:     return 3;
            }
        }
    }

    ! Genus zurücksetzen
    if (~~printing_command) GenderNotice(obj, 0);

    ! Übliches Verfahren nach Attributen
    if (obj has pluralname) return 0;
    if (obj has male) return 1;
    if (obj has female) return 2;
    return 3;
];

[ SetLowStrings k g;

!   Endungen für Substantive: @01 bis @04

    if (k==Dat || (g==1 && k~=Nom))
                string 1 "n"; else string 1 "";
    if (k~=Nom) string 2 "en"; else string 2 "";
    if (k==Gen) { string 3 "s"; string 4 "es"; }
    else        { string 3 "";  string 4 ""; }

!   Endungen für Adjektive: @00

    if (indef_mode==0) {
        switch(k*4 + g) {
            1, 2, 3, 14, 15:    string 0 "e";
            default:            string 0 "en";
        }
    } else if (indef_mode==1){
        switch(k*4 + g) {
            0, 2, 12, 14:       string 0 "e";
            1, 4:               string 0 "er";
            3, 15:              string 0 "es";
            default:            string 0 "en";
        }
    } else {
        switch(k*4 + g) {
            0, 2, 12, 14:       string 0 "e";
            1, 4, 6, 10:        string 0 "er";
            3, 15:              string 0 "es";
            9, 11:              string 0 "em";
            default:            string 0 "en";
        }
    }
];

[ DeclinationSuffix o d;
    if (~~(o provides dekl)) rfalse;
    if (o.#dekl/WORDSIZE > 1) {
        ! dekl ist ein Feld, ähnlich wie suffixes;
        if (o.#dekl/WORDSIZE == 4) return o.&dekl-->short_name_case;
        print "^[** Wenn dekl ein Feld ist mu@31 es vier Einträge haben. **]^";
        rfalse;
    }
    if (o.dekl == 0) {
        if (o provides suffixes) {
            if (o.#suffixes/WORDSIZE == 4) return o.&suffixes-->short_name_case;
            if (o.suffixes ofclass Routine) return o.suffixes(short_name_case);
            print "^[** Wenn suffix ein Feld ist mu@31 es vier Einträge haben. **]^";
            rfalse;
        }
    }
    d = o.dekl;
    if (d ofclass Routine) d = o.dekl();
    if (d > 0) {
        if (o has pluralname) d = d + 20;
        switch (d) {
            21, 22, 24, 27, 28:         d = Dativ_n;
            1, 2, 3, 5:                 d = Genitiv_s;
            4:                          d = Genitiv_es;
            6:                          d = Akkusativ_en;
        }
        if (d > 0) d = 0;
    }
    if (d == Dativ_n && short_name_case == Dat) return "n";
    if (d == Akkusativ_en && short_name_case ~= Nom) return "en";
    if (d == Genitiv_s && short_name_case == Gen) return "s";
    if (d == Genitiv_es && short_name_case == Gen) return "es";
    rfalse;
];

[ print_adj s;
    print (string) s, "@00";
];

[ print_subst s;
    print (string) s;
    s = DeclinationSuffix(self);
    if (s ofclass String) print (string) s;
];

[ print_article indef g   s;
    if (indef < 0) return;
    s = LanguageArticles-->(indef*16 + short_name_case*4 + g);
    if (s) print (string) s;
];

[ PrefaceByArticle o acode pluralise capitalise  g s;
    ! acode ist die von Inform vorgesehene Art des Artikels
    ! 0 "The ", 1 "the ", 2, "a(n) ". Das dieses System in deform
    ! abgeschafft ist, müssen wir uns was anderes überlegen.
    !
    ! capitalise gibt an, ob das Gedruckte groß geschrieben werden
    ! soll. Das wird in deform direkt in GDer() usw. festgelegt,
    ! diese Flagge wird also hier ignoriert.

    if (pluralise) {
        SetLowStrings(short_name_case, 0);
        if (acode < 2) {
            ! Modus ist 0 (best. artikel) und Genus ist auch 0 (Plural)
            print (string) LanguageArticles-->(short_name_case*4);
        }
        #ifdef TARGET_ZCODE;
        print (EnglishNumber) pluralise;
        #ifnot;
        PrintAnything(EnglishNumber, pluralise);
        #endif;
    } else {
        if (indef_mode >= 0) {
            g = Gender(o);
            s = LanguageArticles-->(indef_mode*16 + short_name_case*4 + g);
            if (s) print (string) s;
        }
        print (PSN__) o;
    }
    capitalise = 0;
];

[ PSN__ o i    suppress_sn;
    if (o == 0) { print (string) NOTHING__TX; rtrue; }
    switch (metaclass(o)) {
      Routine:  print "<Routine ", o, ">"; rtrue;
      String:   print "<String ~", (string) o, "~>"; rtrue;
      nothing:  print "<Illegales Objekt Nr. ", o, ">"; rtrue;
    }
    if (o == player) return print_yourself(short_name_case);

    #Ifdef LanguagePrintShortName;
    if (LanguagePrintShortName(o)) rtrue;
    #Endif; ! LanguagePrintShortName

    if (o.&short_name ~= 0 && o.short_name == no_short_name) suppress_sn = true;

    if (o provides adj) {
        for (i=0: i<o.#adj/WORDSIZE: i++) {
            print (string) o.&adj-->i, "@00";
            if (suppress_sn==0 || i+1 < o.#adj/WORDSIZE) print " ";
        }
    }

    if (indef_mode==1 && o.&short_name_indef ~= 0
        && PrintOrRun(o, short_name_indef, 1) ~= 0) rtrue;
    if (~~suppress_sn) {
        if (o.&short_name == 0 || PrintOrRun(o, short_name, 1) == 0)
        print (object) o;
        i = DeclinationSuffix(o);
        if (i) print (string) i;
    }

    if (o provides post) {
        print " ";
        PrintOrRun(o, post, 1);
    }
];

[ Indefart o k    i g;
    i = indef_mode; indef_mode = true;
    g = Gender(o);
    short_name_case = k;
    SetLowStrings(k, g);
    if (o has proper) { indef_mode = NULL; print (PSN__) o; return; }
    if (o provides article) {
        if (o.article == definite) {
            return Defart(o, k);
        } else if (o.article == yours) {

            !*** Die Low-Strings für die Ausgabe des Plurals mit "dein" anpassen.
            !    Flektiert wird wie beim bestimmten Artikel:
            !    "nervige Nachbarn" vs. "deine nervigen Nachbarn"

            if (o has pluralname) {
                indef_mode = false;
                SetLowStrings(k, g);
            }

            print "dein",
            (string) LanguageSuffixes-->(possessive_suffix_base + 4*k + g),
            " ", (PSN__) o; indef_mode = i; return;
        } else if (o.article ~= no_article) {
            PrintOrRun(o, article, 1); print " ";
        } else {
            indef_mode = -1;
            SetLowStrings(k, g);
        }
        print (PSN__) o; indef_mode = i; return;
    }
    PrefaceByArticle(o, 2); indef_mode = i;
];

[ CInDefArt o k;
    RunCapitalised(Indefart, o, k);
];

[ Defart o k    i g;
    i = indef_mode; indef_mode = false;
    g = Gender(o);
    short_name_case = k;
    SetLowStrings(k, g);
    if ((~~o ofclass Object) || o has proper) {
        indef_mode = NULL; print (PSN__) o; indef_mode = i;
        return;
    }
    PrefaceByArticle(o, 1); indef_mode = i;
];

[ CDefart o k;
    RunCapitalised(Defart, o, k);
];

[ WithoutArt o k    i g;
    i = indef_mode; indef_mode = -1;
    g = Gender(o);
    short_name_case = k;
    SetLowStrings(k, g);
    if ((~~o ofclass Object) || o has proper) {
        indef_mode = NULL; print (PSN__) o; indef_mode = i;
        return;
    }
    PrefaceByArticle(o, 1); indef_mode = i;
];

[ NegativeArt obj k;
    print (string) LanguageArticles-->(neg_article_base + 4*k + Gender(obj));
];

[ PersonalPron obj k;
    print (string) PersonalPronouns-->(4*k + Gender(obj, true));
];

[ RunCapitalised a b c    i length;
!   Wie PrintCapitalised, nur für Routinen, so dass die Ausgabe von
!   a(b, c) am Anfang groß geschrieben wird.

    if (~~(a ofclass Routine)) return;
    PrintToBuffer(StorageForShortName, 160, a, b, c);
    StorageForShortName->WORDSIZE = UpperCase(StorageForShortName->WORDSIZE);
    length = StorageForShortName-->0;
    for (i=WORDSIZE: i<length+WORDSIZE: i++) print (char) StorageForShortName->i;
];

[ GDer obj; CDefArt(obj, 0); ];
[ GDes obj; CDefArt(obj, 1); ];
[ GDem obj; CDefArt(obj, 2); ];
[ GDen obj; CDefArt(obj, 3); ];

[ der obj; DefArt(obj, 0); ];
[ des obj; DefArt(obj, 1); ];
[ dem obj; DefArt(obj, 2); ];
[ den obj; DefArt(obj, 3); ];

[ GEin obj;   RunCapitalised(IndefArt, obj, 0); ];
[ GEines obj; RunCapitalised(IndefArt, obj, 1); ];
[ GEinem obj; RunCapitalised(IndefArt, obj, 2); ];
[ GEinen obj; RunCapitalised(IndefArt, obj, 3); ];

[ ein obj;   IndefArt(obj, 0); ];
[ eines obj; IndefArt(obj, 1); ];
[ einem obj; IndefArt(obj, 2); ];
[ einen obj; IndefArt(obj, 3); ];

[ kein obj;   NegativeArt(obj, 0); ];
[ keines obj; NegativeArt(obj, 1); ];
[ keinem obj; NegativeArt(obj, 2); ];
[ keinen obj; NegativeArt(obj, 3); ];

[ er obj;     PersonalPron(obj, 0); ];
[ seiner obj; PersonalPron(obj, 1); ];
[ ihm obj;    PersonalPron(obj, 2); ];
[ ihn obj;    PersonalPron(obj, 3); ];

[ GEr obj;     RunCapitalised(PersonalPron, obj, 0); ];
[ GSeiner obj; RunCapitalised(PersonalPron, obj, 1); ];
[ GIhm obj;    RunCapitalised(PersonalPron, obj, 2); ];
[ GIhn obj;    RunCapitalised(PersonalPron, obj, 3); ];

[ ist obj;
    if (obj == player) { print "bist"; return; }
    if (obj has pluralname) print "sind"; else print "ist";
];

[ hat obj;
    if (obj == player) { print "hast"; return; }
    if (obj has pluralname) print "haben"; else print "hat";
];

[ wird obj;
    if (obj == player) { print "wirst"; return; }
    if (obj has pluralname) print "werden"; else print "wird";
];

[ ___t obj;
    if (obj == player) { print "st"; return; }
    if (obj has pluralname) print "en"; else print "t";
];

[ ___et obj;
    if (obj == player) { print "est"; return; }
    if (obj has pluralname) print "en"; else print "et";
];

[ plur pl sg obj;
    if (obj has pluralname) print (string) pl; else print (string) sg;
];

[ WriteListFromCase o style case depth   old_case old_indef;
    !   Das ISARE_BIT ist nur sinnvoll, wenn man die Liste im Nominativ
    !   ausgibt, daher wird die Liste im Nominativ ausgegeben.
    if (case ~= Nom && style & ISARE_BIT) case = Nom;

    old_case = short_name_case; ! Alten Fall merken. (In ListWriteR gab es keine
    short_name_case = case;     ! freien lokalen Var. mehr, um case zu übergeben.)

    old_indef = indef_mode;
    if (style & DEFART_BIT) indef_mode = 0;
    else if (style & NOARTICLE_BIT) indef_mode = -1;
    else indef_mode = 1;

    #ifdef NO_NESTED_LISTS;
    if ((style & RECURSE_BIT) && (style & ENGLISH_BIT)) style = style | APPEND_BIT;
    #endif;

    WriteListFrom(o, style, depth);

    short_name_case = old_case;
    indef_mode = old_indef;
];

[ GWomit w dative_flag  first;
    ! Das schreibt ein Fragewort, das nach einem indirekten Objekt fragt,
    ! wie das namengebende Womit, oder Woraus, Wonach, Worein. Das wird
    ! zusammen mit PrintCommand benutzt: "Woran möchtest du ziehen?"

    print "Wo";
    if (w=='in') {
        if (dative_flag) print "rin";
        else             print "rein";
        rtrue;
    }

    #ifdef TARGET_ZCODE;
    if (standard_interpreter ~= 0) {
        StorageForShortName-->0 = 16;
        @output_stream 3 StorageForShortName;
        print (address) w;
        @output_stream -3;
        first = StorageForShortName->2;
    }
    #ifnot;
    PrintAnyToArray(StorageForShortName, 16, w);
    first = StorageForShortName->0;
    #endif;
    if (first == 'a' or 'e' or 'i' or 'o' or 'u') print "r";
    print (address) w;
];

#ifdef COMPATIBILITY_MODE;
Constant endT = ___t;
Constant endET = ___et;
Constant definit = definite;
#endif;

#ifdef DEBUG;
[ DekliniereSub;
  print (withoutArt) noun,":^",
    (der) noun,", ", (des) noun,", ", (dem) noun,", ", (den) noun,"^",
    (ein) noun,", ", (eines) noun,", ", (einem) noun,", ", (einen) noun,"^";
];
#endif;

[ WordIsKnown__    wd own k;
  ! Untersucht, ob das Wort an der Stelle wn dem parser bekannt ist.
  ! Der Autor kann mit dem Einhänger WordIsKnown() eingreifen, um
  ! zum Beispiel explizit mit WordMatch() geparste Strings abzufangen.

  own = wn; k = WordIsKnown(); wn = own;
  if (k >= 0) return k;

  wd = NextWord();
  wn = own;
  if (wd) rtrue;
  rfalse;
];

#ifndef WordIsKnown;
[ WordIsKnown; return -1; ];
#endif;

#ifdef EXPLICIT_ERROR_MESSAGES;

[ ExplicitError silent;
    if (max_wn < 1) rfalse;
    if (max_wn > num_words) rfalse;
    if (silent) rtrue;

    wn = max_wn;
    if (WordIsKnown__()) {
        L__M(##Miscellany, 98);
    } else {
        L__M(##Miscellany, 99);
    }
    rtrue;
];

#ifnot; ! EXPLICIT

[ ExplicitError; rfalse; ];

#endif;

[ LanguageLM n x1;
  Answer, Ask:
            "Keine Antwort.";
! Ask:      see Answer
  Attack:   "Gewalt ist keine Lösung.";
  Blow:     "Es bringt wohl nichts, ", (den) x1, " zu blasen.";
  Burn:     "Das ist gefährlich und wenig sinnvoll.";
  Buy:      "Hier gibt es nichts zu kaufen.";
  Climb:    "Damit wirst du nichts erreichen.";
  Close: switch (n) {
        1:  print_ret "Du kannst ", (den) x1, " nicht schlie@30en.";
        2:  print_ret (GDer) x1, " ", (ist) x1, " bereits geschlossen.";
        3:  "Du schlie@30t ", (den) x1, ".";
    }
  CommandsOff: switch (n) {
        1: "[Aufzeichnung der Befehle aus.]";
        #Ifdef TARGET_GLULX;
        2: "[Aufzeichnung der Befehle ist bereits aus.]";
        #Endif; ! TARGET_
    }
  CommandsOn: switch (n) {
        1: "[Aufzeichnung der Befehle ein.]";
        #Ifdef TARGET_GLULX;
        2: "[Befehle werden nun aufgezeichnet.]";
        3: "[Aufzeichnung der Befehle bereits aktiv.]";
        4: "[Aufzeichnung der Befehle fehlgeschlagen.]";
        #Endif; ! TARGET_
    }
  CommandsRead: switch (n) {
        1: "[Befehle werden abgespielt.]";
        #Ifdef TARGET_GLULX;
        2: "[Befehle werden bereits abgespielt.]";
        3: "[Abspielen der Befehle fehlgeschlagen. Aufzeichnung ist aktiv.]";
        4: "[Abspielen der Befehle fehlgeschlagen.]";
        5: "[Abspielen der Befehle beendet.]";
        #Endif; ! TARGET_
    }
  Consult:  "In ", (dem) x1, " findest du nichts Interessantes darüber.";
  Cut:      "Es bringt nichts, ", (den) x1, " zu zerschneiden.";
  Dig:      "Dort kannst du nicht gut graben.";
  Disrobe: switch (n) {
        1:  "Du trägst ", (den) x1, " gar nicht.";
        2:  "Du ziehst ", (den) x1, " aus.";
    }
  Dress: switch (n) {
        1:  "Du lä@31t ", (den) x1, " besser wie ", (er) x1, " ", (ist) x1, ".";
        2:  "Das ist nicht nötig.";
    }
  Drink:    "Das kann man nicht trinken.";
  Drop: switch (n) {
        1:  print_ret (GDer) x1, " ", (ist) x1, " bereits hier.";
        2:  "Du hast ", (den) x1, " gar nicht.";
        3:  "(Dazu ziehst du ", (den) x1, " erst aus.)";
        4:  "In Ordnung.";
    }
  Eat: switch (n) {
        1:  print_ret (GDer) x1, " ", (ist) x1, " nicht e@31bar.";
        2:  "Du i@31t ", (den) x1, ". Nicht schlecht.";
    }
  EmptyT: switch (n) {
        1:  print_ret "In ", (dem) x1, " kann nichts abgelegt werden.";
        2:  print_ret (GDer) x1, " ", (ist) x1, " geschlossen.";
        3:  print_ret (GDer) x1, " ", (ist) x1, " bereits leer.";
        4:  "Das ist sinnlos.";
    }
  Enter: switch (n) {
        1:  print "Aber du bist doch schon ";
            if (x1 has supporter) print "auf "; else print "in ";
            print_ret (dem) x1, ".";
        2:  print "Du kannst ";
            switch (verb_word) {
              'stell', 'steh':  "auf ", (dem) x1, " nicht stehen";
              'sitz', 'setz':   "auf ", (dem) x1, " nicht sitzen.";
              'lieg', 'leg':    "auf ", (dem) x1, " nicht liegen.";
              default:          print_ret (den) x1, " nicht betreten.";
            }
        3:  "Du kannst nicht in ", (den) x1, " wenn ", (er) x1,
            " geschlossen ", (ist) x1, ".";
        4:  "Du kannst nur frei stehende Dinge betreten.";
        5:  switch (verb_word) {
              'stell', 'steh':  print "Du stellst dich ";
              'sitz', 'setz':   print "Du setzt dich ";
              'lieg', 'leg':    print "Du legst dich ";
              default:          print "Du steigst ";
            }
            if (x1 has supporter) print "auf "; else print "in ";
            print_ret (den) x1, ".";
        6:  print "(Dazu steigst du erst ";
            if (x1 has supporter) print "von "; else print "aus ";
            print (dem) x1; ".)";
        7:  if (x1 has supporter || x1 has container) {
                print "(Du steigst zuerst ";
                if (x1 has container) print "in "; else print "auf ";
                print_ret (den) x1, ".)";
            }
            "(Du betrittst zuerst ", (den) x1, ".)";
    }
  Examine: switch (n) {
        1:  "Es ist dunkel hier.";
        2:  "Du siehst nichts Besonderes an ", (dem) x1, ".";
        3:  print (GDer) x1, " ", (ist) x1, " im Moment ";
            if (x1 has on) "an."; else "aus.";
    }
  Exit: switch (n) {
        1:  "Aber du bist im Moment nirgendwo drin.";
        2:  "Du kannst ", (den) x1, " nicht verlassen, wenn ",
            (er) x1, " geschlossen ", (ist) x1, ".";
        3:  print "Du steigst ";
            if (x1 has supporter) print "von "; else print "aus ";
            print_ret (dem) x1, ".";
        4:  print "Aber du bist gar nicht ";
            if (x1 has supporter) print "auf "; else print "in ";
            print_ret (dem) x1, ".";
    }
  Fill:     "Du kannst nichts in ", (den) x1, " füllen.";
            ! "Hier gibt es nichts, was man auffüllen könnte.";
  FullScore: switch (n) {
        1:  if (deadflag) print "Deine Punkte setzten ";
            else print "Deine Punkte setzen ";
            "sich wie folgt zusammen:^";
        2:  "für das Finden diverser Gegenstände";
        3:  "für das Erreichen verschiedener Orte";
        4:  print "insgesamt (von ", MAX_SCORE; ")";
    }
  GetOff:   "Aber du bist gar nicht auf ", (dem) x1, ".";
  Give: switch (n) {
        1:  "Du hast ", (den) x1, " gar nicht.";
        2:  "Jetzt willst du dir es aber mal richtig geben, was?";
        3:  print (GDer) x1; plur(" scheinen", " scheint", x1);
            " nicht besonders interessiert zu sein.";
    }
  Go: switch (n) {
        1:  print "Das geht nicht, solange du noch ";
            if (x1 has supporter) print "auf "; else print "in ";
            print_ret (dem) x1, " bist.";
        2:  print_ret (string) CANTGO__TX;   ! "You can't go that way."
        3:  "Du kannst nicht auf ", (den) x1, " klettern.";
        4:  "Du kannst ", (den) x1, " nicht hinabsteigen.";
        5:  "Das geht nicht, ", (der) x1, " ", (ist) x1, " im Weg.";
        6:  print "Das geht nicht, ", (der) x1; plur(" führen", " führt", x1);
            " nirgendwohin";
    }
  Insert: switch (n) {
        1:  "Du mu@31t ", (den) x1, " in der Hand halten, um ", (ihn) x1,
            " in etwas anderes legen zu können.";
        2:  print_ret (GDer) x1, " ", (ist) x1, " kein Behälter.";
        3:  print_ret (GDer) x1, " ", (ist) x1, " geschlossen.";
        4:  "Dazu mu@31t du ", (den) x1, " erst ausziehen.";
        5:  "Du kannst nichts in sich selbst legen.";
        6:  "(Du ziehst ", (den) x1, " erst aus.)^";
        7:  "Es ist kein Platz mehr in ", (dem) x1, ".";
        8:  "In Ordnung.";
        9:  "Du legst ", (den) x1, " in ", (den) second, ".";
    }
  Inv: switch (n) {
        1:  "Du hast nichts bei dir.";
        2:  print "Du hast"; short_name_case = Akk;
        3:  print " Folgendes:^";
        4:  print " bei dir.";
    }
  Jump:     "Du springst etwas motivationslos auf der Stelle.";
  JumpOver, Tie:
            "Dadurch würdest du nichts erreichen.";
  Kiss:     "Konzentrier dich auf das Spiel.";
  Lift: switch(n) {
        1:  print_ret (GDer) x1, " ", (ist) x1, " fest.";
        2:  print_ret (GDen) x1, " kannst du nicht hochheben.";
        3:  "Das würde ", (dem) x1, " bestimmt nicht gefallen.";
    }
  Listen:   "Du hörst nichts Unerwartetes.";
  ListMiscellany: switch (n) {
        1:  print " (", (string) LIT__TX, ")";
        2:  print " (", (string) LanguageArticles-->(Gender(x1)),
                  "geschlossen ", (ist) x1, ")";
        3:  print " (geschlossen und ", (string) LIT__TX, ")";
        4:  print " (", (string) LanguageArticles-->(Gender(x1)),
                  "leer ", (ist) x1, ")";
        5:  print " (leer und ", (string) LIT__TX, ")";
        6:  print " (", (string) LanguageArticles-->(Gender(x1)),
                  "geschlossen und leer ", (ist) x1, ")";
        7:  print " (geschlossen, leer und ", (string) LIT__TX, ")";
        8:  print " (", (string) LIT__TX, " und angezogen";
        9:  print " (", (string) LIT__TX;
        10: print " (angezogen";
        11: print " (", (string) LanguageArticles-->(Gender(x1));
        12: print "offen ", (ist)x1;
        13: print "offen, aber leer ", (ist) x1;
        14: print "geschlossen ", (ist) x1;
        15: print "abgeschlossen ", (ist) x1;
        16: print " und leer ", (ist) x1;
        17: print " (", (string) LanguageArticles-->(Gender(x1)),
                  "leer ", (ist) x1, ")";
        18: print " (", (er) x1; plur(" enthalten ", " enthält ", x1);
        19: print " (darauf ";
        20: print ", darauf ";
        21: print " (darin ";
        22: print ", darin ";
    }
  LMode1:   " ist nun im knappen Modus, in dem Raumbeschreibungen nur beim
              ersten Betreten eines Raums angezeigt werden.";
  LMode2:   " ist nun im ausführlichen Modus, der immer die langen
              Raumbeschreibungen zeigt, auch wenn du schon einmal dort warst.";
  LMode3:   " ist nun im superknappen Modus, der immer nur die kurze
              Raumbeschreibung anzeigt, auch wenn du einen Raum zum ersten Mal
              betrittst.";
  Lock: switch (n) {
        1:  "Du kannst ", (den) x1, " nicht abschlie@30en.";
        2:  print_ret (GDer) x1, " ", (ist) x1, " schon abgeschlossen.";
        3:  "Du mu@31t ", (den) x1, " dazu erst zumachen.";
        4:  print (GDer) x1; plur(" passen", " pa@31t", x1); " nicht.";
        5:  "Du schlie@30t ", (den) x1, " ab.";
    }
  Look: switch (n) {
        1:  print " (auf ", (dem) x1, ")";
        2:  print " (in ", (dem) x1, ")";
        3:  print " (als ", (object) x1, ")";
        4:  print "^Auf ", (dem) x1;
            WriteListFromCase(child(x1),
              ENGLISH_BIT + RECURSE_BIT + PARTINV_BIT + TERSE_BIT + CONCEAL_BIT
                          + ISARE_BIT, Nom);
            print "."; if (WriteSublists()==0) "";
        5, 6:
            if (x1 ~= location) {
                if (x1 has supporter) print "^Auf "; else print "^In ";
                print (dem) x1, " siehst du ";
            }
            else print "^Du siehst hier ";
            if (n == 5) print "au@30erdem ";
            WriteListFromCase(child(x1),
              ENGLISH_BIT + RECURSE_BIT + PARTINV_BIT + TERSE_BIT + CONCEAL_BIT
              + WORKFLAG_BIT, Akk);
            print "."; if (WriteSublists()==0) "";
        -1:
            if (x1 has supporter) print "Auf "; else print "In ";
            print (dem) x1;
            WriteListFromCase(child(x1),
              ENGLISH_BIT + RECURSE_BIT + PARTINV_BIT + TERSE_BIT + CONCEAL_BIT
              + ISARE_BIT, Nom, 1);
              ! Tiefe eins, um die Zeilenumbrüche zu steuern
            print "."; if (WriteSublists()==0) "";
        7:  "Dort siehst du nichts Au@30ergewöhnliches.";
    }
  LookUnder: switch (n) {
        1:  "Aber es ist dunkel.";
        2:  "Du findest nichts Interessantes.";
    }
  Mild:     "So, so.";
  Miscellany: switch (n) {
        1:  "(Es werden nur die ersten sechzehn Objekte berücksichtigt.)^";
        2:  "Hier gibt es nichts zu tun!";
        3:  print " Du bist gestorben ";
        4:  print " Du hast gewonnen ";
        5:  print "^Möchtest du einen NEUSTART, einen vorher gespeicherten
                Spielstand LADEN";
            #Ifdef DEATH_MENTION_UNDO;
            print ", den letzten Zug mit UNDO rückgängig machen";
            #Endif;
            if (TASKS_PROVIDED == 0) print ", deine Punkte KOMPLETT auflisten";
            if (deadflag == 2 && AMUSING_PROVIDED == 0)
                print ", im NACHWORT über lustige Dinge im Spiel erfahren";
            " oder das Spiel beENDEn?";
        6:  "[Dein Interpreter kann leider keine Spielzüge rückgängig machen.]";
            #Ifdef TARGET_ZCODE;
        7:  "Der Spielzug konnte nicht rückgängig gemacht werden.
             [Nicht alle Interpreter verfügen über diese Funktionalität.]";
            #Ifnot; ! TARGET_GLULX
        7:  "[Du kannst keine weiteren Spielzüge rückgängig machen.]";
            #Endif; ! TARGET_
        8:  "Bitte antworte mit einer der oben genannten Möglichkeiten.";
        9:  "^Es ist nun stockfinster hier!";
        10: "Wie bitte?";
        11: "[Erst machen, dann rückgängig machen.]";
        12: "[Du kannst leider immer nur einen Zug in Folge rückgängig machen.]";
        13: "[Der letzte Zug wurde rückgängig gemacht.]";
        14: "Da gibt es leider nichts zu korrigieren.";
        15: "Denk dir nichts weiter.";
        16: "Mit ~hoppla~ kann immer nur ein Wort korrigiert werden.";
        17: "Es ist stockfinster, du siehst nichts.";
        18: print_yourself();
        19: "Gutaussehend wie immer.";
        20: "Wenn du einen Befehl wie ~Häschen, hüpf~ wiederholen willst, sag
            ~nochmal~, nicht ~Häschen, nochmal~.";
        21: "Das kann man nicht wiederholen.";
        22: "Du kannst den Satz nicht mit einem Komma beginnen.";
        23: "Du möchtest wahrscheinlich jemandem eine Anweisung erteilen,
            aber mir ist nicht klar wem.";
        24: "Mit ", (dem) x1, " kann man nicht reden.";
        25: "Um mit jemandem zu reden, benutze bitte ~Jemand, hallo~.";
        26: "(Dazu hebst du ", (den) not_holding, " erst auf.)";
        27: if (ExplicitError()) return;
            "Diesen Satz habe ich nicht verstanden.";
        28: #IfDef EXPLICIT_ERROR_MESSAGES; if (ExplicitError()) rtrue;
                "Diesen Satz habe ich nicht verstanden.";
            #IfNot;
            print "Ich habe nur Folgendes verstanden: ";
            #Endif;
        29: "Diese Zahl habe ich nicht verstanden.";
        30: if (ExplicitError()) return;
            "So etwas kannst du hier nicht sehen.";
        31: "Du scheinst nicht alles gesagt zu haben!";
        32: "Aber das hast du nicht bei dir!";
        33: "Hier kannst du nur ein Objekt angeben.";
        34: "Du kannst in jedem Satz nur einmal Listen von Objekten angeben.";
        35: "Mir ist nicht klar, worauf sich ~", (address) pronoun_word,
            "~ bezieht.";
        36: "Du hast etwas ausgeschlossen, das gar nicht zur Ausgangsmenge gehört!";
        37: "Das kannst du nur mit Lebewesen sinnvoll machen.";
        38: if (ExplicitError()) return;
            "Ich habe dieses Verb nicht verstanden.";
        39: "Damit brauchst du dich in diesem Spiel nicht zu beschäftigen.";
        40: "Du siehst ~", (address) pronoun_word, "~ (", (den) pronoun_obj,
            ") hier im Moment nicht.";
        41: "Das Satzende habe ich leider nicht verstanden.";
        42: if (x1 == 0) print "Nichts "; else print "Nur ", (number) x1;
            print " davon ";
            if (x1 <= 1) print "steht"; else print "stehen";
            " zur Verfügung.";
        43: "Es gibt hier nichts zu tun!";
        !44: if (action==##Drop) "Du hast aber nichts.";
        !    if (second) print "Dort"; else print "Hier"; " ist aber nichts!";
        44: if (action==##Drop) "Du hast aber nichts, das du hinlegen kannst.";
            if (second) print "Dort"; else print "Hier"; " ist aber nichts,
                das du einfach so nehmen kannst.";
        45: print "Wen meinst du, ";
        46: print "Was meinst du, ";
        47: "Hier kannst du nur ein Objekt angeben. Welches?";
        48: PrintWemCommand();
        ! *** der Kontext wird nicht ganz korrekt erkannt, deshalb die neue Variante:
        49: if (action_to_be==##Ask or ##Tell or ##Answer) PrintWemCommand(); else PrintWomitCommand();
        50: print "Du hast gerade ";
            if (x1 > 0) {
                print x1, " Punkt";
                if (x1 ~= 1) print "e";
                print " bekommen";
            } else {
                print -x1, " Punkt";
                if (x1 ~= -1) print "e";
                print " verloren";
            }
        51: "(Da etwas Dramatisches passiert ist, wurde die Liste deiner
            Anweisungen nicht komplett ausgeführt.)";
        52: "^Bitte eine Nummer von 1 bis ", x1, " angeben, mit 0 die Anzeige
            auffrischen oder die Eingabetaste drücken.";
        53: "^[Bitte die Leertaste drücken.]";
        54: "[Kommentar notiert.]";
        55: "[Kommentar NICHT notiert.]";
        56: print ".^";
        57: print "?^";
        98: "Ich verstehe das Wort ~", (PrintOriginal) wn,
            "~ in diesem Zusammenhang nicht.";
        99: "Ich kenne das Wort ~", (PrintOriginal) wn, "~ nicht.";
    }
  No, Yes:  "Das war eine rhetorische Frage.";
  NotifyOff:
            "Meldungen bei Änderung des Punktestands aus.";
  NotifyOn: "Meldungen bei Änderung des Punktestands ein.";
  Objects: switch (n) {
        1:  "Objekte, die du benutzt hast:^";
        2:  "keins.";
        3:  print "   (angezogen)";
        4:  print "   (in der Hand)";
        5:  print "   (weggegeben)";
        6:  print "   (", (name) x1, ")";
        7:  print "   (in ", (dem) x1, ")";
        8:  print "   (in ", (dem) x1, ")";
        9:  print "   (auf ", (dem) x1, ")";
        10: print "   (verloren)";
    }
  Open: switch (n) {
        1:  "Du kannst ", (den) x1, " nicht öffnen.";
        2:  print_ret (GDer) x1, " ", (ist) x1, " abgeschlossen.";
        3:  print_ret (GDer) x1, " ", (ist) x1, " bereits offen.";
        4:  print "Du öffnest ", (den) x1, " und findest ";
            if (WriteListFromCase(child(x1), ENGLISH_BIT
                + TERSE_BIT + CONCEAL_BIT, Akk) ~= 0) " darin.";
            "nichts.";
        5:  "Du öffnest ", (den) x1, ".";
    }
  Order:    print_ret (GDer) x1, " ", (hat) x1, " Besseres zu tun.";
  Places: switch (n) {
        1:  print "Du hast folgende Orte besucht: ";
        2:  print ".^";
    }
  Pray:     "Dein Gebet zeigt keine Wirkung.";
  Prompt:   print "^>";
  Pronouns: switch (n) {
        1:  print "Die Pronomen beziehen sich im Moment auf Folgendes:^";
        2:  print ": ";
        3:  print ": nicht gesetzt";
        4:  "Dieses Spiel kennt keine Pronomen. (Schade!)";
    }
  Pull,Push,Turn: switch (n) {
        1:  print_ret (GDer) x1, " ", (ist) x1, " fest.";
        2:  "Du bist nicht dazu in der Lage.";
        3:  "Nichts passiert.";
        4:  "Das wäre sehr unhöflich.";
    }
! Push: see Pull
  PushDir: switch (n) {
        1:  "Fällt dir nichts Besseres ein?";
        2:  "Das ist keine Richtung.";
        3:  "Nein, in diese Richtung geht das nicht.";
    }
  PutOn: switch (n) {
        1:  "Du mu@31t ", (den) x1, " in der Hand halten, um ", (ihn) x1,
            " auf etwas anderem ablegen zu können.";
        2:  "Du kannst nichts auf sich selbst ablegen.";
        3:  "Dinge auf ", (den) x1, " zu tun bringt vermutlich nichts.";
        4:  "Dir fehlt die nötige Geschicklichkeit.";
        5:  "(Du ziehst ", (den) x1, " erst aus)^";
        6:  "Es ist kein Platz mehr auf ", (dem) x1, ".";
        7:  "In Ordnung.";
        8:  "Du legst ", (den) x1, " auf ", (den) second, ".";
    }
  Quit: switch (n) {
        1:  print "Bitte antworte mit Ja oder Nein. ";
        2:  print "Möchtest du das Spiel wirklich beenden? ";
    }
  Remove: switch (n) {
        1:  print_ret (GDer) second, " ", (ist) second, " leider geschlossen.";
        2:  print "Aber ", (der) x1, " ", (ist) x1, " gar nicht ";
            if (second has supporter) print "auf "; else print "in ";
            print_ret (dem) second, ".";
        3:  "In Ordnung.";
    }
  Restart: switch (n) {
        1:  print "Möchtest du wirklich neu starten? ";
        2:  "Fehlgeschlagen.";
    }
  Restore: switch (n) {
        1:  "Laden des Spielstands fehlgeschlagen.";
        2:  "In Ordnung.";
    }
  Rub:      "Du erreichst nichts.";
  Save: switch (n) {
        1:  "Der Spielstand konnte nicht abgespeichert werden.";
        2:  "In Ordnung.";
    }
  Score: switch (n) {
        1:  if (deadflag) print "In diesem Spiel "; else print "Bislang ";
            print "hast du ", score, " von ", MAX_SCORE, " möglichen Punkten in ",
            turns; if (turns ~= 1) print " Zügen"; else print " Zug";
            print " erreicht";
        2:  "In diesem Spiel gibt es keine Punkte.";
    }
  ScriptOff: switch (n) {
        1:  "Es wird gar kein Protokoll mitgeschrieben.";
        2:  "^Ende des Protokolls.";
        3:  "Der Versuch, das Protokoll zu schlie@30en, scheiterte.";
    }
  ScriptOn: switch (n) {
        1:  "Es wird bereits ein Protokoll mitgeschrieben.";
        2:  "Es wird nun ein Protokoll angelegt von^";
        3:  "Der Versuch, ein Protokoll anzulegen, scheiterte.";
    }
  Search: switch (n) {
        1:  "Aber es ist dunkel.";
        2:  "Auf ", (dem) x1, " ist nichts.";
        3:  print "Auf ", (dem) x1, " siehst du ";
            WriteListFromCase(child(x1),
                ENGLISH_BIT + TERSE_BIT + CONCEAL_BIT, Akk);
            print "."; WriteSublists(); "";
        4:  "Du findest nichts Interessantes.";
        5:  "Du kannst nicht hineinschauen, ", (der) x1, " ", (ist) x1,
            " geschlossen.";
        6:  print_ret (GDer) x1, " ", (ist) x1, " leer.";
        7:  print "In ", (dem) x1, " siehst du ";
            WriteListFromCase(child(x1), ENGLISH_BIT + TERSE_BIT + CONCEAL_BIT, Akk);
            print "."; WriteSublists(); "";
    }
  Set:      "Du kannst ", (den) x1, " nicht einstellen.";
  SetTo:    "Du kannst ", (den) x1, " nicht auf irgendetwas einstellen.";
  Show: switch (n) {
        1:  "Aber du hast ", (den) x1, " gar nicht.";
        2:  print_ret (GDer) x1, " ", (ist) x1, " nicht beeindruckt.";
    }
  Sing:     "Du singst. Nicht sehr schön.";
  Sleep:    "Du fühlst dich nicht müde.";
  Smell:    "Du riechst nichts Ungewohntes.";
  Sorry:    "Schwamm drüber.";
  Squeeze: switch (n) {
        1:  "La@31 deine Hände bei dir.";
        2:  "Das bringt nichts.";
    }
  Strong:   "Das stand so aber nicht in der Musterlösung.";
  Swim:     "Hier kannst du nirgendwo schwimmen.";
  Swing:    "Dort gibt es nichts zu schaukeln.";
  SwitchOff: switch (n) {
        1:  "Du kannst ", (den) x1, " nicht ausschalten.";
        2:  print_ret (GDer) x1, " ", (ist) x1, " bereits aus.";
        3:  "Du schaltest ", (den) x1, " aus.";
    }
  SwitchOn: switch (n) {
        1:  "Du kannst ", (den) x1, " nicht einschalten.";
        2:  print_ret (GDer) x1, " ", (ist) x1, " bereits an.";
        3:  "Du schaltest ", (den) x1, " an.";
    }
  Take: switch (n) {
        1:  "In Ordnung.";
        2:  "Immer diese Selbstversessenheit.";
        3:  "Das würde ", (dem) x1, " bestimmt nicht gefallen.";
        4:  print "Dazu mü@31test du zunächst ";
            if (x1 has supporter) print "von "; else print "aus ";
            print (dem) x1; if (x1 has supporter) " herunter."; " heraus.";
        5:  "Du hast ", (den) x1, " bereits.";
        6:  print_ret (GDer) noun, " gehör", (___t) noun, " offenbar zu ",
                (dem) x1, ".";
        7:  print_ret (GDer) noun, " ", (ist) noun, " offenbar ein Teil ",
                (des) x1, ".";
        8:  print_ret (GDen) x1, " gibt es hier nicht.";
        9:  print_ret (GDer) x1, " ", (ist) x1, " nicht offen.";
        10: print_ret (GDen) x1, " kannst du nicht mitnehmen.";
        11: print_ret (GDer) x1, " ", (ist) x1, " fest.";
        12: "Du trägst bereits zu viele Dinge.";
        13: "(Du verstaust ", (den) x1, " in ", (dem) SACK_OBJECT,
            " um Platz zu schaffen)";
    }
  Taste:    "Du schmeckst nichts Unerwartetes.";
  Tell: switch (n) {
        1:  "Du unterhältst dich ein wenig und erzählst dir alte Geschichten.";
        2:  "Keine Antwort.";
    }
  Think:    "Brillante Idee, das!";
  ThrowAt: switch (n) {
        1:  "Witzlos.";
        2:  "Im kritischen Augenblick fehlen dir die Nerven dazu.";
    }
! Tie:  see JumpOver.
  Touch: switch (n) {
        1:  "La@31 deine Hände bei dir!";
        2:  "Du fühlst nichts Unerwartetes.";
        3:  "Wenn du meinst.";
    }
! Turn: see Pull.
  Undress: switch (n) {
        1:  "Du lä@31t ", (den) x1, " besser wie ", (er) x1, " ", (ist) x1, ".";
        2:  "Besser nicht.";
    }
  Unlock:  switch (n) {
        1:  "Du kannst ", (den) x1, " nicht aufschlie@30en.";
        2:  print_ret (GDer) x1, " ", (ist) x1, " bereits aufgeschlossen.";
        3:  print (GDer) x1; plur(" passen", " pa@31t", x1); " nicht.";
        4:  "Du schlie@30t ", (den) x1, " auf.";
    }
  VagueGo:  "Sag bitte, in welche Himmelsrichtung du gehen willst.";
  Verify: switch (n) {
        1:  "Die Spieldatei ist intakt.";
        2:  "Die Spieldatei ist korrupt.";
    }
  Wait:     "Die Zeit verstreicht.";
  Wake:     "Die bittere Wahrheit ist: Dies ist kein Traum.";
  WakeOther:"Das ist unnötig.";
  Wave: switch (n) {
        1:  "Du hast ", (den) x1, " gar nicht.";
        2:  "Es sieht blöd aus, wie du mit ", (dem) x1, " wedelst.";
    }
  WaveHands: "Du winkst und fühlst dich dabei etwas komisch.";
  Wear: switch (n) {
        1:  "Du kannst ", (den) x1, " nicht anziehen!";
        2:  "Du hast ", (den) x1, " gar nicht!";
        3:  "Du trägst ", (den) x1, " bereits!";
        4:  "Du ziehst ", (den) x1, " an.";
    }
! Yes:  see No.
];

! ==============================================================================

Constant LIBRARY_GERMAN;                               ! for dependency checking

Constant DEFORM___2010_03_16___GERMAN;
Constant DEFORM___GERMAN;

! ==============================================================================
