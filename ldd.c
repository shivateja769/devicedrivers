#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shiva Teja");
MODULE_DESCRIPTION("Simple Hello World Kernel Module");


static ssize_t	ldd_read(struct file *functionpointer, 
                        char *user_space_buffer,
                        size_t count,
                        loff_t *offset){
  char msg[]="ACK!\n";
  size_t len=strlen(msg);
  
  int result;
  printk("Ldd read\n");
    
  if(*offset>=len)
    return 0;
 
  result=copy_to_user(user_space_buffer,msg,len);
  
  *offset=*offset+len;
     
  return len;

}

struct proc_dir_entry *custom_proc_node;

struct proc_ops driver_proc_ops={

.proc_read=ldd_read

};
static int __init lddinit(void)
{
    //printk(KERN_INFO "Hello, world from lddinit!\n");
    
    printk("Ldd init: Entry\n");
    custom_proc_node = proc_create("ldd_driver", 
                                    0, 
                                   NULL, 
                                   &driver_proc_ops);
    printk("Ldd init: Exit\n");
    return 0;
}

static void __exit lddexit(void)
{
    //printk(KERN_INFO "Goodbye from lddexit!\n");
     printk("Ldd exit: Entry\n");
    proc_remove(custom_proc_node);
     printk("Ldd exit: Exit\n");
}

/*struct proc_dir_entry *proc_create(const char *name, 
                                    umode_t mode, 
                                    struct proc_dir_entry *parent, 
                                    const struct proc_ops *proc_ops);*/
                                    

module_init(lddinit);
module_exit(lddexit);
