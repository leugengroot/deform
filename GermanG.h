! ==============================================================================
!   GERMAN GRAMMAR:  Grammar table entries for the standard verbs library.
!
!   Supplied for use with deform 6 -- Release 6/11 -- Serial number 050825
!
!   Inform 6 und Original-Library 6/11:
!   Copyright Graham Nelson 1993-2004 but freely usable (see manuals)
!
!   deform 6 -- deutsche Übersetzung der Library, (C) 2005 Martin Oehm
!   Weitergabe unter denselben Bedingungen wie Inform 6 und die Original-Lib.
!
!   Im Quelltext des Spiels müssen die Dateien wie folgt eingebunden werden:
!
!       !... Story und Headline
!       !... Ersetzen von Routinen wenn gewünscht
!
!       Include "Parser";
!       Include "VerbLib";
!
!       !... Hauptteil der Definitionen
!
!       Include "GermanG";
!
!       !... Eigene Grammatikdefinitionen
!
!   Die Verben müssen im Imperativ der zweiten Person Singular angegeben werden,
!   Die Konvention ist hier, dass der Spieler das Spiel duzt. Die regelmäßigen
!   Verben lassen hier mist zwei Forme zu, die sich nur durch ein angehängtes
!   'e' unterscheiden, etwa 'schau' und 'schaue'.
!
!   In der offiziellen Lib wird das 'e' automatisch vom Parser abgeschnitten,
!   daher dürfen dort Verb-Definitionen kein 'e' am Ende haben. Hier dürfen
!   Verben ein 'e' am Ende haben, was besonders bei "Verben", die keine Verben,
!   sondern Substantive oder englische Debug-Kommandos sind sinnvoll ist. Wenn
!   ein Verb aber zwei Formen hat und beide erkannt werden sollen, sollte man
!   *nur* die Form *ohne* e definieren, also 'schau', 'geh', 'spring' usw.
!
!   Die Verben dürfen wie alle Vokabeln keine Umlaute enthalten, es müssen die
!   Umschreibungen 'ae', 'oe', 'ue' und 'ss' verwendet werden.
!
! ==============================================================================

System_file;

! ----------------------------------------------------------------------------
!  The "meta-verbs", commands to the game rather than in the game, come first:
! ----------------------------------------------------------------------------

Verb meta 'punkte' 'punktestand' 'score'
    *                                                       -> Score
    * 'on'/'an'/'ein'/'einschalten'                         -> NotifyOn
    * 'off'/'aus'/'ausschalten'                             -> NotifyOff
    * 'voll'                                                -> FullScore
    ;
Verb meta 'voll' 'fullscore' 'komplett' 'punkteliste'
    *                                                       -> FullScore
    * 'punkte'                                              -> FullScore
    ;
Verb meta 'notify' 'meldungen'
    * 'on'/'an'/'ein'/'einschalten'                         -> NotifyOn
    * 'off'/'aus'/'ausschalten'                             -> NotifyOff
    ;
Verb meta 'q//' 'quit' 'stirb' 'ende' 'beend' * -> Quit;
Verb meta 'lad' 'laden' 'load' 'restor' *                   -> Restore;
Verb meta 'neustart' 'neu' 'restart' *                      -> Restart;
Verb meta 'filecheck' 'verify' *                            -> Verify;
Verb meta 'speicher' 'save' 'speichern' *                   -> Save;
Verb meta 'skript' 'script' 'transkript'
    'transcript' 'protokoll' 'mitschrift'
    *                                                       -> ScriptOn
    * 'off'/'aus'                                           -> ScriptOff
    * 'on'/'an'/'ein'                                       -> ScriptOn
    ;
Verb meta 'recording'
    *                                                       -> CommandsOn
    * 'on'                                                  -> CommandsOn
    * 'off'                                                 -> CommandsOff
    ;
Verb meta 'replay' *                                        -> CommandsRead;
Verb meta 'befehl' 'aufnahme'
    *                                                       -> CommandsOn
    * 'ein'/'an'                                            -> CommandsOn
    * 'aus'                                                 -> CommandsOff
    * 'abspielen'                                           -> CommandsRead
    ;


Verb meta 'knapp' 'normal' *                                -> LMode1;
Verb meta 'ausfuehrlich' *                                  -> LMode2;
Verb meta 'superknapp' *                                    -> LMode3;
Verb meta 'pronomen' 'pronomina' 'fuerwoert' *              -> Pronouns;
Verb meta 'version' *                                       -> Version;

#IFNDEF NO_PLACES;
Verb meta 'orte' 'places' *                                 -> Places;
Verb meta 'objekte' 'objects' *                             -> Objects;
#ENDIF;

! ----------------------------------------------------------------------------
!  Debugging grammar
! ----------------------------------------------------------------------------

#ifdef DEBUG;
Verb meta 'trace'
    *                                                       -> TraceOn
    * number                                                -> TraceLevel
    * 'on'                                                  -> TraceOn
    * 'off'                                                 -> TraceOff
    ;
Verb meta 'actions'
    *                                                       -> ActionsOn
    * 'on'                                                  -> ActionsOn
    * 'off'                                                 -> ActionsOff
    ;
Verb meta 'routines' 'messages'
    *                                                       -> RoutinesOn
    * 'on'                                                  -> RoutinesOn
    * 'off'                                                 -> RoutinesOff
    ;
Verb meta 'timers' 'daemons'
    *                                                       -> TimersOn
    * 'on'                                                  -> TimersOn
    * 'off'                                                 -> TimersOff
    ;
Verb meta 'changes'
    *                                                       -> ChangesOn
    * 'on'                                                  -> ChangesOn
    * 'off'                                                 -> ChangesOff
    ;
Verb meta 'random' *                                        -> Predictable;
Verb meta 'purloin' * multi                                 -> XPurloin;
Verb meta 'abstract' * noun 'to' noun                       -> XAbstract;
Verb meta 'tree'
    *                                                       -> XTree
    * noun                                                  -> XTree
    ;
Verb meta 'goto' * number                                   -> Goto;
Verb meta 'gonear' * noun                                   -> Gonear;
Verb meta 'scope'
    *                                                       -> Scope
    * noun                                                  -> Scope
    ;
Verb meta 'showverb' * special                              -> Showverb;
Verb meta 'showobj'
    *                                                       -> Showobj
    * multi                                                 -> Showobj
    ;

!   Nur für die deutsche Lib: Überprüfung der Objektdefinitionen

Verb meta 'libcheck' *                                      -> XLibCheck;

!   Nur für die deutsche Lib: Hier kann man schnell die Deklination
!   eines Objekts überprüfen.

Verb meta 'deklinier' 'dekl'
    * noun                                                  -> Dekliniere
    ;

