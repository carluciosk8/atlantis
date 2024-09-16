; Atlantis, Song part, encoded in the AKM (minimalist) format V0.


Atlantis_Start
Atlantis_StartDisarkGenerateExternalLabel

Atlantis_DisarkPointerRegionStart0
	dw Atlantis_InstrumentIndexes	; Index table for the Instruments.
Atlantis_DisarkForceNonReferenceDuring2_1
	dw 0	; Index table for the Arpeggios.
Atlantis_DisarkForceNonReferenceDuring2_2
	dw 0	; Index table for the Pitches.

; The subsongs references.
	dw Atlantis_Subsong0
	dw Atlantis_Subsong1
	dw Atlantis_Subsong2
	dw Atlantis_Subsong3
Atlantis_DisarkPointerRegionEnd0

; The Instrument indexes.
Atlantis_InstrumentIndexes
Atlantis_DisarkPointerRegionStart3
	dw Atlantis_Instrument0
	dw Atlantis_Instrument1
	dw Atlantis_Instrument2
	dw Atlantis_Instrument3
	dw Atlantis_Instrument4
	dw Atlantis_Instrument5
	dw Atlantis_Instrument6
	dw Atlantis_Instrument7
Atlantis_DisarkPointerRegionEnd3

; The Instrument.
Atlantis_DisarkByteRegionStart4
Atlantis_Instrument0
	db 255	; Speed.

Atlantis_Instrument0Loop	db 0	; Volume: 0.

	db 4	; End the instrument.
Atlantis_DisarkPointerRegionStart5
	dw Atlantis_Instrument0Loop	; Loops.
Atlantis_DisarkPointerRegionEnd5

Atlantis_Instrument1
	db 0	; Speed.

	db 57	; Volume: 14.

	db 185	; Volume: 14.
	db 8	; Arpeggio: 4.

	db 185	; Volume: 14.
	db 14	; Arpeggio: 7.

	db 185	; Volume: 14.
	db 232	; Arpeggio: -12.

	db 181	; Volume: 13.
	db 240	; Arpeggio: -8.

	db 181	; Volume: 13.
	db 14	; Arpeggio: 7.

	db 49	; Volume: 12.

	db 177	; Volume: 12.
	db 8	; Arpeggio: 4.

	db 173	; Volume: 11.
	db 14	; Arpeggio: 7.

	db 173	; Volume: 11.
	db 232	; Arpeggio: -12.

	db 169	; Volume: 10.
	db 240	; Arpeggio: -8.

	db 169	; Volume: 10.
	db 14	; Arpeggio: 7.

	db 37	; Volume: 9.

	db 165	; Volume: 9.
	db 8	; Arpeggio: 4.

	db 161	; Volume: 8.
	db 14	; Arpeggio: 7.

	db 161	; Volume: 8.
	db 232	; Arpeggio: -12.

	db 161	; Volume: 8.
	db 240	; Arpeggio: -8.

	db 157	; Volume: 7.
	db 14	; Arpeggio: 7.

	db 4	; End the instrument.
Atlantis_DisarkPointerRegionStart6
	dw Atlantis_Instrument0Loop	; Loop to silence.
Atlantis_DisarkPointerRegionEnd6

Atlantis_Instrument2
	db 2	; Speed.

	db 0	; Volume: 0.

	db 9	; Volume: 2.

	db 17	; Volume: 4.

Atlantis_Instrument2Loop	db 153	; Volume: 6.
	db 14	; Arpeggio: 7.

	db 33	; Volume: 8.

	db 173	; Volume: 11.
	db 24	; Arpeggio: 12.

	db 49	; Volume: 12.

	db 181	; Volume: 13.
	db 24	; Arpeggio: 12.

	db 185	; Volume: 14.
	db 14	; Arpeggio: 7.

	db 185	; Volume: 14.
	db 24	; Arpeggio: 12.

	db 181	; Volume: 13.
	db 14	; Arpeggio: 7.

	db 177	; Volume: 12.
	db 24	; Arpeggio: 12.

	db 45	; Volume: 11.

	db 165	; Volume: 9.
	db 24	; Arpeggio: 12.

	db 4	; End the instrument.
Atlantis_DisarkPointerRegionStart7
	dw Atlantis_Instrument2Loop	; Loops.
Atlantis_DisarkPointerRegionEnd7

Atlantis_Instrument3
	db 0	; Speed.

	db 189	; Volume: 15.
	db 25	; Arpeggio: 12.
	db 11	; Noise: 11.

	db 185	; Volume: 14.
	db 27	; Arpeggio: 13.
	db 10	; Noise: 10.

	db 181	; Volume: 13.
	db 33	; Arpeggio: 16.
	db 9	; Noise: 9.

	db 181	; Volume: 13.
	db 19	; Arpeggio: 9.
	db 8	; Noise: 8.

	db 177	; Volume: 12.
	db 1	; Arpeggio: 0.
	db 7	; Noise: 7.

	db 169	; Volume: 10.
	db 19	; Arpeggio: 9.
	db 6	; Noise: 6.

	db 161	; Volume: 8.
	db 29	; Arpeggio: 14.
	db 5	; Noise: 5.

	db 153	; Volume: 6.
	db 1	; Arpeggio: 0.
	db 4	; Noise: 4.

	db 145	; Volume: 4.
	db 1	; Arpeggio: 0.
	db 3	; Noise: 3.

	db 141	; Volume: 3.
	db 1	; Arpeggio: 0.
	db 2	; Noise: 2.

	db 4	; End the instrument.
Atlantis_DisarkPointerRegionStart8
	dw Atlantis_Instrument0Loop	; Loop to silence.
Atlantis_DisarkPointerRegionEnd8

Atlantis_Instrument4
	db 0	; Speed.

	db 248	; Volume: 15.
	db 1	; Noise.

	db 232	; Volume: 13.
	db 1	; Noise.

	db 216	; Volume: 11.
	db 1	; Noise.

	db 192	; Volume: 8.
	db 1	; Noise.

	db 168	; Volume: 5.
	db 1	; Noise.

	db 4	; End the instrument.
Atlantis_DisarkPointerRegionStart9
	dw Atlantis_Instrument0Loop	; Loop to silence.
Atlantis_DisarkPointerRegionEnd9

