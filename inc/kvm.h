#ifndef KVM_H
#define KVM_H

int kvm_init(void);
int kvm_load(long addr, long data, int size);
void kvm_exec(void);
void kvm_destroy(void);

#endif
