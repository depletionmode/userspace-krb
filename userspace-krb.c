/* 
 * userspace-krb linux kernel module
 * (C) 2010, David Kaplan <david@2of1.org>
 *
 * Module to allow writing to the kernel ring buffer from userspace
 *
 */

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE( "GPL" );
MODULE_DESCRIPTION( "Interface for userspace writing to kernel ring buffer" );
MODULE_AUTHOR( "David Kaplan");

static struct proc_dir_entry *proc_entry;

int krb_write( struct file *fp, const char __user *buf,
               unsigned long len, void *data )
{
    int res = len;
    char *msg = kmalloc( len + 1, GFP_ATOMIC );

    msg[len] = 0;    /* force \0 termination */

    if ( copy_from_user( msg, buf, len ) )
        res = -EFAULT;
    else
        printk( KERN_INFO "%s", msg );

    kfree( msg );

    return res;
}

int krb_init()
{
    if ( ( proc_entry = create_proc_entry( "krb", 0222, NULL ) ) ) {
        proc_entry->write_proc = krb_write;
    
        printk( KERN_INFO "userspace-krb: module loaded.\n" );
        printk( KERN_INFO "userspace-krb: proc entry' krb' created.\n" );

        return 0;
    }

    printk( KERN_INFO "userspace-krb: could not create proc entry!\n" );

    return -ENOMEM;
}

void krb_exit()
{
    remove_proc_entry( "krb", NULL );
    printk( KERN_INFO "userspace-krb: module unloaded.\n" );
}

module_init( krb_init );
module_exit( krb_exit );
