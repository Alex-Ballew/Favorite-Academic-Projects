.global main

            .text

main:       
            mov 8(%rsi), %rdi
            mov %rdi, %r8     #setting up command line accept
            call atoi
        
            mov %eax, %r13d

            mov $0, %r9d    #store 0 in register 9 like var
top2:           
            cmp %r13d, %r9d   #check if loops enough
            je bottom
            inc     %r9d

            mov $0, %r10d            # set up inner loop

top:        cmp %r9d, %r10d
            je bottom1
                # prints message
            mov     $1, %rdi            
            mov     $message, %rsi      
            mov     $1, %rdx           

            mov     $1, %rax            # rax holds system call id
            syscall                     # call into OS

            inc     %r10d    #restart loop
            jmp     top
            
bottom1:    #print new message
            mov     $1, %rdi            # arg 0 holds stdout
            mov     $message2, %rsi      # arg 1 holds address of message
            mov     $1, %rdx           # arg 2 holds # of bytes
            syscall
            je top2

bottom:           #print message
            mov     $1, %rdi         # 1 is fd for stdout
            mov     $message3, %rsi   # address of message
            mov     $7, %rdx        # write 13 bytes
            mov     $1,%rax 
            syscall

                #exit
            mov       $60, %rax               # system call for exit
            mov       $0, %rdi                # exit code 0
            syscall
            ret

            .data
message3:   .ascii  "Finito\n"
message:    .ascii  "*"
message2:   .ascii  "\n"

