#include <stdio.h>
#include "codegen.h"
#include "ast.h"

void generateCode(ASTNode* node) {
    FILE* out = fopen("output.asm", "w");
    if (!out) {
        printf("Error: Cannot open output.asm\n");
        return;
    }

   
    fprintf(out, "section .data\n");
    fprintf(out, "a: dq 0\n");
    fprintf(out, "b: dq 0\n");
    fprintf(out, "c: dq 0\n");
    fprintf(out, "buf: times 32 db 0\n"); 
    fprintf(out, "newline: db 0xA\n");

    
    fprintf(out, "section .text\n");
    fprintf(out, "global _start\n");
    fprintf(out, "_start:\n");

    
    fprintf(out, "    mov rax, 5\n");
    fprintf(out, "    mov [a], rax\n");

    
    fprintf(out, "    mov rax, 10\n");
    fprintf(out, "    mov [b], rax\n");

    
    fprintf(out, "    mov rax, [a]\n");
    fprintf(out, "    cmp rax, 0\n");
    fprintf(out, "    je else_block\n");


    fprintf(out, "    mov rax, [a]\n");
    fprintf(out, "    mov rbx, [b]\n");
    fprintf(out, "    imul rax, rbx\n");
    fprintf(out, "    mov [c], rax\n");
    fprintf(out, "    jmp end_if\n");

    
    fprintf(out, "else_block:\n");
    fprintf(out, "    mov rax, [b]\n");
    fprintf(out, "    mov rbx, [a]\n");
    fprintf(out, "    sub rax, rbx\n");
    fprintf(out, "    mov [c], rax\n");

    
    fprintf(out, "end_if:\n");
    fprintf(out, "    mov rax, [c]\n");           
    fprintf(out, "    mov rcx, buf\n");           
    fprintf(out, "    add rcx, 31\n");            
    fprintf(out, "    mov byte [rcx], 0\n");      

    fprintf(out, "print_loop:\n");
    fprintf(out, "    xor rdx, rdx\n");
    fprintf(out, "    mov rbx, 10\n");
    fprintf(out, "    div rbx\n");               
    fprintf(out, "    add rdx, '0'\n");           
    fprintf(out, "    dec rcx\n");
    fprintf(out, "    mov [rcx], dl\n");         
    fprintf(out, "    test rax, rax\n");
    fprintf(out, "    jnz print_loop\n");


    fprintf(out, "    mov rax, 1\n");            
    fprintf(out, "    mov rdi, 1\n");            
    fprintf(out, "    mov rsi, rcx\n");           
    fprintf(out, "    mov rdx, 32\n");            
    fprintf(out, "    syscall\n");

    
    fprintf(out, "    mov rax, 1\n");
    fprintf(out, "    mov rdi, 1\n");
    fprintf(out, "    mov rsi, newline\n");
    fprintf(out, "    mov rdx, 1\n");
    fprintf(out, "    syscall\n");

    
    fprintf(out, "    mov rax, 60\n");
    fprintf(out, "    xor rdi, rdi\n");
    fprintf(out, "    syscall\n");

    fclose(out);
    printf(" Real assembly written to output.asm (with print for c)\n");
}