!   Nur für die deutsche Lib: Zeigt den Satz nach Informisierung an.
!   Da trace 1 bereits viel Zeug ausgibt und die Beschneidung der
!   Wörter wichtig ist, wurden die beiden Befehle getrennt.

Verb meta 'echo'
    *                                                       -> EchoOn
    * 'on'                                                  -> EchoOn
    * 'off'                                                 -> EchoOff
    ;
#ifdef TARGET_GLULX;
Verb meta 'glklist' *                                       -> Glklist;
#endif; ! TARGET_;
#endif;

! ----------------------------------------------------------------------------
! Spezielle Token:
!
! [prep_hinein]          Die prep-Tokens überlesen die Adverbialpronomen als
! [prep_heraus]          Präposition ohne Ergebnis. Ist ähnlich der Notation
! [prep_weg]             'weg'/'fort' und nützlich, wenn diese Wörter gesagt
!                        werden können, aber keine Information tragen wie bei
!                        "Schau in Kiste hinein/rein".
!
! [noun_hinein]          Die noun-Tokens überlesen ebefnfalls eine Reihe von
! [noun_heraus]          Wörtern jedoch als Adverbialpronomen. Das heißt es
! [noun_damit]           wird als Ergebnis das letzte Objekt, das nicht
! [noun_darauf]          'animate' ist zurückgegeben, oder ein Fehler, wenn
! ...                    es kein solches Objekt gibt. Hier haben die Wörter
!                        sehr wohl eine Information: "schau hinein" schaut
!                        in das zuletzt angegebene Objekt.
!
!                        (Diese Tokens heißen in der offiziellen Lib [hinein]
!                        [xdamit], [xhinweg] usw. Die Benutzung von x für ein
!                        Ergebnistoken war aber nicht eindeutig, so habe ich
!                        sie umbenannt.)
!
! [held_damit]           Die held-Tokens sind wie die noun-Tokens oben, nur
! ...                    dass sie zusätzlich verlangen, dass das Objekt beim
!                        Spieler ist und auch ein implizites ##Take versuchen.
!                        Dunmmerweise wird die Gültigkeit in Inform nur beim
!                        Token geprüft.
!
! [force_nach]           Prüft den gesamten Satz und lässt das Satzmuster
!                        nur zu, wenn es das Wort 'nach' enthält. Dies wird
!                        in Sätzen mit <topic>-Token verwendet, um fehler-
!                        hafte Eingaben wie >>schau xxx<< nicht fälschlich
!                        als Consult zu interpretieren.
!
! [force_in]             Dito, erzwingt 'in' im Satz.
!
! [force_nach_in]        Erzwingt 'nach', lässt den Satz aber nicht zu, wenn
!                        vor 'nach' ein 'in' gefunden wird. Das wird auch bei
!                        <topic> benötigt, um >>schau italien im atlas nach<<
!                        nicht "* topic 'nach' 'in' noun" zu überlassen - >>im
!                        atlas<< würde hier als Teil des <topic> überlesen,
!                        das Nachschalgewerk impliziert.
!
! [force_pronoun]        Kann in Kombination mit Präpositionen verwendet
!                        werden, zum Beispiel ist 'damit' [force_pronoun]
!                        dasselbe wie [noun_damit]. Die erste Variante kann
!                        aber nach einem [topic] verwendet werden, da einem
!                        Topic-Token nur Präpositionen oder das Satzende
!                        folgen können. Deshalb wird es hier auch nur als
!                        Krücke für ##Consult verwendet.
!
! [dative]               Kennzeichnet ein nachfolgendes Token als Dativ.
!                        (Wird von der Lib nur zur Ausgabe von PrintCommand
!                        benutzt. Die Routine parse_name kann aber überprüfen,
!                        ob der dative_mode gesetzt ist.)
! ----------------------------------------------------------------------------

[ parse_pronomial_adverb a mode    w;
    ! Diese Routine überliest entweder Adverbialpronomen (mode==0) oder
    ! bildet sie auf das "Pronomen" 'spez.' ab, das passende Pronomen
    ! für Objekte, die nicht 'animate' sind. Aus der offiziellen Lib
    ! übernommen. Originalcode Toni Arnold?

    ! In deform erweitert, um [hald] und [noun] zu unterscheiden.
    ! Der mode ist 0 für überspringen, 1 für [noun] und 2 für [held].

    w = NextWordStopped();
    if (w == -1 or THEN1__WD && mode == 0) {
        wn--; return GPR_PREPOSITION;
    }
    if (w == 'da' or 'dort' or 'hier') w = NextWordStopped();

    switch (a) {
        1: if (w == 'hinein' or 'rein' or 'darein') a = -1;
        2: if (w == 'heraus' or 'hinaus' or 'raus' or 'daraus') a = -1;
        3: if (w == 'hinweg' or 'weg' or 'fort') a = -1;
        4: if (w == 'damit') a = -1;
        5: if (w == 'darauf' or 'drauf' or 'herauf' or 'rauf' or 'hinauf') a=-1;
        6: if (w == 'darunter' or 'drunter') a=-1;
        7: if (w == 'dahinter') a=-1;
        8: if (w == 'nach') a=-1;
        9: if (w == 'durch' or 'hindurch') a=-1;
        10: if (w == 'darueber' or 'hinueber' or 'drueber' or 'herueber') a=-1;
        11: if (w == 'herunter' or 'hinunter' or 'runter') a=-1;
    }
    if (a == -1) {
        if (mode == 0) return GPR_PREPOSITION;
        w = PronounValue('spez.');
        if (w == 0) return GPR_FAIL;
        if (TestScope(w, actor)) {
            if (mode == 1) return w;
            if (mode == 2 && parent(w) ~= actor) {
                if (notheld_mode == 1) {
                    etype = NOTHELD_PE;
                    return GPR_FAIL;
                }
                not_holding = w;
            }
            return w;
        }
    }
    return GPR_FAIL;
];

[ prep_hinein;   return parse_pronomial_adverb(1, 0); ];
[ prep_heraus;   return parse_pronomial_adverb(2, 0); ];
[ prep_weg;      return parse_pronomial_adverb(3, 0); ];
[ prep_darauf;   return parse_pronomial_adverb(5, 0); ];
[ prep_nach;     return parse_pronomial_adverb(8, 0); ];
[ prep_hindurch; return parse_pronomial_adverb(9, 0); ];
[ prep_hinueber; return parse_pronomial_adverb(10, 0); ];
[ prep_herunter; return parse_pronomial_adverb(11, 0); ];