Atlantis_Instrument5
	db 5	; Speed.

	db 61	; Volume: 15.

	db 33	; Volume: 8.

	db 49	; Volume: 12.

	db 25	; Volume: 6.

	db 37	; Volume: 9.

	db 17	; Volume: 4.

	db 25	; Volume: 6.

	db 4	; End the instrument.
Atlantis_DisarkPointerRegionStart10
	dw Atlantis_Instrument0Loop	; Loop to silence.
Atlantis_DisarkPointerRegionEnd10

Atlantis_Instrument6
	db 0	; Speed.

	db 248	; Volume: 15.
	db 1	; Noise.

	db 121	; Volume: 14.
	dw 150	; Pitch: 150.

	db 117	; Volume: 13.
	dw 300	; Pitch: 300.

	db 113	; Volume: 12.
	dw 400	; Pitch: 400.

	db 109	; Volume: 11.
	dw 500	; Pitch: 500.

	db 105	; Volume: 10.
	dw 600	; Pitch: 600.

	db 4	; End the instrument.
Atlantis_DisarkPointerRegionStart11
	dw Atlantis_Instrument0Loop	; Loop to silence.
Atlantis_DisarkPointerRegionEnd11

Atlantis_Instrument7
	db 0	; Speed.

	db 26
	db 26
	db 26
	db 26
	db 4	; End the instrument.
Atlantis_DisarkPointerRegionStart12
	dw Atlantis_Instrument0Loop	; Loop to silence.
Atlantis_DisarkPointerRegionEnd12

Atlantis_DisarkByteRegionEnd4
Atlantis_ArpeggioIndexes
Atlantis_DisarkPointerRegionStart13
Atlantis_DisarkPointerRegionEnd13

Atlantis_DisarkByteRegionStart14
Atlantis_DisarkByteRegionEnd14

Atlantis_PitchIndexes
Atlantis_DisarkPointerRegionStart15
Atlantis_DisarkPointerRegionEnd15

Atlantis_DisarkByteRegionStart16
Atlantis_DisarkByteRegionEnd16

; Atlantis, Subsong 0.
; ----------------------------------

Atlantis_Subsong0
Atlantis_Subsong0DisarkPointerRegionStart0
	dw Atlantis_Subsong0_NoteIndexes	; Index table for the notes.
	dw Atlantis_Subsong0_TrackIndexes	; Index table for the Tracks.
Atlantis_Subsong0DisarkPointerRegionEnd0

Atlantis_Subsong0DisarkByteRegionStart1
	db 6	; Initial speed.

	db 7	; Most used instrument.
	db 4	; Second most used instrument.

	db 0	; Most used wait.
	db 1	; Second most used wait.

	db 0	; Default start note in tracks.
	db 0	; Default start instrument in tracks.
	db 0	; Default start wait in tracks.

	db 12	; Are there effects? 12 if yes, 13 if not. Don't ask.
Atlantis_Subsong0DisarkByteRegionEnd1

; The Linker.
Atlantis_Subsong0DisarkByteRegionStart2
; Pattern 0
Atlantis_Subsong0_Loop
	db 170	; State byte.
	db 63	; New height.
	db 128	; New track (0) for channel 1, as a reference (index 0).
	db 128	; New track (0) for channel 2, as a reference (index 0).
	db 128	; New track (0) for channel 3, as a reference (index 0).

	db 1	; End of the Song.
	db 0	; Speed to 0, meaning "end of song".
Atlantis_Subsong0DisarkByteRegionEnd2
Atlantis_Subsong0DisarkPointerRegionStart3
	dw Atlantis_Subsong0_Loop

Atlantis_Subsong0DisarkPointerRegionEnd3
; The indexes of the tracks.
Atlantis_Subsong0_TrackIndexes
Atlantis_Subsong0DisarkPointerRegionStart4
	dw Atlantis_Subsong0_Track0	; Track 0, index 0.
Atlantis_Subsong0DisarkPointerRegionEnd4

Atlantis_Subsong0DisarkByteRegionStart5
Atlantis_Subsong0_Track0
	db 205	; New wait (127).
	db 127	;   Escape wait value.

Atlantis_Subsong0DisarkByteRegionEnd5
; The note indexes.
Atlantis_Subsong0_NoteIndexes
Atlantis_Subsong0DisarkByteRegionStart6
Atlantis_Subsong0DisarkByteRegionEnd6

; Atlantis, Subsong 1.
; ----------------------------------

Atlantis_Subsong1
Atlantis_Subsong1DisarkPointerRegionStart0
	dw Atlantis_Subsong1_NoteIndexes	; Index table for the notes.
	dw Atlantis_Subsong1_TrackIndexes	; Index table for the Tracks.
Atlantis_Subsong1DisarkPointerRegionEnd0

Atlantis_Subsong1DisarkByteRegionStart1
	db 8	; Initial speed.

	db 7	; Most used instrument.
	db 4	; Second most used instrument.

	db 0	; Most used wait.
	db 1	; Second most used wait.

	db 69	; Default start note in tracks.
	db 6	; Default start instrument in tracks.
	db 8	; Default start wait in tracks.

	db 12	; Are there effects? 12 if yes, 13 if not. Don't ask.
Atlantis_Subsong1DisarkByteRegionEnd1

