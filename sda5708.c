#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/ctype.h>
#include "font5x7.h"

#define LOAD  7
#define DATA  8
#define SDCLK 18
#define RESET 23

static dev_t sda5708_dev_number=MKDEV(247,0);
static struct cdev *driver_object;
static struct class *sda5708_class;
static struct device *sda5708_dev;
static char textbuffer[8];

static void sda5708_write(int value);

struct timespec ts;
static int gpio_request_output(int nr)
{
	char gpio_name[12];
	int err;
	snprintf(gpio_name, sizeof(gpio_name), "rpi-gpio-%d", nr);
	err = gpio_request(nr, gpio_name);
	if(err){
		printk("gpio_request for %s failed %d\n", gpio_name, err);
		return -1;
	}
	err = gpio_direction_output(nr, 0);
	if(err){
		printk("gpio_direction_output for %s  failed %d\n", gpio_name, err);
		gpio_free(nr);
		return -1;
	}	
	return 0;
}

static void sda5708_wr_char(int pos, int value)
{
	int val = (value-32)*5;
	int d0 = Font5x7[val];
	int d1 = Font5x7[val+1];
	int d2 = Font5x7[val+2];
	int d3 = Font5x7[val+3];
	int d4 = Font5x7[val+4];
	int i;

	sda5708_write(0xa0+pos);
	for(i=7;i>0;i--)
	{
		sda5708_write(((d0&0x01)<<4)+((d1&0x01)<<3)+((d2&0x01)<<2)+((d3&0x01)<<1)+(d4&0x01));
		d0=d0>>1;
		d1=d1>>1;
		d2=d2>>1;
		d3=d3>>1;
		d4=d4>>1;
	}
}

static int sda5708_init(void)
{
	printk("sda5708_init!\n");
	if(gpio_request_output(LOAD)==-1)  return -EIO;
	if(gpio_request_output(DATA)==-1)  goto freeLOAD;
	if(gpio_request_output(SDCLK)==-1) goto freeDATA;
	if(gpio_request_output(RESET)==-1) goto freeSDCLK;

	gpio_set_value( RESET, 0);
	gpio_set_value( RESET, 0);
	gpio_set_value( RESET, 1);
	sda5708_write(0xe2); //Controll Register: Normal Operation and 40% Brightness

	sda5708_wr_char(0, 70); // F
	sda5708_wr_char(1, 101);// e
	
	return 0;
freeSDCLK: gpio_free(SDCLK);
freeDATA:  gpio_free(DATA);
freeLOAD:  gpio_free(LOAD);
return -EIO;
}

static int sda5708_exit(void)
{
	printk("sda5708_exit!\n");
	gpio_set_value( RESET, 0);
	gpio_free(RESET);
	gpio_free(SDCLK);
	gpio_free(DATA);
	gpio_free(LOAD);
	return 0;
}

static void sda5708_write(int value)
{
	int i;
	gpio_set_value(SDCLK, 0);
	gpio_set_value(LOAD, 0);
	
	for(i=8;i>0;i--)
	{
		gpio_set_value(DATA, value&0x1);
		gpio_set_value(SDCLK, 1);
		gpio_set_value(SDCLK, 0);
		value = value >> 1;
	}

	gpio_set_value(SDCLK, 0);
	gpio_set_value(LOAD, 1);
}

static ssize_t driver_write(struct file *instanz, const char __user *user, size_t
count, loff_t *offset)
{
	unsigned long not_copied;
	unsigned long to_copy;
	int i;

	to_copy = min(count, sizeof(textbuffer));
	not_copied = copy_from_user(textbuffer, user, to_copy);
	
	printk("driver_write(%s) \n",textbuffer);

	for(i=0;i<to_copy && textbuffer[i]; i++)
	{
		if(isprint(textbuffer[i]))
			sda5708_wr_char(i,textbuffer[i]);
	}
	return ((ssize_t) (to_copy-not_copied));
}

static struct file_operations fops = {
	.owner=THIS_MODULE,
	.write=driver_write,
};

static int __init mod_init(void)
{
	if(register_chrdev_region(sda5708_dev_number, 1, "SDA-5708")<0){
//	if(alloc_chrdev_region(&sda5708_dev_number, 0, 1, "SDA-5708")<0){
		printk("devicenumber (252,0) in use!!!\n");
		return -EIO;
	}

	driver_object=cdev_alloc();
	if(driver_object==NULL)
		goto free_device_number;
	driver_object->owner=THIS_MODULE;
	driver_object->ops = &fops;
	if( cdev_add(driver_object, sda5708_dev_number, 1) )
		goto free_cdev;
	
	sda5708_class = class_create( THIS_MODULE, "SDA-5708" );
	if( IS_ERR( sda5708_class) ){
		pr_err( "sda5708: no udev support\n" );
		goto free_cdev;
	}
	sda5708_dev=device_create(sda5708_class, NULL, sda5708_dev_number, NULL, "%s", "sda5708");
	dev_info( sda5708_dev, "mod_init called\n");
	if(sda5708_init()==0)
		return 0;
free_cdev:
	kobject_put( &driver_object->kobj);
free_device_number:
	unregister_chrdev_region( sda5708_dev_number, 1);
	printk("mod_init failed\n");
	return -EIO;
}

static void __exit mod_exit(void)
{
	dev_info( sda5708_dev, "mod_exit called\n");
	sda5708_exit();
	device_destroy(sda5708_class, sda5708_dev_number);
	class_destroy( sda5708_class);
	cdev_del( driver_object );
	unregister_chrdev_region( sda5708_dev_number, 1);
	return;
}

module_init(mod_init);
module_exit(mod_exit);
MODULE_LICENSE("GPL");