[ noun_hinein;   return parse_pronomial_adverb(1, 1); ];
[ noun_heraus;   return parse_pronomial_adverb(2, 1); ];
[ noun_darauf;   return parse_pronomial_adverb(5, 1); ];
[ noun_darunter; return parse_pronomial_adverb(6, 1); ];
[ noun_dahinter; return parse_pronomial_adverb(7, 1); ];
[ noun_hindurch; return parse_pronomial_adverb(9, 1); ];
[ noun_darueber; return parse_pronomial_adverb(10, 1); ];
[ noun_herunter; return parse_pronomial_adverb(11, 1); ];

[ held_hinein;   return parse_pronomial_adverb(1, 2); ];
[ held_darauf;   return parse_pronomial_adverb(5, 2); ];

#ifdef COMPATIBILITY_MODE;
[ hinein;  return prep_hinein(); ];
[ heraus;  return prep_heraus(); ];
[ xhinein; return noun_hinein(); ];
[ xheraus; return noun_heraus(); ];
[ hinweg;  return prep_weg(); ];
[ xdamit;  return noun_damit(); ];
[ xdarauf; return noun_darauf(); ];
#endif;

[ force_prep required stop   w n;
    w = wn;
    if (stop == 0) stop = -1;
    wn = verb_wordnum + 1;
    n = NextWordStopped();
    while (n ~= -1) {
        if (n == stop) return GPR_FAIL;
        if (n == required) {
            wn = w;
            return GPR_PREPOSITION;
        }
        n = NextWordStopped();
    }
    return GPR_FAIL;
];

[ force_nach; return force_prep('nach'); ];
[ force_nach_in; return force_prep('nach', 'in'); ];
[ force_in; return force_prep('in'); ];

[ force_pronoun wd;
    wd = PronounValue('spez.');
    if (wd == 0) return GPR_FAIL;
    if (TestScope(wd, actor)) return wd;
    return GPR_FAIL;
];

[ ADirection; if (noun in compass) rtrue; rfalse; ];

! Token, das das nachfolgende Token als Dativ kennzeichnet. So kann der
! Fall berücksichtigt werden (wird er aber noch nicht) und trotzdem bleibt
! der Scope des Tokens erhalten: dative noun, dative held, dative edible usw.

[ dative; dative_mode = true; return GPR_PREPOSITION; ];

! Krücke für 'u' -> 'h', ohne 'u busch' zu beeinträchtigen

#ifdef u_obj;
[ implicit_up; return u_obj; ];
#endif;

[ dich;
    if (NextWord() == 'dich' or 'sich' or 'euch' or 'mich')
        return GPR_PREPOSITION;
    wn--; return GPR_FAIL;
];

[ dir;
    if (NextWord() ~= 'dir' or 'sich' or 'mir') wn--;
    return GPR_PREPOSITION;
];

! ----------------------------------------------------------------------------
!   Das Nachfolgende ist nur dazu da, um bei den Tokens [multiinside] und
!   [multiexcept] nicht auf das von der Lib vorgegebene Schema
!
!      * multiinside 'preps' noun
!
!   festgelegt zu sein. Skip gibt an, welche Tokens übersprungen werden,
!   Valid gibt an, welche Tokens als nächstes, nicht übersprungenes Token
!   gültig sind und Parse gibt die Vorschrift zum Parsen.
! ----------------------------------------------------------------------------

[ Lookahead_Skip type data;
    if (type == PREPOSITION_TT) rtrue;
    if (Type == GPR_TT && data == dative) rtrue;
    rfalse;
];

[ Lookahead_Valid type data;
    if (type == ELEMENTARY_TT && data == NOUN_TOKEN) rtrue;
    if (Type == GPR_TT && data == force_pronoun or noun_darauf
        or noun_hinein or noun_darunter or noun_heraus) rtrue;
    rfalse;
];

[ Lookahead_Parse type data;
    if (type == ELEMENTARY_TT)
        return NounDomain(actors_location, actor, NOUN_TOKEN);
    if (type == GPR_TT) { return data(); }
    return GPR_FAIL;
];

! ----------------------------------------------------------------------------
!  And now the game verbs.
! ----------------------------------------------------------------------------

Verb 'nimm' 'nehm' 'hol'
    * dir multi                                             -> Take
    * multi 'auf'/'mit'                                     -> Take
    * worn 'ab'                                             -> Disrobe
    * noun 'ab'                                             -> Take
    * multiinside 'aus' dative noun prep_heraus             -> Remove
    * multiinside 'in' dative noun prep_heraus              -> Remove
    * multiinside 'von' dative noun prep_herunter           -> Remove
    * multiinside noun_heraus                               -> Remove
    * multiinside noun_darunter                             -> Remove
    * 'platz' 'auf' dative noun                             -> Enter
    * 'platz' 'in' dative noun                              -> Enter
    * 'auf' dative noun 'platz'                             -> Enter
    * 'in' dative noun 'platz'                              -> Enter
    ;

Verb 'greif' 'fass' 'ergreif'
    * dir multi                                             -> Take
    * multiinside 'in' dative noun                          -> Remove
    * multiinside 'auf' dative noun                         -> Remove
    * multiinside noun_heraus                               -> Remove
    * 'an' noun                                             -> Touch
    * creature 'an'                                         -> Attack
    * creature 'mit' dative held 'an'                       -> Attack
    * 'mit' dative held creature 'an'                       -> Attack reverse
    * creature 'an' 'mit' dative held                       -> Attack
    * noun 'an'                                             -> Touch
    * 'nach' dative multi                                   -> Take
    * 'nach' dative multiinside 'in' noun                   -> Remove
    * 'nach' dative multiinside 'auf' noun                  -> Remove
    * multiinside 'von'/'aus' noun prep_heraus              -> Remove
    ;

Verb 'pack'
    * multi                                                 -> Take
    * multi 'ein'                                           -> Take
    * creature                                              -> Attack
    * multiexcept 'auf' noun                                -> PutOn
    * multiexcept noun_darauf                               -> PutOn
    * multiexcept 'in' noun prep_hinein                     -> Insert
    * multiexcept noun_hinein                               -> Insert
    ;

Verb 'heb'
    * multi 'auf'                                           -> Take
    * multiinside noun_heraus                               -> Remove
    * multiinside noun_darunter                             -> Remove
    * noun 'an'/'hoch'                                      -> Lift
    ;

Verb 'trag'
    * held                                                  -> wear
    ;

Verb 'steh'
    *                                                       -> Exit
    * 'auf'                                                 -> Exit
    * 'auf' 'von' dative noun                               -> GetOff
    * 'auf' 'aus' dative noun                               -> GetOff
    * 'auf' dative noun                                     -> Enter
    ;

Verb 'entfern'
    * worn                                                  -> Disrobe
    * multi                                                 -> Take
    * multiinside 'von' dative noun                         -> Remove
    * multiinside 'aus' dative noun                         -> Remove
    ;

