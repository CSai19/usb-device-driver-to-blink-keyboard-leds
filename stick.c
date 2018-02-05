#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include<linux/configfs.h>
#include<linux/init.h>
#include<linux/tty.h>
#include<linux/kd.h>
#include<linux/vt.h>
#include<linux/console_struct.h>
#include<linux/vt_kern.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CJB");
MODULE_DESCRIPTION("USB LED BLINKING");

struct timer_list my_timer;
struct tty_driver *my_driver;
char kbledstatus=0;
#define BLINK_DELAY HZ/5
#define ALL_LEDS_ON 0x07
#define RESTORE_LEDS 0xFF

static void my_timer_func(unsigned long ptr)
{
    int *pstatus = (int *)ptr;
    if(*pstatus == ALL_LEDS_ON)
        *pstatus=RESTORE_LEDS;
    else
	*pstatus=ALL_LEDS_ON;
    (my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty,KDSETLED,*pstatus);
    my_timer.expires=jiffies+BLINK_DELAY;
    add_timer(&my_timer);
}



//probe function

static int pen_probe(struct usb_interface *interface,const struct usb_device_id *id)
{
int i;
printk(KERN_INFO "[*] Chandra USB (%04X:%04X) plugged \n", id->idVendor, id->idProduct);
printk(KERN_INFO "kbleds:loading\n");
printk(KERN_INFO ,"kbleds:fgconsole is %x\n",fg_console);
for(i=0;i<MAX_NR_CONSOLES;i++){
if(!vc_cons[i].d)
break;
printk(KERN_INFO "poet_atkm:console[%i%i] #%i,tty %lx\n",i,MAX_NR_CONSOLES,vc_cons[i].d->vc_num,(unsigned long)vc_cons[i].d->port.tty);
}
printk(KERN_INFO "kbleds:finished scanning consoles\n");
my_driver=vc_cons[fg_console].d->port.tty->driver;
printk(KERN_INFO "kbleds:tty driver magic %x\n",my_driver->magic);
init_timer(&my_timer);
my_timer.function=my_timer_func;
my_timer.data=(unsigned long)&kbledstatus;
my_timer.expires=jiffies+BLINK_DELAY;
add_timer(&my_timer);

return 0;
}

//disconnect 


//usb_device_id
static struct usb_device_id pen_table[]={

{USB_DEVICE(0x0e8d,0x2008)},
{USB_DEVICE(0X2717,0X1360)}, 
{USB_DEVICE(0x0781,0x5590)},
{USB_DEVICE(0x0781,0x5567)}, 

{}
};

static void pen_disconnect(struct usb_interface *interface){
printk(KERN_INFO "[*] Chandra USB removed\n");
printk(KERN_INFO "kbleds:unloading...\n");
del_timer(&my_timer);
(my_driver->ops->ioctl)(vc_cons[fg_console].d->port.tty,KDSETLED,RESTORE_LEDS);

}

MODULE_DEVICE_TABLE(usb,pen_table);

//usb driver
static struct usb_driver pen_driver =
{
.name="chandra-USB stick driver",
.id_table=pen_table, //usb_device_id
.probe=pen_probe,
.disconnect=pen_disconnect,
};

static int __init pen_init(void){
int ret=-1;
printk(KERN_INFO "[*]Chandra Constuction od driver");
printk(KERN_INFO "\tRegistering Driver with Kernel");
ret=usb_register(&pen_driver);
printk(KERN_INFO "\tRegistration is complete");
return ret;
}

static void __exit pen_exit(void){
//deregister
printk(KERN_INFO "[*] Chandra Destructor of driver");
usb_deregister(&pen_driver);
printk(KERN_INFO "\tunregistration complete!");
}

module_init(pen_init);
module_exit(pen_exit);
