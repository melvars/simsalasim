#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/kvm.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <kvm.h>
#include <log.h>

static int kvm = 0, vm = 0;
static struct kvm_run *kvm_run;
static void *memory;
static size_t memory_size;

struct {
	pthread_t thread;
	enum { OFFLINE = 0, RUNNING, ONLINE } state;
	int vcpu;
} cpu[1] = { 0 };

static int kvm_slot = 0;
static int kvm_register(int flags, long guest_addr, long host_addr, size_t size)
{
	struct kvm_userspace_memory_region region;
	region.slot = kvm_slot++;
	region.flags = flags;
	region.guest_phys_addr = guest_addr;
	region.memory_size = size;
	region.userspace_addr = host_addr;

	if (ioctl(vm, KVM_SET_USER_MEMORY_REGION, &region) < 0) {
		errln("Couldn't set memory region: %s", strerror(errno));
		return -1;
	}
	return 0;
}

static void kvm_signal(int number)
{
	(void)number;
	logln("KVM signal");
}

static int kvm_init_cpu(int index)
{
	/**
	 * VCPU
	 */

	cpu[index].vcpu = ioctl(vm, KVM_CREATE_VCPU, index);
	if (cpu[index].vcpu < 0) {
		errln("Couldn't create VCPU%d: %s", index, strerror(errno));
		return -1;
	}

	/**
	 * KVM_RUN
	 */

	int mmap_size = ioctl(kvm, KVM_GET_VCPU_MMAP_SIZE, 0);
	if (mmap_size <= 0 || mmap_size & 0xfff) {
		errln("Invalid mmap_size: %d", mmap_size);
		return -1;
	}

	kvm_run = mmap(NULL, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, cpu[index].vcpu, 0);
	if (kvm_run == MAP_FAILED) {
		errln("Couldn't map kvm_run");
		return -1;
	}

	/**
	 * Registers
	 */

	struct kvm_regs regs = { 0 };
	struct kvm_sregs sregs = { 0 };

	// Get registers

	if (ioctl(cpu[index].vcpu, KVM_GET_REGS, &regs) < 0) {
		errln("Couldn't get REGS of VCPU%d", index);
		return -1;
	}

	if (ioctl(cpu[index].vcpu, KVM_GET_SREGS, &sregs) < 0) {
		errln("Couldn't get SREGS of VCPU%d", index);
		return -1;
	}

	// Modify registers accordingly

	regs.rflags = 2;
	regs.rip = 0;

	sregs.cs.selector = 0;
	sregs.cs.base = 0;

	// Set registers

	if (ioctl(cpu[index].vcpu, KVM_SET_REGS, &regs) < 0) {
		errln("Couldn't set REGS of VCPU%d", index);
		return -1;
	}

	if (ioctl(cpu[index].vcpu, KVM_SET_SREGS, &sregs) < 0) {
		errln("Couldn't set SREGS of VCPU%d", index);
		return -1;
	}

	cpu[index].state = ONLINE;
	return 0;
}