Verb 'tu' 'platzier' 'plazier'
    * dich 'auf'/'in' noun                                  -> Enter
    * multiheld 'weg'                                       -> Drop
    * multiexcept 'in' noun prep_hinein                     -> Insert
    * multiexcept noun_hinein                               -> Insert
    * multiexcept 'auf' noun                                -> PutOn
    ;

Verb 'steck'
    * multiexcept 'in' noun prep_hinein                     -> Insert
    * dir multi 'ein'                                       -> Take
    * multiexcept noun_hinein                               -> Insert
    * multiexcept 'auf' noun                                -> PutOn
    * clothing 'an'                                         -> Wear
    * noun 'an'                                             -> Burn
    * noun 'mit' dative held 'an'                           -> Burn
    * 'mit' dative held noun 'an'                           -> Burn
    ;

Verb 'leer' 'schuett' 'giess' 'entleer'
    * noun                                                  -> Empty
    * noun 'aus'                                            -> Empty
    * noun 'zu'/'in'/'auf' noun                             -> EmptyT
    * noun noun_hinein                                      -> EmptyT
    ;

Verb 'lass'
    * dich 'auf' dative noun 'nieder'                       -> Enter
    * dich 'in' dative noun 'nieder'                        -> Enter
    * multiheld                                             -> Drop
    * multiheld 'fallen'/'liegen'/'hier'                    -> Drop
    * multiheld 'ab'/'aus'                                  -> Drop
    * multiexcept 'in' dative noun                          -> Insert
    * multiexcept 'unter' dative noun                       -> Insert
    * multiexcept 'auf' dative noun                         -> PutOn
    * multiexcept 'ueber' dative noun                       -> PutOn
    ;

Verb 'wirf' 'werf' 'schmeiss' 'schleuder'
    * dich 'auf' creature                                   -> Attack
    * dich 'gegen' noun                                     -> Attack
    * dich 'auf'/'in' noun                                  -> Enter
    * multiheld prep_weg                                    -> Drop
    * multiexcept 'in'/'unter' noun                         -> Insert
    * multiexcept 'in' noun prep_hinein                     -> Insert
    * multiexcept noun_hinein                               -> Insert
    * multiexcept 'auf'/'ueber' noun                        -> PutOn
    * held 'nach'/'gegen'/'auf'/'zu' noun                   -> ThrowAt
    * dative creature held 'zu'/'nach'                      -> ThrowAt reverse
    * creature 'nieder'                                     -> Attack
    ;

Verb 'gib' 'geb' 'offerier' 'reich' 'uebertrag'
    'ueberreich' 'uebergib'
    * held dative creature                                  -> Give
    * held 'an' creature                                    -> Give
    * dative creature held                                  -> Give reverse
    ;

Verb 'fuetter' 'bezahl' 'zahl'
    * creature 'mit' dative held                            -> Give reverse
    * 'mit' dative held creature                            -> Give
    * held 'an' creature                                    -> Give
    * 'an' creature held                                    -> Give reverse
    * noun 'mit' dative multiexcept                         -> Insert reverse
    ;

Verb 'biet'
    * dative creature held 'an'                             -> Give reverse
    * held dative creature 'an'                             -> Give
    * 'fuer' noun                                           -> Buy
    ;

Verb 'zeig' 'praesentier' 'fuehr'
    * dative creature held                                  -> Show reverse
    * held dative creature                                  -> Show
    * dative creature held 'vor'                            -> Show reverse
    * held dative creature 'vor'                            -> Show
    * 'inventar'/'besitz'/'eigentum'                        -> Inv
    ;

Verb 'geh' 'lauf' 'renn' 'wander' 'fluecht'
    'flieh' 'schreit' 'spazier'
    *                                                       -> VagueGo
    * 'umher'/'weg'/'fort'                                  -> VagueGo
    * 'in'/'durch'/'ueber' noun                             -> Enter
    * 'auf' noun                                            -> Enter
    * noun=ADirection                                       -> Go
    * 'nach' noun=ADirection                                -> Go
    * 'nach' 'draussen'                                     -> Exit
    * 'raus'/'hinaus'/'heraus'                              -> Exit
    * 'rein'/'hinein'/'herein'                              -> GoIn
    ;

Verb 'verlass'
    *                                                       -> VagueGo
    * creature                                              -> VagueGo
    * noun                                                  -> Exit
    * 'dies' 'ort'/'raum'/'platz'                           -> VagueGo
    * 'ort'/'raum'/'platz'                                  -> VagueGo
    ;

Verb 'mach'
    * noun 'zu'                                             -> Close
    * noun 'auf'                                            -> Open
    * creature 'an'                                         -> Attack
    * noun 'an'                                             -> SwitchOn
    * noun 'aus'                                            -> SwitchOff
    * noun 'kaputt'                                         -> Attack
    ;

Verb 'inventar' 'inv' 'i//' 'besitz' 'eigentum'
    *                                                       -> Inv
    * 'hoch'/'liste'/'lang'                                 -> InvTall
    * 'quer'/'satz'/'breit'                                 -> InvWide
    * 'als' 'liste'                                         -> InvTall
    * 'als' 'satz'                                          -> InvWide
    ;

Verb 'schau' 'seh' 'sieh' 'blick' 'lug' 'guck' 'kuck'
    *                                                       -> Look
    * dich 'um'                                             -> Look
    * 'herum'/'umher'                                       -> Look
    * dir noun 'an'                                         -> Examine
    * noun                                                  -> Examine
    * 'nach' dative noun=ADirection                         -> Examine
    * 'in' noun=ADirection                                  -> Examine
    * 'durch' noun prep_hindurch                            -> Search
    * 'in' noun prep_hinein                                 -> Search
    * 'aus' noun prep_heraus                                -> Search
    * 'in'/'auf' dative noun prep_nach                      -> Search
    * noun_hinein                                           -> Search
    * noun_hindurch                                         -> Search
    * noun_darunter prep_nach                               -> LookUnder
    * noun_dahinter prep_nach                               -> LookUnder
    * 'unter' dative noun prep_nach                         -> LookUnder
    * 'hinter' dative noun prep_nach                        -> LookUnder
    * 'nach' 'in' dative noun                               -> Search

    * 'nach' topic 'in' dative noun                         -> Consult
    * 'in' dative noun
      'unter' topic 'nach'                                  -> Consult
    * 'in' dative noun
      'nach'/'unter' topic                                  -> Consult
    * 'in' dative noun
      'ueber'/'ob'/'zu' topic 'nach'                        -> Consult
    * 'in' dative noun topic 'nach'                         -> Consult
    * force_nach_in
      topic 'nach' 'in' dative noun                         -> Consult
    * force_nach
      topic 'in' dative noun 'nach'                         -> Consult
    ;

