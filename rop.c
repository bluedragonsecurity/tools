//rop.c - vulner : kernel stack overflow
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/slab.h>

#define DEVICE_NAME "vuln_device"

static struct class* vunl_class    = NULL;
static struct device* vunl_device  = NULL;
static int major;

static char *vunl_devnode(const struct device *dev, umode_t *mode) {
    if (mode) *mode = 0666; 
    return NULL;
}

__attribute__((optimize(0)))
static ssize_t device_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
    char buffer[64]; 

    if (_copy_from_user(buffer, buf, count)) {
        return -EFAULT;
    }

    return count;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .write = device_write,
};

static int __init vuln_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) return major;

    vunl_class = class_create(DEVICE_NAME);
    if (IS_ERR(vunl_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(vunl_class);
    }
    
    vunl_class->devnode = vunl_devnode;
    vunl_device = device_create(vunl_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    
    printk(KERN_INFO "[+] %s loaded with major %d\n", DEVICE_NAME, major);
    return 0;
}

static void __exit vuln_exit(void) {
    device_destroy(vunl_class, MKDEV(major, 0));
    class_destroy(vunl_class);
    unregister_chrdev(major, DEVICE_NAME);
}

module_init(vuln_init);
module_exit(vuln_exit);
MODULE_LICENSE("GPL");
