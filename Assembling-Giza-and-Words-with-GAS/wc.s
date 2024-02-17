        .global main

        .text
main:                                   # This is called by C library's startup code
        mov     $0, %r15d  # tracker
        mov     $-1, %r12d   # check for end of file
        
main_loop: #loop through file

        
        push    %r14
        call    skipws  
        pop     %r14

        cmp     %r12d, %eax
        je end

        push    %r14
        call    readword   
        pop     %r14

        cmp     %r12d, %eax
        je end         #check for end of file
        inc     %r15d   # increment tracker
                

        cmp     %r12d, %eax
        je end    # check for end of file
         

        jmp      main_loop   #restart loop
        ret        
        
end:    mov     %r15d, %edi
        push    %r14
        call    print_result
        pop     %r14

                
        mov     $0, %eax

        ret


# The print_result function takes one arg in %edi, and prints the
# result as "Result=xxx" on standard output using printf.
print_result:
        push    %rbp
        mov     %rsp, %rbp
        mov     %edi, %esi       
        mov     $format, %rdi        
        call    printf
        pop     %rbp

        ret

# The isspace function takes one arg in %edi, and returns 1 in %eax if
# if the arg is a whitespace, or 0 in %eax if the arg is not a whitespace
isspace:
        # arg1 in %edi, compare it to space (32), newline (10),
        # carriage return (13) and tab (9)
        cmp     $32, %edi
        je confirm
        
        cmp     $10, %edi
        je confirm

        cmp     $13, %edi
        je confirm

        cmp     $9, %edi
        je confirm

        mov     $0, %eax
        ret

confirm:
        mov     $1, %eax
        ret


# The skipws function reads from standard input, until it sees a non
# space character or end of file. It returns -1 on EOF or 0 otherwise.
skipws:

skipws_top:
        # read a character, the result is in %eax
    	push 	%rbp
    	mov   	%rsp, %rbp
    	call	getchar
        pop     %rbp

        # check if getchar returned EOF
    	cmp    	$-1, %eax
        jne	    skipws_checkspace

        # We got an EOF, so return -1 in %eax
        mov	    $-1, %eax
        ret

skipws_checkspace:
        # Check whether the character we read was a space
        push    %rbp
        mov     %rsp, %rbp
        movl	%eax, %edi      
        call	isspace
        pop     %rbp

        cmp 	$1, %eax
        je	    skipws_top       

        mov 	$0, %eax      
    	ret




# The readword function reads from standard input, until it sees a white
# space character or end of file. It returns -1 on EOF or 0 otherwise.
readword:
readword_top:
        # read a character, the result is in %eax
    	push 	%rbp
    	mov   	%rsp, %rbp
    	call	getchar
        pop     %rbp

        # check if getchar returned EOF
    	cmp    	$-1, %eax
        jne	    readword_checkspace

        # We got an EOF, so return -1 in %eax
        mov	    $-1, %eax
        ret

readword_checkspace:
        # Check whether the character we read was a space
        push    %rbp
        mov     %rsp, %rbp
        movl	%eax, %edi       # the char read was in %eax
        call	isspace
        pop     %rbp

        cmp 	$0, %eax
        je	    readword_top       # was a space, so we keep looping

        mov 	$1, %eax         # not a space, so we return from the function
        ret
format:
        .asciz  "Result=%d\n"