Verb 'lage' 'l//'
    *                                                       -> Look
    ;

Verb 'forsch' 'lern'
    * 'nach'/'ob'/'ueber'/'von'
      topic 'in'/'aus' noun                                 -> Consult
    * 'in'/'aus' dative noun
      'nach'/'ob'/'ueber'/'von' topic                       -> Consult
    * topic 'in'/'aus' dative noun                          -> Consult
    ;

Verb 'konsultier'
    * noun 'ueber' topic                                    -> Consult
    * noun 'bezueglich' topic                               -> Consult
    ;

Verb 'oeffne'
    * noun                                                  -> Open
    * 'mit' dative held noun                                -> Unlock reverse
    * noun 'mit' dative held                                -> Unlock
    ;

Verb 'durchquer' 'betret' 'betritt'
    * noun                                                  -> Enter
    ;

Verb 'tret' 'tritt'
    * noun                                                  -> Attack
    * 'ein'                                                 -> GoIn
    * 'aus'/'heraus'/'hinaus'                               -> Exit
    * 'aus' dative noun                                     -> Exit
    * 'aus' dative noun 'heraus'/'hinaus'                   -> Exit
    * 'in' noun                                             -> Enter
    * 'in' noun 'hinein'/'ein'                              -> Enter
    * 'ein' 'in' noun                                       -> Enter
    * noun_hinein                                           -> Enter
    ;

Verb 'setz'
    * dich 'in' noun prep_hinein                            -> Enter
    * dich 'auf' noun prep_darauf                           -> Enter
    * dich 'auf'/'in' noun 'nieder'                         -> Enter
    * dich noun_hinein                                      -> Enter
    * dich noun_darauf                                      -> Enter
    * multiheld 'ab'/'hin'                                  -> Drop
    * multiexcept 'in' noun prep_hinein                     -> Insert
    * multiexcept 'in' dative noun 'ab'                     -> Insert
    * multiexcept noun_hinein                               -> Insert
    * multiexcept 'auf' noun  prep_darauf                   -> PutOn
    * multiexcept 'auf' noun 'ab'                           -> PutOn
    * multiexcept noun_darauf                               -> PutOn
    ;

Verb 'leg'
    * dich 'hin'                                            -> Sleep
    * dich 'auf' noun prep_darauf                           -> Enter
    * dich 'in' noun prep_hinein                            -> Enter
    * dich 'auf'/'in' noun 'nieder'                         -> Enter
    * dich noun_hinein                                      -> Enter
    * dich noun_darauf                                      -> Enter
    * worn 'ab'                                             -> Disrobe
    * multiheld 'ab'/'hin'/'weg'                            -> Drop
    * multiheld prep_weg                                    -> Drop
    * multiexcept 'in' noun prep_hinein                     -> Insert
    * multiexcept 'in' noun 'ab'                            -> Insert
    * multiexcept noun_hinein                               -> Insert
    * multiexcept 'auf' noun prep_darauf                    -> PutOn
    * multiexcept 'auf' noun 'ab'                           -> PutOn
    * multiexcept noun_darauf                               -> PutOn
    * creature 'um'                                         -> Attack
    * held 'an'/'um'                                        -> Wear
    ;

Verb 'sitz' 'lieg'
    * 'auf' dative noun                                     -> Enter
    * noun_darauf                                           -> Enter
    * 'in' dative noun                                      -> Enter
    ;

Verb 'rein' 'hinein' 'herein'
    *                                                       -> GoIn
    * 'in' noun                                             -> GoIn
    ;

Verb 'raus' 'hinaus' 'heraus'
    *                                                       -> Exit
    * 'aus' dative noun                                     -> Exit
    ;

Verb 'untersuch' 'x//' 'b//' 'betracht'
    'beschreib' 'check' 'begutacht' 'inspizier'
    * noun                                                  -> Examine
    ;

#ifdef u_obj;
Verb 'u//'
    * implicit_up                                           -> Go
    * noun                                                  -> Examine
    ;
#ifnot;
Verb 'u//'
    * noun                                                  -> Examine
    ;
#endif;

Verb 'durchsuch' 'durchwuehl' 'durchstoeber'
    * noun                                                  -> Search
    ;

Verb 'lies' 'les'
    * noun                                                  -> Examine
    * multi 'auf'                                           -> Take
    * 'in' dative noun                                      -> Examine
    * 'nach' 'in' dative noun                               -> Examine
    * 'nach' 'in' dative noun 'ueber'/'von' topic           -> Consult
    * 'nach' 'ueber'/'von' topic 'in' dative noun           -> Consult
    * force_in
      'in' dative noun 'ueber'/'von' topic 'nach'           -> Consult
    * force_in
      'in' dative noun 'ueber'/'von' topic                  -> Consult
    * force_in
      'in' dative noun 'nach' 'ueber'/'von' topic           -> Consult
    * force_in
      'ueber'/'von' topic 'in' dative noun prep_nach        -> Consult
    * force_nach_in
      'ueber'/'von' topic 'nach' 'in' dative noun           -> Consult
    * force_nach_in
      topic 'nach' 'in' dative noun                         -> Consult
    * force_nach
      topic 'in' dative noun 'nach'                         -> Consult
    * force_in
      topic 'in' dative noun                                -> Consult
    ;

Verb 'ja' 'j//' 'jawohl'
    *                                                       -> Yes
    ;

Verb 'nein' 'nee' 'noe'
    *                                                       -> No
    ;

Verb 'sorry' 'entschuldigung' 'entschuldig'
    'verzeih' 'pardon'
    *                                                       -> Sorry
    * 'mir'                                                 -> Sorry
    * 'mir' 'bitt'                                          -> Sorry
    * 'bitt'                                                -> Sorry
    ;

Verb 'scheiss' 'kack' 'arschloch' 'wichser'
    'piss' 'verpiss' 'fick' 'shit' 'fuck'
    *                                                       -> Strong
    * topic                                                 -> Strong
    ;

Verb 'verdammt' 'mist' 'schiet' 'scheibenkl' 'depp'
    *                                                       -> Mild
    * topic                                                 -> Mild
    ;

Verb 'such' 'spuer' 'stoeber' 'wuehl'
    * 'in'/'auf'/'ab'/'hinter'/'neben' noun                 -> Search
    * noun 'ab'                                             -> Search
    * noun_darunter                                         -> LookUnder
    * noun_dahinter                                         -> LookUnder
    * 'unter'/'hinter' dative noun                          -> LookUnder
    * 'in' dative noun 'nach' topic                         -> Consult
    * 'nach' topic 'in' dative noun                         -> Consult
    ;

Verb 'wink'
    *                                                       -> WaveHands
    * 'mit' dative noun                                     -> Wave
    ;

