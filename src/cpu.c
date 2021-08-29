#include <errno.h>
#include <fcntl.h>
#include <linux/kvm.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cpu.h>
#include <log.h>

static int kvm, vm, vcpu;
static struct kvm_run *kvm_run;

int cpu_init(void)
{
	kvm = open("/dev/kvm", O_RDWR);
	if (kvm < 0) {
		errln("KVM is required: %s", strerror(errno));
		return -1;
	}

	int version = ioctl(kvm, KVM_GET_API_VERSION, 0);
	if (version != KVM_API_VERSION) {
		close(kvm);
		errln("Invalid KVM version: %d", version);
		return -1;
	}

	vm = ioctl(kvm, KVM_CREATE_VM, 0);
	if (vm < 0) {
		close(kvm);
		errln("Couldn't create VM: %s", strerror(errno));
		return -1;
	}

	vcpu = ioctl(vm, KVM_CREATE_VCPU, 0);
	if (vcpu < 0) {
		close(vm);
		close(kvm);
		errln("Couldn't create VCPU: %s", strerror(errno));
		return -1;
	}

	int mmap_sz = ioctl(kvm, KVM_GET_VCPU_MMAP_SIZE, 0);
	if (mmap_sz <= 0 || mmap_sz & 0xfff) {
		close(vcpu);
		close(vm);
		close(kvm);
		errln("Invalid mmap_sz: %d", mmap_sz);
		return -1;
	}

	kvm_run = mmap(NULL, mmap_sz, PROT_READ | PROT_WRITE, MAP_SHARED, vcpu, 0);
	if (kvm_run == MAP_FAILED) {
		close(vcpu);
		close(vm);
		close(kvm);
		errln("Couldn't map kvm_run");
		return -1;
	}

	logln("Done");
	return 0;
}

void cpu_destroy(void)
{
	close(vcpu);
	close(vm);
	close(kvm);
	logln("Done");
}

void cpu_run(void)
{
	int run = ioctl(vcpu, KVM_RUN, 0);
	if (run < 0) {
		errln("CPU can't be run: %s", strerror(errno));
		return;
	}
}
