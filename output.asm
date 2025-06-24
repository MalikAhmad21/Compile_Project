section .data
a: dq 0
b: dq 0
c: dq 0
buf: times 32 db 0
newline: db 0xA
section .text
global _start
_start:
    mov rax, 5
    mov [a], rax
    mov rax, 10
    mov [b], rax
    mov rax, [a]
    cmp rax, 0
    je else_block
    mov rax, [a]
    mov rbx, [b]
    imul rax, rbx
    mov [c], rax
    jmp end_if
else_block:
    mov rax, [b]
    mov rbx, [a]
    sub rax, rbx
    mov [c], rax
end_if:
    mov rax, [c]
    mov rcx, buf
    add rcx, 31
    mov byte [rcx], 0
print_loop:
    xor rdx, rdx
    mov rbx, 10
    div rbx
    add rdx, '0'
    dec rcx
    mov [rcx], dl
    test rax, rax
    jnz print_loop
    mov rax, 1
    mov rdi, 1
    mov rsi, rcx
    mov rdx, 32
    syscall
    mov rax, 1
    mov rdi, 1
    mov rsi, newline
    mov rdx, 1
    syscall
    mov rax, 60
    xor rdi, rdi
    syscall
