MAIN:    mov @r3  ,LENGTH2
.entry LOOP
LOOP:    jmp L1 
LOOP:    jmp L1 
.extern hello  
         mcro m1
         inc @r2
         mov A,@r1
         endmcro    
         m1
         prn -509
         bne LOOP1
         sub @r1,    	@r4
         jmp L1
         mov 12,@r1
L1:      inc K
.extern hello  
.extern mov
         bne LOOP
         mcro m2
         aaaaaa
         bbbbbbb
         endmcro   
         m2 
         LOOP  
END:     stop
STR:    .string "abcdef"
LENGTH: .data 6000,-9,-25
LENGTH: .data 6000, 90
STR1:    .string "shoval"  
STR1: 	 .string "shmuel"
STR2:	.string "oop" oop
K:      .data 22