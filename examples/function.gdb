shell echo -e '\033[32mWhen this occurs, hit enter to continue:\033[0m'
shell echo -e '\033[32m...' && read

shell echo -e '\033[32mRight, here we go. Forwarding to test0.. \033[0m'
    break test0
    run
    stepi 1

shell echo -e '\033[32m\nCheck out the assembler:\033[0m'
    disas
shell echo -e '\033[32mThis is our function test0. It is a simple f(a,b) = a+b\033[0m'
shell echo
shell echo -e '\033[32mIn the lines right above the arrow, we moved the %rsp down the\033[0m'
shell echo -e '\033[32mstack by 4 (remember the stack extends top to bottom), which effectively\033[0m'
shell echo -e '\033[32mgives us a local stack space of four bytes.\033[0m'
shell echo -e '\033[32m...' && read

shell echo -e '\033[32mOk, here are some interesting bits:\033[0m'
shell echo -e '\033[32mThis is %rbp: \033[0m'
    x/1xg $rbp
shell echo -e '\033[32mIt is the memory address the stack pointer pointed at when\033[0m'
shell echo -e '\033[32mtest0 was called. We need to remember it, because the\033[0m'
shell echo -e '\033[32mstack pointer may change when we push and pop registers.\033[0m'
shell echo
shell echo -e '\033[32mNote how the addresses of all following variables are relative\033[0m'
shell echo -e '\033[32mto %rbp, which stays constant until the end of this function.\033[0m'
shell echo -e '\033[32m...' && read

shell echo -e '\033[32mNote also that %rbp = %rsp-4.\033[0m'
    x/1xg $rsp
shell echo -e '\033[32m...' && read

shell echo -e '\033[32mThis is %rbp+8:\033[0m'
    x/1xg $rbp+8
shell echo -e '\033[32mIt is the address right after the call instruction to this\033[0m'
shell echo -e '\033[32mfunction, to where we jump back later using the return function.\033[0m'
shell echo -e '\033[32m...' && read

shell echo -e '\033[32mThis is %rbp+16:\033[0m'
    x/1xg $rbp+16
shell echo -e '\033[32mIt is the first parameter of our test0 function\033[0m'
shell echo -e '\033[32m...' && read

shell echo -e '\033[32mThis is %rbp+24: parameter 2\033[0m'
    x/1xg $rbp+24
shell echo -e '\033[32mIt is the second parameter of our test0 function\033[0m'
shell echo -e '\033[32m... \033[0m' && read

shell echo -e '\033[32mAt %rbp+20, there is only the upper half of the second parameter:\033[0m'
    x/1xw $rbp+20
shell echo -e '\033[32mSince we pushq all our parameters, even uint32 is stored in 8 bytes.\033[0m'
shell echo -e '\033[32m... \033[0m' && read

shell echo -e '\033[32mEnough with the vars. Here is the code again:\033[0m'
    disas
shell echo -e '\033[32m... \033[0m' && read

shell echo -e '\033[32mFirst, we push %rbp on the stack and load the address\033[0m'
shell echo -e '\033[32mof -4(%rbp) into it:\033[0m'
    stepi 3
shell echo -e '\033[32mAddress of %rbp-4:\033[0m'
    x/1xw $rbp-4
shell echo -e '\033[32mNew content of %rbx:\033[0m'
    info registers rbx
shell echo -e '...' && read

shell echo -e '\033[32mNow we load the two parameters into %eax and %ebx:\033[0m'
    stepi 4
shell echo -e '\033[32m%eax:\033[0m'
    info registers eax
shell echo -e '\033[32m%ebx:\033[0m'
    info registers ebx
shell echo -e '\033[32m... \033[0m' && read

shell echo -e '\033[32mAnd add them together..\033[0m'
    disas $rip,+1
    stepi 2
shell echo -e '\033[32mStoring the result in %eax again:\033[0m'
    info registers eax
shell echo -e '\033[32m... \033[0m' && read

shell echo -e '\033[32mNow we move the content of %eax, to the location we\033[0m'
shell echo -e '\033[32mstored before in %rbx from the lea.\033[0m'
shell echo -e '\033[32mNote the parantheses on the right hand side of the mov!\033[0m'
    disas $rip,+1
    stepi 2
shell echo -e '\033[32m... \033[0m' && read

shell echo -e '\033[32mAt this point, the result of our addition should be in\033[0m'
shell echo -e '\033[32mmemory at address -4(%rbp):\033[0m'
    x/1xw $rbp-4
shell echo -e '\033[32m... \033[0m' && read

shell echo -e '\033[32mNow all we do is write that variable back to %eax,\033[0m'
shell echo -e '\033[32mwhich as always is our return value:\033[0m'
    disas $rip,+1
    stepi
shell echo -e '\033[32m... \033[0m' && read

shell echo -e '\033[32mSome cleanup, leave resets %rsp to %rbp for us:\033[0m'
    disas $rip,+2
shell echo -e '\033[32mAfter which on top of the stack ($rsp) we should have\033[0m'
shell echo -e '\033[32mthe return address:\033[0m'
    stepi
    x/1xg $rsp
shell echo -e '\033[32m... \033[0m' && read

shell echo -e '\033[32mRet pops the top of the stack into the instruction pointer,\033[0m'
shell echo -e '\033[32mand we are back in our main function.\033[0m'
    stepi
    disas
shell echo -e '\033[32m... \033[0m' && read

shell echo -e '\033[32mAt this point we have the returned result of test0(5 6) in\033[0m'
shell echo -e '\033[32m%eax, and might do additional stuff with it.\033[0m'
shell echo -e '\033[32m... \033[0m' && read

shell echo -e '\033[32mHowever, we do not do that so it stays as return value of our program:\033[0m'
    continue
shell echo -e '\033[32m... \033[0m' && read

shell echo -e '\033[32m(the 0XX is an octal value, so the result fits!).\033[0m'
shell echo -e '\033[32mAnd that is it.\033[0m'
shell echo -e '\033[32m... \033[0m' && read
