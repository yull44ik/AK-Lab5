#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Yulia Mikhaylenko <mikhaylenko.yulia@lll.kpi.ua>");
MODULE_DESCRIPTION("Bug testing in Linux Kernel Lab 5");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int h_count = 1;
// Access for all users to read-only the parameter h_count
module_param(h_count, uint, 0444);
MODULE_PARM_DESC(h_count, "How many times to display 'Hello, world!'");

struct node
{
	struct list_head list_node;
	ktime_t timestamp;
};

static LIST_HEAD(Head_Node);

static int __init hello_init(void)
{

	int i;

	// Triggers BUG with condition
	BUG_ON(h_count > 10);

	if (h_count == 0 || (h_count >= 5 && h_count <= 10))
	{
		pr_warn("Wrong number of hello.\n");
	}

	for (i = 0; i < h_count; i++)
	{
		struct node *linkedlist = kmalloc(sizeof(*linkedlist), GFP_KERNEL);

		// Forcing a bug
		// Note: it's better to clear previously allocated memory
		if(i == 5)
			linkedlist = 0;

		linkedlist->timestamp = ktime_get();
		list_add(&linkedlist->list_node, &Head_Node);
		pr_emerg("Hello, world!\n");
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct node *linkedlist, *tmp;

	list_for_each_entry_safe(linkedlist, tmp, &Head_Node, list_node)
	{
		ktime_t duration = ktime_sub(ktime_get(), linkedlist->timestamp);
        
		pr_info("Elapsed time: %lld\n", ktime_to_ns(duration));
		list_del(&linkedlist->list_node);
		kfree(linkedlist);
	}
}

module_init(hello_init);
module_exit(hello_exit);