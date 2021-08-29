xor dx, dx

test:
in ax, 0x60
mov bx, 42

mov cx, 1
add dx, cx
cmp dx, 255
jne test

hlt