int kvm_init(void)
{
	kvm = open("/dev/kvm", O_RDWR);
	if (kvm < 0) {
		errln("KVM is required: %s", strerror(errno));
		return -1;
	}

	/**
	 * Verify and create VM
	 */

	int version = ioctl(kvm, KVM_GET_API_VERSION, 0);
	if (version != KVM_API_VERSION) {
		errln("Invalid KVM version: %d", version);
		return -1;
	}

	vm = ioctl(kvm, KVM_CREATE_VM, 0);
	if (vm < 0) {
		errln("Couldn't create VM: %s", strerror(errno));
		return -1;
	}

	// Verify max CPU count

	int max_cpus = ioctl(kvm, KVM_CHECK_EXTENSION, KVM_CAP_MAX_VCPUS);
	if (max_cpus < 0) {
		errln("Couldn't get CPU count: %s", strerror(errno));
		return -1;
	}
	assert((unsigned long)max_cpus >= sizeof(cpu) / sizeof(cpu[0]));

	/**
	 * Memory
	 */

	memory_size = 10 << 20;
	memory = mmap(NULL, memory_size, PROT_READ | PROT_WRITE,
		      MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
	if (memory == MAP_FAILED) {
		errln("Couldn't map memory");
		return -1;
	}

	if (kvm_register(0, 0, (long)memory, memory_size) < 0)
		return -1;

	/**
	 * CPUs
	 */

	for (unsigned long i = 0; i < sizeof(cpu) / sizeof(cpu[0]); i++)
		if (kvm_init_cpu(i) < 0)
			return -1;

	/**
	 * Okay!
	 */

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = kvm_signal;
	sigaction(SIGALRM, &sa, NULL);

	return 0;
}

int kvm_load(long addr, long data, int size)
{
	memcpy((char *)memory + addr, (void *)data, size);
	return 0;
}

static void *kvm_loop(void *arg)
{
	long index = (long)arg;
	assert(cpu[index].state == ONLINE);

	while (cpu[index].state = RUNNING, ioctl(cpu[index].vcpu, KVM_RUN, 0) >= 0) {
		cpu[index].state = ONLINE;

		switch (kvm_run->exit_reason) {
		case KVM_EXIT_HLT:
			logln("Halting CPU");
			goto done;
		case KVM_EXIT_FAIL_ENTRY:
			errln("CPU failure");
			goto done;
		case KVM_EXIT_IO:
			logln("IO");
			break;
		case KVM_EXIT_MMIO:
		case KVM_EXIT_IRQ_WINDOW_OPEN:
		default:
			errln("Unknown KVM exit reason %d", kvm_run->exit_reason);
			goto done;
		}
	}

	errln("CPU can't be run: %s", strerror(errno));
done:
	cpu[index].state = OFFLINE;

	logln("Done");
	pthread_exit(NULL);
}

static void *kvm_observe(void *arg)
{
	long index = (long)arg;

#define R(n) ((int)(regs.r##n)) // TODO: Adjust per arch?
	while (1) {
		if (cpu[index].state == OFFLINE)
			break;

		struct kvm_regs regs = { 0 };
		if (ioctl(cpu[index].vcpu, KVM_GET_REGS, &regs) < 0) {
			errln("Couldn't get KVM regs: %s", strerror(errno));
			break;
		}

		struct kvm_sregs sregs = { 0 };
		if (ioctl(cpu[index].vcpu, KVM_GET_SREGS, &sregs) < 0) {
			errln("Couldn't get KVM sregs: %s", strerror(errno));
			break;
		}

		logln("EAX: %08x ECX: %08x EDX: %08x EBX: %08x", R(ax), R(cx), R(dx), R(bx));
		logln("ESP: %08x EBP: %08x ESI: %08x EDI: %08x", R(ax), R(cx), R(dx), R(bx));
		logln("EFLAGS: %08x EIP: %08x", R(flags), R(ip));

		logln("ES.sel=%04x ES.base=%08x, ES.lim=%08x", sregs.es.selector, sregs.es.base,
		      sregs.es.limit);
		logln("CS.sel=%04x CS.base=%08x, CS.lim=%08x", sregs.cs.selector, sregs.cs.base,
		      sregs.cs.limit);
		logln("SS.sel=%04x SS.base=%08x, SS.lim=%08x", sregs.ss.selector, sregs.ss.base,
		      sregs.ss.limit);
		logln("DS.sel=%04x DS.base=%08x, DS.lim=%08x", sregs.ds.selector, sregs.ds.base,
		      sregs.ds.limit);
		logln("FS.sel=%04x FS.base=%08x, FS.lim=%08x", sregs.fs.selector, sregs.fs.base,
		      sregs.fs.limit);
		logln("GS.sel=%04x GS.base=%08x, GS.lim=%08x", sregs.gs.selector, sregs.gs.base,
		      sregs.gs.limit);
		logln("CR0: %08x CR2: %08x CR3: %08x CR4: %08x", sregs.cr0, sregs.cr2, sregs.cr3,
		      sregs.cr4);

		logln("GDT.base=%08x GDT.limit=%08x", sregs.gdt.base, sregs.gdt.limit);
		logln("LDT.base=%08x LDT.limit=%08x", sregs.ldt.base, sregs.ldt.limit);
		logln("IDT.base=%08x IDT.limit=%08x", sregs.idt.base, sregs.idt.limit);
		logln("TR.base =%08x TR.limit =%08x", sregs.tr.base, sregs.tr.limit);

		struct timespec req = { 0 }, rem = { 0 };
		req.tv_sec = 1;
		int slept = nanosleep(&req, &rem);
		assert(slept >= 0);
	}

	logln("Done");
	pthread_exit(NULL);
}

static void *kvm_exec_cpu(void *arg)
{
	pthread_attr_t attr = { 0 };
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	int err;

	pthread_t observee = { 0 };
	err = pthread_create(&observee, &attr, kvm_loop, arg);
	if (err) {
		errln("Couldn't create observee thread: %d", err);
		pthread_exit(NULL);
	}

	pthread_t observer = { 0 };
	err = pthread_create(&observer, &attr, kvm_observe, arg);
	if (err) {
		errln("Couldn't create observer thread: %d", err);
		pthread_exit(NULL);
	}

	pthread_attr_destroy(&attr);

	logln("Done");
	pthread_exit(NULL);
}

void kvm_exec(void)
{
	for (unsigned long i = 0; i < sizeof(cpu) / sizeof(cpu[0]); i++) {
		if (cpu[i].state == ONLINE) {
			logln("Booting CPU %d", i);

			pthread_attr_t attr = { 0 };
			pthread_attr_init(&attr);
			pthread_create(&cpu[i].thread, &attr, kvm_exec_cpu, (void *)i);
			pthread_attr_destroy(&attr);
		}
	}

	// Wait until all CPUs are finished
	for (unsigned long i = 0; i < sizeof(cpu) / sizeof(cpu[0]); i++)
		pthread_join(cpu[i].thread, NULL);

	kvm_destroy();
}

void kvm_destroy(void)
{
	for (unsigned long i = 0; i < sizeof(cpu) / sizeof(cpu[0]); i++)
		if (cpu[i].vcpu)
			close(cpu[i].vcpu);

	if (vm)
		close(vm);
	if (kvm)
		close(kvm);

	logln("Done");
}