; The Linker.
Atlantis_Subsong1DisarkByteRegionStart2
; Pattern 0
	db 170	; State byte.
	db 63	; New height.
	db ((Atlantis_Subsong1_Track0 - ($ + 2)) & #ff00) / 256	; New track (0) for channel 1, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track0 - ($ + 1)) & 255)
	db ((Atlantis_Subsong1_Track1 - ($ + 2)) & #ff00) / 256	; New track (1) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track1 - ($ + 1)) & 255)
	db ((Atlantis_Subsong1_Track2 - ($ + 2)) & #ff00) / 256	; New track (2) for channel 3, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track2 - ($ + 1)) & 255)

; Pattern 1
	db 160	; State byte.
	db ((Atlantis_Subsong1_Track3 - ($ + 2)) & #ff00) / 256	; New track (3) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track3 - ($ + 1)) & 255)
	db ((Atlantis_Subsong1_Track4 - ($ + 2)) & #ff00) / 256	; New track (4) for channel 3, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track4 - ($ + 1)) & 255)

; Pattern 2
Atlantis_Subsong1_Loop
	db 168	; State byte.
	db ((Atlantis_Subsong1_Track5 - ($ + 2)) & #ff00) / 256	; New track (5) for channel 1, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track5 - ($ + 1)) & 255)
	db ((Atlantis_Subsong1_Track6 - ($ + 2)) & #ff00) / 256	; New track (6) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track6 - ($ + 1)) & 255)
	db ((Atlantis_Subsong1_Track7 - ($ + 2)) & #ff00) / 256	; New track (7) for channel 3, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track7 - ($ + 1)) & 255)

; Pattern 3
	db 168	; State byte.
	db ((Atlantis_Subsong1_Track8 - ($ + 2)) & #ff00) / 256	; New track (8) for channel 1, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track8 - ($ + 1)) & 255)
	db ((Atlantis_Subsong1_Track9 - ($ + 2)) & #ff00) / 256	; New track (9) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track9 - ($ + 1)) & 255)
	db ((Atlantis_Subsong1_Track10 - ($ + 2)) & #ff00) / 256	; New track (10) for channel 3, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track10 - ($ + 1)) & 255)

; Pattern 4
	db 136	; State byte.
	db ((Atlantis_Subsong1_Track11 - ($ + 2)) & #ff00) / 256	; New track (11) for channel 1, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track11 - ($ + 1)) & 255)
	db ((Atlantis_Subsong1_Track12 - ($ + 2)) & #ff00) / 256	; New track (12) for channel 3, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track12 - ($ + 1)) & 255)

; Pattern 5
	db 168	; State byte.
	db ((Atlantis_Subsong1_Track13 - ($ + 2)) & #ff00) / 256	; New track (13) for channel 1, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track13 - ($ + 1)) & 255)
	db ((Atlantis_Subsong1_Track14 - ($ + 2)) & #ff00) / 256	; New track (14) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track14 - ($ + 1)) & 255)
	db ((Atlantis_Subsong1_Track15 - ($ + 2)) & #ff00) / 256	; New track (15) for channel 3, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong1_Track15 - ($ + 1)) & 255)

	db 1	; End of the Song.
	db 0	; Speed to 0, meaning "end of song".
Atlantis_Subsong1DisarkByteRegionEnd2
Atlantis_Subsong1DisarkPointerRegionStart3
	dw Atlantis_Subsong1_Loop

Atlantis_Subsong1DisarkPointerRegionEnd3
; The indexes of the tracks.
Atlantis_Subsong1_TrackIndexes
Atlantis_Subsong1DisarkPointerRegionStart4
Atlantis_Subsong1DisarkPointerRegionEnd4

Atlantis_Subsong1DisarkByteRegionStart5
Atlantis_Subsong1_Track0
	db 12	; Note with effects flag.
	db 254	; New instrument (2). New escaped note: 21. New wait (15).
	db 21	;   Escape note value.
	db 2	;   Escape instrument value.
	db 15	;   Escape wait value.
	db 98	;    Volume effect, with inverted volume: 6.
	db 14	; New escaped note: 26. 
	db 26	;   Escape note value.
	db 12	; Note with effects flag.
	db 14	; New escaped note: 21. 
	db 21	;   Escape note value.
	db 82	;    Volume effect, with inverted volume: 5.
	db 206	; New escaped note: 26. New wait (7).
	db 26	;   Escape note value.
	db 7	;   Escape wait value.
	db 206	; New escaped note: 28. New wait (127).
	db 28	;   Escape note value.
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track1
	db 205	; New wait (7).
	db 7	;   Escape wait value.
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 207	; Same escaped note: 33. New wait (2).
	db 2	;   Escape wait value.
	db 191	; New instrument (3). Same escaped note: 33. Secondary wait (1).
	db 3	;   Escape instrument value.
	db 191	; New instrument (6). Same escaped note: 33. Secondary wait (1).
	db 6	;   Escape instrument value.
	db 207	; Same escaped note: 33. New wait (3).
	db 3	;   Escape wait value.
	db 63	; New instrument (3). Same escaped note: 33. 
	db 3	;   Escape instrument value.
	db 191	; New instrument (6). Same escaped note: 33. Secondary wait (1).
	db 6	;   Escape instrument value.
	db 207	; Same escaped note: 33. New wait (5).
	db 5	;   Escape wait value.
	db 255	; New instrument (3). Same escaped note: 33. New wait (8).
	db 3	;   Escape instrument value.
	db 8	;   Escape wait value.
	db 127	; New instrument (6). Same escaped note: 33. Primary wait (0).
	db 6	;   Escape instrument value.
	db 143	; Same escaped note: 33. Secondary wait (1).
	db 191	; New instrument (3). Same escaped note: 33. Secondary wait (1).
	db 3	;   Escape instrument value.
	db 191	; New instrument (6). Same escaped note: 33. Secondary wait (1).
	db 6	;   Escape instrument value.
	db 255	; New instrument (3). Same escaped note: 33. New wait (3).
	db 3	;   Escape instrument value.
	db 3	;   Escape wait value.
	db 126	; New instrument (6). New escaped note: 45. Primary wait (0).
	db 45	;   Escape note value.
	db 6	;   Escape instrument value.
	db 143	; Same escaped note: 45. Secondary wait (1).
	db 207	; Same escaped note: 45. New wait (2).
	db 2	;   Escape wait value.
	db 254	; New instrument (3). New escaped note: 33. New wait (3).
	db 33	;   Escape note value.
	db 3	;   Escape instrument value.
	db 3	;   Escape wait value.
	db 207	; Same escaped note: 33. New wait (127).
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track2
	db 191	; New instrument (5). Same escaped note: 69. Secondary wait (1).
	db 5	;   Escape instrument value.
	db 206	; New escaped note: 57. New wait (13).
	db 57	;   Escape note value.
	db 13	;   Escape wait value.
	db 142	; New escaped note: 62. Secondary wait (1).
	db 62	;   Escape note value.
	db 14	; New escaped note: 50. 
	db 50	;   Escape note value.
	db 142	; New escaped note: 69. Secondary wait (1).
	db 69	;   Escape note value.
	db 14	; New escaped note: 57. 
	db 57	;   Escape note value.
	db 142	; New escaped note: 62. Secondary wait (1).
	db 62	;   Escape note value.
	db 206	; New escaped note: 50. New wait (5).
	db 50	;   Escape note value.
	db 5	;   Escape wait value.
	db 142	; New escaped note: 64. Secondary wait (1).
	db 64	;   Escape note value.
	db 206	; New escaped note: 52. New wait (2).
	db 52	;   Escape note value.
	db 2	;   Escape wait value.
	db 190	; New instrument (3). New escaped note: 21. Secondary wait (1).
	db 21	;   Escape note value.
	db 3	;   Escape instrument value.
	db 206	; New escaped note: 33. New wait (127).
	db 33	;   Escape note value.
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track3
	db 206	; New escaped note: 45. New wait (3).
	db 45	;   Escape note value.
	db 3	;   Escape wait value.
	db 62	; New instrument (3). New escaped note: 33. 
	db 33	;   Escape note value.
	db 3	;   Escape instrument value.
	db 62	; New instrument (6). New escaped note: 45. 
	db 45	;   Escape note value.
	db 6	;   Escape instrument value.
	db 62	; New instrument (3). New escaped note: 33. 
	db 33	;   Escape note value.
	db 3	;   Escape instrument value.
	db 62	; New instrument (6). New escaped note: 45. 
	db 45	;   Escape note value.
	db 6	;   Escape instrument value.
	db 190	; New instrument (3). New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 3	;   Escape instrument value.
	db 191	; New instrument (6). Same escaped note: 33. Secondary wait (1).
	db 6	;   Escape instrument value.
	db 191	; New instrument (3). Same escaped note: 33. Secondary wait (1).
	db 3	;   Escape instrument value.
	db 143	; Same escaped note: 33. Secondary wait (1).
	db 15	; Same escaped note: 33. 
	db 191	; New instrument (6). Same escaped note: 33. Secondary wait (1).
	db 6	;   Escape instrument value.
	db 191	; New instrument (3). Same escaped note: 33. Secondary wait (1).
	db 3	;   Escape instrument value.
	db 63	; New instrument (6). Same escaped note: 33. 
	db 6	;   Escape instrument value.
	db 63	; New instrument (3). Same escaped note: 33. 
	db 3	;   Escape instrument value.
	db 191	; New instrument (6). Same escaped note: 33. Secondary wait (1).
	db 6	;   Escape instrument value.
	db 191	; New instrument (3). Same escaped note: 33. Secondary wait (1).
	db 3	;   Escape instrument value.
	db 15	; Same escaped note: 33. 
	db 191	; New instrument (6). Same escaped note: 33. Secondary wait (1).
	db 6	;   Escape instrument value.
	db 207	; Same escaped note: 33. New wait (2).
	db 2	;   Escape wait value.
	db 143	; Same escaped note: 33. Secondary wait (1).
	db 79	; Same escaped note: 33. Primary wait (0).
	db 143	; Same escaped note: 33. Secondary wait (1).
	db 207	; Same escaped note: 33. New wait (127).
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track4
	db 191	; New instrument (5). Same escaped note: 69. Secondary wait (1).
	db 5	;   Escape instrument value.
	db 206	; New escaped note: 57. New wait (13).
	db 57	;   Escape note value.
	db 13	;   Escape wait value.
	db 142	; New escaped note: 62. Secondary wait (1).
	db 62	;   Escape note value.
	db 14	; New escaped note: 50. 
	db 50	;   Escape note value.
	db 142	; New escaped note: 69. Secondary wait (1).
	db 69	;   Escape note value.
	db 14	; New escaped note: 57. 
	db 57	;   Escape note value.
	db 142	; New escaped note: 62. Secondary wait (1).
	db 62	;   Escape note value.
	db 206	; New escaped note: 50. New wait (2).
	db 50	;   Escape note value.
	db 2	;   Escape wait value.
	db 190	; New instrument (3). New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 3	;   Escape instrument value.
	db 79	; Same escaped note: 33. Primary wait (0).
	db 190	; New instrument (5). New escaped note: 64. Secondary wait (1).
	db 64	;   Escape note value.
	db 5	;   Escape instrument value.
	db 14	; New escaped note: 52. 
	db 52	;   Escape note value.
	db 190	; New instrument (3). New escaped note: 21. Secondary wait (1).
	db 21	;   Escape note value.
	db 3	;   Escape instrument value.
	db 206	; New escaped note: 33. New wait (127).
	db 33	;   Escape note value.
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track5
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 26. Primary wait (0).
	db 26	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 26. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 28. Primary wait (0).
	db 28	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 28. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 26. Primary wait (0).
	db 26	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 26. Primary wait (0).
	db 142	; New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 28. Primary wait (0).
	db 28	;   Escape note value.
	db 223	; Primary instrument (7). Same escaped note: 28. New wait (127).
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track6
	db 190	; New instrument (3). New escaped note: 21. Secondary wait (1).
	db 21	;   Escape note value.
	db 3	;   Escape instrument value.
	db 238	; Secondary instrument (4). New escaped note: 36. New wait (2).
	db 36	;   Escape note value.
	db 2	;   Escape wait value.
	db 46	; Secondary instrument (4). New escaped note: 38. 
	db 38	;   Escape note value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 238	; Secondary instrument (4). New escaped note: 38. New wait (3).
	db 38	;   Escape note value.
	db 3	;   Escape wait value.
	db 239	; Secondary instrument (4). Same escaped note: 38. New wait (2).
	db 2	;   Escape wait value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 238	; Secondary instrument (4). New escaped note: 38. New wait (3).
	db 38	;   Escape note value.
	db 3	;   Escape wait value.
	db 239	; Secondary instrument (4). Same escaped note: 38. New wait (2).
	db 2	;   Escape wait value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 238	; Secondary instrument (4). New escaped note: 38. New wait (3).
	db 38	;   Escape note value.
	db 3	;   Escape wait value.
	db 239	; Secondary instrument (4). Same escaped note: 38. New wait (2).
	db 2	;   Escape wait value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 238	; Secondary instrument (4). New escaped note: 38. New wait (3).
	db 38	;   Escape note value.
	db 3	;   Escape wait value.
	db 239	; Secondary instrument (4). Same escaped note: 38. New wait (2).
	db 2	;   Escape wait value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 238	; Secondary instrument (4). New escaped note: 38. New wait (3).
	db 38	;   Escape note value.
	db 3	;   Escape wait value.
	db 239	; Secondary instrument (4). Same escaped note: 38. New wait (2).
	db 2	;   Escape wait value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 238	; Secondary instrument (4). New escaped note: 38. New wait (3).
	db 38	;   Escape note value.
	db 3	;   Escape wait value.
	db 239	; Secondary instrument (4). Same escaped note: 38. New wait (2).
	db 2	;   Escape wait value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 78	; New escaped note: 45. Primary wait (0).
	db 45	;   Escape note value.
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 78	; New escaped note: 45. Primary wait (0).
	db 45	;   Escape note value.
	db 206	; New escaped note: 33. New wait (127).
	db 33	;   Escape note value.
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track7
	db 191	; New instrument (5). Same escaped note: 69. Secondary wait (1).
	db 5	;   Escape instrument value.
	db 206	; New escaped note: 57. New wait (13).
	db 57	;   Escape note value.
	db 13	;   Escape wait value.
	db 142	; New escaped note: 62. Secondary wait (1).
	db 62	;   Escape note value.
	db 206	; New escaped note: 50. New wait (9).
	db 50	;   Escape note value.
	db 9	;   Escape wait value.
	db 254	; New instrument (3). New escaped note: 45. New wait (3).
	db 45	;   Escape note value.
	db 3	;   Escape instrument value.
	db 3	;   Escape wait value.
	db 190	; New instrument (5). New escaped note: 69. Secondary wait (1).
	db 69	;   Escape note value.
	db 5	;   Escape instrument value.
	db 206	; New escaped note: 57. New wait (13).
	db 57	;   Escape note value.
	db 13	;   Escape wait value.
	db 142	; New escaped note: 62. Secondary wait (1).
	db 62	;   Escape note value.
	db 206	; New escaped note: 50. New wait (5).
	db 50	;   Escape note value.
	db 5	;   Escape wait value.
	db 142	; New escaped note: 64. Secondary wait (1).
	db 64	;   Escape note value.
	db 206	; New escaped note: 52. New wait (127).
	db 52	;   Escape note value.
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track8
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 26. Primary wait (0).
	db 26	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 26. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 26. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 28. Primary wait (0).
	db 28	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 28. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 28. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 26. Primary wait (0).
	db 26	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 26. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 26. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 28. Primary wait (0).
	db 28	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 28. Primary wait (0).
	db 223	; Primary instrument (7). Same escaped note: 28. New wait (127).
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track9
	db 126	; New instrument (3). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 3	;   Escape instrument value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 36. Primary wait (0).
	db 36	;   Escape note value.
	db 174	; Secondary instrument (4). New escaped note: 40. Secondary wait (1).
	db 40	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 111	; Secondary instrument (4). Same escaped note: 41. Primary wait (0).
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 174	; Secondary instrument (4). New escaped note: 41. Secondary wait (1).
	db 41	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 174	; Secondary instrument (4). New escaped note: 41. Secondary wait (1).
	db 41	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 111	; Secondary instrument (4). Same escaped note: 41. Primary wait (0).
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 174	; Secondary instrument (4). New escaped note: 41. Secondary wait (1).
	db 41	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 174	; Secondary instrument (4). New escaped note: 41. Secondary wait (1).
	db 41	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 174	; Secondary instrument (4). New escaped note: 41. Secondary wait (1).
	db 41	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 174	; Secondary instrument (4). New escaped note: 41. Secondary wait (1).
	db 41	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 174	; Secondary instrument (4). New escaped note: 41. Secondary wait (1).
	db 41	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 238	; Secondary instrument (4). New escaped note: 41. New wait (127).
	db 41	;   Escape note value.
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track10
	db 191	; New instrument (5). Same escaped note: 69. Secondary wait (1).
	db 5	;   Escape instrument value.
	db 206	; New escaped note: 57. New wait (13).
	db 57	;   Escape note value.
	db 13	;   Escape wait value.
	db 142	; New escaped note: 62. Secondary wait (1).
	db 62	;   Escape note value.
	db 14	; New escaped note: 50. 
	db 50	;   Escape note value.
	db 142	; New escaped note: 69. Secondary wait (1).
	db 69	;   Escape note value.
	db 14	; New escaped note: 57. 
	db 57	;   Escape note value.
	db 142	; New escaped note: 62. Secondary wait (1).
	db 62	;   Escape note value.
	db 206	; New escaped note: 50. New wait (5).
	db 50	;   Escape note value.
	db 5	;   Escape wait value.
	db 142	; New escaped note: 64. Secondary wait (1).
	db 64	;   Escape note value.
	db 206	; New escaped note: 52. New wait (127).
	db 52	;   Escape note value.
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track11
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 26. Primary wait (0).
	db 26	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 26. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 28. Primary wait (0).
	db 28	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 28. Primary wait (0).
	db 94	; Primary instrument (7). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 21. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 26. Primary wait (0).
	db 26	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 26. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 28. Primary wait (0).
	db 28	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 28. Primary wait (0).
	db 222	; Primary instrument (7). New escaped note: 40. New wait (127).
	db 40	;   Escape note value.
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track12
	db 191	; New instrument (5). Same escaped note: 69. Secondary wait (1).
	db 5	;   Escape instrument value.
	db 142	; New escaped note: 57. Secondary wait (1).
	db 57	;   Escape note value.
	db 255	; New instrument (1). Same escaped note: 57. New wait (7).
	db 1	;   Escape instrument value.
	db 7	;   Escape wait value.
	db 207	; Same escaped note: 57. New wait (3).
	db 3	;   Escape wait value.
	db 190	; New instrument (5). New escaped note: 62. Secondary wait (1).
	db 62	;   Escape note value.
	db 5	;   Escape instrument value.
	db 142	; New escaped note: 50. Secondary wait (1).
	db 50	;   Escape note value.
	db 62	; New instrument (1). New escaped note: 57. 
	db 57	;   Escape note value.
	db 1	;   Escape instrument value.
	db 206	; New escaped note: 55. New wait (7).
	db 55	;   Escape note value.
	db 7	;   Escape wait value.
	db 190	; New instrument (5). New escaped note: 69. Secondary wait (1).
	db 69	;   Escape note value.
	db 5	;   Escape instrument value.
	db 142	; New escaped note: 57. Secondary wait (1).
	db 57	;   Escape note value.
	db 63	; New instrument (1). Same escaped note: 57. 
	db 1	;   Escape instrument value.
	db 207	; Same escaped note: 57. New wait (3).
	db 3	;   Escape wait value.
	db 190	; New instrument (5). New escaped note: 62. Secondary wait (1).
	db 62	;   Escape note value.
	db 5	;   Escape instrument value.
	db 142	; New escaped note: 50. Secondary wait (1).
	db 50	;   Escape note value.
	db 62	; New instrument (1). New escaped note: 57. 
	db 57	;   Escape note value.
	db 1	;   Escape instrument value.
	db 190	; New instrument (5). New escaped note: 64. Secondary wait (1).
	db 64	;   Escape note value.
	db 5	;   Escape instrument value.
	db 142	; New escaped note: 52. Secondary wait (1).
	db 52	;   Escape note value.
	db 190	; New instrument (1). New escaped note: 62. Secondary wait (1).
	db 62	;   Escape note value.
	db 1	;   Escape instrument value.
	db 207	; Same escaped note: 62. New wait (127).
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track13
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 79	; Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 79	; Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 79	; Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 79	; Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 79	; Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 79	; Same escaped note: 33. Primary wait (0).
	db 94	; Primary instrument (7). New escaped note: 26. Primary wait (0).
	db 26	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 26. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 26. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 28. Primary wait (0).
	db 28	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 28. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 28. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 79	; Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 79	; Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 79	; Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 79	; Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 79	; Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 33. Primary wait (0).
	db 79	; Same escaped note: 33. Primary wait (0).
	db 94	; Primary instrument (7). New escaped note: 26. Primary wait (0).
	db 26	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 26. Primary wait (0).
	db 95	; Primary instrument (7). Same escaped note: 26. Primary wait (0).
	db 78	; New escaped note: 33. Primary wait (0).
	db 33	;   Escape note value.
	db 94	; Primary instrument (7). New escaped note: 28. Primary wait (0).
	db 28	;   Escape note value.
	db 95	; Primary instrument (7). Same escaped note: 28. Primary wait (0).
	db 223	; Primary instrument (7). Same escaped note: 28. New wait (127).
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track14
	db 126	; New instrument (3). New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 3	;   Escape instrument value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 36. Primary wait (0).
	db 36	;   Escape note value.
	db 174	; Secondary instrument (4). New escaped note: 40. Secondary wait (1).
	db 40	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 111	; Secondary instrument (4). Same escaped note: 41. Primary wait (0).
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 174	; Secondary instrument (4). New escaped note: 41. Secondary wait (1).
	db 41	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 78	; New escaped note: 31. Primary wait (0).
	db 31	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 174	; Secondary instrument (4). New escaped note: 41. Secondary wait (1).
	db 41	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 111	; Secondary instrument (4). Same escaped note: 41. Primary wait (0).
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 78	; New escaped note: 31. Primary wait (0).
	db 31	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 78	; New escaped note: 31. Primary wait (0).
	db 31	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 174	; Secondary instrument (4). New escaped note: 41. Secondary wait (1).
	db 41	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 78	; New escaped note: 31. Primary wait (0).
	db 31	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 78	; New escaped note: 31. Primary wait (0).
	db 31	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 78	; New escaped note: 31. Primary wait (0).
	db 31	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 78	; New escaped note: 21. Primary wait (0).
	db 21	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 38. Primary wait (0).
	db 38	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 40. Primary wait (0).
	db 40	;   Escape note value.
	db 110	; Secondary instrument (4). New escaped note: 41. Primary wait (0).
	db 41	;   Escape note value.
	db 78	; New escaped note: 31. Primary wait (0).
	db 31	;   Escape note value.
	db 79	; Same escaped note: 31. Primary wait (0).
	db 79	; Same escaped note: 31. Primary wait (0).
	db 207	; Same escaped note: 31. New wait (127).
	db 127	;   Escape wait value.

Atlantis_Subsong1_Track15
	db 191	; New instrument (5). Same escaped note: 69. Secondary wait (1).
	db 5	;   Escape instrument value.
	db 142	; New escaped note: 57. Secondary wait (1).
	db 57	;   Escape note value.
	db 255	; New instrument (1). Same escaped note: 57. New wait (7).
	db 1	;   Escape instrument value.
	db 7	;   Escape wait value.
	db 143	; Same escaped note: 57. Secondary wait (1).
	db 190	; New instrument (5). New escaped note: 69. Secondary wait (1).
	db 69	;   Escape note value.
	db 5	;   Escape instrument value.
	db 142	; New escaped note: 57. Secondary wait (1).
	db 57	;   Escape note value.
	db 190	; New instrument (1). New escaped note: 60. Secondary wait (1).
	db 60	;   Escape note value.
	db 1	;   Escape instrument value.
	db 206	; New escaped note: 57. New wait (3).
	db 57	;   Escape note value.
	db 3	;   Escape wait value.
	db 206	; New escaped note: 55. New wait (5).
	db 55	;   Escape note value.
	db 5	;   Escape wait value.
	db 190	; New instrument (5). New escaped note: 62. Secondary wait (1).
	db 62	;   Escape note value.
	db 5	;   Escape instrument value.
	db 206	; New escaped note: 50. New wait (3).
	db 50	;   Escape note value.
	db 3	;   Escape wait value.
	db 254	; New instrument (1). New escaped note: 57. New wait (7).
	db 57	;   Escape note value.
	db 1	;   Escape instrument value.
	db 7	;   Escape wait value.
	db 143	; Same escaped note: 57. Secondary wait (1).
	db 190	; New instrument (5). New escaped note: 69. Secondary wait (1).
	db 69	;   Escape note value.
	db 5	;   Escape instrument value.
	db 142	; New escaped note: 57. Secondary wait (1).
	db 57	;   Escape note value.
	db 190	; New instrument (1). New escaped note: 60. Secondary wait (1).
	db 60	;   Escape note value.
	db 1	;   Escape instrument value.
	db 206	; New escaped note: 57. New wait (3).
	db 57	;   Escape note value.
	db 3	;   Escape wait value.
	db 14	; New escaped note: 62. 
	db 62	;   Escape note value.
	db 143	; Same escaped note: 62. Secondary wait (1).
	db 207	; Same escaped note: 62. New wait (127).
	db 127	;   Escape wait value.

Atlantis_Subsong1DisarkByteRegionEnd5
; The note indexes.
Atlantis_Subsong1_NoteIndexes
Atlantis_Subsong1DisarkByteRegionStart6
Atlantis_Subsong1DisarkByteRegionEnd6

; Atlantis, Subsong 2.
; ----------------------------------

Atlantis_Subsong2
Atlantis_Subsong2DisarkPointerRegionStart0
	dw Atlantis_Subsong2_NoteIndexes	; Index table for the notes.
	dw Atlantis_Subsong2_TrackIndexes	; Index table for the Tracks.
Atlantis_Subsong2DisarkPointerRegionEnd0

Atlantis_Subsong2DisarkByteRegionStart1
	db 6	; Initial speed.

	db 7	; Most used instrument.
	db 4	; Second most used instrument.

	db 0	; Most used wait.
	db 1	; Second most used wait.

	db 42	; Default start note in tracks.
	db 5	; Default start instrument in tracks.
	db 0	; Default start wait in tracks.

	db 12	; Are there effects? 12 if yes, 13 if not. Don't ask.
Atlantis_Subsong2DisarkByteRegionEnd1

; The Linker.
Atlantis_Subsong2DisarkByteRegionStart2
; Pattern 0
Atlantis_Subsong2_Loop
	db 170	; State byte.
	db 63	; New height.
	db ((Atlantis_Subsong2_Track0 - ($ + 2)) & #ff00) / 256	; New track (0) for channel 1, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong2_Track0 - ($ + 1)) & 255)
	db ((Atlantis_Subsong2_Track1 - ($ + 2)) & #ff00) / 256	; New track (1) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong2_Track1 - ($ + 1)) & 255)
	db ((Atlantis_Subsong2_Track2 - ($ + 2)) & #ff00) / 256	; New track (2) for channel 3, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong2_Track2 - ($ + 1)) & 255)

	db 1	; End of the Song.
	db 0	; Speed to 0, meaning "end of song".
Atlantis_Subsong2DisarkByteRegionEnd2
Atlantis_Subsong2DisarkPointerRegionStart3
	dw Atlantis_Subsong2_Loop

Atlantis_Subsong2DisarkPointerRegionEnd3
; The indexes of the tracks.
Atlantis_Subsong2_TrackIndexes
Atlantis_Subsong2DisarkPointerRegionStart4
Atlantis_Subsong2DisarkPointerRegionEnd4

Atlantis_Subsong2DisarkByteRegionStart5
Atlantis_Subsong2_Track0
	db 254	; New instrument (1). New escaped note: 58. New wait (3).
	db 58	;   Escape note value.
	db 1	;   Escape instrument value.
	db 3	;   Escape wait value.
	db 143	; Same escaped note: 58. Secondary wait (1).
	db 143	; Same escaped note: 58. Secondary wait (1).
	db 14	; New escaped note: 54. 
	db 54	;   Escape note value.
	db 207	; Same escaped note: 54. New wait (5).
	db 5	;   Escape wait value.
	db 206	; New escaped note: 51. New wait (3).
	db 51	;   Escape note value.
	db 3	;   Escape wait value.
	db 143	; Same escaped note: 51. Secondary wait (1).
	db 143	; Same escaped note: 51. Secondary wait (1).
	db 142	; New escaped note: 49. Secondary wait (1).
	db 49	;   Escape note value.
	db 206	; New escaped note: 51. New wait (127).
	db 51	;   Escape note value.
	db 127	;   Escape wait value.

Atlantis_Subsong2_Track1
	db 206	; New escaped note: 46. New wait (3).
	db 46	;   Escape note value.
	db 3	;   Escape wait value.
	db 62	; New instrument (3). New escaped note: 24. 
	db 24	;   Escape note value.
	db 3	;   Escape instrument value.
	db 62	; New instrument (5). New escaped note: 44. 
	db 44	;   Escape note value.
	db 5	;   Escape instrument value.
	db 62	; New instrument (3). New escaped note: 24. 
	db 24	;   Escape note value.
	db 3	;   Escape instrument value.
	db 62	; New instrument (5). New escaped note: 42. 
	db 42	;   Escape note value.
	db 5	;   Escape instrument value.
	db 62	; New instrument (3). New escaped note: 24. 
	db 24	;   Escape note value.
	db 3	;   Escape instrument value.
	db 62	; New instrument (5). New escaped note: 39. 
	db 39	;   Escape note value.
	db 5	;   Escape instrument value.
	db 254	; New instrument (3). New escaped note: 24. New wait (127).
	db 24	;   Escape note value.
	db 3	;   Escape instrument value.
	db 127	;   Escape wait value.

Atlantis_Subsong2_Track2
	db 207	; Same escaped note: 42. New wait (3).
	db 3	;   Escape wait value.
	db 15	; Same escaped note: 42. 
	db 14	; New escaped note: 39. 
	db 39	;   Escape note value.
	db 15	; Same escaped note: 39. 
	db 14	; New escaped note: 37. 
	db 37	;   Escape note value.
	db 15	; Same escaped note: 37. 
	db 14	; New escaped note: 34. 
	db 34	;   Escape note value.
	db 206	; New escaped note: 37. New wait (127).
	db 37	;   Escape note value.
	db 127	;   Escape wait value.

Atlantis_Subsong2DisarkByteRegionEnd5
; The note indexes.
Atlantis_Subsong2_NoteIndexes
Atlantis_Subsong2DisarkByteRegionStart6
Atlantis_Subsong2DisarkByteRegionEnd6

; Atlantis, Subsong 3.
; ----------------------------------

Atlantis_Subsong3
Atlantis_Subsong3DisarkPointerRegionStart0
	dw Atlantis_Subsong3_NoteIndexes	; Index table for the notes.
	dw Atlantis_Subsong3_TrackIndexes	; Index table for the Tracks.
Atlantis_Subsong3DisarkPointerRegionEnd0

Atlantis_Subsong3DisarkByteRegionStart1
	db 6	; Initial speed.

	db 7	; Most used instrument.
	db 4	; Second most used instrument.

	db 0	; Most used wait.
	db 1	; Second most used wait.

	db 25	; Default start note in tracks.
	db 5	; Default start instrument in tracks.
	db 0	; Default start wait in tracks.

	db 12	; Are there effects? 12 if yes, 13 if not. Don't ask.
Atlantis_Subsong3DisarkByteRegionEnd1

; The Linker.
Atlantis_Subsong3DisarkByteRegionStart2
; Pattern 0
Atlantis_Subsong3_Loop
	db 170	; State byte.
	db 63	; New height.
	db ((Atlantis_Subsong3_Track0 - ($ + 2)) & #ff00) / 256	; New track (0) for channel 1, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong3_Track0 - ($ + 1)) & 255)
	db ((Atlantis_Subsong3_Track1 - ($ + 2)) & #ff00) / 256	; New track (1) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong3_Track1 - ($ + 1)) & 255)
	db ((Atlantis_Subsong3_Track2 - ($ + 2)) & #ff00) / 256	; New track (2) for channel 3, as an offset. Offset MSB, then LSB.
	db ((Atlantis_Subsong3_Track2 - ($ + 1)) & 255)

	db 1	; End of the Song.
	db 0	; Speed to 0, meaning "end of song".
Atlantis_Subsong3DisarkByteRegionEnd2
Atlantis_Subsong3DisarkPointerRegionStart3
	dw Atlantis_Subsong3_Loop

Atlantis_Subsong3DisarkPointerRegionEnd3
; The indexes of the tracks.
Atlantis_Subsong3_TrackIndexes
Atlantis_Subsong3DisarkPointerRegionStart4
Atlantis_Subsong3DisarkPointerRegionEnd4

Atlantis_Subsong3DisarkByteRegionStart5
Atlantis_Subsong3_Track0
	db 254	; New instrument (1). New escaped note: 49. New wait (5).
	db 49	;   Escape note value.
	db 1	;   Escape instrument value.
	db 5	;   Escape wait value.
	db 143	; Same escaped note: 49. Secondary wait (1).
	db 207	; Same escaped note: 49. New wait (3).
	db 3	;   Escape wait value.
	db 15	; Same escaped note: 49. 
	db 206	; New escaped note: 51. New wait (5).
	db 51	;   Escape note value.
	db 5	;   Escape wait value.
	db 143	; Same escaped note: 51. Secondary wait (1).
	db 207	; Same escaped note: 51. New wait (3).
	db 3	;   Escape wait value.
	db 15	; Same escaped note: 51. 
	db 206	; New escaped note: 49. New wait (5).
	db 49	;   Escape note value.
	db 5	;   Escape wait value.
	db 143	; Same escaped note: 49. Secondary wait (1).
	db 207	; Same escaped note: 49. New wait (3).
	db 3	;   Escape wait value.
	db 15	; Same escaped note: 49. 
	db 206	; New escaped note: 51. New wait (7).
	db 51	;   Escape note value.
	db 7	;   Escape wait value.
	db 78	; New escaped note: 49. Primary wait (0).
	db 49	;   Escape note value.
	db 78	; New escaped note: 61. Primary wait (0).
	db 61	;   Escape note value.
	db 206	; New escaped note: 73. New wait (127).
	db 73	;   Escape note value.
	db 127	;   Escape wait value.

Atlantis_Subsong3_Track1
	db 207	; Same escaped note: 25. New wait (3).
	db 3	;   Escape wait value.
	db 62	; New instrument (3). New escaped note: 24. 
	db 24	;   Escape note value.
	db 3	;   Escape instrument value.
	db 190	; New instrument (5). New escaped note: 27. Secondary wait (1).
	db 27	;   Escape note value.
	db 5	;   Escape instrument value.
	db 174	; Secondary instrument (4). New escaped note: 24. Secondary wait (1).
	db 24	;   Escape note value.
	db 63	; New instrument (3). Same escaped note: 24. 
	db 3	;   Escape instrument value.
	db 62	; New instrument (5). New escaped note: 27. 
	db 27	;   Escape note value.
	db 5	;   Escape instrument value.
	db 190	; New instrument (3). New escaped note: 24. Secondary wait (1).
	db 24	;   Escape note value.
	db 3	;   Escape instrument value.
	db 175	; Secondary instrument (4). Same escaped note: 24. Secondary wait (1).
	db 62	; New instrument (5). New escaped note: 30. 
	db 30	;   Escape note value.
	db 5	;   Escape instrument value.
	db 62	; New instrument (3). New escaped note: 24. 
	db 24	;   Escape note value.
	db 3	;   Escape instrument value.
	db 190	; New instrument (5). New escaped note: 30. Secondary wait (1).
	db 30	;   Escape note value.
	db 5	;   Escape instrument value.
	db 174	; Secondary instrument (4). New escaped note: 24. Secondary wait (1).
	db 24	;   Escape note value.
	db 63	; New instrument (3). Same escaped note: 24. 
	db 3	;   Escape instrument value.
	db 62	; New instrument (5). New escaped note: 34. 
	db 34	;   Escape note value.
	db 5	;   Escape instrument value.
	db 190	; New instrument (3). New escaped note: 24. Secondary wait (1).
	db 24	;   Escape note value.
	db 3	;   Escape instrument value.
	db 175	; Secondary instrument (4). Same escaped note: 24. Secondary wait (1).
	db 254	; New instrument (5). New escaped note: 34. New wait (7).
	db 34	;   Escape note value.
	db 5	;   Escape instrument value.
	db 7	;   Escape wait value.
	db 142	; New escaped note: 37. Secondary wait (1).
	db 37	;   Escape note value.
	db 207	; Same escaped note: 37. New wait (127).
	db 127	;   Escape wait value.

Atlantis_Subsong3_Track2
	db 206	; New escaped note: 30. New wait (3).
	db 30	;   Escape note value.
	db 3	;   Escape wait value.
	db 15	; Same escaped note: 30. 
	db 14	; New escaped note: 34. 
	db 34	;   Escape note value.
	db 15	; Same escaped note: 34. 
	db 14	; New escaped note: 37. 
	db 37	;   Escape note value.
	db 15	; Same escaped note: 37. 
	db 14	; New escaped note: 39. 
	db 39	;   Escape note value.
	db 15	; Same escaped note: 39. 
	db 14	; New escaped note: 42. 
	db 42	;   Escape note value.
	db 15	; Same escaped note: 42. 
	db 14	; New escaped note: 46. 
	db 46	;   Escape note value.
	db 15	; Same escaped note: 46. 
	db 206	; New escaped note: 49. New wait (7).
	db 49	;   Escape note value.
	db 7	;   Escape wait value.
	db 142	; New escaped note: 58. Secondary wait (1).
	db 58	;   Escape note value.
	db 207	; Same escaped note: 58. New wait (127).
	db 127	;   Escape wait value.

Atlantis_Subsong3DisarkByteRegionEnd5
; The note indexes.
Atlantis_Subsong3_NoteIndexes
Atlantis_Subsong3DisarkByteRegionStart6
Atlantis_Subsong3DisarkByteRegionEnd6