Verb 'stell'
    * dich 'hin'                                            -> Exit
    * dich 'auf' noun prep_darauf                           -> Enter
    * dich 'in' noun prep_hinein                            -> Enter
    * dich 'auf'/'in' noun 'nieder'                         -> Enter
    * dich noun_hinein                                      -> Enter
    * dich noun_darauf                                      -> Enter
    * noun                                                  -> Set
    * multiexcept 'in' noun prep_hinein                     -> Insert
    * multiexcept 'auf' noun prep_darauf                    -> PutOn
    * multiexcept noun_hinein                               -> Insert
    * multiexcept noun_darauf                               -> PutOn
    * noun 'auf' special 'ein'                              -> SetTo
    * noun 'auf' special                                    -> SetTo
    * noun 'auf' topic 'ein'                                -> SetTo
    * noun 'auf' topic                                      -> SetTo
    * noun 'an'                                             -> SwitchOn
    * switchable 'ab'                                       -> SwitchOff
    * multiheld 'ab'/'weg'/'hin'                            -> Drop
    * multiheld prep_weg                                    -> Drop
    ;

Verb 'zieh'
    * noun                                                  -> Pull
    * 'an' dative noun                                      -> Pull
    * noun 'weg'                                            -> Pull
    * worn 'ab'                                             -> Disrobe
    * noun 'ab'                                             -> Take
    * noun prep_heraus                                      -> Remove
    * creature 'aus'                                        -> Undress
    * creature 'an'                                         -> Dress
    * held 'an'/'ueber'/'auf'                               -> Wear
    * noun 'aus'                                            -> Disrobe
    ;

Verb 'reiss' 'zerr' 'rupf'
    * noun                                                  -> Pull
    * 'an' dative noun                                      -> Pull
    * noun 'weg'                                            -> Pull
    * noun 'ab'                                             -> Take
    * noun prep_heraus                                      -> Remove
    ;

Verb 'kleid' 'bekleid' 'schmueck'
    * creature                                              -> Dress
    * dich 'an' 'mit' dative held                           -> Wear
    * dich 'mit' dative held 'an'                           -> Wear
    * dich 'mit' dative held                                -> Wear
    * dich 'in' held                                        -> Wear
    ;

Verb 'entkleid'
    * creature                                              -> Undress
    ;

Verb 'drueck' 'press' 'beweg' 'schieb' 'verschieb'
    * noun                                                  -> Push
    * noun_hinein                                           -> Push
    * noun 'weg'                                            -> Push
    * 'gegen' noun                                          -> Push
    * noun noun                                             -> PushDir
    * noun 'nach'/'richtung' noun                           -> PushDir
    * noun 'aus'/'zusammen'                                 -> Squeeze
    * noun 'zu' dative noun                                 -> Transfer
    * multiexcept 'in' noun                                 -> Insert
    * multiexcept noun_hinein                               -> Insert
    * noun 'auf'                                            -> Open
    * noun 'zu'                                             -> Close
    * noun 'hoch'/'hinauf'                                  -> LookUnder
    * noun 'nach' 'oben'                                    -> LookUnder
    ;

Verb 'dreh' 'rotier' 'schraub'
    * noun                                                  -> Turn
    * 'an' dative noun                                      -> Turn
    * noun 'ein'/'an'                                       -> Switchon
    * noun 'aus'                                            -> Switchoff
    * noun 'aus'                                            -> Switchoff
    * noun 'auf' special                                    -> SetTo
    * noun 'auf'                                            -> Open
    * noun 'zu'                                             -> Close
    ;

Verb 'schalt'
    * noun                                                  -> Switchon
    * noun 'ein'/'an'                                       -> Switchon
    * noun 'aus'/'ab'                                       -> Switchoff
    ;

Verb 'schliess' 'sperr'
    * noun                                                  -> Close
    * noun 'mit' dative held                                -> Lock
    * noun 'mit' dative held 'ab'/'zu'                      -> Lock
    * noun 'ab' 'mit' dative held                           -> Lock
    * noun 'mit' dative held 'auf'                          -> Unlock
    * noun 'auf' 'mit' dative held                          -> Unlock
    ;

Verb 'verschliess' 'verriegel' 'verriegl'
    * lockable 'mit' held                                   -> Lock
    * noun                                                  -> Close
    * noun 'mit' dative held                                -> Lock
    ;

Verb 'schlag'
    * creature                                              -> Attack
    * noun_hinein                                           -> Attack
    * noun                                                  -> Attack
    * 'auf' noun                                            -> Attack
    * noun_darauf                                           -> Attack
    * noun 'auf'                                            -> Open
    * noun 'zu'                                             -> Close
    * 'nach' 'in' dative noun                               -> Examine
    * 'mit' dative held 'gegen' noun                        -> Attack reverse
    * held 'gegen' noun                                     -> Attack reverse
    * noun 'um'/'ab'/'entzwei'/'kaputt'                     -> Attack
    * 'in' dative noun topic 'nach'                         -> Consult
    * 'in' dative noun 'ueber'/'unter'/'zu' topic 'nach'    -> Consult
    * force_nach_in
      'ueber'/'unter'/'zu' topic 'nach' 'in' dative noun    -> Consult
    * force_nach_in
      topic 'nach' 'in' dative noun                         -> Consult
    * force_nach
      topic 'in' dative noun 'nach'                         -> Consult
    ;


Verb 'hau'
    * creature                                              -> Attack
    * noun                                                  -> Attack
    * noun_hinein                                           -> Attack
    * 'auf' noun                                            -> Attack
    * noun_darauf                                           -> Attack
    * 'mit' dative held 'gegen' noun                        -> Attack reverse
    * held 'gegen' noun                                     -> Attack reverse
    * noun 'um'/'ab'/'entzwei'/'kaputt'                     -> Attack
    ;

Verb 'brich' 'brech'
    * noun                                                  -> Attack
    * noun 'ab'/'auseinander'                               -> Attack
    * noun 'ab'/'auseinander' 'mit' dative held             -> Attack
    ;

Verb 'zerbrich' 'vernicht' 'zerstoer' 'zerschlag' 'zertruemmer'
    * noun                                                  -> Attack
    * noun 'mit' dative held                                -> Attack
    ;

Verb 'attackier' 'toet' 'ermord' 'mord'
     'bekaempf' 'folter' 'quael' 'pruegel'
    * creature                                              -> Attack
    * creature 'mit' dative held                            -> Attack
    * noun                                                  -> Attack
    * noun 'mit' dative held                                -> Attack
    ;

Verb 'kaempf'
    * 'mit' dative creature                                 -> Attack
    * 'gegen' creature                                      -> Attack
    * 'mit' dative held 'gegen' creature                    -> Attack reverse
    ;

