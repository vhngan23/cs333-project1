#include <linux/module.h> 	//defines macros (eg.module_init,...)
#include <linux/fs.h>		//defines register and unregister functions for device number
#include <linux/device.h> 	//defines functions for creating device file
#include <linux/kdev_t.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/random.h>
#include <linux/uaccess.h>
#include "Project1.h"
#include <linux/slab.h>


static struct cdev c_dev; // global variable for the character device 
static struct class *cl; // global variable for the device class
static char RandNum;

typedef struct vchar_dev{
	unsigned char *control_regs;
	unsigned char *status_regs;
	unsigned char *data_regs;
} vchar_dev_t;

struct _vchar_drv
{
	vchar_dev_t *chardrv_hw;
	struct cdev *vcdev;
	unsigned int open_cnt;
} vchar_drv;



/***************************** DEVICE SPECIFIC: START **************************/
/* INITIALIZE THE DEVICE */
int chardrv_hw_init(vchar_dev_t *hw)
{
	char * buf;
	buf = kzalloc(NUM_DEV_REGS * REG_SIZE, GFP_KERNEL);
	if (!buf) {	return -ENOMEM; 	}
	
	hw->control_regs = buf;
	hw->status_regs = hw->control_regs + NUM_CTRL_REGS;
	hw->data_regs = hw->status_regs + NUM_STS_REGS;
	
	hw->control_regs[CONTROL_ACCESS_REG] = 0x03;
	hw->status_regs[DEVICE_STATUS_REG] = 0x03;
	
	return 0;
}

/* RELEASE THE DEVICE */
void chardrv_hw_exit(vchar_dev_t *hw)
{
	kfree(hw->control_regs);
}

/***************************** DEVICE SPECIFIC: END **************************/




/***************************** OS SPECIFIC: START **************************/


static dev_t first; // Global variable for the first device number

/* ENTRY POINT FUNCTIONS */

static int my_open(struct inode *inode, struct file *f){
	vchar_drv.open_cnt++;
	printk("Handle opened event (%d)\n", vchar_drv.open_cnt);
	return 0;
}

static int my_release(struct inode *inode, struct file *f){
	printk("Handle closed event\n");
	return 0;
}


static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
 	printk(KERN_INFO "Driver: read()\n");

 	get_random_bytes(&RandNum, sizeof(char));
 	printk(KERN_INFO "Random number is %d\n", RandNum);
 	if(off > 0)
        	return 0;
 	if (copy_to_user(buf, &RandNum, 1))
 		return -EFAULT;
 	off++;
 	return 1;
}

static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
 	printk(KERN_INFO "Driver: write()\n"); 
 	return 0;
}
 	
 	
static struct file_operations pugs_fops =
{
 	.owner = THIS_MODULE,
 	.open = my_open,
 	.read = my_read,
 	.write = my_write,
 	.release = my_release,
};

/* CHARACTER DRIVER CONSTRUCTOR */
static int __init chardrv_init(void) 
{
 	printk(KERN_INFO "Thi, Ngan, Trong: character driver registered\n");
 
 	/* Register device number */
 	if (alloc_chrdev_region(&first, 0, 3, "Random Number Generator") < 0)
 	{	
 		printk("Failed to register device number\n");
 		return -1;
 	}
 
 	/* Create device file */
 	if ((cl = class_create(THIS_MODULE, "Project1_class")) == NULL)
 	{
 		printk("Failed to create class file\n");
 		unregister_chrdev_region(first, 1);
 		return -1;
 	}
 	if (device_create(cl, NULL, first, NULL, "Project1_device") == NULL)
 	{
 		printk("Failed to create device file\n");
 		class_destroy(cl);
 		unregister_chrdev_region(first, 1);
  		return -1;
 	}
 	
 	/* Allocate data structure of driver */
 	vchar_drv.chardrv_hw = kzalloc(sizeof(vchar_dev_t), GFP_KERNEL);
 	if (!vchar_drv.chardrv_hw)
 	{
 		printk("Failed to allocate data structure of driver\n");
 		device_destroy(cl, first);
 		return -1;
 	}
 	
 	/* Initialize a virtual character device */
 	if (chardrv_hw_init(vchar_drv.chardrv_hw) < 0)
 	{
 		printk("Failed to initialize a virtual character device\n");
 		kfree(vchar_drv.chardrv_hw);
 		return -1;
 	}
 	
 	/* Register entry points with kernel */
	vchar_drv.vcdev = cdev_alloc();
	if(vchar_drv.vcdev == NULL){
		printk("Failed to allocate cdev structure\n");
		device_destroy(cl, first);
 		return -1;
	}
	
	cdev_init(&c_dev, &pugs_fops);
	
 	if (cdev_add(&c_dev, first, 1) == -1)
 	{
 		printk("Failed to add a character device to the system\n");
  		device_destroy(cl, first);
  		class_destroy(cl);
  		unregister_chrdev_region(first, 1);
  		return -1;
 	}
		
 

 	printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(first), MINOR(first));
 	return 0;
}


/* CHARACTER DRIVER DESTRUCTOR */
static void __exit chardrv_exit(void) 
{
	/* Unregister entry point with kernel */
	cdev_del(&c_dev);

	/* Release device */
	chardrv_hw_exit(vchar_drv.chardrv_hw);
	
	/* Release allocated memory */
	kfree(vchar_drv.chardrv_hw);
	
	/* Destroy device file */
	device_destroy(cl, first);
 	class_destroy(cl);
 	
	/* Destroy device number */
 	unregister_chrdev_region(first, 3);
 	
 	printk(KERN_INFO "Thi, Ngan, Trong: character driver unregistered\n");
}

/***************************** OS SPECIFIC: END **************************/

module_init(chardrv_init);
module_exit(chardrv_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thi Nguyen, Ngan Vo, Trong Tran");
MODULE_DESCRIPTION("A kernel module to generate random number");