Verb 'wart' 'z//' 'verharr' 'verweil' 'harr'
    *                                                       -> Wait
    * 'ab'/'aus'                                            -> Wait
    ;

Verb 'antwort' 'sag' 'schrei' 'beantwort'
    * dative creature topic                                 -> Answer reverse
    * topic 'zu' dative creature                            -> Answer
    ;

Verb 'red' 'sprech' 'sprich' 'schwatz' 'schwaetz'
    * creature 'ueber' topic 'an'                           -> Tell
    * 'mit'/'zu' dative creature 'ueber' topic              -> Tell
    * 'ueber' topic 'mit'/'zu' dative creature              -> Tell reverse
    ;

Verb 'erzaehl' 'unterricht' 'bericht' 'erklaer'
    * dative creature 'ueber'/'von' topic                   -> Tell
    ;

Verb 'frag' 'befrag'
    * creature 'ueber'/'zu'/'nach'/'ob' topic               -> Ask
    * creature 'ueber'/'zu'/'nach' topic 'aus'              -> Ask
    ;

Verb 'bitt'
    * creature 'um' noun                                    -> AskFor
    ;

Verb 'ess' 'iss' 'friss' 'verspeis' 'verzehr'
    * held                                                  -> Eat
    ;

Verb 'schlaf' 'nick' 'schlummer' 'does'
    * 'ein'                                                 -> Sleep
    *                                                       -> Sleep
    ;

Verb 'streif'
    * noun                                                  -> Touch
    * noun 'ab'                                             -> Disrobe
    ;

Verb 'sing' 'traeller' 'pfeif' 'jodel'
    *                                                       -> Sing
    ;

Verb 'kletter' 'klettr'
    * noun 'hoch'                                           -> Climb
    * 'auf' noun prep_darauf                                -> Climb
    * 'ueber' noun prep_hinueber                            -> Climb
    * noun_darauf                                           -> Climb
    * 'in' noun prep_hinein                                 -> Enter
    * noun_hinein                                           -> Enter
    * 'aus'/'ab'                                            -> Exit
    * noun_heraus                                           -> Exit
    * 'aus' noun prep_heraus                                -> Exit
    * noun_herunter                                         -> GetOff
    * 'von' noun prep_herunter                              -> GetOff
    ;

Verb 'steig'
    * noun 'hoch'                                           -> Enter
    * 'auf' noun prep_darauf                                -> Enter
    * 'ueber' noun prep_hinueber                            -> Enter
    * noun_darauf                                           -> Enter
    * 'in' noun prep_hinein                                 -> Enter
    * noun_hinein                                           -> Enter
    * 'aus'/'ab'                                            -> Exit
    * noun_heraus                                           -> Exit
    * 'aus' noun prep_heraus                                -> Exit
    * noun_herunter                                         -> GetOff
    * 'von' noun prep_herunter                              -> GetOff
    ;

Verb 'erklimm' 'erkletter'
    * noun                                                  -> Climb
    ;

Verb 'kauf' 'erwerb' 'erwirb'
    * noun                                                  -> Buy
    ;

Verb 'zerdrueck' 'quetsch' 'zerquetsch'
    * noun                                                  -> Squeeze
    ;

Verb 'schwimm' 'tauch'
    *                                                       -> Swim
    ;

Verb 'schwing' 'schwenk' 'wedel' 'wedl' 'bauml' 'baumel'
    * dich 'auf'/'in' noun                                  -> Swing
    * dich noun_darauf                                      -> Swing
    * noun                                                  -> Wave
    * 'auf'/'an' noun                                       -> Swing
    * noun_darauf                                           -> Swing
    * 'mit' dative noun                                     -> Wave
    * 'mit' dative 'der' 'hand'                             -> WaveHands
    * 'mit' dative 'den' 'haenden'                          -> WaveHands
    ;

Verb 'blas' 'pust'
    * noun                                                  -> Blow
    * 'in'/'auf' noun                                       -> Blow
    * noun_darauf                                           -> Blow
    * noun_hinein                                           -> Blow
    ;

Verb 'bet'
    *                                                       -> Pray
    ;

Verb 'wach' 'erwach'
    *                                                       -> Wake
    * 'auf'                                                 -> Wake
    ;

Verb 'weck' 'erweck'
    * creature                                              -> WakeOther
    * creature 'auf'                                        -> WakeOther
    ;

Verb 'kuess' 'umarm' 'lieb' 'streichel' 'streichl'
    'knutsch' 'liebkos'
    * creature                                              -> Kiss;

Verb 'denk'
    *                                                       -> Think
    * 'nach'                                                -> Think
    * 'nach' 'ueber' topic                                  -> Think
    * 'ueber' topic 'nach'                                  -> Think
    ;

Verb 'riech' 'schnueffl' 'schnueffel' 'schnupper'
    'beschnupp' 'beschnuef'
    *                                                       -> Smell
    * noun                                                  -> Smell
    * 'an' noun                                             -> Smell
    ;

Verb 'hoer' 'horch' 'lausch' 'belausch'
    *                                                       -> Listen
    * noun                                                  -> Listen
    * dative noun 'zu'                                      -> Listen
    * 'an' dative noun                                      -> Listen
    ;

Verb 'schmeck' 'leck' 'kost' 'probier'
    * noun                                                  -> Taste
    * 'an' dative noun                                      -> Taste
    ;

Verb 'beruehr' 'ertast' 'befuehl' 'betast'
    * noun                                                  -> Touch
    ;

Verb 'fuehl' 'tast'
    * noun                                                  -> Touch
    * noun 'an'                                             -> Touch
    * 'an'/'ueber'/'nach' dative noun                       -> Touch
    ;

Verb 'wisch' 'reinig' 'putz' 'reib' 'schrubb' 'saeuber'
    'polier' 'glaett' 'schmirgel' 'buerst'
    * noun                                                  -> Rub
    * 'an' dative noun                                      -> Rub
    * noun 'mit' dative held                                -> Rub
    ;

Verb 'bind' 'befestig' 'knot' 'verknot'
    * noun                                                  -> Tie
    * noun 'an'/'mit' dative noun                           -> Tie
    ;

Verb 'zuend'
    * noun 'an'                                             -> Burn
    * noun 'mit' dative held 'an'                           -> Burn
    * 'mit' dative held noun 'an'                           -> Burn reverse
    ;

Verb 'brenn'
    * noun 'an'/'ab'/'nieder'                               -> Burn
    * noun 'mit' dative held 'an'/'ab'/'nieder'             -> Burn
    ;

Verb 'entzuend' 'entflamm' 'verbrenn'
    * noun                                                  -> Burn
    * noun 'mit' dative held                                -> Burn
    * 'mit' dative held noun                                -> Burn reverse
    ;

Verb 'trink' 'sauf' 'schluerf' 'schluck'
    * noun                                                  -> Drink
    * noun 'aus'                                            -> Drink
    ;

Verb 'fuell' 'befuell'
    * noun                                                  -> Fill
    * noun 'auf'                                            -> Fill
    * noun 'in' noun                                        -> Fill reverse
    * noun 'in' noun 'um'                                   -> EmptyT
    * noun 'mit' dative noun                                -> Fill
    ;

Verb 'schneid' 'trenn' 'spalt' 'teil'
    * dich 'von' multiheld                                  -> Drop
    * noun                                                  -> Cut
    * creature                                              -> Attack
    * noun 'durch'/'ab'/'auf'                               -> Cut
    * noun 'mit' dative held                                -> Cut
    * noun 'durch'/'ab'/'auf' 'mit' dative held             -> Cut
    * noun 'mit' dative held 'durch'/'ab'/'auf'             -> Cut
    ;

Verb 'zertrenn' 'durchschneid' 'zerteil' 'zerschneid'
    * noun                                                  -> Cut
    * creature                                              -> Attack
    ;

Verb 'spring' 'huepf'
    *                                                       -> Jump
    * 'hoch'/'herum'/'umher'                                -> Jump
    * 'ueber'/'auf' noun                                    -> JumpOver
    * 'ueber' noun 'hinueber'                               -> JumpOver
    * 'auf' noun 'hinauf'                                   -> JumpOver
    ;

Verb 'grab' 'buddel' 'buddl'
    * 'in' noun                                             -> Dig
    * noun                                                  -> Dig
    * 'in' noun 'mit' dative held                           -> Dig
    * 'mit' dative held 'in' noun                           -> Dig reverse
    * noun 'mit' dative held                                -> Dig
    ;

!   Etwas unsinniges Verb zum Schluss, dessen einziger Sinn es ist, die
!   Adverbialpronomen als Präpositionen zu flaggen.

Verb '...'
    * 'hinein'/'rein'/'darein'                              -> Insert
    * 'darauf'/'drauf'/'herauf'/'rauf'/'hinauf'             -> PutOn
    * 'heraus'/'hinaus'/'raus'/'daraus'                     -> Remove
    * 'herunter'/'hinunter'/'runter'/'drunter'              -> Remove
    ;

! ----------------------------------------------------------------------------
!  This routine is no longer used here, but provided to help existing games
!  which use it as a general parsing routine:

[ ConTopic    w;
    consult_from = wn;
    do w=NextWordStopped();
    until (w==-1 || (w=='to' && action_to_be==##Answer));
    wn--;
    consult_words = wn-consult_from;
    if (consult_words==0) return -1;
    if (action_to_be==##Ask or ##Answer or ##Tell) {
        w=wn; wn=consult_from; parsed_number=NextWord();
        if (parsed_number=='the' && consult_words>1) parsed_number=NextWord();
        wn=w; return 1;
    }
    return 0;
];

! ------------------------------------------------------------------------------
!  Final task: provide trivial routines if the user hasn't already:
! ------------------------------------------------------------------------------

#Stub AfterLife         0;
#Stub AfterPrompt       0;
#Stub Amusing           0;
#Stub BeforeParsing     0;
#Stub ChooseObjects     3;
#Stub DarkToDark        0;
#Stub DeathMessage      0;
#Stub GamePostRoutine   0;
#Stub GamePreRoutine    0;
#Stub InScope           1;
#Stub LookRoutine       0;
#Stub NewRoom           0;
#Stub ParseNumber       2;
#Stub ParserError       1;
#Stub PrintTaskName     1;
#Stub PrintVerb         1;
#Stub TimePasses        0;
#Stub UnknownVerb       1;

#Stub PreInformese      0;
#Stub PostInformese     0;

#Ifdef TARGET_GLULX;
#Stub HandleGlkEvent    2;
#Stub IdentifyGlkObject 4;
#Stub InitGlkWindow     1;
#Endif; ! TARGET_GLULX

#Ifndef PrintRank;
! Constant Make__PR;
! #Endif;
! #Ifdef Make__PR;
[ PrintRank; "."; ];
#Endif;

#Ifndef ParseNoun;
! Constant Make__PN;
! #Endif;
! #Ifdef Make__PN;
[ ParseNoun obj; obj = obj; return -1; ];
#Endif;

#Default Story 0;
#Default Headline 0;

#Ifdef INFIX;
#Include "infix";
#Endif;

! ==============================================================================

Constant LIBRARY_GRAMMAR;                              ! for dependency checking

! ==============================================================================
!   Sanity Check deform:
!   Das ist wichtig, weil Leute oft die Lib-Dateien durcheinanderwerfen.
!
!   Wenn nicht die richtigen Dateien angezogen wurden, wird mit einem
!   Fatal Error abgebrochen. Bei Versionsinkompabilität gibt's eine Warnung.
!
!   Ordnung in seinen Verzeichnissen muss der Autor selber halten. :-)
! ==============================================================================

#ifndef DEFORM___PARSER;
    Default DEFORM___DEP_ERROR 0;
    Message error "[deform] Parser.h not included properly";
#endif;
#ifndef DEFORM___PARSERM;
    Default DEFORM___DEP_ERROR 0;
    Message error "[deform] parserm.h not included properly";
#endif;
#ifndef DEFORM___GERMAN;
    Default DEFORM___DEP_ERROR 0;
    Message error "[deform] German.h not included properly";
#endif;
#ifndef DEFORM___VERBLIB;
    Default DEFORM___DEP_ERROR 0;
    Message error "[deform] VerbLib.h not included properly";
#endif;
#ifndef DEFORM___VERBLIBM;
    Default DEFORM___DEP_ERROR 0;
    Message error "[deform] verblibm.h not included properly";
#endif;
#ifdef DEFORM___DEP_ERROR;
    Message fatalerror
    "[deform] The include files are not correct. Check your include paths.";
#endif;

#ifndef DEFORM___2010_03_16___PARSER;
    Default DEFORM___VERSION_ERROR 0;
#endif;
#ifndef DEFORM___2010_03_16___PARSERM;
    Default DEFORM___VERSION_ERROR 0;
#endif;
#ifndef DEFORM___2010_03_16___GERMAN;
    Default DEFORM___VERSION_ERROR 0;
#endif;
#ifndef DEFORM___2010_03_16___VERBLIB;
    Default DEFORM___VERSION_ERROR 0;
#endif;
#ifndef DEFORM___2010_03_16___VERBLIBM;
    Default DEFORM___VERSION_ERROR 0;
#endif;
#ifdef DEFORM___VERSION_ERROR;
    Message warning
    "[deform] Library files are not of the same version/release.";
#endif;

! ==============================================================================